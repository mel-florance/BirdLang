#include "pch.h"

////////////////////////////////////////////////////////////////////////////
//
//	LEXER
//
////////////////////////////////////////////////////////////////////////////

TEST(Lexer, SetLexerFilename) {
	Lexer lexer("test");
	EXPECT_STREQ("test", lexer.filename.c_str());
}

TEST(Lexer, AdvanceCursor) {
	Lexer lexer("test");
	lexer.input = "1";
	lexer.advance();
	EXPECT_STREQ(std::string(1, lexer.current_char).c_str(), "1");
}

TEST(Lexer, RecognizeTokenTypes) {
	Lexer lexer("test");
	lexer.input = "1";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("INT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeFloat) {
	Lexer lexer("test");
	lexer.input = "1.1";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("FLOAT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizePlus) {
	Lexer lexer("test");
	lexer.input = "+";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("PLUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeMinus) {
	Lexer lexer("test");
	lexer.input = "-";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("MINUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizePow) {
	Lexer lexer("test");
	lexer.input = "^";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("POW", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeMultiply) {
	Lexer lexer("test");
	lexer.input = "*";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("MUL", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeDivide) {
	Lexer lexer("test");
	lexer.input = "/";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("DIV", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeLeftParenthese) {
	Lexer lexer("test");
	lexer.input = "(";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("LPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeRightParentheses) {
	Lexer lexer("test");
	lexer.input = ")";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("RPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeVarIdentifiers) {
	Lexer lexer("test");
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("IDENTIFIER", Token::toString(tokens.at(1)->type).c_str());
}

TEST(Lexer, RecognizeVarKeyWord) {
	Lexer lexer("test");
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("KEYWORD", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeEquals) {
	Lexer lexer("test");
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("EQ", Token::toString(tokens.at(2)->type).c_str());
}

TEST(Lexer, RecognizeEOT) {
	Lexer lexer("test");
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("EOT", Token::toString(tokens.at(4)->type).c_str());
}


////////////////////////////////////////////////////////////////////////////
//
//	PARSER
//
////////////////////////////////////////////////////////////////////////////

TEST(Parser, AdvanceCursor) {
	Lexer lexer("test");
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();

	Parser parser(tokens);
	parser.advance();
	EXPECT_STREQ("IDENTIFIER", Token::toString(parser.current_token->type).c_str());
}

TEST(Parser, CreateFactor) {
	EXPECT_EQ(0, 1);
}

TEST(Parser, CreateExpr) {
	EXPECT_EQ(0, 1);
}

TEST(Parser, CreateAtom) {
	EXPECT_EQ(0, 1);
}

////////////////////////////////////////////////////////////////////////////
//
//	INTERPRETER
//
////////////////////////////////////////////////////////////////////////////

TEST(Interpreter, VisitNumericNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitBinaryNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitUnaryNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitVariableAccessNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitVariableAssignNode) {
	EXPECT_EQ(0, 1);
}

////////////////////////////////////////////////////////////////////////////
//
//	SYMBOLS
//
////////////////////////////////////////////////////////////////////////////

TEST(Symbols, SetSymbol) {
	Symbols symbols;
	symbols.set("test", 2);
	EXPECT_EQ(std::get<int>(symbols.symbols["test"]), 2);
}

TEST(Symbols, RemoveSymbol) {
	Symbols symbols;
	symbols.set("test", 2);
	symbols.remove("test");
	EXPECT_TRUE(symbols.get("test") == symbols.symbols.end());
}

TEST(Symbols, GetSymbol) {
	Symbols symbols;
	symbols.set("test", 2);
	EXPECT_EQ(std::get<int>(symbols.get("test")->second), 2);
}

////////////////////////////////////////////////////////////////////////////
//
//	CONTEXT
//
////////////////////////////////////////////////////////////////////////////

TEST(Context, GetContextName) {
	Context context("test");
	EXPECT_STREQ(context.display_name.c_str(), "test");
}

////////////////////////////////////////////////////////////////////////////
//
//	TYPES
//
////////////////////////////////////////////////////////////////////////////

TEST(Number, OperationAdd) {
	Number a(1);
	Number b(1);

	auto r = a.add(&b);

	EXPECT_EQ(std::get<int>(r.first->value), 2);
}

TEST(Number, OperationSubtract) {
	Number a(1);
	Number b(1);

	auto r = a.subtract(&b);

	EXPECT_EQ(std::get<int>(r.first->value), 0);
}

TEST(Number, OperationMultiply) {
	Number a(6);
	Number b(7);

	auto r = a.multiply(&b);

	EXPECT_EQ(std::get<int>(r.first->value), 42);
}

TEST(Number, OperationDivide) {
	Number a(100);
	Number b(3);

	auto r = a.divide(&b);

	EXPECT_EQ(std::get<int>(r.first->value), 33);
}

TEST(Number, OperationPower) {
	Number a(6);
	Number b(4);

	auto r = a.power(&b);
	EXPECT_EQ(std::get<float>(r.first->value), 1296);
}