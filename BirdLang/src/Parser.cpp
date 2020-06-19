#include "Parser.h"

Parser::Parser(const std::vector<Token*>& tokens) :
	tokens(tokens),
	current_token(nullptr),
	index(-1),
	debug(true)
{
	advance();
}

Parser::Result* Parser::parse()
{
	Result* result = expr();

	if (result != nullptr) {
		if (result->error == nullptr && current_token->type != Token::Type::EOT) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected '+', '-', '*' or '/'"
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
	if (current_token != nullptr) {

		Result* result = new Result();

		if (current_token->type == Token::Type::PLUS ||
			current_token->type == Token::Type::MINUS) {
			Token* token = new Token(current_token);

			result->record(advance());
			auto fac = result->record(factor());

			if (result->error != nullptr)
				return result;

			return result->success(new UnaryOperationNode(fac, token));
;		}
		else if (current_token->type == Token::Type::FLOAT ||
			current_token->type == Token::Type::INT) {
			Token* token = new Token(current_token);
			
			result->record(advance());

			return result->success(new NumericNode(token));
		}
		else if (current_token->type == Token::Type::LPAREN) {
			Token* token = new Token(current_token);

			result->record(advance());
			auto exp = result->record(expr());

			if (result->error != nullptr)
				return result;

			if (current_token->type == Token::Type::RPAREN) {
				result->record(advance());
				return result->success(exp);
			}
			else {
				return result->failure(new InvalidSyntaxError(
					current_token->start, 
					current_token->end, 
					"Expected ')'"
				));
			}
		}

		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected Integer or Float"
		));
	}

	return nullptr;
}

Parser::Result* Parser::term()
{
	return binary_operation([=]() {
		return factor();
	}, { Token::Type::MUL, Token::Type::DIV });
}

Parser::Result* Parser::expr()
{
	return binary_operation([=]() {
		return term();
	}, { Token::Type::PLUS, Token::Type::MINUS });
}

Parser::Result* Parser::binary_operation(std::function<Result*()> fn, const std::vector<Token::Type>& operations)
{
	if (current_token != nullptr) {

		Result* result = new Result();
		Node* left = result->record(fn());

		if (result->error != nullptr)
			return result;

		while (std::find(operations.begin(), operations.end(), current_token->type) != operations.end()) {
			Token* token = new Token(current_token);

 			result->record(advance());
			Node* right = result->record(fn());

			if (result->error != nullptr)
				return result;

			left = new BinaryOperationNode(left, token, right);
		}

		return result->success(left);
	}

	return nullptr;
}
