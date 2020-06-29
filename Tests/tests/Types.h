#pragma once

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
	EXPECT_EQ((float)std::get<float>(r.first->value), 33.3333f);
}

TEST(Number, OperationPower) {
	Number a(6);
	Number b(4);

	auto r = a.power(&b);
	EXPECT_EQ(std::get<float>(r.first->value), 1296);
}
