#pragma once

#include "Type.h"
#include "Nodes.h"
#include "Context.h"

class RuntimeResult;
class BaseFunction : public Type {
public:
	BaseFunction(
		const std::string& name,
		Node* body,
		const std::vector<std::string>& args_names,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	virtual ~BaseFunction() = default;

	Context* generate_context();

	RuntimeResult* check_arguments(
		const std::vector<std::string>& names,
		const std::vector<Type*>& args
	);

	void populate_arguments(
		const std::vector<std::string>& names,
		const std::vector<Type*>& args,
		Context* ctx
	);

	RuntimeResult* check_and_populate_arguments(
		const std::vector<std::string>& names,
		const std::vector<Type*>& args,
		Context* ctx
	);

	std::string name;
	Node* body;
	std::vector<std::string> args_names;
};
