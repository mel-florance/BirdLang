#pragma once

#include <string>
#include <vector>
#include "Token.h"
#include "Nodes.h"
#include "Context.h"
#include "Interpreter.h"
#include "Type.h"
#include "BaseFunction.h"

class Function : public BaseFunction {
public:
	Function(
		const std::string& name,
		Node* body,
		const std::vector<std::string>& args_names,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	RuntimeResult* execute(const std::vector<Type*>& args, Context* context) override;

	std::string name;
	Node* body;
	std::vector<std::string> args_names;
};
