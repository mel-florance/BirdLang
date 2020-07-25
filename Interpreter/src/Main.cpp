#include <iostream>
#include <string>
#include <cstring>
#include <string.h>
#include <BirdLang.h>

#ifdef PLATFORM_WINDOWS
#include <Windows.h>
#endif

int main(int argc, char** argv) {
#ifdef PLATFORM_WINDOWS
	SetConsoleTitle(L"Bird Lang Interpreter");
#endif

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

	bool debug_lexer = false;
	bool debug_parser = false;
	bool profiling = false;

	for (int i = 0; i < argc; ++i) {
		if (strstr(argv[i], "-") == argv[i]) {
			for (unsigned int j = 1; j < strlen(argv[i]); j++) {
				if (argv[i][j] == 'l')
					debug_lexer = true;
				else if (argv[i][j] == 'p')
					debug_parser = true;
				else if (argv[i][j] == 's')
					profiling = true;
			}
		}
	}

	std::unique_ptr<Compiler> compiler = std::make_unique<Compiler>(
		debug_lexer,
		debug_parser,
		profiling
	);

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

		compiler->interpret(input);
	}

	return 0;
}
