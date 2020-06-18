#include "Interpreter.h"

Interpreter::Interpreter()
{

}

Number Interpreter::visit(Node* node)
{
	auto type = typeid(*node).name();

	if (strcmp(type, "class BinaryOperationNode") == 0) {
		return visit_binary_operation_node(node);
	}
	else if (strcmp(type, "class NumericNode") == 0) {
		return visit_numeric_node(node);
	} 
	else if (strcmp(type, "class UnaryOperationNode") == 0) {
		return visit_unary_operation_node(node);
	} 
	else {
		std::cout << "No visit " << type << " method defined" << std::endl;
	}
}

Number Interpreter::visit_numeric_node(Node* node)
{
	Number number;
	number.start = node->token->start;
	number.end = node->token->end;

	if (node->token->value.index() == 0) {
		try { number.value = std::get<float>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { number.value = std::get<int>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}

	return number;
}

Number Interpreter::visit_binary_operation_node(Node* node)
{
	Number result;
	result.start = node->token->start;
	result.end = node->token->end;

	auto left = visit(node->left);
	auto right = visit(node->right);

	if (node->token->type == Token::Type::PLUS) {
		result = left.add(right);
	}
	else if (node->token->type == Token::Type::MINUS) {
		result = left.subtract(right);
	}
	else if (node->token->type == Token::Type::MUL) {
		result = left.multiply(right);
	}
	else if (node->token->type == Token::Type::DIV) {
		result = left.divide(right);
	}

	return result;
}

Number Interpreter::visit_unary_operation_node(Node* node)
{
	auto number = visit(node->right);
	number.start = node->token->start;
	number.end = node->token->end;

	if (node->token->type == Token::Type::MINUS) {
		number = number.multiply(Number(-1));
	}

	return number;
}
