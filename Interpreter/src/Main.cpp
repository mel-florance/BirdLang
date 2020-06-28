#include <iostream>
#include <string>

#include <BirdLang.h>

int main(int argc, char** argv) {
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

	for (int i = 0; i < argc; ++i) {
		if (strstr(argv[i], "-") == argv[i]) {
			for (unsigned int j = 1; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 'l') {
					compiler.debug_lexer = true;
				}
				else if (argv[i][j] == 'p') {
					compiler.debug_parser = true;
				}
			}
		}
	}

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