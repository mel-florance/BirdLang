#include "Parser.h"

Parser::Parser(const std::vector<Token*>& tokens) :
	tokens(tokens),
	current_token(nullptr),
	index(-1),
	debug(true)
{
	advance();
}

Node* Parser::parse()
{
	Node* node = expr();

	if (debug) {
		traverse(node, 0);
	}

	return node;
}

void Parser::traverse(Node* node, unsigned int depth)
{
	if (node == nullptr)
		return;

	++depth;

	traverse(node->right, depth);
	std::cout << std::string(depth, '\t') << node << std::endl;
	traverse(node->left, depth);
}

Token* Parser::advance()
{
	++index;

	if (index < tokens.size()) {
		current_token = tokens.at(index);
	}

	return current_token;
}

Node* Parser::factor()
{
	if (current_token != nullptr) {
		if (current_token->type == Token::Type::FLOAT ||
			current_token->type == Token::Type::INT) {
			Token* token = new Token(current_token);
			advance();

			return new NumericNode(token);
		}
	}

	return nullptr;
}

Node* Parser::term()
{
	return binary_operation([=]() {
		return factor();
	}, { Token::Type::MUL, Token::Type::DIV });
}

Node* Parser::expr()
{
	return binary_operation([=]() {
		return term();
	}, { Token::Type::PLUS, Token::Type::MINUS });
}

Node* Parser::binary_operation(std::function<Node*()> fn, const std::vector<Token::Type>& operations)
{
	if (current_token != nullptr) {
		Node* left = fn();

		while (std::find(operations.begin(), operations.end(), current_token->type) != operations.end()) {
			Token* token = new Token(current_token);

 			advance();
			Node* right = fn();
			left = new BinaryOperationNode(left, token, right);
		}

		return left;
	}

	return nullptr;
}
