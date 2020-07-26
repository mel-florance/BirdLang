#include "pch.h"
#include "NativeFunction.h"
#include "Interpreter.h"

NativeFunction::NativeFunction(
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

RuntimeResult* NativeFunction::execute(const std::vector<Type*>& args, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto ctx = generate_context();

	result->record(check_and_populate_arguments(args_names, args, context));

	if (result->error != nullptr)
		return result;

	Type* return_value = nullptr;

	if (name == "print") {
		args_names = { "value" };
		return_value = result->record(fn_print(context));
	} else if (name == "abs") {
		args_names = { "value" };
		return_value = result->record(fn_abs(context));
	} else if (name == "abs") {
		args_names = { "value" };
		return_value = result->record(fn_abs(context));
	} else if (name == "acos") {
		args_names = { "value" };
		return_value = result->record(fn_acos(context));
	} else if (name == "acosh") {
		args_names = { "value" };
		return_value = result->record(fn_acosh(context));
	} else if (name == "asin") {
		args_names = { "value" };
		return_value = result->record(fn_asin(context));
	} else if (name == "asinh") {
		args_names = { "value" };
		return_value = result->record(fn_asinh(context));
	} else if (name == "atan") {
		args_names = { "value" };
		return_value = result->record(fn_atan(context));
	} else if (name == "atan2") {
		args_names = { "x", "y" };
		return_value = result->record(fn_atan2(context));
	} else if (name == "atanh") {
		args_names = { "value" };
		return_value = result->record(fn_atanh(context));
	} else if (name == "cbrt") {
		args_names = { "value" };
		return_value = result->record(fn_cbrt(context));
	} else if (name == "ceil") {
		args_names = { "value" };
		return_value = result->record(fn_ceil(context));
	} else if (name == "cos") {
		args_names = { "value" };
		return_value = result->record(fn_cos(context));
	} else if (name == "cosh") {
		args_names = { "value" };
		return_value = result->record(fn_cosh(context));
	} else if (name == "exp") {
		args_names = { "value" };
		return_value = result->record(fn_exp(context));
	} else if (name == "floor") {
		args_names = { "value" };
		return_value = result->record(fn_floor(context));
	} else if (name == "log") {
		args_names = { "value" };
		return_value = result->record(fn_log(context));
	} else if (name == "max") {
		args_names = { "x", "y" };
		return_value = result->record(fn_max(context));
	} else if (name == "min") {
		args_names = { "x", "y" };
		return_value = result->record(fn_min(context));
	} else if (name == "pow") {
		args_names = { "n", "exp" };
		return_value = result->record(fn_pow(context));
	} else if (name == "round") {
		args_names = { "value" };
		return_value = result->record(fn_round(context));
	} else if (name == "sin") {
		args_names = { "value" };
		return_value = result->record(fn_sin(context));
	} else if (name == "sinh") {
		args_names = { "value" };
		return_value = result->record(fn_sinh(context));
	} else if (name == "sqrt") {
		args_names = { "value" };
		return_value = result->record(fn_sqrt(context));
	} else if (name == "tan") {
		args_names = { "value" };
		return_value = result->record(fn_tan(context));
	} else if (name == "tanh") {
		args_names = { "value" };
		return_value = result->record(fn_tanh(context));
	} else if (name == "trunc") {
		args_names = { "value" };
		return_value = result->record(fn_trunc(context));
	}

	if (result->error != nullptr)
		return result;

	result->success(return_value);
}

RuntimeResult* NativeFunction::fn_print(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;

	switch (value.index()) {
	case 0:
		try { std::cout << std::get<double>(value); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { std::cout << std::get<int>(value); }
		catch (const std::bad_variant_access&) {}
	case 2:
		try { std::cout << (std::get<bool>(value) ? "true" : "false"); }
		catch (const std::bad_variant_access&) {}
	case 3:
		try { std::cout << std::get<Function*>(value);  }
		catch (const std::bad_variant_access&) {}
	case 4:
		try { std::cout << std::get<std::string>(value); }
		catch (const std::bad_variant_access&) {}
	//case 5:
		//std::cout << new Type(std::get<std::vector<Type*>>(value));
	}

	return result->success(nullptr);
}

RuntimeResult* NativeFunction::fn_abs(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = abs(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = abs(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_acos(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = acos(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = acos(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_acosh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = acosh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = acosh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_asin(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = asin(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = asin(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_asinh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = asinh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = asinh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_atan(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = atan(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = atan(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_atan2(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto y = ctx->symbols->get("y")->second;
	auto x = ctx->symbols->get("x")->second;
	auto number = new Number(0);

	if (x.index() == 0 && y.index() == 0) {
		try { number->value = atan2(std::get<double>(y), std::get<double>(x)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (x.index() == 1 && y.index() == 1) {
		try { number->value = atan2(std::get<int>(y), std::get<int>(x)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_atanh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = atanh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = atanh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_cbrt(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = cbrt(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = cbrt(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_ceil(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = ceil(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = ceil(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_cos(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;

	auto cosine_result = new Number(0);

	switch (value.index()) {
	case 0:
		try { cosine_result->value = cos(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { cosine_result->value = cos(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(cosine_result);
}

RuntimeResult* NativeFunction::fn_cosh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = cosh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = cosh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_exp(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = exp(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = exp(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_floor(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = floor(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = floor(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_log(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = log(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = log(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_max(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_min(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_pow(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto n = ctx->symbols->get("n")->second;
	auto exp = ctx->symbols->get("exp")->second;
	auto number = new Number(0);

	if (n.index() == 1 && exp.index() == 1) {
		try { number->value = pow(std::get<int>(n), std::get<int>(exp)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_random(Context* ctx)
{
	return nullptr;
}

RuntimeResult* NativeFunction::fn_round(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = round(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = round(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sin(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = sin(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = sin(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sinh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = sinh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = sinh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sqrt(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = sqrt(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = sqrt(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_tan(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = tan(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = tan(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_tanh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = tanh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = tanh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_trunc(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case 0:
		try { number->value = trunc(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
	case 1:
		try { number->value = trunc(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}
