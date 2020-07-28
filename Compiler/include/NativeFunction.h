#pragma once

#include "BaseFunction.h"

class NativeFunction : public BaseFunction {
public:
	NativeFunction(
		const std::string& name,
		Node* body,
		const std::vector<std::string>& args_names,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	RuntimeResult* execute(const std::vector<Type*>& args, Context* context) override;

	RuntimeResult* fn_print(Context* ctx);
	RuntimeResult* fn_str(Context* ctx);
	RuntimeResult* fn_sizeof(Context* ctx);
	RuntimeResult* fn_typeof(Context* ctx);
	RuntimeResult* fn_char_at(Context* ctx);

	RuntimeResult* fn_abs(Context* ctx);
	RuntimeResult* fn_acos(Context* ctx);
	RuntimeResult* fn_acosh(Context* ctx);
	RuntimeResult* fn_asin(Context* ctx);
	RuntimeResult* fn_asinh(Context* ctx);
	RuntimeResult* fn_atan(Context* ctx);
	RuntimeResult* fn_atan2(Context* ctx);
	RuntimeResult* fn_atanh(Context* ctx);
	RuntimeResult* fn_cbrt(Context* ctx);
	RuntimeResult* fn_ceil(Context* ctx);
	RuntimeResult* fn_cos(Context* ctx);
	RuntimeResult* fn_cosh(Context* ctx);
	RuntimeResult* fn_exp(Context* ctx);
	RuntimeResult* fn_floor(Context* ctx);
	RuntimeResult* fn_log(Context* ctx);
	RuntimeResult* fn_max(Context* ctx);
	RuntimeResult* fn_min(Context* ctx);
	RuntimeResult* fn_pow(Context* ctx);
	RuntimeResult* fn_random(Context* ctx);
	RuntimeResult* fn_round(Context* ctx);
	RuntimeResult* fn_sin(Context* ctx);
	RuntimeResult* fn_sinh(Context* ctx);
	RuntimeResult* fn_sqrt(Context* ctx);
	RuntimeResult* fn_tan(Context* ctx);
	RuntimeResult* fn_tanh(Context* ctx);
	RuntimeResult* fn_trunc(Context* ctx);
};
