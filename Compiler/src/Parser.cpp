#include "pch.h"

#include "Parser.h"
#include "Profiler.h"
#include "Utils.h"

Parser::Parser() :
	current_token(nullptr),
	index(-1),
	debug(false),
	parsing_time(0.0)
{
}

Parser::Result* Parser::parse()
{
	if (tokens.size() == 0)
		return nullptr;

	Profiler profiler;
	profiler.start = clock();
	index = -1;

	advance();

	Result* result = expr();

	if (result != nullptr) {
		if (result->error == nullptr && current_token->type != Token::Type::EOL) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected '+', '-', '*', '/', '^', '==', '!=', '<', '>', '<=', '>=', 'and' or 'or'"
			));
		}

		if (debug) {
			Utils::title("ABSTRACT SYNTAX TREE", 4);
			traverse(result->node);
		}
	}

	profiler.end = clock();
	parsing_time = profiler.getReport();

	return result;
}

void Parser::traverse(Node* node, const std::string& prefix, bool isLeft)
{
	if (node == nullptr)
		return;

	std::cout << prefix;
	std::cout << (isLeft ? "├──" : "└──");
	std::cout << node << '\n';

	traverse(node->left, prefix + (isLeft ? "│   " : "    "), true);
	traverse(node->right, prefix + (isLeft ? "│   " : "    "), false);
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
	});
}

Parser::Result* Parser::arithm()
{
	return binary_operation([=]() {
		return term();
	}, {
		Token::Type::PLUS,
		Token::Type::MINUS
	});
}

Parser::Result* Parser::power()
{
	return binary_operation([=]() {
		return function_call();
	}, {
		Token::Type::POW,
		Token::Type::MOD
	}, [=]() {
		return factor();
	});
}

Parser::Result* Parser::expr()
{
	if (current_token != nullptr) {
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
					var_name->start,
					var_name->end,
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
			std::make_pair(Token::Type::KEYWORD, "and"),
			std::make_pair(Token::Type::KEYWORD, "or")
		}));

		if (result->error != nullptr) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected 'var', int, double, identifier, '+', '-', '(' or 'not'"
			));
		}

		return result->success(node);
	}
	
	return nullptr;
}

Parser::Result* Parser::if_expr()
{
	if (current_token != nullptr) {
		Result* result = new Result();
		Node* else_expr = nullptr;
		Node* condition = nullptr;
		std::vector<std::pair<Node*, Node*>> cases;

		std::string if_value;
		try { if_value = std::get<std::string>(current_token->value); }
		catch (const std::bad_variant_access&) {}

		if (if_value != "if") {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected 'if'"
			));
		}

		result->record_advance();
		advance();

		condition = result->record(expr());

		if (result->error != nullptr)
			return result;

		std::string then_value;
		try { then_value = std::get<std::string>(current_token->value); }
		catch (const std::bad_variant_access&) {}

		if (then_value != "then") {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected 'then'"
			));
		}

		result->record_advance();
		advance();

		auto exp = result->record(expr());

		if (result->error != nullptr)
			return result;

		cases.push_back(std::make_pair(condition, exp));

		auto isElseIf = [=]() {
			std::string val;
			try { val = std::get<std::string>(current_token->value); }
			catch (const std::bad_variant_access&) {}

			return current_token->type == Token::Type::KEYWORD && val == "else if";
		};

		while (isElseIf()) {
			result->record_advance();
			advance();

			auto cond = result->record(expr());

			if (result->error != nullptr)
				return result;

			std::string elseif_then;
			try { elseif_then = std::get<std::string>(current_token->value); }
			catch (const std::bad_variant_access&) {}

			if (current_token->type != Token::Type::KEYWORD || elseif_then != "then") {
				return result->failure(new InvalidSyntaxError(
					current_token->start,
					current_token->end,
					"Expected 'then'"
				));
			}

			result->record_advance();
			advance();

			auto then_expr = result->record(expr());

			if (result->error != nullptr)
				return result;

			cases.push_back(std::make_pair(cond, then_expr));
		}

		std::string else_value;
		try { else_value = std::get<std::string>(current_token->value); }
		catch (const std::bad_variant_access&) {}

		if (current_token->type == Token::Type::KEYWORD && else_value == "else") {
			result->record_advance();
			advance();

			else_expr = result->record(expr());

			if (result->error != nullptr)
				return result;
		}

		return result->success(new IfStatementNode(current_token, cases, else_expr));
	}
	
	return nullptr;
}

