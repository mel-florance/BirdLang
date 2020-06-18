#pragma once

#include "Nodes.h"
#include "Number.h"

class Interpreter {
public:
	Interpreter();
	Number visit(Node* node);
	Number visit_numeric_node(Node* node);
	Number visit_binary_operation_node(Node* node);
	Number visit_unary_operation_node(Node* node);
};