#pragma once

#include "Nodes.h"

class Interpreter {
public:
	Interpreter();
	void visit(Node* node);
	void visit_numeric_node(Node* node);
	void visit_binary_operation_node(Node* node);
	void visit_unary_operation_node(Node* node);
};