Parser::Result* Parser::for_expr()
{
	Result* result = new Result();

	std::string for_value;
	try { for_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (for_value != "for") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'for'"
		));
	}

	result->record_advance();
	advance();

	if (current_token->type != Token::Type::IDENTIFIER) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected identifier"	
		));
	}

	auto var_name = new Token(current_token);
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

	auto start_value = result->record(expr());

	if (result->error != nullptr)
		return result;

	std::string to_value;
	try { to_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (current_token->type != Token::Type::KEYWORD || to_value != "to") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'to'"
		));
	}

	result->record_advance();
	advance();

	auto end_value = result->record(expr());
	Node* step = nullptr;

	if (result->error != nullptr)
		return result;

	std::string step_value;
	try { step_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (current_token->type == Token::Type::KEYWORD && step_value == "step") {
		result->record_advance();
		advance();

		step = result->record(expr());

		if (result->error != nullptr)
			return result;
	}

	std::string then_value;
	try { then_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (current_token->type != Token::Type::KEYWORD || then_value != "then") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'then'"
		));
	}

	result->record_advance();
	advance();

	auto body = result->record(expr());

	if (result->error != nullptr)
		return result;

	return result->success(new ForStatementNode(var_name, start_value, end_value, step, body));
}

Parser::Result* Parser::while_expr()
{
	Result* result = new Result();

	std::string while_value;
	try { while_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (while_value != "while") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'while'"
		));
	}

	result->record_advance();
	advance();

	auto condition = result->record(expr());

	if (result->error != nullptr)
		return result;

	std::string then_value;
	try { then_value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}

	if (then_value != "then") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'then'"
		));
	}

	result->record_advance();
	advance();

	auto body = result->record(expr());

	if (result->error != nullptr)
		return result;

	return result->success(new WhileStatementNode(current_token, condition, body));
}

Parser::Result* Parser::atom()
{
	if (current_token != nullptr) {

		Result* result = new Result();

		if (current_token->type == Token::Type::DOUBLE ||
			current_token->type == Token::Type::INT) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			return result->success(new NumericNode(token));
		}
		if (current_token->type == Token::Type::STRING) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			return result->success(new StringNode(token));
		}
		else if (current_token->type == Token::Type::IDENTIFIER) {
			Token* token = new Token(current_token);

			result->record_advance();
			advance();

			if (current_token->type == Token::Type::DOT) {
				result->record_advance();
				advance();

				Token* prop_name = new Token(current_token);

				result->record_advance();
				advance();

				return result->success(new PropertyAccessNode(prop_name, std::get<std::string>(token->value)));
			}
			else if (current_token->type == Token::Type::LSBRACKET) {
				result->record_advance();
				advance();

				auto exp = result->record(expr());

				if (result->error != nullptr)
					return result;

				if (current_token->type == Token::Type::RSBRACKET) {
					result->record_advance();
					advance();

					return result->success(new IndexAccessNode(
						token,
						exp,
						current_token->start,
						current_token->end
					));
				}

				return result->failure(new InvalidSyntaxError(
					current_token->start,
					current_token->end,
					"Expected ']'"
				));
			}
			else {
				return result->success(new VariableAccessNode(token));
			}
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
		else {
			std::string value;
			try { value = std::get<std::string>(current_token->value); }
			catch (const std::bad_variant_access&) {}

			if (current_token->type == Token::Type::LSBRACKET) {
				auto array = result->record(array_expr());

				if (result->error != nullptr)
					return result;

				return result->success(array);
			}
			if (current_token->type == Token::Type::LCBRACKET) {
				auto map = result->record(map_expr());

				if (result->error != nullptr)
					return result;

				return result->success(map);
			}
			else if (current_token->type == Token::Type::KEYWORD && value == "if") {
				auto exp = result->record(if_expr());

				if (result->error != nullptr)
					return result;

				return result->success(exp);
			}
			else if (current_token->type == Token::Type::KEYWORD && value == "for") {
				auto exp = result->record(for_expr());

				if (result->error != nullptr)
					return result;

				return result->success(exp);
			}
			else if (current_token->type == Token::Type::KEYWORD && value == "while") {
				auto exp = result->record(while_expr());

				if (result->error != nullptr)
					return result;

				return result->success(exp);
			}
			else if (current_token->type == Token::Type::KEYWORD && value == "fn") {
				auto exp = result->record(function_definition());

				if (result->error != nullptr)
					return result;

				return result->success(exp);
			}
		}


		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected integer, float, identifier, if, for, while, fn,'+', '-', '(', '['"
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
	}));

	if (result->error != nullptr) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected Integer, Float, '+', '-', '(', 'not'"
		));
	}

	return result->success(node);
}

