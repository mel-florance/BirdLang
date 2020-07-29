#include "pch.h"
#include "Interpreter.h"
#include "Function.h"
#include "Str.h"
#include "Array.h"

Interpreter::Interpreter()
{
}

RuntimeResult* Interpreter::visit(Node* node, Context* context)
{
	if (node != nullptr && context != nullptr) {
		BinaryOperationNode* binary_operation_node = dynamic_cast<BinaryOperationNode*>(node);
		if (binary_operation_node)
			return visit_binary_operation_node(binary_operation_node, context);

		NumericNode* numeric_node = dynamic_cast<NumericNode*>(node);
		if (numeric_node)
			return visit_numeric_node(numeric_node, context);

		UnaryOperationNode* unary_operation_node = dynamic_cast<UnaryOperationNode*>(node);
		if (unary_operation_node)
			return visit_unary_operation_node(unary_operation_node, context);

		VariableAccessNode* variable_access_node = dynamic_cast<VariableAccessNode*>(node);
	    if (variable_access_node)
			return visit_variable_access_node(variable_access_node, context);

		VariableAssignmentNode* variable_assignment_node = dynamic_cast<VariableAssignmentNode*>(node);
		if (variable_assignment_node)
			return visit_variable_assignment_node(variable_assignment_node, context);

		IfStatementNode* if_statement_node = dynamic_cast<IfStatementNode*>(node);
		if (if_statement_node)
			return visit_if_statement_node(if_statement_node, context);
		
		ForStatementNode* for_statement_node = dynamic_cast<ForStatementNode*>(node);
		if (for_statement_node)
			return visit_for_statement_node(for_statement_node, context);

		WhileStatementNode* while_statement_node = dynamic_cast<WhileStatementNode*>(node);
		if (while_statement_node)
			return visit_while_statement_node(while_statement_node, context);

		FunctionDefinitionNode* function_definition_node = dynamic_cast<FunctionDefinitionNode*>(node);
		if (function_definition_node)
			return visit_function_definition_node(function_definition_node, context);

		FunctionCallNode* function_call_node = dynamic_cast<FunctionCallNode*>(node);
		if (function_call_node)
			return visit_function_call_node(function_call_node, context);

		StringNode* string_node = dynamic_cast<StringNode*>(node);
		if (string_node)
			return visit_string_node(string_node, context);

		ArrayNode* array_node = dynamic_cast<ArrayNode*>(node);
		if (array_node)
			return visit_array_node(array_node, context);

		PropertyAccessNode* property_access_node = dynamic_cast<PropertyAccessNode*>(node);
		if (property_access_node)
			return visit_property_access_node(property_access_node, context);


		auto type = typeid(*node).name();
		std::cout << "No visit " << type << " method defined." << '\n';
	}

	return nullptr;
}

