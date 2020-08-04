#include "pch.h"

#include "Compiler.h"
#include "Profiler.h"
#include "Utils.h"
#include "ConsoleTable.h"
#include "NativeFunction.h"

using ConsoleTable = samilton::ConsoleTable;

Compiler::Compiler(
	bool debug_lexer,
	bool debug_parser,
	bool profiling
) :
	debug_lexer(debug_lexer),
	debug_parser(debug_parser),
	profiling(profiling),
	interpreting_time(0.0)
{
	context = new Context("<program>");
	symbols = new Symbols();

	symbols->set("null", Type::Null);
	symbols->set("true", Type::True);
	symbols->set("false", Type::False);

	symbols->set("PI", PI);
	symbols->set("TAU", TAU);
	symbols->set("PHI", 1.618033988749895);
	symbols->set("EULER", 2.718281828459045);
	symbols->set("SQRT1_2", 0.7071067811865476);
	symbols->set("SQRT2", 1.4142135623730951);

	NativeFunction* fn_print = new NativeFunction("print", nullptr, { "value" });
	NativeFunction* fn_str = new NativeFunction("str", nullptr, { "value" });
	NativeFunction* fn_sizeof = new NativeFunction("sizeof", nullptr, { "value" });
	NativeFunction* fn_typeof = new NativeFunction("typeof", nullptr, { "value" });
	NativeFunction* fn_char_at = new NativeFunction("charAt", nullptr, { "string", "index" });

	NativeFunction* fn_open = new NativeFunction("open", nullptr, { "filename", "mode" });

	NativeFunction* fn_abs = new NativeFunction("abs", nullptr, { "value" });
	NativeFunction* fn_acos = new NativeFunction("acos", nullptr, { "value" });
	NativeFunction* fn_acosh = new NativeFunction("acosh", nullptr, { "value" });
	NativeFunction* fn_asin = new NativeFunction("asin", nullptr, { "value" });
	NativeFunction* fn_asinh = new NativeFunction("asinh", nullptr, { "value" });
	NativeFunction* fn_atan = new NativeFunction("atan", nullptr, { "value" });
	NativeFunction* fn_atan2 = new NativeFunction("atan2", nullptr, { "x", "y" });
	NativeFunction* fn_atanh = new NativeFunction("atanh", nullptr, { "value" });
	NativeFunction* fn_cbrt = new NativeFunction("cbrt", nullptr, { "value" });
	NativeFunction* fn_ceil = new NativeFunction("ceil", nullptr, { "value" });
	NativeFunction* fn_cos = new NativeFunction("cos", nullptr, { "value" });
	NativeFunction* fn_cosh = new NativeFunction("cosh", nullptr, { "value" });
	NativeFunction* fn_exp = new NativeFunction("exp", nullptr, { "value" });
	NativeFunction* fn_floor = new NativeFunction("floor", nullptr, { "value" });
	NativeFunction* fn_log = new NativeFunction("log", nullptr, { "value" });
	NativeFunction* fn_max = new NativeFunction("max", nullptr, { "x", "y" });
	NativeFunction* fn_min = new NativeFunction("min", nullptr, { "x", "y" });
	NativeFunction* fn_pow = new NativeFunction("pow", nullptr, { "n", "exp" });
	NativeFunction* fn_random = new NativeFunction("random", nullptr, {});
	NativeFunction* fn_round = new NativeFunction("round", nullptr, { "value" });
	NativeFunction* fn_sin = new NativeFunction("sin", nullptr, { "value" });
	NativeFunction* fn_sinh = new NativeFunction("sinh", nullptr, { "value" });
	NativeFunction* fn_sqrt = new NativeFunction("sqrt", nullptr, { "value" });
	NativeFunction* fn_tan = new NativeFunction("tan", nullptr, { "value" });
	NativeFunction* fn_tanh = new NativeFunction("tanh", nullptr, { "value" });
	NativeFunction* fn_trunc = new NativeFunction("trunc", nullptr, { "value" });

	symbols->set("print", (Function*)fn_print);
	symbols->set("str", (Function*)fn_str);
	symbols->set("sizeof", (Function*)fn_sizeof);
	symbols->set("typeof", (Function*)fn_typeof);
	symbols->set("charAt", (Function*)fn_char_at);

	symbols->set("open", (Function*)fn_open);

	symbols->set("abs", (Function*)fn_abs);
	symbols->set("acos", (Function*)fn_acos);
	symbols->set("acosh", (Function*)fn_acosh);
	symbols->set("asin", (Function*)fn_asin);
	symbols->set("asinh", (Function*)fn_asinh);
	symbols->set("atan", (Function*)fn_atan);
	symbols->set("atan2", (Function*)fn_atan2);
	symbols->set("atanh", (Function*)fn_atanh);
	symbols->set("trunc", (Function*)fn_trunc);
	symbols->set("cbrt", (Function*)fn_cbrt);
	symbols->set("ceil", (Function*)fn_ceil);
	symbols->set("cos", (Function*)fn_cos);
	symbols->set("cosh", (Function*)fn_cosh);
	symbols->set("exp", (Function*)fn_exp);
	symbols->set("floor", (Function*)fn_floor);
	symbols->set("log", (Function*)fn_log);
	symbols->set("max", (Function*)fn_max);
	symbols->set("min", (Function*)fn_min);
	symbols->set("pow", (Function*)fn_pow);
	symbols->set("random", (Function*)fn_random);
	symbols->set("round", (Function*)fn_round);
	symbols->set("sin", (Function*)fn_sin);
	symbols->set("sinh", (Function*)fn_sinh);
	symbols->set("sqrt", (Function*)fn_sqrt);
	symbols->set("tan", (Function*)fn_tan);
	symbols->set("tanh", (Function*)fn_tanh);

	context->symbols = symbols;

	lexer = std::make_unique<Lexer>("<stdin>");
	lexer->debug = debug_lexer;

	parser = std::make_unique<Parser>();
	parser->debug = debug_parser;

	interpreter = std::make_unique<Interpreter>();
	std::cout.precision(std::numeric_limits<double>::max_digits10);
}