Parser::Result* Parser::function_definition()
{
	Result* result = new Result();
	Token* fn_name = nullptr;
	std::vector<Token*> args_names = {};

	std::string value;
	try { value = std::get<std::string>(current_token->value); }
	catch (const std::bad_variant_access&) {}


	if (current_token->type != Token::Type::KEYWORD || value != "fn") {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected 'fn'"
		));
	}

	result->record_advance();
	advance();


	if (current_token->type == Token::Type::IDENTIFIER) {
		fn_name = new Token(current_token);
		result->record_advance();
		advance();

		if (current_token->type != Token::Type::LPAREN) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected '('"
			));
		}
	}
	else {
		if (current_token->type != Token::Type::LPAREN) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected identifier or '('"
			));
		}
	}

	result->record_advance();
	advance();

	if (current_token->type == Token::Type::IDENTIFIER) {
		args_names.push_back(current_token);
		result->record_advance();
		advance();

		while (current_token->type == Token::Type::COMMA) {
			result->record_advance();
			advance();

			if (current_token->type != Token::Type::IDENTIFIER) {
				return result->failure(new InvalidSyntaxError(
					current_token->start,
					current_token->end,
					"Expected identifier"
				));
			}

			args_names.push_back(current_token);
			result->record_advance();
			advance();
		}

		if (current_token->type != Token::Type::RPAREN) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ',' or ')'"
			));
		}
	}
	else {
		if (current_token->type != Token::Type::RPAREN) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected identifier or ')'"
			));
		}
	}

	result->record_advance();
	advance();

	if (current_token->type != Token::Type::ARROW) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected '->'"
		));
	}

	result->record_advance();
	advance();

	auto return_node = result->record(expr());

	if (result->error != nullptr)
		return result;

	return result->success(new FunctionDefinitionNode(args_names, return_node, fn_name));
}

Parser::Result* Parser::function_call()
{
	Result* result = new Result();

	auto atm = result->record(atom());

	if (result->error != nullptr)
		return result;

	if (current_token->type == Token::Type::LPAREN) {
		result->record_advance();
		advance();

		std::vector<Node*> args_nodes = {};

		if (current_token->type == Token::Type::RPAREN) {
			result->record_advance();
			advance();
		}
		else {
			args_nodes.push_back(result->record(expr()));

			if (result->error != nullptr) {
				return result->failure(new InvalidSyntaxError(
					current_token->start,
					current_token->end,
					"Expected ')', 'var', 'if', 'for', 'while', 'fn', 'integer', 'double', 'identifier'"
				));
			}

			while (current_token->type == Token::Type::COMMA) {
				result->record_advance();
				advance();

				args_nodes.push_back(result->record(expr()));

				if (result->error != nullptr)
					return result;
			}

			if (current_token->type != Token::Type::RPAREN) {
				return result->failure(new InvalidSyntaxError(
					current_token->start,
					current_token->end,
					"Expected ',' or ')'"
				));
			}

			result->record_advance();
			advance();
		}

		return result->success(new FunctionCallNode(current_token, atm, args_nodes));
	}

	return result->success(atm);
}

