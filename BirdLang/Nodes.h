#pragma once

#include "Token.h"

class Node {
public:
	Node(const Token& token) : 
		token(token) {}
	Token token;

	inline friend std::ostream& operator << (std::ostream& stream, const NumericNode& node) {
		return stream << node.token;
	}
};

class NumericNode : public Node {
public:
	NumericNode(const Token& token) : 
		Node(token) {}
};

class BinaryOperationNode : public Node {
public:
	BinaryOperationNode(const Node& left, const Token& token, const Node& right) : 
		Node(token),
		left(left), 
		right(right) {}

	Node left;
	Node right;
};