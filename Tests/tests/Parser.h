#pragma once

TEST(Parser, AdvanceCursor) {
	Lexer lexer("test");
	auto tokens = lexer.index_tokens("var a = 2");

	Parser parser;
	parser.setTokens(tokens);
	parser.advance();
	parser.advance();
	EXPECT_STREQ("IDENTIFIER", Token::toString(parser.current_token->type).c_str());
}
