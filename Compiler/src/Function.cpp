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
	Type(),
	body(body),
	args_names(args_names)
{
	this->name = name.size() == 0
		? "<anonymous>"
		: name;
}

Function::~Function()
{
}

RuntimeResult* Function::execute(const std::vector<Type*>& args, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	std::unique_ptr<Interpreter> interpreter = std::make_unique<Interpreter>();

	auto ctx = new Context(name, context, this->start);
	ctx->symbols = new Symbols(ctx->parent->symbols);

	if (args.size() > args_names.size()) {
		return result->failure(new RuntimeError(
			this->start,
			this->end,
			std::to_string(args.size() - args_names.size()) + " too many args passed into" + name,
			context
		));
	}

	if (args.size() < args_names.size()) {
		return result->failure(new RuntimeError(
			this->start,
			this->end,
			std::to_string(args_names.size() - args.size()) + " too few args passed into" + name,
			context
		));
	}

	for (unsigned int i = 0; i < args.size(); ++i) {
		auto name = args_names.at(i);
		auto value = args.at(i);
		value->context = ctx;
		ctx->symbols->set(name, value->value);
	}

	auto body_visit = interpreter->visit(body, ctx);
	auto result_value = result->record(body_visit);

	if (result->error != nullptr)
		return result;

	return result->success(result_value);
}
