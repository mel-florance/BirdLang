#pragma once

#include "Nodes.h"
#include "Number.h"
#include "Error.h"

class Interpreter {
public:
	Interpreter();
	
	struct Result {

		Number* record(Result* result) {
			if (result->error != nullptr) {
				error = result->error;
			}

			return result->value;
		}

		Result* success(Number* value) {
			this->value = value;
			return this;
		}

		Result* failure(Error* error) {
			this->error = error;
			return this;
		}

		Number* value = nullptr;
		Error* error = nullptr;
	};
	
	Result* visit(Node* node);
	Result* visit_numeric_node(Node* node);
	Result* visit_binary_operation_node(Node* node);
	Result* visit_unary_operation_node(Node* node);
};