RuntimeResult* Interpreter::visit_numeric_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	Number* number = new Number();
	number->context = context;
	number->start = node->token->start;
	number->end = node->token->end;

	if (node->token->value.index() == 0) {
		try { number->value = std::get<double>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { number->value = std::get<int>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

RuntimeResult* Interpreter::visit_binary_operation_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	Type* number = nullptr;

	auto left_visit = visit(node->left, context);
	Type* left = (Type*)result->record(left_visit);

	if (result->error != nullptr)
		return result;

	auto right_visit = visit(node->right, context);
	Type* right = (Type*)result->record(right_visit);

	if (result->error != nullptr)
		return result;

	Error* error = nullptr;

	std::string value;
	try { value = std::get<std::string>(node->token->value); }
	catch (const std::bad_variant_access&) {}

	if (node->token->type == Token::Type::PLUS) {
		auto op_result = left->add(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::MINUS) {
		auto op_result = left->subtract(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::MUL) {
		auto op_result = left->multiply(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::MOD) {
		auto op_result = left->modulus(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::DIV) {
		auto op_result = left->divide(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::POW) {
		auto op_result = left->power(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::EE) {
		auto op_result = left->compare_equal(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::NE) {
		auto op_result = left->compare_not_equal(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::LT) {
		auto op_result = left->compare_less_than(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::GT) {
		auto op_result = left->compare_greater_than(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::LTE) {
		auto op_result = left->compare_less_or_equal(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::GTE) {
		auto op_result = left->compare_greater_or_equal(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::KEYWORD && value == "and") {
		auto op_result = left->compare_and(right);
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::KEYWORD && value == "or") {
		auto op_result = left->compare_or(right);
		number = op_result.first;
		error = op_result.second;
	}

	if (error != nullptr) {
		return result->failure(error);
	}

	//delete left_visit;
	//delete right_visit;
	//delete left;
	//delete right;

	if (number != nullptr) {
		//((Type*)number)->start = node->token->start;
		//((Type*)number)->end = node->token->end;
	}

	return result->success(number);
}

RuntimeResult* Interpreter::visit_unary_operation_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	Type* number = (Type*)result->record(visit(node->right, context));
	
	if (result->error != nullptr) {
		return result;
	}

	((Type*)number)->start = node->token->start;
	((Type*)number)->end = node->token->end;

	Error* error = nullptr;

	std::string value;
	try { value = std::get<std::string>(node->token->value); }
	catch (const std::bad_variant_access&) {}

	if (node->token->type == Token::Type::MINUS) {
		auto op_result = number->multiply(new Number(-1));
		number = op_result.first;
		error = op_result.second;
	}
	else if (node->token->type == Token::Type::KEYWORD && value == "not") {
		auto op_result = number->compare_not(nullptr);
		number = op_result.first;
		error = op_result.second;
	}

	if (error != nullptr) {
		return result->failure(error);
	}

	return result->success(number);
}

RuntimeResult* Interpreter::visit_variable_access_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto n = (VariableAccessNode*)node;

	auto var_name = n->token->value;
	auto name = std::get<std::string>(var_name);
	auto value = context->symbols->get(name);

	if (value == context->symbols->symbols.end()) {
		return result->failure(new RuntimeError(n->start, n->end, "'" + name + "' is not defined", context));
	}

	switch (value->second.index()) {
	default:
	case 0:
	case 1:
		return result->success(new Number(value->second));
	case 4:
		return result->success(new String(value->second));
	case 5:
		return result->success(new Array(std::get<std::vector<Type*>>(value->second)));
	}
}

RuntimeResult* Interpreter::visit_variable_assignment_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto var_name = node->token->value;
	auto number_visit = visit(node->left, context);
	Type* number = result->record(number_visit);

	if (result->error != nullptr)
		return result;

	if (number->value.index() == 0) {
		context->symbols->set(std::get<std::string>(var_name), std::get<double>(number->value));
	}
	else if (number->value.index() == 1) {
		context->symbols->set(std::get<std::string>(var_name), std::get<int>(number->value));
	}
	else if (number->value.index() == 2) {
		context->symbols->set(std::get<std::string>(var_name), std::get<bool>(number->value));
	}
	else if (number->value.index() == 3) {
		context->symbols->set(std::get<std::string>(var_name), (Function*)std::get<Function*>(number->value));
	}
	else if (number->value.index() == 4) {
		context->symbols->set(std::get<std::string>(var_name), std::get<std::string>(number->value));
	}
	else if (number->value.index() == 5) {
		context->symbols->set(std::get<std::string>(var_name), std::get<std::vector<Type*>>(number->value));
	}

	return result->success(number);
}

RuntimeResult* Interpreter::visit_if_statement_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto if_node = (IfStatementNode*)node;

	for (auto& if_case : if_node->cases) {
		auto visit_left = visit(if_case.first, context);
		auto left = result->record(visit_left);

		if (result->error != nullptr)
			return result;

		auto op_result = left->is_true();

		bool value;
		try { value = std::get<bool>(op_result.first->value); }
		catch (const std::bad_variant_access&) {}

		if (value == true) {
			auto visit_else_value = visit(if_case.second, context);
			auto else_value = result->record(visit_else_value);

			if (result->error != nullptr)
				return result;

			delete visit_else_value;
			delete visit_left;

			return result->success(else_value);
		}

		delete visit_left;
	}

	if (if_node->else_case != nullptr) {
		auto visit_else_case = visit(if_node->else_case, context);
		auto else_value = result->record(visit_else_case);

		if (result->error != nullptr)
			return result;

		delete visit_else_case;

		return result->success(else_value);
	}

	return result->success(nullptr);
}

RuntimeResult* Interpreter::visit_for_statement_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto for_node = (ForStatementNode*)node;

	auto visit_start = visit(for_node->start_value, context);
	auto start_value = result->record(visit_start);

	if (result->error != nullptr)
		return result;

	auto visit_end = visit(for_node->end_value, context);
	auto end_value = result->record(visit_end);

	if (result->error != nullptr)
		return result;

	Type* step = new Number(1);

	if (for_node->step != nullptr) {
		auto visit_step = visit(for_node->step, context);
		step = result->record(visit_step);

		if (result->error != nullptr)
			return result;
	}

	int increment = 0;
	try { increment = std::get<int>(start_value->value); }
	catch (const std::bad_variant_access&) {}

	int step_value = -1;
	try { step_value = std::get<int>(step->value); }
	catch (const std::bad_variant_access&) {}

	std::function<bool(int)> condition;

	if (step_value > 0) {
		condition = [=](int i) {
			int end_val = 0;
			try { end_val = std::get<int>(end_value->value); }
			catch (const std::bad_variant_access&) {}
			return (bool)(i < end_val);
		};
	}
	else {
		condition = [=](int i) {
			int end_val = 0;
			try { end_val = std::get<int>(end_value->value); }
			catch (const std::bad_variant_access&) {}
			return (bool)(i > end_val); 
		};
	}

	std::string var_name;
	try { var_name = std::get<std::string>(for_node->token->value); }
	catch (const std::bad_variant_access&) {}

	while (condition(increment)) {
		auto num = Number(increment);
		int num_value;

		try { num_value = std::get<int>(num.value); }
		catch (const std::bad_variant_access&) {}

		context->symbols->set(var_name, num_value);
		increment += step_value;

		auto visit_body = visit(for_node->body, context);
		auto body_result = result->record(visit_body);

		delete visit_body;
		delete body_result;

		if (result->error != nullptr)
			return result;
	}

	return result->success(nullptr);
}

RuntimeResult* Interpreter::visit_while_statement_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto while_node = (WhileStatementNode*)node;

	while (true) {
		auto res = visit(while_node->condition, context);
		auto condition = result->record(res);

		if (result->error != nullptr)
			return result;

		auto op_result = condition->is_true();

		bool value = false;
		try { value = std::get<bool>(op_result.first->value); }
		catch (const std::bad_variant_access&) {}

		if (!value)
			break;

		auto body_visit = visit(while_node->body, context);
		auto body_value = result->record(body_visit);

		if (result->error != nullptr)
			return result;

		delete op_result.first;
		delete res;
		delete condition;
		delete body_visit->value;
		delete body_visit;
	}

	delete while_node;

	return result->success(nullptr);
}

RuntimeResult* Interpreter::visit_function_definition_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();

	auto fn_node = (FunctionDefinitionNode*)node;

	std::string fn_name;

	if (fn_node->token != nullptr) {
		try { fn_name = std::get<std::string>(fn_node->token->value); }
		catch (const std::bad_variant_access&) {}
	}

	std::vector<std::string> args_names;

	for (auto arg : fn_node->args_names) {
		std::string str;
		try { str = std::get<std::string>(arg->value); }
		catch (const std::bad_variant_access&) {}
		args_names.push_back(str);
	}

	auto fn_value = new Function(
		fn_name,
		fn_node->body,
		args_names,
		nullptr,
		nullptr,
		context
	);

	if (fn_node->token != nullptr) {
		context->symbols->set(fn_name, fn_value);
	}

	return result->success(fn_value);
}

RuntimeResult* Interpreter::visit_function_call_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	auto fn_call = (FunctionCallNode*)node;
	std::vector<Type*> args;

	auto visit_callee = visit(fn_call->callee, context);
	auto to_call = result->record(visit_callee);

	if (result->error != nullptr)
		return result;

	for (auto arg : fn_call->args_nodes) {
		auto arg_visit = visit(arg, context);
		args.push_back(result->record(arg_visit));

		if (result->error != nullptr)
			return result;

		delete arg_visit;
	}

	Function* to_call_value = nullptr;

	try { to_call_value = std::get<Function*>(to_call->value); }
	catch (const std::bad_variant_access&) {}

	to_call_value->context = context;

	auto call_visit = to_call_value->execute(args, context);
	auto return_value = result->record(call_visit);

	delete visit_callee;
	delete to_call;
	delete call_visit;

	if (result->error != nullptr)
		return result;

	return result->success(return_value);
}

RuntimeResult* Interpreter::visit_string_node(Node* node, Context* context)
{
	RuntimeResult* result = new RuntimeResult();
	String* str = new String(std::get<std::string>(node->token->value));

	str->context = context;
	str->start = node->token->start;
	str->end = node->token->end;

	return result->success(str);
}

RuntimeResult* Interpreter::visit_array_node(Node* node, Context* context)
{
	auto array_node = (ArrayNode*)node;
	RuntimeResult* result = new RuntimeResult();
	std::vector<Type*> elements;

	for (auto element : array_node->elements) {
		auto visit_element = visit(element, context);
		elements.push_back(result->record(visit_element));

		if (result->error != nullptr)
			return result;
	}

	return result->success(new Array(elements));
}

RuntimeResult* Interpreter::visit_property_access_node(Node* node, Context* context)
{
	auto property_node = (PropertyAccessNode*)node;
	RuntimeResult* result = new RuntimeResult();
	Type* result_value = nullptr;

	auto it = context->symbols->get(property_node->var_name);

	if (it == context->symbols->symbols.end()) {
		return result->failure(new RuntimeError(
			node->token->start,
			node->token->end,
			"'" + property_node->var_name + "' is not defined",
			context
		));
	}

	if (it->second.index() == Type::Native::STRING) {
		result_value = new String();

		auto value = std::get<std::string>(it->second);
		auto prop_value = std::get<std::string>(property_node->token->value);
		auto constant = String::constants.find(prop_value);

		if (constant != String::constants.end())
			result_value->value = constant->second;
		else {
			return result->failure(new RuntimeError(
				node->token->start,
				node->token->end,
				"'" + prop_value + "' is not defined",
				context
			));
		}
	}

	return result->success(result_value);
}
