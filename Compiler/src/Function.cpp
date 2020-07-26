#include "pch.h"
#include "Function.h"

Function::Function(
	const std::string& name,
	Node* body,
	const std::vector<std::string>& args_names,
	std::shared_ptr<Cursor> start,
	std::shared_ptr<Cursor> end,
	Context* context
) :
	BaseFunction(name, body, args_names, start, end, context)
{
	this->name = name;
	this->body = body;
	this->args_names = args_names;
	this->start = start;
	this->end = end;
	this->context = context;
}

RuntimeResult* Function::execute(const std::vector<Type*>& args, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	Interpreter* interpreter = new Interpreter();

	auto scope = generate_context();
	result->record(check_and_populate_arguments(this->args_names, args, scope));

	if (result->error != nullptr)
		return result;

	auto body_visit = interpreter->visit(body, scope);
	auto result_value = result->record(body_visit);

	delete body_visit;
	delete scope->symbols;
	delete scope;
	delete interpreter;

	if (result->error != nullptr)
		return result;

	return result->success(result_value);
}
