#include "Interpreter.h"

Interpreter::Interpreter()
{

}

Interpreter::Result* Interpreter::visit(Node* node)
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

Interpreter::Result* Interpreter::visit_numeric_node(Node* node)
{
	Result* result = new Result();
	Number* number = new Number();
	number->start = node->token->start;
	number->end = node->token->end;

	if (node->token->value.index() == 0) {
		try { number->value = std::get<float>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { number->value = std::get<int>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_binary_operation_node(Node* node)
{
	Result* result = new Result();
	Number* number = new Number();
	number->start = node->token->start;
	number->end = node->token->end;

	Number* left = result->record(visit(node->left));

	if (result->error != nullptr)
		return result;

	Number* right = result->record(visit(node->right));

	if (result->error != nullptr)
		return result;

	if (node->token->type == Token::Type::PLUS) {
		number = left->add(right);
	}
	else if (node->token->type == Token::Type::MINUS) {
		number = left->subtract(right);
	}
	else if (node->token->type == Token::Type::MUL) {
		number = left->multiply(right);
	}
	else if (node->token->type == Token::Type::DIV) {
		number = left->divide(right);
	}

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_unary_operation_node(Node* node)
{
	Result* result = new Result();
	Number* number = result->record(visit(node->right));
	
	if (result->error != nullptr) {
		return result;
	}

	number->start = node->token->start;
	number->end = node->token->end;

	if (node->token->type == Token::Type::MINUS) {
		number = number->multiply(new Number(-1));
	}

	return result->success(number);
}
