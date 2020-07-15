#pragma once

TEST(Interpreter, VisitNumericNode) {
	auto token_int = new Token(Token::Type::INT, 5);
	auto node_int = new NumericNode(token_int);

	auto token_float = new Token(Token::Type::FLOAT, 3.14f);
	auto node_float = new NumericNode(token_float);

	auto interp = new Interpreter();
	auto ctx = new Context("<test>");

	auto result_int = interp->visit_numeric_node(node_int, ctx);
	auto value_int = std::get<int>(result_int->value->value);
	EXPECT_EQ(value_int, 5);

	auto result_float = interp->visit_numeric_node(node_float, ctx);
	auto value_float = std::get<float>(result_float->value->value);
	EXPECT_EQ(value_float, 3.14f);
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
	auto number_int = new Token(Token::Type::INT, -5);
	auto numeric_node_int = new NumericNode(number_int);
	auto unary_node_int = new UnaryOperationNode(numeric_node_int, number_int);

	auto number_float = new Token(Token::Type::FLOAT, -2.0f);
	auto numeric_node_float = new NumericNode(number_float);
	auto unary_node_float = new UnaryOperationNode(numeric_node_float, number_float);

	auto interp = new Interpreter();
	auto ctx = new Context("<test>");

	auto result_int = interp->visit_unary_operation_node(unary_node_int, ctx);
	auto value_int = std::get<int>(result_int->value->value);
	EXPECT_EQ(value_int, -5);

	auto result_float = interp->visit_unary_operation_node(unary_node_float, ctx);
	auto value_float = std::get<float>(result_float->value->value);
	EXPECT_EQ(value_float, -2.0f);
}

TEST(Interpreter, VisitVariableAccessNode) {
	auto variable_name = new Token(Token::Type::IDENTIFIER, "myvar");
	auto variable_access_node = new VariableAccessNode(variable_name);

	auto interp = new Interpreter();
	auto ctx = new Context("<test>");
	auto symbols = new Symbols();
	symbols->set("myvar", (int)20);
	ctx->symbols = symbols;

	auto result = interp->visit_variable_access_node(variable_access_node, ctx);
	auto value = std::get<int>(result->value->value);

	EXPECT_EQ(value, 20);
}

TEST(Interpreter, VisitVariableAssignNode) {
	auto name = new Token(Token::Type::IDENTIFIER, "myvar");
	auto number = new Token(Token::Type::FLOAT, -51.3f);
	auto number_node = new NumericNode(number);
	auto assign_node = new VariableAssignmentNode(name, number_node);

	auto interp = new Interpreter();
	auto ctx = new Context("<test>");
	auto symbols = new Symbols();
	ctx->symbols = symbols;

	auto result = interp->visit_variable_assignment_node(assign_node, ctx);
	auto value = std::get<float>(result->value->value);

	EXPECT_EQ(value, -51.3f);
}
