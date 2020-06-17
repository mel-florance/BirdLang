#include <vector>
#include <functional>
#include <algorithm>

#include "Token.h"
#include "Nodes.h"

class Parser {
public:
	Parser(const std::vector<Token*>& tokens);

	Node* parse();
	void traverse(Node* node, unsigned int depth);
	Token* advance();
	Node* factor();
	Node* term();
	Node* expr();
	Node* binary_operation(std::function<Node*()> fn, const std::vector<Token::Type>& operations);

	std::vector<Token*> tokens;
	Token* current_token;
	size_t index;
	bool debug;
};