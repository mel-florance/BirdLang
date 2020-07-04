#pragma once

#include "Token.h"
#include "Platform.h"

class Node {
public:
	Node(Token* token, Node* left = nullptr, Node* right = nullptr) : 
		token(token),
		left(left),
		right(right)
	{}

	virtual ~Node() {}

	Token* token;
	Node* left;
	Node* right;

	inline friend std::ostream& operator << (std::ostream& stream, Node* node) {
		return stream << "Node, Token:" << node->token;
	}
};

class NumericNode : public Node {
public:
	NumericNode(Token* token) :
		Node(token) {}

	inline friend std::ostream& operator << (std::ostream& stream, NumericNode* node) {
		return stream << "NumericNode, Token:" << node->token;
	}
};

class BinaryOperationNode : public Node {
public:
	BinaryOperationNode(Node* left, Token* token, Node* right) :
		Node(token, left, right) {}

	inline friend std::ostream& operator << (std::ostream& stream, BinaryOperationNode* node) {
		return stream << "BinaryOperationNode, Token:" << node->token;
	}
};

class UnaryOperationNode : public Node {
public:
	UnaryOperationNode(Node* node, Token* token) :
		Node(token, nullptr, node),
		node(node) {}

	inline friend std::ostream& operator << (std::ostream& stream, UnaryOperationNode* node) {
		return stream << "UnaryOperationNode, Token:" << node->token;
	}

	Node* node;
};

class VariableAccessNode : public Node {
public:
	VariableAccessNode(Token* token) : 
		Node(token),
		start(token->start),
		end(token->end)
	{}

	Cursor* start;
	Cursor* end;
};

class VariableAssignmentNode : public Node {
public:
	VariableAssignmentNode(Token* token, Node* node) :
		Node(token, node),
		start(token->start),
		end(token->end) {}

	Cursor* start;
	Cursor* end;
};

class IfStatementNode : public Node {
public:
	IfStatementNode(Token* token, const std::vector<std::pair<Node*, Node*>>& cases, Node* else_case) :
		Node(token),
		start(token->start),
		end(token->end),
		cases(cases),
		else_case(else_case) {}

	Cursor* start;
	Cursor* end;
	std::vector<std::pair<Node*, Node*>> cases;
	Node* else_case;
};

class ForStatementNode : public Node {
public:
	ForStatementNode(Token* token, Node* start_value, Node* end_value, Node* step, Node* body) : 
		Node(token),
		start(token->start),
		end(token->end),
		start_value(start_value),
		end_value(end_value),
		step(step),
		body(body)
	{}

	Cursor* start;
	Cursor* end;
	Node* start_value;
	Node* end_value;
	Node* step;
	Node* body;
};

class WhileStatementNode : public Node {
public:
	WhileStatementNode(Token* token, Node* condition, Node* body) : 
		Node(token),
		start(token->start),
		end(token->end),
		condition(condition),
		body(body) {}

	Cursor* start;
	Cursor* end;
	Node* condition;
	Node* body;
};