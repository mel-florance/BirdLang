#pragma once

#include <map>
#include "BaseFunction.h"

class NativeFunction : public BaseFunction {
public:
	using NativeFunctionPtr = std::function<RuntimeResult*(Context*)>;
	typedef std::unordered_map<std::string, std::pair<std::vector<std::string>, NativeFunctionPtr>> NativeFunctionList;

	NativeFunction(
		const std::string& name,
		Node* body,
		const std::vector<std::string>& args_names,
		std::shared_ptr<Cursor> start = nullptr,
		std::shared_ptr<Cursor> end = nullptr,
		Context* context = nullptr
	);

	RuntimeResult* execute(const std::vector<Type*>& args, Context* context) override;

	static RuntimeResult* fn_str(Context* ctx);
	static RuntimeResult* fn_bool(Context* ctx);
	static RuntimeResult* fn_int(Context* ctx);
	static RuntimeResult* fn_float(Context* ctx);

	static RuntimeResult* fn_keys(Context* ctx);
	static RuntimeResult* fn_values(Context* ctx);

	static RuntimeResult* fn_print(Context* ctx);
	static RuntimeResult* fn_sizeof(Context* ctx);
	static RuntimeResult* fn_hsize(Context* ctx);
	static RuntimeResult* fn_typeof(Context* ctx);
	static RuntimeResult* fn_chr(Context* ctx);

	static RuntimeResult* fn_exec(Context* ctx);
	static RuntimeResult* fn_open(Context* ctx);

	static RuntimeResult* fn_bin(Context* ctx);
	static RuntimeResult* fn_hex(Context* ctx);
	static RuntimeResult* fn_dec(Context* ctx);
	static RuntimeResult* fn_oct(Context* ctx);

	static RuntimeResult* fn_abs(Context* ctx);
	static RuntimeResult* fn_acos(Context* ctx);
	static RuntimeResult* fn_acosh(Context* ctx);
	static RuntimeResult* fn_asin(Context* ctx);
	static RuntimeResult* fn_asinh(Context* ctx);
	static RuntimeResult* fn_atan(Context* ctx);
	static RuntimeResult* fn_atan2(Context* ctx);
	static RuntimeResult* fn_atanh(Context* ctx);
	static RuntimeResult* fn_cbrt(Context* ctx);
	static RuntimeResult* fn_ceil(Context* ctx);
	static RuntimeResult* fn_cos(Context* ctx);
	static RuntimeResult* fn_cosh(Context* ctx);
	static RuntimeResult* fn_exp(Context* ctx);
	static RuntimeResult* fn_floor(Context* ctx);
	static RuntimeResult* fn_log(Context* ctx);
	static RuntimeResult* fn_max(Context* ctx);
	static RuntimeResult* fn_min(Context* ctx);
	static RuntimeResult* fn_pow(Context* ctx);
	static RuntimeResult* fn_random(Context* ctx);
	static RuntimeResult* fn_round(Context* ctx);
	static RuntimeResult* fn_sin(Context* ctx);
	static RuntimeResult* fn_sinh(Context* ctx);
	static RuntimeResult* fn_sqrt(Context* ctx);
	static RuntimeResult* fn_tan(Context* ctx);
	static RuntimeResult* fn_tanh(Context* ctx);
	static RuntimeResult* fn_trunc(Context* ctx);

	static NativeFunctionList list;
};
