#include "pch.h"
#include "Parser.h"

Parser::Parser() :
	current_token(nullptr),
	index(-1),
	debug(true)
{

}

Parser::Result* Parser::parse()
{
	advance();
	Result* result = expr();

	if (result != nullptr) {
		if (result->error == nullptr && current_token->type != Token::Type::EOT) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected '+', '-', '*', '/', '^', '==', '!=', '<', '>', '<=', '>= 'and' or 'or'"
			));
		}

		if (debug) {
			traverse(result->node, 0);
		}
	}

	return result;
}

void Parser::traverse(Node* node, unsigned int depth)
{
	if (node == nullptr)
		return;
	
	++depth;

	traverse(node->left, depth);
	std::cout << std::string(depth, '\t') << node << std::endl;
	traverse(node->right, depth);
}

Token* Parser::advance()
{
	++index;

	if (index < tokens.size()) {
		current_token = tokens.at(index);
	}

	return current_token;
}

Parser::Result* Parser::factor()
{
	Result* result = new Result();

	if (current_token->type == Token::Type::PLUS ||
		current_token->type == Token::Type::MINUS) {
		Token* token = new Token(current_token);

		result->record_advance();
		advance();
		auto component = result->record(factor());

		if (result->error != nullptr)
			return result;

		return result->success(new UnaryOperationNode(component, token));
	}

	return power();
}

Parser::Result* Parser::term()
{
	return binary_operation([=]() {
		return factor();
	}, { 
		Token::Type::MUL,
		Token::Type::DIV
	}, {});
}

Parser::Result* Parser::arithm()
{
	return binary_operation([=]() {
		return term();
	}, {
		Token::Type::PLUS,
		Token::Type::MINUS
	}, {});
}

Parser::Result* Parser::power()
{
	return binary_operation([=]() {
		return atom();
	}, {
		Token::Type::POW
	}, {}, [=]() {
		return factor();
	});
}

Parser::Result* Parser::expr()
{
	Result* result = new Result();

	std::string value;
	try { value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (current_token->type == Token::Type::KEYWORD && value == "var") {
		result->record_advance();
		advance();

		if (current_token->type != Token::Type::IDENTIFIER) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end, 
				"Expected Identifier"
			));
		}

		Token* var_name = new Token(current_token);
		result->record_advance();
		advance();

		if (current_token->type != Token::Type::EQ) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end, 
				"Expected '='"
			));
		}

		result->record_advance();
		advance();

		Node* expression = result->record(expr());

		if (result->error != nullptr) {
			return result;
		}

		return result->success(new VariableAssignmentNode(var_name, expression));
	}

	auto node = result->record(binary_operation([=]() {
		return compare();
	}, { 
		Token::Type::KEYWORD,
		Token::Type::KEYWORD 
	}, { "and", "or" }));

	if (result->error != nullptr) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'var', int, float, identifier, '+', '-', '(' or 'not'"
		));
	}

	return result->success(node);
}

Parser::Result* Parser::atom()
{
	if (current_token != nullptr) {

		Result* result = new Result();

		if (current_token->type == Token::Type::FLOAT ||
			current_token->type == Token::Type::INT) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			return result->success(new NumericNode(token));
		}
		else if (current_token->type == Token::Type::IDENTIFIER) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			return result->success(new VariableAccessNode(token));
		}
		else if (current_token->type == Token::Type::LPAREN) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			auto exp = result->record(expr());

			if (result->error != nullptr)
				return result;

			if (current_token->type == Token::Type::RPAREN) {
				result->record_advance();
				advance();

				return result->success(exp);
			}

			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ')'"
			));
		}

		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected Integer, Float, '+', '-' or '('"
		));
	}

	return nullptr;
}

Parser::Result* Parser::compare()
{
	Result* result = new Result();

	std::string value;
	try { value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (current_token->type == Token::Type::KEYWORD && value == "not") {

		auto token = new Token(current_token);

		result->record_advance();
		advance();

		auto node = result->record(compare());

		if (result->error != nullptr)
			return result;

		return result->success(new UnaryOperationNode(node, token));
	}

	auto node = result->record(binary_operation([=]() {
		return arithm();
	}, {
		Token::Type::EE,
		Token::Type::NE,
		Token::Type::LT,
		Token::Type::GT,
		Token::Type::LTE,
		Token::Type::GTE
	}, {}));

	if (result->error != nullptr) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected Integer, Float, '+', '-', '(', 'not' "
		));
	}

	return result->success(node);
}

Parser::Result* Parser::binary_operation(
	std::function<Result*()> fna, 
	const std::vector<Token::Type>& operations, 
	const std::vector<std::string>& values, 
	std::function<Result*()> fnb
)
{
	if (current_token != nullptr) {

		if (fnb == nullptr)
			fnb = fna;

		Result* result = new Result();
		Node* left = result->record(fna());
		
		if (result->error != nullptr)
			return result;

		std::string value;
		try { value = std::get<std::string>(current_token->value); }
		catch (const std::bad_variant_access&) {}

		while (std::find(operations.begin(), operations.end(), current_token->type) != operations.end() || 
			(std::find(operations.begin(), operations.end(), current_token->type) != operations.end() &&
				std::find(values.begin(), values.end(), value) != values.end())) {

			Token* token = new Token(current_token);

			result->record_advance();
			advance();
			Node* right = result->record(fnb());

			if (result->error != nullptr)
				return result;

			left = new BinaryOperationNode(left, token, right);
		}

		return result->success(left);
	}

	return nullptr;
}
