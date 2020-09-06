#include "pch.h"
#include "Interpreter.h"
#include "Function.h"
#include "Str.h"
#include "Array.h"
#include "Map.h"
#include "File.h"

Interpreter::Interpreter()
{
}

RuntimeResult* Interpreter::visit(Node* node, Context* context)
{
	if (node != nullptr && context != nullptr) {
		if (BinaryOperationNode* binary_operation_node = dynamic_cast<BinaryOperationNode*>(node)) 
			return visit_binary_operation_node(binary_operation_node, context);
		else if (NumericNode* numeric_node = dynamic_cast<NumericNode*>(node))
			return visit_numeric_node(numeric_node, context);
		else if (UnaryOperationNode* unary_operation_node = dynamic_cast<UnaryOperationNode*>(node))
			return visit_unary_operation_node(unary_operation_node, context);
		else if (VariableAccessNode* variable_access_node = dynamic_cast<VariableAccessNode*>(node))
			return visit_variable_access_node(variable_access_node, context);
		else if (VariableAssignmentNode* variable_assignment_node = dynamic_cast<VariableAssignmentNode*>(node))
			return visit_variable_assignment_node(variable_assignment_node, context);
		else if (IfStatementNode* if_statement_node = dynamic_cast<IfStatementNode*>(node))
			return visit_if_statement_node(if_statement_node, context);
		else if (ForStatementNode* for_statement_node = dynamic_cast<ForStatementNode*>(node))
			return visit_for_statement_node(for_statement_node, context);
		else if (WhileStatementNode* while_statement_node = dynamic_cast<WhileStatementNode*>(node))
			return visit_while_statement_node(while_statement_node, context);
		else if (FunctionDefinitionNode* function_definition_node = dynamic_cast<FunctionDefinitionNode*>(node))
			return visit_function_definition_node(function_definition_node, context);
		else if (FunctionCallNode* function_call_node = dynamic_cast<FunctionCallNode*>(node))
			return visit_function_call_node(function_call_node, context);
		else if(StringNode* string_node = dynamic_cast<StringNode*>(node))
			return visit_string_node(string_node, context);
		else if(ArrayNode* array_node = dynamic_cast<ArrayNode*>(node))
			return visit_array_node(array_node, context);
		else if (PropertyAccessNode* property_access_node = dynamic_cast<PropertyAccessNode*>(node))
			return visit_property_access_node(property_access_node, context);
		else if(IndexAccessNode* index_access_node = dynamic_cast<IndexAccessNode*>(node))
			return visit_index_access_node(index_access_node, context);
		else if (MapNode* map_node = dynamic_cast<MapNode*>(node))
			return visit_map_node(map_node, context);

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
	case Type::Native::DOUBLE:
	case Type::Native::INT:
	case Type::Native::BOOL:
		return result->success(new Number(value->second));
	case Type::Native::STRING:
		return result->success(new String(value->second));
	case Type::Native::ARRAY:
		return result->success(new Array(std::get<std::vector<Type*>>(value->second)));
	case Type::Native::MAP:
		return result->success(new Map(std::get<std::map<std::string, Type*>>(value->second)));
	case Type::Native::FILE:
		auto file = new File();
		auto ref = std::get<File*>(value->second);
		file->name = ref->name;
		file->size = ref->size;
		file->value = ref->value;
		file->closed = ref->closed;

		auto typePtr = new Type();
		typePtr->value = file;
		return result->success(typePtr);
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

	if (number->value.index() == Type::Native::DOUBLE)
		context->symbols->set(std::get<std::string>(var_name), std::get<double>(number->value));
	else if (number->value.index() == Type::Native::INT)
		context->symbols->set(std::get<std::string>(var_name), std::get<int>(number->value));
	else if (number->value.index() == Type::Native::BOOL)
		context->symbols->set(std::get<std::string>(var_name), std::get<bool>(number->value));
	else if (number->value.index() == Type::Native::FUNCTION)
		context->symbols->set(std::get<std::string>(var_name), (Function*)std::get<Function*>(number->value));
	else if (number->value.index() == Type::Native::STRING)
		context->symbols->set(std::get<std::string>(var_name), std::get<std::string>(number->value));
	else if (number->value.index() == Type::Native::ARRAY)
		context->symbols->set(std::get<std::string>(var_name), std::get<std::vector<Type*>>(number->value));
	else if (number->value.index() == Type::Native::FILE)
		context->symbols->set(std::get<std::string>(var_name), (File*)std::get<File*>(number->value));
	else if (number->value.index() == Type::Native::MAP)
		context->symbols->set(std::get<std::string>(var_name), std::get<std::map<std::string, Type*>>(number->value));

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
	else if (it->second.index() == Type::Native::FILE) {
		auto prop_value = std::get<std::string>(property_node->token->value);
		auto file = std::get<File*>(it->second);

		if (prop_value == "name") {
			result_value = new String();
			result_value->value = std::filesystem::path(file->name).filename().string();
		}
		else if (prop_value == "extension") {
			result_value = new String();
			result_value->value = std::filesystem::path(file->name).extension().string();
		}
		else if (prop_value == "path") {
			result_value = new String();
			result_value->value = file->name;
		}
		else if (prop_value == "size") {
			result_value = new Number();
			result_value->value = (int)file->size;
		}
		else if (prop_value == "data") {
			result_value = new String();
			result_value->value = std::get<std::string>(file->value);
		}
		else if (prop_value == "closed") {
			result_value = new Number();
			result_value->value = file->closed;
		}
		else if (prop_value == "mode") {
			result_value = new String();
			result_value->value = File::modeToStr(static_cast<File::Mode>(file->mode));
		}
	}
	else if (it->second.index() == Type::Native::ARRAY) {
		auto prop_value = std::get<std::string>(property_node->token->value);
		auto array = std::get<std::vector<Type*>>(it->second);

		if (prop_value == "size") {
			result_value = new Number();
			result_value->value = (int)array.size();
		}
		else if (prop_value == "keys") {
			result_value = new Array();
			std::vector<Type*> keys = {};

			for (int i = 0; i < array.size(); ++i)
				keys.push_back(new Number(i));

			result_value->value = keys;
		}
		else if (prop_value == "values") {
			result_value = new Array();
			std::vector<Type*> keys = {};

			for (auto item : array)
				keys.push_back(item);

			result_value->value = keys;
		}
	}
	else if (it->second.index() == Type::Native::MAP) {

		auto it = context->symbols->get(property_node->var_name);
		auto value = std::get<std::map<std::string, Type*>>(it->second);

		result_value = find_map_recursive(
			property_node->var_name,
			context,
			property_node->path,
			value
		);
	}

	return result->success(result_value);
}

Type* Interpreter::find_map_recursive(
	const std::string& needle,
	Context* context,
	const std::vector<Token*>& path,
	const std::map<std::string, Type*>& object
)
{
	Type* result = nullptr;

	for (auto part : path) {
		auto search = object.find(std::get<std::string>(part->value));

		if (search != object.end()) {
			if (search->second->is(Type::Native::MAP)) {
				result = find_map_recursive(
					search->first,
					context,
					path,
					std::get<std::map<std::string, Type*>>(search->second->value)
				);
			}
			else {
				result = search->second;
			}
		}
	}

	return result;
}

RuntimeResult* Interpreter::visit_index_access_node(Node* node, Context* context)
{
	auto index_node = (IndexAccessNode*)node;
	RuntimeResult* result = new RuntimeResult();
	auto number_visit = visit(index_node->left, context);
	Type* number = result->record(number_visit);

	Type* result_value = nullptr;
	auto var_name = std::get<std::string>(index_node->token->value);
	auto it = context->symbols->get(var_name);

	if (it == context->symbols->symbols.end()) {
		return result->failure(new RuntimeError(
			node->token->start,
			node->token->end,
			"'" + var_name + "' is not defined",
			context
		));
	}

	if (it->second.index() == Type::Native::ARRAY) {
		auto array = std::get<std::vector<Type*>>(it->second);
		auto index = array.at(std::get<int>(number->value));

		result_value = index;
	}
	else if (it->second.index() == Type::Native::STRING) {
		auto string = std::get<std::string>(it->second);
		auto index = string.at(std::get<int>(number->value));

		result_value = new String();
		result_value->value = std::string(1, index);
	}
	else if (it->second.index() == Type::Native::MAP) {
		switch (number->value.index()) {
		case Type::Native::STRING:
			auto map = std::get<std::map<std::string, Type*>>(it->second);
			auto found = map.find(std::get<std::string>(number->value));
			result_value = found->second;
			break;
		}
	}

	return result->success(result_value);
}

RuntimeResult* Interpreter::visit_map_node(Node* node, Context* context)
{
	auto map_node = (MapNode*)node;
	RuntimeResult* result = new RuntimeResult();
	std::map<std::string, Type*> elements;

	for (auto it = map_node->elements.begin(); it != map_node->elements.end(); ++it) {
		auto visit_element = visit(it->second, context);
		elements.emplace(it->first, result->record(visit_element));

		if (result->error != nullptr)
			return result;
	}

	return result->success(new Map(elements));
}
