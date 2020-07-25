#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "Nodes.h"
#include "Context.h"
#include "Interpreter.h"
#include "Type.h"


class Function : public Type {
public:
	Function(
		const std::string& name,
		Node* body,
		const std::vector<std::string>& args_names,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);
	~Function();

	RuntimeResult* execute(const std::vector<Type*>& args, Context* context) override;

	std::string name;
	Node* body;
	std::vector<std::string> args_names;
};
