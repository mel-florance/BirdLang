#include "pch.h"

#include "tests/Lexer.h"
#include "tests/Parser.h"
#include "tests/Interpreter.h"
#include "tests/Symbols.h"
#include "tests/Context.h"
#include "tests/Types.h"

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}