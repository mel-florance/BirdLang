#pragma once

#include "Nodes.h"
#include "Type.h"
#include "Number.h"
#include "Error.h"
#include "Context.h"
#include "Platform.h"

class RuntimeResult {
public:
	RuntimeResult() = default;

	Type* record(RuntimeResult* result) {
		if (result->error != nullptr) {
			error = result->error;
		}

		return result->value;
	}

	RuntimeResult* success(Type* value) {
		this->value = value;
		return this;
	}

	RuntimeResult* failure(Error* error) {
		this->error = error;
		return this;
	}

	Type* value = nullptr;
	Error* error = nullptr;
};

class Interpreter {
public:
	Interpreter();
	
	RuntimeResult* visit(Node* node, Context* context);
	RuntimeResult* visit_numeric_node(Node* node, Context* context);
	RuntimeResult* visit_binary_operation_node(Node* node, Context* context);
	RuntimeResult* visit_unary_operation_node(Node* node, Context* context);
	RuntimeResult* visit_variable_access_node(Node* node, Context* context);
	RuntimeResult* visit_variable_assignment_node(Node* node, Context* context);
	RuntimeResult* visit_if_statement_node(Node* node, Context* context);
	RuntimeResult* visit_for_statement_node(Node* node, Context* context);
	RuntimeResult* visit_while_statement_node(Node* node, Context* context);
	RuntimeResult* visit_function_definition_node(Node* node, Context* context);
	RuntimeResult* visit_function_call_node(Node* node, Context* context);
	RuntimeResult* visit_string_node(Node* node, Context* context);
	RuntimeResult* visit_array_node(Node* node, Context* context);
	RuntimeResult* visit_property_access_node(Node* node, Context* context);
	RuntimeResult* visit_index_access_node(Node* node, Context* context);
	RuntimeResult* visit_map_node(Node* node, Context* context);

	Type* find_map_recursive(
		const std::string& needle,
		Context* context,
		const std::vector<Token*>& path,
		const std::map<std::string, Type*>& object
	);
};
