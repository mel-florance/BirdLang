#include "pch.h"
#include "Nodes.h"

std::ostream& operator << (std::ostream& stream, Node* node)
{
	BinaryOperationNode* binary_operation_node = dynamic_cast<BinaryOperationNode*>(node);
	if (binary_operation_node)
		return stream << "BinaryOperation:" << node->token;

	NumericNode* numeric_node = dynamic_cast<NumericNode*>(node);
	if (numeric_node)
		return stream << "Numeric:" << node->token;

	UnaryOperationNode* unary_operation_node = dynamic_cast<UnaryOperationNode*>(node);
	if (unary_operation_node)
		return stream << "UnaryOperation:" << node->token;

	VariableAccessNode* variable_access_node = dynamic_cast<VariableAccessNode*>(node);
	if (variable_access_node)
		return stream << "VariableAccess:" << node->token;

	VariableAssignmentNode* variable_assignment_node = dynamic_cast<VariableAssignmentNode*>(node);
	if (variable_assignment_node)
		return stream << "VariableAssignment:" << node->token;

	IfStatementNode* if_statement_node = dynamic_cast<IfStatementNode*>(node);
	if (if_statement_node)
		return stream << "IfStatement:" << node->token;

	ForStatementNode* for_statement_node = dynamic_cast<ForStatementNode*>(node);
	if (for_statement_node)
		return stream << "ForStatement:" << node->token;

	WhileStatementNode* while_statement_node = dynamic_cast<WhileStatementNode*>(node);
	if (while_statement_node)
		return stream << "WhileStatement:" << node->token;

	return stream;
}
