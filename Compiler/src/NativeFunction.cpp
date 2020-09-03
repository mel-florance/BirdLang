#include "pch.h"
#include "NativeFunction.h"
#include "Interpreter.h"
#include "Str.h"
#include "File.h"
#include "Array.h"
#include "Map.h"
#include "Function.h"
#include "Utils.h"

NativeFunction::NativeFunctionList NativeFunction::list = {
	{"str", {{ "value" }, &NativeFunction::fn_str}},
	{"bool", {{ "value" }, &NativeFunction::fn_bool}},
	{"int", {{ "value" }, &NativeFunction::fn_int}},
	{"float", {{ "value" }, &NativeFunction::fn_float}},

	{"keys", {{ "value"}, &NativeFunction::fn_keys} },
	{"values", {{ "value"}, &NativeFunction::fn_values}},

	{"print", {{ "value" }, &NativeFunction::fn_print}},
	{"sizeof", {{ "value" }, &NativeFunction::fn_sizeof}},
	{"hsize", {{ "value" }, &NativeFunction::fn_hsize}},
	{"typeof", {{ "value" }, &NativeFunction::fn_typeof}},
	{"chr", {{ "string", "index" }, &NativeFunction::fn_chr}},

	{"exec", {{ "command" }, &NativeFunction::fn_exec}},
	{"open", {{ "filename", "mode?" }, &NativeFunction::fn_open}},

	{"bin", {{ "value" }, &NativeFunction::fn_bin}},
	{"hex", {{ "value" }, &NativeFunction::fn_hex}},
	{"dec", {{ "value" }, &NativeFunction::fn_dec}},
	{"bin", {{ "value" }, &NativeFunction::fn_bin}},
	{"oct", {{ "value" }, &NativeFunction::fn_oct}},

	{"abs", {{ "value" }, &NativeFunction::fn_abs}},
	{"acos", {{ "value" }, &NativeFunction::fn_acos}},
	{"acosh", {{ "value" }, &NativeFunction::fn_acosh}},
	{"asin", {{ "value" }, &NativeFunction::fn_asin}},
	{"asinh", {{ "value" }, &NativeFunction::fn_asinh}},
	{"atan", {{ "value" }, &NativeFunction::fn_atan}},
	{"atan2", {{ "x", "y" }, &NativeFunction::fn_atan2}},
	{"atanh", {{ "value" }, &NativeFunction::fn_atanh}},
	{"cbrt", {{ "value" }, &NativeFunction::fn_cbrt}},
	{"ceil", {{ "value" }, &NativeFunction::fn_ceil}},
	{"cos", {{ "value" }, &NativeFunction::fn_cos}},
	{"cosh", {{ "value" }, &NativeFunction::fn_cosh}},
	{"exp", {{ "value" }, &NativeFunction::fn_exp}},
	{"floor", {{ "value" }, &NativeFunction::fn_floor}},
	{"log", {{ "value" }, &NativeFunction::fn_log}},
	{"max", {{ "x", "y" }, &NativeFunction::fn_max}},
	{"min", {{ "x", "y" }, &NativeFunction::fn_min}},
	{"pow", {{ "n", "exp" }, &NativeFunction::fn_exp}},
	{"random", {{}, &NativeFunction::fn_random}},
	{"round", {{ "value" }, &NativeFunction::fn_round}},
	{"sin", {{ "value" }, &NativeFunction::fn_sin}},
	{"sinh", {{ "value" }, &NativeFunction::fn_sinh}},
	{"sqrt", {{ "value" }, &NativeFunction::fn_sqrt}},
	{"tan", {{ "value" }, &NativeFunction::fn_tan}},
	{"tanh", {{ "value" }, &NativeFunction::fn_tanh}},
	{"trunc", {{ "value" }, &NativeFunction::fn_trunc}}
};

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
	auto it = NativeFunction::list.begin();

	for (; it != NativeFunction::list.end(); ++it) {
		if (name == it->first) {
			args_names = std::get<0>(it->second);
			return_value = result->record(std::get<1>(it->second)(context));
		}
	}

	if (result->error != nullptr)
		return result;

	return result->success(return_value);
}

