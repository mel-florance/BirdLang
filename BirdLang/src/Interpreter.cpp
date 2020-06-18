#include "Interpreter.h"

Interpreter::Interpreter()
{

}

void Interpreter::visit(Node* node)
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

void Interpreter::visit_numeric_node(Node* node)
{
	std::cout << "Found numeric node" << std::endl;
}

void Interpreter::visit_binary_operation_node(Node* node)
{
	std::cout << "Found binary node" << std::endl;

	visit(node->left);
	visit(node->right);
}

void Interpreter::visit_unary_operation_node(Node* node)
{
	std::cout << "Found unary node" << std::endl;

	visit(node->right);
}
