#include "pch.h"
#include "Interpreter.h"

Interpreter::Interpreter()
{

}

Interpreter::Result* Interpreter::visit(Node* node, Context* context)
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

		auto type = typeid(*node).name();
		std::cout << "No visit " << type << " method defined" << std::endl;
	}

	return nullptr;
}

Interpreter::Result* Interpreter::visit_numeric_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = new Number();
	number->context = context;
	number->start = node->token->start;
	number->end = node->token->end;

	if (node->token->value.index() == 0) {
		try { number->value = std::get<float>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}
	else {
		try { number->value = std::get<int>(node->token->value); }
		catch (const std::bad_variant_access&) {}
	}

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_binary_operation_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = nullptr;

	auto left_visit = visit(node->left, context);
	Number* left = result->record(left_visit);

	if (result->error != nullptr)
		return result;

	auto right_visit = visit(node->right, context);
	Number* right = result->record(right_visit);

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

	delete left_visit;
	delete right_visit;
	delete left;
	delete right;

	if (number != nullptr) {
		number->start = node->token->start;
		number->end = node->token->end;
	}

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_unary_operation_node(Node* node, Context* context)
{
	Result* result = new Result();
	Number* number = result->record(visit(node->right, context));
	
	if (result->error != nullptr) {
		return result;
	}

	number->start = node->token->start;
	number->end = node->token->end;

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

Interpreter::Result* Interpreter::visit_variable_access_node(Node* node, Context* context)
{
	Result* result = new Result();
	auto n = (VariableAccessNode*)node;

	auto var_name = n->token->value;
	auto name = std::get<std::string>(var_name);
	auto value = context->symbols->get(name);

	if (value == context->symbols->symbols.end()) {
		return result->failure(new RuntimeError(n->start, n->end, "'" + name + "' is not defined", context));
	}

	return result->success(new Number(value->second));
}

Interpreter::Result* Interpreter::visit_variable_assignment_node(Node* node, Context* context)
{
	Result* result = new Result();
	auto var_name = node->token->value;
	auto number_visit = visit(node->left, context);
	Number* number = result->record(number_visit);

	if (result->error != nullptr)
		return result;

	if (number->value.index() == 0) {
		context->symbols->set(std::get<std::string>(var_name), std::get<float>(number->value));
	}
	else if (number->value.index() == 1) {
		context->symbols->set(std::get<std::string>(var_name), std::get<int>(number->value));
	}
	else if (number->value.index() == 2) {
		context->symbols->set(std::get<std::string>(var_name), std::get<bool>(number->value));
	}

	delete number_visit;

	return result->success(number);
}

Interpreter::Result* Interpreter::visit_if_statement_node(Node* node, Context* context)
{
	Result* result = new Result();
	auto if_node = (IfStatementNode*)node;

	for (auto& if_case : if_node->cases) {
		auto left = result->record(visit(if_case.first, context));

		if (result->error != nullptr)
			return result;

		auto op_result = left->is_true();

		bool value;
		try { value = std::get<bool>(op_result.first->value); }
		catch (const std::bad_variant_access&) {}

		if (value == true) {
			auto else_value = result->record(visit(if_case.second, context));

			if (result->error != nullptr)
				return result;

			return result->success(else_value);
		}
	}

	if (if_node->else_case != nullptr) {
		auto else_value = result->record(visit(if_node->else_case, context));

		if (result->error != nullptr)
			return result;

		return result->success(else_value);
	}

	return result->success(nullptr);
}

Interpreter::Result* Interpreter::visit_for_statement_node(Node* node, Context* context)
{
	Result* result = new Result();
	auto for_node = (ForStatementNode*)node;

	auto start_value = result->record(visit(for_node->start_value, context));

	if (result->error != nullptr)
		return result;

	auto end_value = result->record(visit(for_node->end_value, context));

	if (result->error != nullptr)
		return result;

	Number* step = new Number(1);

	if (for_node->step != nullptr) {
		step = result->record(visit(for_node->step, context));

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

		std::cout << result->record(visit(for_node->body, context)) << '\n';

		if (result->error != nullptr)
			return result;
	}

	return result->success(nullptr);
}

Interpreter::Result* Interpreter::visit_while_statement_node(Node* node, Context* context)
{
	Result* result = new Result();
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

		//std::cout << body_value << '\n';

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
