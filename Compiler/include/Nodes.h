#pragma once

#include "Token.h"
#include "Platform.h"



class Node {
public:
	enum Type {
		NONE,
		NUMERIC,
		BINARY,
		UNARY,
		VARIABLE_ACCESS,
		VARIABLE_ASSIGN,
		IF_STATEMENT,
		FOR_STATEMENT,
		WHILE_STATEMENT
	};

	Node(
		Token* token,
		Node* left = nullptr,
		Node* right = nullptr,
		Type type = Type::NONE
	) : 
		token(token),
		left(left),
		right(right),
		type(type)
	{}

	virtual ~Node() {
		delete token;
		delete left;
		delete right;
	}

	Token* token;
	Node* left;
	Node* right;
	Type type;

	inline std::string typeToStr() {
		switch (type) {
		default:
		case Type::NONE:			return "NONE";
		case Type::NUMERIC:			return "NUMERIC";
		case Type::BINARY:			return "BINARY";
		case Type::UNARY:			return "UNARY";
		case Type::VARIABLE_ACCESS: return "VARIABLE_ACCESS";
		case Type::VARIABLE_ASSIGN: return "VARIABLE_ASSIGN";
		case Type::IF_STATEMENT:	return "IF_STATEMENT";
		case Type::FOR_STATEMENT:	return "FOR_STATEMENT";
		case Type::WHILE_STATEMENT: return "WHILE_STATEMENT";
		}
	}

	friend std::ostream& operator << (std::ostream& stream, Node* node);
};

class NumericNode : public Node {
public:
	NumericNode(Token* token) :
		Node(token, nullptr, nullptr, Type::NUMERIC)
	{}
};

class BinaryOperationNode : public Node {
public:
	BinaryOperationNode(Node* left, Token* token, Node* right) :
		Node(token, left, right, Type::BINARY)
	{}
};

class UnaryOperationNode : public Node {
public:
	UnaryOperationNode(Node* node, Token* token) :
		Node(token, nullptr, node, Type::UNARY),
		node(node)
	{}

	Node* node;
};

class VariableAccessNode : public Node {
public:
	VariableAccessNode(Token* token) : 
		Node(token, nullptr, nullptr, Type::VARIABLE_ACCESS),
		start(token->start),
		end(token->end)
	{}

	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
};

class VariableAssignmentNode : public Node {
public:
	VariableAssignmentNode(Token* token, Node* node) :
		Node(token, node, nullptr, Type::VARIABLE_ASSIGN),
		start(token->start),
		end(token->end)
	{}

	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
};

class IfStatementNode : public Node {
public:
	IfStatementNode(
		Token* token,
		const std::vector<std::pair<Node*, Node*>>& cases,
		Node* else_case
	) :
		Node(token, nullptr, nullptr, Type::IF_STATEMENT),
		start(token->start),
		end(token->end),
		cases(cases),
		else_case(else_case) {}

	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	std::vector<std::pair<Node*, Node*>> cases;
	Node* else_case;
};

class ForStatementNode : public Node {
public:
	ForStatementNode(
		Token* token,
		Node* start_value,
		Node* end_value,
		Node* step,
		Node* body
	) : 
		Node(token, nullptr, nullptr, Type::FOR_STATEMENT),
		start(token->start),
		end(body->token->end),
		start_value(start_value),
		end_value(end_value),
		step(step),
		body(body)
	{}

	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	Node* start_value;
	Node* end_value;
	Node* step;
	Node* body;
};

class WhileStatementNode : public Node {
public:
	WhileStatementNode(Token* token, Node* condition, Node* body) : 
		Node(token, nullptr, nullptr, Type::WHILE_STATEMENT),
		start(condition->token->start),
		end(body->token->end),
		condition(condition),
		body(body)
	{}

	~WhileStatementNode() {
		delete condition;
		delete body;
	}

	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
	Node* condition;
	Node* body;
};
