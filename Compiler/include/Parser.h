#include "Token.h"
#include "Nodes.h"
#include "Error.h"
#include "Platform.h"

#include <functional>

class Parser {
public:
	Parser();

	struct Result {
		template<typename T>
		Node* record(T* result) {
			if (result->error != nullptr) {
				error = result->error;
			}

			return result->node;
		}

		void record_advance() {
			count++;
		}

		Result* success(Node* node) {
			this->node = node;
			return this;
		}

		Result* failure(Error* error) {
			this->error = error;
			return this;
		}

		Error* error = nullptr;
		Node* node = nullptr;
		unsigned int count = 0;
	};

	Result* parse();
	void traverse(Node* node, const std::string& prefix = "", bool isLeft = false);
	Token* advance();
	Result* factor();
	Result* term();
	Result* expr();
	Result* if_expr();
	Result* for_expr();
	Result* while_expr();
	Result* atom();
	Result* arithm();
	Result* power();
	Result* compare();
	Result* function_definition();
	Result* function_call();
	Result* array_expr();
	Result* map_expr();
	
	Result* binary_operation(
		std::function<Result*()> fna,
		const std::vector<std::variant<Token::Type, std::pair<Token::Type, std::string>>>& operations,
		std::function<Result* ()> fnb = nullptr
	);

	inline void setTokens(const std::vector<Token*>& tokens) {
		this->tokens = tokens;
	}

	std::vector<Token*> tokens;
	Token* current_token;
	size_t index;
	bool debug;
	double parsing_time;
};
