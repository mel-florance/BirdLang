#include "Interpreter.h"

Interpreter::Interpreter()
{

}

Interpreter::Result* Interpreter::visit(Node* node, Context* context)
{
	auto type = typeid(*node).name();

	if (strcmp(type, "class BinaryOperationNode") == 0) {
		return visit_binary_operation_node(node, context);
	}
	else if (strcmp(type, "class NumericNode") == 0) {
		return visit_numeric_node(node, context);
	} 
	else if (strcmp(type, "class UnaryOperationNode") == 0) {
		return visit_unary_operation_node(node, context);
	} 
	else {
		std::cout << "No visit " << type << " method defined" << std::endl;
	}

	return nullptr;
}

Interpreter::Result* Interpreter::visit_numeric_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = new Number();
	number->context = context;
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

Interpreter::Result* Interpreter::visit_binary_operation_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = new Number();
	number->start = node->token->start;
	number->end = node->token->end;

	Number* left = result->record(visit(node->left, context));

	if (result->error != nullptr)
		return result;

	Number* right = result->record(visit(node->right, context));

	if (result->error != nullptr)
		return result;

	Error* error = nullptr;

	if (node->token->type == Token::Type::PLUS) {
		auto op_result = left->add(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::MINUS) {
		auto op_result = left->subtract(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::MUL) {
		auto op_result = left->multiply(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::DIV) {
		auto op_result = left->divide(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::POW) {
		auto op_result = left->power(right);
		number = op_result.first;
		error = op_result.second;
	}

	if (error != nullptr) {
		return result->failure(error);
	}

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_unary_operation_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = result->record(visit(node->right, context));
	
	if (result->error != nullptr) {
		return result;
	}

	number->start = node->token->start;
	number->end = node->token->end;

	Error* error = nullptr;

	if (node->token->type == Token::Type::MINUS) {
		auto op_result = number->multiply(new Number(-1));
		number = op_result.first;
		error = op_result.second;
	}

	if (error != nullptr) {
		return result->failure(error);
	}

	return result->success(number);
}
