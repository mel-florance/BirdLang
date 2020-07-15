#pragma once

TEST(Interpreter, VisitNumericNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitBinaryNode) {
	auto a = new Token(Token::Type::INT, 5);
	auto b = new Token(Token::Type::INT, 3);
	auto first = new NumericNode(a);
	auto second = new NumericNode(b);

	auto c = new Token(Token::Type::PLUS, '+');
	auto add = new BinaryOperationNode(first, c,second);
	auto interp = new Interpreter();
	auto ctx = new Context("<test>");
	auto result = interp->visit_binary_operation_node(add, ctx);
	auto value = std::get<int>(result->value->value);

	EXPECT_EQ(value, 8);
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
