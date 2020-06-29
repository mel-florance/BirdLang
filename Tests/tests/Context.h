#pragma once

TEST(Context, GetContextName) {
	Context context("test");
	EXPECT_STREQ(context.display_name.c_str(), "test");
}