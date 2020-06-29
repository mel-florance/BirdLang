#pragma once

#include "pch.h"

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
	auto tokens = lexer.index_tokens("1");
	EXPECT_STREQ("INT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeFloat) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("1.1");
	EXPECT_STREQ("FLOAT", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizePlus) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("+");
	EXPECT_STREQ("PLUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeMinus) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("-");
	EXPECT_STREQ("MINUS", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizePow) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("^");
	EXPECT_STREQ("POW", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeMultiply) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("*");
	EXPECT_STREQ("MUL", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeDivide) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("/");
	EXPECT_STREQ("DIV", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeLeftParenthese) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("(");
	EXPECT_STREQ("LPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeRightParentheses) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens(")");
	EXPECT_STREQ("RPAREN", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeVarIdentifiers) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("var a = 2");
	EXPECT_STREQ("IDENTIFIER", Token::toString(tokens.at(1)->type).c_str());
}

TEST(Lexer, RecognizeVarKeyWord) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("var a = 2");
	EXPECT_STREQ("KEYWORD", Token::toString(tokens.at(0)->type).c_str());
}

TEST(Lexer, RecognizeEquals) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("var a = 2");
	EXPECT_STREQ("EQ", Token::toString(tokens.at(2)->type).c_str());
}

TEST(Lexer, RecognizeEOT) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("var a = 2");
	EXPECT_STREQ("EOT", Token::toString(tokens.at(4)->type).c_str());
}