#pragma once

#include "Nodes.h"
#include "Number.h"
#include "Error.h"
#include "Context.h"
#include "Platform.h"

class API Interpreter {
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
	
	Result* visit(Node* node, Context* context);
	Result* visit_numeric_node(Node* node, Context* context);
	Result* visit_binary_operation_node(Node* node, Context* context);
	Result* visit_unary_operation_node(Node* node, Context* context);
	Result* visit_variable_access_node(Node* node, Context* context);
	Result* visit_variable_assignment_node(Node* node, Context* context);
};