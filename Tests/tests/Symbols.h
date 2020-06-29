#pragma once

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