Parser::Result* Parser::array_expr()
{
	Result* result = new Result();
	std::vector<Node*> elements;
	std::shared_ptr<Cursor> start = std::make_shared<Cursor>(*current_token->start);

	if (current_token->type != Token::Type::LSBRACKET) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected '['"
		));
	}

	result->record_advance();
	advance();

	if (current_token->type == Token::Type::RSBRACKET) {
		result->record_advance();
		advance();
	}
	else {
		elements.push_back(result->record(expr()));

		if (result->error != nullptr) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ']', 'var', 'if', 'for', 'while', 'fn', 'integer', 'double', 'identifier'"
			));
		}

		while (current_token->type == Token::Type::COMMA) {
			result->record_advance();
			advance();

			elements.push_back(result->record(expr()));

			if (result->error != nullptr)
				return result;
		}

		if (current_token->type != Token::Type::RSBRACKET) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ',' or ']'"
			));
		}

		result->record_advance();
		advance();
	}

	return result->success(new ArrayNode(current_token, elements));
}

Parser::Result* Parser::map_expr()
{
	Result* result = new Result();
	std::map<std::string, Node*> elements;
	std::shared_ptr<Cursor> start = std::make_shared<Cursor>(*current_token->start);

	if (current_token->type != Token::Type::LCBRACKET) {
		return result->failure(new InvalidSyntaxError(
			current_token->start,
			current_token->end,
			"Expected '{'"
		));
	}

	result->record_advance();
	advance();

	auto var_name = new Token(current_token);

	if (current_token->type == Token::Type::RCBRACKET) {
		result->record_advance();
		advance();
	}
	else {
		result->record_advance();
		advance();

		if (current_token->type != Token::Type::COLON) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ':'"
			));
		}

		result->record_advance();
		advance();

		elements[std::get<std::string>(var_name->value)] = result->record(expr());

		if (result->error != nullptr) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ']', 'var', 'if', 'for', 'while', 'fn', 'integer', 'double', 'identifier'"
			));
		}

		while (current_token->type == Token::Type::COMMA) {
			result->record_advance();
			advance();

			if (current_token->type == Token::Type::IDENTIFIER) {
				auto prop_name = std::get<std::string>(current_token->value);

				result->record_advance();
				advance();

				if (current_token->type != Token::Type::COLON) {
					return result->failure(new InvalidSyntaxError(
						current_token->start,
						current_token->end,
						"Expected ':'"
					));
				}

				result->record_advance();
				advance();

				elements[prop_name] = result->record(expr());

				if (result->error != nullptr)
					return result;
			}
		}

		if (current_token->type != Token::Type::RCBRACKET) {
			return result->failure(new InvalidSyntaxError(
				current_token->start,
				current_token->end,
				"Expected ',' or '}'"
			));
		}

		result->record_advance();
		advance();
	}

	return result->success(new MapNode(current_token, elements));
}

Parser::Result* Parser::binary_operation(
	std::function<Result*()> fna, 
	const std::vector<std::variant<Token::Type, std::pair<Token::Type, std::string>>>& operations, 
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

		auto type_exists = [=]() {
			bool found_type = false;
			bool found_value = false;

			for (const auto& var : operations) {
				Token::Type type = Token::Type::NONE;
				try {type = std::get<Token::Type>(var); }
				catch (const std::bad_variant_access&) {}

				if (type == current_token->type)
					found_type = true;

				std::pair<Token::Type, std::string> pair;
				try { pair = std::get<std::pair<Token::Type, std::string>>(var); }
				catch (const std::bad_variant_access&) {}

				std::string value;
				try { value = std::get<std::string>(current_token->value); }
				catch (const std::bad_variant_access&) {}

				if (pair.first == current_token->type && pair.second == value)
					found_value = true;
			}

			return found_type || found_value;
		};

		while (type_exists()) {
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
