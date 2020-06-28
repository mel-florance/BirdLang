#include <iostream>
#include <string>

#include <BirdLang.h>

int main() {
	std::cout << R"(
       _________
      /_  ___   \
     /  \/   \   \
     \@_/\@__/   /
      \_\/______/
      /     /\\\\\
     |     |\\\\\\
      \      \\\\\\
       \______/\\\\
 _______ ||_||_______
(______(((_(((______(@)
|                     |
|      BIRD LANG      |
|     INTERPRETER     |
|_____________________|
)";
	Compiler compiler;
	std::pair<Token::Type, std::string> p = {Token::Type::EQ, ""};

	while (true)
	{
		std::cout << "\n" << "> ";
		std::string input;
		std::getline(std::cin >> std::ws, input);

		if (input == "clear") {
#ifdef PLATFORM_WINDOWS
			system("cls");
#endif
#ifdef PLATFORM_LINUX
			system("clear");
#endif
			continue;
		}

		compiler.interpret(input);
	}

	return 0;
}