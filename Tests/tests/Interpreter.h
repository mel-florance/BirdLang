#pragma once

TEST(Interpreter, VisitNumericNode) {
	EXPECT_EQ(0, 1);
}

TEST(Interpreter, VisitBinaryNode) {
	Token a(Token::Type::INT, 5);
	Token b(Token::Type::INT, 3);
	NumericNode first(&a);
	NumericNode second(&b);

	Token c(Token::Type::PLUS, '+');
	BinaryOperationNode add(&first, &c,&second);
	Interpreter interp;
	Context ctx("<test>");
	auto result = interp.visit_binary_operation_node(&add, &ctx);
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
