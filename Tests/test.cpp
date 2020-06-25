#include "pch.h"

TEST(Lexer, ItShouldSetTheLexerFilename) {
	Lexer lexer("test");
	EXPECT_STREQ("test", lexer.filename.c_str());
}
TEST(Lexer, ItShouldRecognizeIntTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "1";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("INT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeFloatTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "1.1";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("FLOAT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizePlusTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "+";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("PLUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeMinusTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "-";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("MINUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizePowTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "^";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("POW", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeMultiplyTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "*";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("MUL", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeDivideTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "/";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("DIV", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeLeftParenthesesTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "(";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("LPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeRightParenthesesTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = ")";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("RPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeIdentifiersTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("IDENTIFIER", Token::toString(tokens.at(1)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeVarKeyWordTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("KEYWORD", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeEqualsTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("EQ", Token::toString(tokens.at(2)->type).c_str());
}

TEST(Lexer, ItShouldRecognizeEOTTokenTypes) {
	Lexer lexer("test");
	lexer.debug = true;
	lexer.input = "var a = 2";
	auto tokens = lexer.index_tokens();
	EXPECT_STREQ("EOT", Token::toString(tokens.at(4)->type).c_str());
}