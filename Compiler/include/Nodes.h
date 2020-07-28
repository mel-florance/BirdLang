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
		WHILE_STATEMENT,
		FN_DEFINITION,
		FN_CALL,
		STRING,
		ARRAY,
		PROPERTY_ACCESS
	};

	Node(
		Token* token,
		Node* left = nullptr,
		Node* right = nullptr,
		Type type = Type::NONE,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr
	) : 
		token(token),
		left(left),
		right(right),
		type(type),
		start(start),
		end(end)
	{}

	virtual ~Node() {
		delete token;
		delete left;
		delete right;
	}

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
		case Type::FN_DEFINITION:	return "FN_DEFINITION";
		case Type::FN_CALL:			return "FN_CALL";
		case Type::STRING:			return "STRING";
		case Type::ARRAY:			return "ARRAY";
		case Type::PROPERTY_ACCESS: return "PROPERTY_ACCESS";
		}
	}

	friend std::ostream& operator << (std::ostream& stream, Node* node);

	Token* token;
	Node* left;
	Node* right;
	Type type;
	std::shared_ptr<Cursor> start;
	std::shared_ptr<Cursor> end;
};

class NumericNode : public Node {
public:
	NumericNode(Token* token) :
		Node(token, nullptr, nullptr, Type::NUMERIC)
	{}
};

class StringNode : public Node {
public:
	StringNode(Token* token) :
		Node(token, nullptr, nullptr, Type::STRING)
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
		Node(
			token,
			nullptr,
			nullptr,
			Type::VARIABLE_ACCESS,
			token->start,
			token->end
		)
	{}
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
		Node(
			token,
			nullptr,
			nullptr,
			Type::IF_STATEMENT,
			token->start,
			token->end
		),
		cases(cases),
		else_case(else_case) {}

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
		Node(
			token,
			nullptr,
			nullptr,
			Type::FOR_STATEMENT,
			token->start,
			body->token->end
		),
		start_value(start_value),
		end_value(end_value),
		step(step),
		body(body)
	{}

	Node* start_value;
	Node* end_value;
	Node* step;
	Node* body;
};

class WhileStatementNode : public Node {
public:
	WhileStatementNode(Token* token, Node* condition, Node* body) : 
		Node(
			token,
			nullptr,
			nullptr,
			Type::WHILE_STATEMENT,
			condition->token->start,
			body->token->end
		),
		condition(condition),
		body(body)
	{}

	~WhileStatementNode() {
		delete condition;
		delete body;
	}

	Node* condition;
	Node* body;
};

class FunctionDefinitionNode : public Node {
public:
	FunctionDefinitionNode(
		const std::vector<Token*>& args_names,
		Node* body,
		Token* token = nullptr
	) :
		Node(token, nullptr, nullptr, Type::FN_DEFINITION),
		args_names(args_names),
		body(body)
	{
		if (token != nullptr)
			start = token->start;
		else if (args_names.size() > 0)
			start = args_names.at(0)->start;
		else
			start = body->start;

		end = body->end;
	}

	std::vector<Token*> args_names;
	Node* body;

	~FunctionDefinitionNode() {
		delete body;
	}
};

class FunctionCallNode : public Node {
public:
	FunctionCallNode(
		Token* token,
		Node* callee,
		const std::vector<Node*>& args_nodes
	) :
		Node(token, nullptr, nullptr, Type::FN_CALL),
		callee(callee),
		args_nodes(args_nodes)
	{
		if (args_nodes.size() > 0)
			end = args_nodes.at(args_nodes.size() - 1)->end;
		else
			end = callee->end;
	}

	~FunctionCallNode() {
		delete callee;
	}

	Node* callee;
	std::vector<Node*> args_nodes;
};

class ArrayNode : public Node {
public:
	ArrayNode(Token* token, const std::vector<Node*>& elements) :
		Node(token, nullptr, nullptr, Type::ARRAY),
		elements(elements)
	{}

	std::vector<Node*> elements;
};

class PropertyAccessNode : public Node {
public:
	PropertyAccessNode(Token* token, const std::string& var_name) :
		Node(token, nullptr, nullptr, Type::PROPERTY_ACCESS),
		var_name(var_name)
	{}

	std::string var_name;
};