void Compiler::interpret(const std::string& input)
{
	auto tokens = lexer->index_tokens(input);
	parser->setTokens(tokens);
	auto ast = parser->parse();

	Profiler profiler;

	if (ast != nullptr) {
		if (ast->error != nullptr) {
			std::cout << ast->error << '\n';
		}
		else {
			if (profiling) {
				profiler.start = clock();
			}

			auto result = interpreter->visit(ast->node, context);

			if (profiling) {
				profiler.end = clock();
				interpreting_time = profiler.getReport();
				printStatistics();
			}

			if (result->error != nullptr) {
				if (strcmp(typeid(*result->error).name(), "class RuntimeError") == 0) {
					RuntimeError* error = static_cast<RuntimeError*>(result->error);

					if (error != nullptr) {
						std::cout << error << '\n';
					}
				}
				else {
					std::cout << result->error << '\n';
				}
			}
			else {
				if (result->value != nullptr) {
					std::cout << result->value << '\n';
				}
			}
		}
	}
}

void Compiler::printStatistics()
{
	std::cout << '\n';
	Utils::title("TIMING", 15, false);

	ConsoleTable table(1, 2);
	ConsoleTable::TableChars chars;

	chars.topLeft = '+';
	chars.topRight = '+';
	chars.downLeft = '+';
	chars.downRight = '+';
	chars.topDownSimple = '-';
	chars.leftRightSimple = '|';
	chars.leftSeparation = '+';
	chars.rightSeparation = '+';
	chars.centreSeparation = '+';
	chars.topSeparation = '+';
	chars.downSeparation = '+';

	table.setTableChars(chars);

	table[0][0] = "Step";
	table[0][1] = "Time";

	table[1][0] = "Tokenization";
	table[1][1] = lexer->lexing_time;

	table[2][0] = "Parsing";
	table[2][1] = parser->parsing_time;

	table[3][0] = "Evaluation";
	table[3][1] = interpreting_time;

	table[4][0] = "TOTAL";
	table[4][1] = lexer->lexing_time + interpreting_time + parser->parsing_time;

	std::cout << table << '\n';
}