RuntimeResult* NativeFunction::fn_print(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { std::cout << std::get<double>(value) << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { std::cout << std::get<int>(value) << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::BOOL:
		try { std::cout << (std::get<bool>(value) ? "true" : "false") << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::FUNCTION:
		try { std::cout << std::get<Function*>(value) << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::STRING:
		try { std::cout << std::get<std::string>(value) << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::FILE:
		try { std::cout << std::get<std::string>(std::get<File*>(value)->value) << '\n'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::MAP:
		try {
			auto map = std::get<std::map<std::string, Type*>>(value);
			auto instance = new Map(map);

			std::cout << instance << std::endl;
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(nullptr);
}

RuntimeResult* NativeFunction::fn_str(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto string = new String();

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { string->value = std::to_string(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { string->value = std::to_string(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::BOOL:
		try { string->value = (std::get<bool>(value) ? "true" : "false"); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::FUNCTION:
		try { string->value = "<function " + (std::get<Function*>(value)->name) + '>'; }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::STRING:	
		try { string->value = std::get<std::string>(value); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(string);
}

RuntimeResult* NativeFunction::fn_bool(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto boolean = new Number();

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { boolean->value = (bool)(std::get<double>(value) != 0.0); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { boolean->value = (bool)(std::get<int>(value) != 0); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::STRING:
		try {
			auto n = std::get<std::string>(value);
			boolean->value = !n.empty() && (strcmp(n.c_str(), "true") == 0 || atoi(n.c_str()) != 0);
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(boolean);
}

RuntimeResult* NativeFunction::fn_int(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto integer = new Number();

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { integer->value = (int)std::get<double>(value);  }
		catch (const std::bad_variant_access&) {}
	case Type::Native::INT:
		try { integer->value = std::get<int>(value); }
		catch (const std::bad_variant_access&) {}
	case Type::Native::STRING:
		try { integer->value = std::stoi(std::get<std::string>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(integer);
}

RuntimeResult* NativeFunction::fn_float(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number();

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = std::get<double>(value); }
		catch (const std::bad_variant_access&) {}
	case Type::Native::INT:
		try { number->value = (double)std::get<int>(value); }
		catch (const std::bad_variant_access&) {}
	case Type::Native::STRING:
		try { number->value = std::stod(std::get<std::string>(value)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_keys(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto result_value = new Array();
	std::vector<Type*> keys = {};

	switch (value.index()) {
	case Type::Native::ARRAY:
		try {
			auto array = std::get<std::vector<Type*>>(value);
			auto it = array.begin();

			for (int i = 0; it != array.end(); ++it, i++)
				keys.push_back(new Number(i));
		}
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::MAP:
		try {
			auto map = std::get<std::map<std::string, Type*>>(value);
			auto it = map.begin();

			for (; it != map.end(); ++it)
				keys.push_back(new String(it->first));
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	result_value->value = keys;

	return result->success(result_value);
}

RuntimeResult* NativeFunction::fn_values(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto result_value = new Array();
	std::vector<Type*> values = {};

	switch (value.index()) {
	case Type::Native::ARRAY: {
		try {
			auto array = std::get<std::vector<Type*>>(value);
			auto array_it = array.begin();

			for (; array_it != array.end(); ++array_it) {
				values.push_back(*array_it);
			}
		}
		catch (const std::bad_variant_access&) {}
	}
	break;
	case Type::Native::MAP: {
		try {
			auto map = std::get<std::map<std::string, Type*>>(value);
			auto map_it = map.begin();

			for (; map_it != map.end(); ++map_it)
				values.push_back(map_it->second);
		}
		catch (const std::bad_variant_access&) {}
	}
	break;
	}
	result_value->value = values;

	return result->success(result_value);
}

RuntimeResult* NativeFunction::fn_sizeof(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto size = new Number();

	switch (value.index()) {
	case Type::Native::STRING:
		try { size->value = (int)std::get<std::string>(value).size(); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::ARRAY:
		try { size->value = (int)std::get<std::vector<Type*>>(value).size(); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::MAP:
		try { size->value = (int)std::get<std::map<std::string, Type*>>(value).size(); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::FILE:
		try { size->value = (int)std::get<File*>(value)->size; }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(size);
}

RuntimeResult* NativeFunction::fn_hsize(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto size = new String();

	switch (value.index()) {
	case Type::Native::STRING:
		try { size->value = Utils::bytesToSize((int)std::get<std::string>(value).length()); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::ARRAY:
		try { size->value = Utils::bytesToSize(sizeof std::get<std::vector<Type*>>(value).size()); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::FILE:
		try { size->value = Utils::bytesToSize((int)std::get<File*>(value)->size); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(size);
}

RuntimeResult* NativeFunction::fn_typeof(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto string = new String();

	switch (value.index()) {
	case Type::Native::DOUBLE   : string->value = std::string("float"); break;
	case Type::Native::INT	    : string->value = std::string("integer"); break;
	case Type::Native::BOOL     : string->value = std::string("boolean"); break;
	case Type::Native::FUNCTION : string->value = std::string("function"); break;
	case Type::Native::STRING   : string->value = std::string("string"); break;
	case Type::Native::ARRAY    : string->value = std::string("array"); break;
	case Type::Native::FILE		: string->value = std::string("file"); break;
	case Type::Native::MAP		: string->value = std::string("map"); break;
	}

	return result->success(string);
}

RuntimeResult* NativeFunction::fn_chr(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto string = ctx->symbols->get("string")->second;
	auto index = ctx->symbols->get("index")->second;
	auto character = new String();

	if (string.index() == Type::Native::STRING && index.index() == Type::Native::INT) {
		auto id = std::get<int>(index);
		auto str = std::get<std::string>(string);

		if (id >= 0 && id < str.size()) {
			try { character->value = std::string(1, str.at(id)); }
			catch (const std::bad_variant_access&) {}
		}
	}

	return result->success(character);
}

RuntimeResult* NativeFunction::fn_exec(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto command = ctx->symbols->get("command")->second;
	auto return_value = new Number();

	if (command.index() == Type::Native::STRING) {
		return_value->value = system(std::get<std::string>(command).c_str());
	}

	return result->success(return_value);
}

RuntimeResult* NativeFunction::fn_open(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto arg_filename = ctx->symbols->get("filename")->second;
	auto arg_mode = ctx->symbols->get("mode")->second;
	std::string arg_mode_str;

	try { arg_mode_str = std::get<std::string>(arg_mode); }
	catch (const std::bad_variant_access&) {}

	if (arg_mode_str.find("?") != std::string::npos)
		arg_mode = "r";

	int read_mode = 0;
	auto out = new Type();

	std::string mode_value;
	try { mode_value = std::get<std::string>(arg_mode); }
	catch (const std::bad_variant_access&) {}

	if (mode_value == "r")
		read_mode = std::ifstream::in;
	else if (mode_value == "w")
		read_mode = std::ifstream::out;
	else if (mode_value == "rw")
		read_mode = read_mode = std::ifstream::in | std::ifstream::out;
	else if (mode_value == "b")
		read_mode = std::ifstream::binary;
	else if (mode_value == "e")
		read_mode = std::ifstream::ate;
	else if (mode_value == "a")
		read_mode = std::ifstream::app;
	else if (mode_value == "t")
		read_mode = std::ifstream::trunc;

	std::string filename_value;
	try { filename_value = std::get<std::string>(arg_filename); }
	catch (const std::bad_variant_access&) {}

	std::ifstream stream;
	stream.open(filename_value, std::ifstream::in);

	if (!stream) {
		return result->failure(new RuntimeError(nullptr, nullptr, "Unable to open file: " + filename_value, nullptr));
	}

	stream.seekg(0, std::ios::end);
	std::streampos length = stream.tellg();
	stream.seekg(0, std::ios::beg);

	std::vector<char> buffer(length);
	stream.read(&buffer[0], length);

	std::string str(&buffer[0]);
	auto file = new File();

	file->closed = false;
	file->name = filename_value;
	file->value = str.substr(0, str.size());
	file->mode = static_cast<File::Mode>(read_mode);
	file->size = std::filesystem::file_size(filename_value);

	out->value = file;

	return result->success(out);
}

RuntimeResult* NativeFunction::fn_bin(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto string = new String();

	switch (value.index()) {
	case Type::Native::INT:
		try {
			unsigned long n = (unsigned long)std::get<int>(value);
			char buffer[(sizeof(unsigned long) * 8) + 1];
			unsigned index = sizeof(unsigned long) * 8;
			char temp = 0;
			buffer[index] = '\0';

			do {
				temp = (n & 1);
				temp = temp + '0';
				buffer[--index] = temp;
			} while (n >>= 1);

			string->value = std::string(buffer + index);
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(string);
}

RuntimeResult* NativeFunction::fn_hex(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto string = new String();

	switch (value.index()) {
	case Type::Native::INT:
		try {
			std::stringstream stream;
			stream << "0x"
				<< std::setfill('0') << std::setw(sizeof(int) * 2)
				<< std::hex << std::get<int>(value);
			string->value = stream.str();
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(string);
}

RuntimeResult* NativeFunction::fn_dec(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number();

	switch (value.index()) {
	case Type::Native::INT:
		try {
			auto n = std::get<int>(value);
			int d = 0, i = 0, r;

			while (n != 0)
			{
				r = n % 10;
				n /= 10;
				d += r * pow(8, i);
				++i;
			}
			number->value = d;
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_oct(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number();

	switch (value.index()) {
	case Type::Native::INT:
		try {
			auto n = std::get<int>(value);
			int o = 0, p = 1;

			while (n != 0) {
				o += (n % 8) * p;
				n /= 8;
				p *= 10;
			}

			number->value = o;
		}
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_abs(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = abs(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = abs(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_acos(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = acos(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = acos(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_acosh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = acosh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = acosh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_asin(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = asin(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = asin(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_asinh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = asinh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = asinh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_atan(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = atan(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = atan(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_atan2(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto y = ctx->symbols->get("y")->second;
	auto x = ctx->symbols->get("x")->second;
	auto number = new Number(0);

	if (x.index() == Type::Native::DOUBLE && y.index() == Type::Native::DOUBLE) {
		try { number->value = atan2(std::get<double>(y), std::get<double>(x)); }
		catch (const std::bad_variant_access&) {}
	}
	else if (x.index() == Type::Native::INT && y.index() == Type::Native::INT) {
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
	case Type::Native::DOUBLE:
		try { number->value = atanh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = atanh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_cbrt(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = cbrt(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = cbrt(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_ceil(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = ceil(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = ceil(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_cos(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;

	auto cosine_result = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { cosine_result->value = cos(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { cosine_result->value = cos(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(cosine_result);
}

RuntimeResult* NativeFunction::fn_cosh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = cosh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = cosh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_exp(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = exp(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = exp(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_floor(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = floor(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = floor(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_log(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = log(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = log(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_max(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto x = ctx->symbols->get("x")->second;
	auto y = ctx->symbols->get("y")->second;
	auto max = new Number();

	if (x.index() == Type::Native::INT && y.index() == Type::Native::INT) {
		try { max->value = std::max(std::get<int>(x), std::get<int>(y)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(max);
}

RuntimeResult* NativeFunction::fn_min(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto x = ctx->symbols->get("x")->second;
	auto y = ctx->symbols->get("y")->second;
	auto min = new Number();

	if (x.index() == Type::Native::INT && y.index() == Type::Native::INT) {
		try { min->value = std::min(std::get<int>(x), std::get<int>(y)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(min);
}

RuntimeResult* NativeFunction::fn_pow(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto n = ctx->symbols->get("n")->second;
	auto exp = ctx->symbols->get("exp")->second;
	auto number = new Number(0);

	if (n.index() == Type::Native::INT && exp.index() == Type::Native::INT) {
		try { number->value = pow(std::get<int>(n), std::get<int>(exp)); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_random(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto number = new Number(0);
	number->value = rand();

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_round(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = round(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = round(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sin(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = sin(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = sin(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sinh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = sinh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = sinh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_sqrt(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = sqrt(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = sqrt(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_tan(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = tan(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = tan(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_tanh(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = tanh(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = tanh(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}

RuntimeResult* NativeFunction::fn_trunc(Context* ctx)
{
	RuntimeResult* result = new RuntimeResult();
	auto value = ctx->symbols->get("value")->second;
	auto number = new Number(0);

	switch (value.index()) {
	case Type::Native::DOUBLE:
		try { number->value = trunc(std::get<double>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	case Type::Native::INT:
		try { number->value = trunc(std::get<int>(value)); }
		catch (const std::bad_variant_access&) {}
		break;
	}

	return result->success(number);
}
