#include <vector>
#include <map>
#include <memory>
#include <string>
#include <fstream>
#include <iostream>

#include "Tokenizer.h"
#include "Instruction.h"
#include "MnemonicTranslator.h"


static inline bool is_register_token(TOKEN_TYPE type) {
	return type == TOKEN_TYPE::R0
		|| type == TOKEN_TYPE::R1
		|| type == TOKEN_TYPE::R2
		|| type == TOKEN_TYPE::R3
		|| type == TOKEN_TYPE::R4
		|| type == TOKEN_TYPE::R5
		|| type == TOKEN_TYPE::R6
		|| type == TOKEN_TYPE::R7;
}

static inline bool is_number_token(TOKEN_TYPE type) {
	return type == TOKEN_TYPE::BINARY_NUMBER
		|| type == TOKEN_TYPE::OCTAL_NUMBER
		|| type == TOKEN_TYPE::DECIMAL_NUMBER
		|| type == TOKEN_TYPE::HEX_NUMBER
		|| type == TOKEN_TYPE::CHAR;
}

static inline int convert_to_int(Token token) {
	std::string s = token.text;

	// Если число записано не в десятичной системе счисления...
	if (token.type != TOKEN_TYPE::DECIMAL_NUMBER) {
		// ...пропускаем префикс системы счисления
		s = s.substr(2);
	}

	switch (token.type) {
	case TOKEN_TYPE::HEX_NUMBER:
		return std::stol(s, 0, 16);
	case TOKEN_TYPE::DECIMAL_NUMBER:
		return std::stol(s, 0, 10);
	case TOKEN_TYPE::OCTAL_NUMBER:
		return std::stol(s, 0, 8);
	case TOKEN_TYPE::BINARY_NUMBER:
		return std::stol(s, 0, 2);
	case TOKEN_TYPE::CHAR:
		return (int)token.text[1];
	default:
		return std::stol(s, 0, 2);
	}
}

static std::map<TOKEN_TYPE, REGISTER> reg_table{
	{ TOKEN_TYPE::R0, REGISTER::R0 },
	{ TOKEN_TYPE::R1, REGISTER::R1 },
	{ TOKEN_TYPE::R2, REGISTER::R2 },
	{ TOKEN_TYPE::R3, REGISTER::R3 },
	{ TOKEN_TYPE::R4, REGISTER::R4 },
	{ TOKEN_TYPE::R5, REGISTER::R5 },
	{ TOKEN_TYPE::R6, REGISTER::R6 },
	{ TOKEN_TYPE::R7, REGISTER::R7 },
};

/*!
Конструктор ошибки
\param[in] m Сообщение об ошибке
*/
SyntaxError::SyntaxError(const std::string& m) {
	msg = m;
}

/*
Возвращает сообщение об ошибке
\return сообщение об ошибке
*/
std::string SyntaxError::what() const {
	return msg;
}

/*!
Меняет сообщение об ошибке
\param[in] m Новое сообщение об ошибке
*/
void SyntaxError::change_error_message(const std::string& m) {
	msg = m;
}

/*!
Проверяет, что токен на заданной позиции является требуемой командной, иначе
бросает исключение, с соотвествующим сообщением, какой токен команды ожидался
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[in] command_type Требуемый тип команды
\param[in] command_name Имя команды для исключения, в случае если команды не было найдено
\return Позиция токена, следующего за искомым токеном
\throw SyntaxError В случае, если искомого токена не оказалось на заданной позици
*/
int MnemonicTranslator::check_command(std::vector<Token> tokens, int pos, TOKEN_TYPE command_type, const std::string& command_name) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидалась команда \"" + command_name + "\"");
	}
	else if (tokens[pos].type != command_type) {
		throw SyntaxError("Ожидалась команда \"" + command_name + "\", получено \"" + tokens[pos].text + "\"");
	}

	return pos + 1;
}

/*!
Проверяет, что токен на заданной позиции является запятой, иначе бросает исключение
с сообщением, что ожидалась запятая
\param[in] tokens Токены
\param[in] pos Заданная позиция
\return Позиция токена, следующего за искомым токеном
\throw SyntaxError В случае, если запятой не оказалось на заданной позиции
*/
int MnemonicTranslator::check_comma(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидалась запятая");
	}
	else if (tokens[pos].type != TOKEN_TYPE::COMMA) {
		throw SyntaxError("Ожидалась запятая, получено \"" + tokens[pos].text + "\"");
	}

	return pos + 1;
}

/*!
Проверят, что на заданной позиции находится токен регистра, иначе бросает исключение
с сообщением, что ожидался регистр
\param[in] tokens Токены
\param[in] pos Заданная позиция
\return Позиция токена, следующего за искомым токеном
\throw SyntaxError В случае, если регистра не оказалось на заданной позиции
*/
void MnemonicTranslator::expect_register(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидался регистр");
	}
	else if (!is_register_token(tokens[pos].type)) {
		throw SyntaxError("Ожидался регистр, получено \"" + tokens[pos].text + "\"");
	}
}

/*!
Проверят, что на заданной позиции находится токен имени, иначе бросает исключение
с сообщением, что ожидалось имя
\param[in] tokens Токены
\param[in] pos Заданная позиция
\return Позиция токена, следующего за искомым токеном
\throw SyntaxError В случае, если имени не оказалось на заданной позиции
*/
void MnemonicTranslator::expect_name(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидалось имя");
	}
	else if (tokens[pos].type != TOKEN_TYPE::NAME) {
		throw SyntaxError("Ожидалось имя, получено \"" + tokens[pos].text + "\"");
	}
}

/*!
Проверят, что на заданной позиции находится токен числа, иначе бросает исключение
с сообщением, что ожидалось число
\param[in] tokens Токены
\param[in] pos Заданная позиция
\return Позиция токена, следующего за искомым токеном
\throw SyntaxError В случае, если числа не оказалось на заданной позиции
*/
void MnemonicTranslator::expect_number(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидалось число");
	}
	else if (!is_number_token(tokens[pos].type)) {
		throw SyntaxError("Ожидалось число, получено \"" + tokens[pos].text + "\"");
	}
}

/*!
Извлекает числовой токен с заданной позиции и конвертирует его в число
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] number Сконвертированное число
\return Позиция токена, следующего числовым токеном
\throw SyntaxError В случае, если числа не оказалось на заданной позиции
*/
int MnemonicTranslator::extract_number(std::vector<Token> tokens, int pos, int& number) const {
	int i = pos;
	if (tokens.size() == 0 || i < 0 || i >= tokens.size()) {
		throw SyntaxError("Ожидалось число");
	}

	int sign = 1;
	if (tokens[i].type == TOKEN_TYPE::MINUS || tokens[i].type == TOKEN_TYPE::PLUS) {
		sign = tokens[i].type == TOKEN_TYPE::MINUS ? -1 : 1;
		i++;
	}

	expect_number(tokens, i);

	number = convert_to_int(tokens[i]) * sign;

	return i + 1;
}

/*!
Извлекает токен регистра с заданной позиции и определяет регистр, который он обозначает
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] reg Считанный регистр
\return Позиция токена, следующего токеном регистра
\throw SyntaxError В случае, если регистра не оказалось на заданной позиции
*/
int MnemonicTranslator::extract_register(std::vector<Token> tokens, int pos, REGISTER& reg) const {
	expect_register(tokens, pos);
	reg = reg_table[tokens[pos].type];
	return pos + 1;
}

/*!
Извлекает токен имени с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] var_name Считанное имя
\return Позиция токена, следующего токеном имени
\throw SyntaxError В случае, если имени не оказалось на заданной позиции
*/
int MnemonicTranslator::extract_name(std::vector<Token> tokens, int pos, std::string& var_name) const {
	if (tokens.size() == 0) {
		throw SyntaxError("Ожидалось имя метки");
	}
	else if (tokens[pos].type != TOKEN_TYPE::NAME) {
		throw SyntaxError("Ожидалось имя метки, получено \"" + tokens[pos].text + "\"");
	}

	var_name = tokens[pos].text;

	return pos + 1;
}

/*!
Извлекает инструкцию "add" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] add_instr Считанная инструкция "add"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_add_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& add_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "add"
	i = check_command(tokens, i, TOKEN_TYPE::ADD, "add");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		add_instr = std::make_shared<AddRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		add_instr = std::make_shared<AddImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "sub" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] sub_instr Считанная инструкция "sub"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_sub_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sub_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "sub"
	i = check_command(tokens, i, TOKEN_TYPE::SUB, "sub");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		sub_instr = std::make_shared<SubRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		sub_instr = std::make_shared<SubImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "and" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] and_instr Считанная инструкция "and"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_and_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& and_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "and"
	i = check_command(tokens, i, TOKEN_TYPE::AND, "and");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		and_instr = std::make_shared<AndRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		and_instr = std::make_shared<AndImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "or" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] or_instr Считанная инструкция "or"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_or_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& or_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "or"
	i = check_command(tokens, i, TOKEN_TYPE::OR, "or");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		or_instr = std::make_shared<OrRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		or_instr = std::make_shared<OrImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "xor" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] or_instr Считанная инструкция "xor"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_xor_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& xor_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "xor"
	i = check_command(tokens, i, TOKEN_TYPE::XOR, "xor");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		xor_instr = std::make_shared<XorRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		xor_instr = std::make_shared<XorImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "not" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] not_instr Считанная инструкция "not"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_not_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& not_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "not"
	i = check_command(tokens, i, TOKEN_TYPE::NOT, "not");

	// Выделяем единственный аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	not_instr = std::make_shared<NotInstr>(dest);

	return i;
}

/*!
Извлекает инструкцию "shr" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] shr_instr Считанная инструкция "shr"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_shr_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shr_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "shr"
	i = check_command(tokens, i, TOKEN_TYPE::SHR, "shr");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		shr_instr = std::make_shared<ShrRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		shr_instr = std::make_shared<ShrImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "shl" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] shl_instr Считанная инструкция "shl"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_shl_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shl_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "shr"
	i = check_command(tokens, i, TOKEN_TYPE::SHL, "shl");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или число
	if (i == tokens.size()) {
		throw SyntaxError("Ожидалось число или регистр");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидалось число или регистр, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - число или регистр
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		shl_instr = std::make_shared<ShlRegInstr>(dest, src);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		shl_instr = std::make_shared<ShlImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "set" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] set_instr Считанная инструкция "set"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_set_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& set_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "set"
	i = check_command(tokens, i, TOKEN_TYPE::SET, "set");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Проверяем, что после запятой идёт регистр или имя переменной
	if (i == tokens.size()) {
		throw SyntaxError("Ожидался регистр или имя переменной");
	}
	else if (!is_register_token(tokens[i].type) && tokens[i].type != TOKEN_TYPE::NAME && !is_number_token(tokens[i].type)) {
		throw SyntaxError("Ожидался регистр или имя переменной, получено \"" + tokens[i].text + "\"");
	}

	// Выделяем второй аргумент - регистр, имя переменной или число
	if (is_register_token(tokens[i].type)) {
		REGISTER src;
		i = extract_register(tokens, i, src);
		set_instr = std::make_shared<SetRegInstr>(dest, src);
	}
	else if (tokens[i].type == TOKEN_TYPE::NAME) {
		std::string var_name;
		i = extract_name(tokens, i, var_name);
		set_instr = std::make_shared<SetNameInstr>(dest, var_name);
	}
	else {
		int imm_value;
		i = extract_number(tokens, i, imm_value);
		set_instr = std::make_shared<SetImmInstr>(dest, imm_value);
	}

	return i;
}

/*!
Извлекает инструкцию "ld" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] ld_instr Считанная инструкция "ld"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_ld_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ld_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "ld"
	i = check_command(tokens, i, TOKEN_TYPE::LD, "ld");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - имя переменной
	std::string var_name;
	i = extract_name(tokens, i, var_name);

	ld_instr = std::make_shared<LdInstr>(dest, var_name);

	return i;
}

/*!
Извлекает инструкцию "st" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] st_instr Считанная инструкция "st"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_st_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& st_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "st"
	i = check_command(tokens, i, TOKEN_TYPE::ST, "st");

	// Выделяем первый аргумент команды - имя переменной
	std::string var_name;
	i = extract_name(tokens, i, var_name);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	st_instr = std::make_shared<StInstr>(var_name, dest);

	return i;
}

/*!
Извлекает инструкцию "ldi" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] ldi_instr Считанная инструкция "ldi"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_ldi_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ldi_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "ldi"
	i = check_command(tokens, i, TOKEN_TYPE::LDI, "ldi");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - регистр
	REGISTER src;
	i = extract_register(tokens, i, src);

	ldi_instr = std::make_shared<LdiInstr>(dest, src);

	return i;
}

/*!
Извлекает инструкцию "sti" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] sti_instr Считанная инструкция "sti"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_sti_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sti_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "ldi"
	i = check_command(tokens, i, TOKEN_TYPE::STI, "sti");

	// Выделяем первый аргумент команды - регистр
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - регистр
	REGISTER src;
	i = extract_register(tokens, i, src);

	sti_instr = std::make_shared<StiInstr>(dest, src);

	return i;
}

/*!
Извлекает инструкцию "jmp" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] jmp_instr Считанная инструкция "jmp"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_jmp_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jmp_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "jmp"
	i = check_command(tokens, i, TOKEN_TYPE::JMP, "jmp");

	// Выделяем единственный аргумент команды - имя метки
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	jmp_instr = std::make_shared<JmpInstr>(label_name);

	return i;
}

/*!
Извлекает инструкцию "jeq" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] jeq_instr Считанная инструкция "jeq"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_jeq_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jeq_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "jeq"
	i = check_command(tokens, i, TOKEN_TYPE::JEQ, "jeq");

	// Выделяем первый аргумент команды - имя метки
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - регистр
	REGISTER reg1;
	i = extract_register(tokens, i, reg1);

	// Пропускаем запятую после второго аргумента
	i = check_comma(tokens, i);

	// Выделяем третий аргумент команды - регистр
	REGISTER reg2;
	i = extract_register(tokens, i, reg2);

	jeq_instr = std::make_shared<JeqInstr>(label_name, reg1, reg2);

	return i;
}

/*!
Извлекает инструкцию "jgt" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] jgt_instr Считанная инструкция "jgt"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_jgt_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jgt_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "jgt"
	i = check_command(tokens, i, TOKEN_TYPE::JGT, "jgt");

	// Выделяем первый аргумент команды - имя метки
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	// Пропускаем запятую после первого аргумента
	i = check_comma(tokens, i);

	// Выделяем второй аргумент команды - регистр
	REGISTER reg1;
	i = extract_register(tokens, i, reg1);

	// Пропускаем запятую после второго аргумента
	i = check_comma(tokens, i);

	// Выделяем третий аргумент команды - регистр
	REGISTER reg2;
	i = extract_register(tokens, i, reg2);

	jgt_instr = std::make_shared<JgtInstr>(label_name, reg1, reg2);

	return i;
}

/*!
Извлекает инструкцию "call" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] call_instr Считанная инструкция "call"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_call_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& call_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "call"
	i = check_command(tokens, i, TOKEN_TYPE::CALL, "call");

	// Выделяем единственный аргумент команды - имя подпрограммы
	std::string subroutine_name;
	i = extract_name(tokens, i, subroutine_name);

	call_instr = std::make_shared<CallInstr>(subroutine_name);

	return i;
}

/*!
Извлекает инструкцию "ret" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] ret_instr Считанная инструкция "ret"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_ret_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ret_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "ret"
	i = check_command(tokens, i, TOKEN_TYPE::RET, "ret");

	ret_instr = std::make_shared<RetInstr>();

	return i;
}

/*!
Извлекает список аргументов для инструкции "data" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] arguments Список считанных аргументов
\return Позиция токена, следующего за список аргументов
\throw SyntaxError В случае, если список аргументов записан неправильно
*/
int MnemonicTranslator::extract_data_argument_list(const std::vector<Token>& tokens, int pos, std::vector<int>& arguments) const {
	int i = pos;

	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("Ожидались данные");
	}

	while (i < tokens.size()) {
		// Если аргумент это строка...
		if (tokens[i].type == TOKEN_TYPE::STRING) {
			std::string str_data = tokens[i].text;

			// Заменяем unescaped управляющий последовательности
			for (int j = 0; j < str_data.length(); j++) {
				if (str_data[j] == '\\') {
					if (j + 1 < str_data.length()) {
						switch (str_data[j + 1]) {
						case 'n':
							str_data.replace(j, 2, "\n");
							break;
						case 't':
							str_data.replace(j, 2, "\t");
							break;
						case '\\':
							str_data.replace(j, 2, "\\");
							break;
						case '\"':
							str_data.replace(j, 2, "\"");
							break;
						}
					}
				}
			}

			// Переводим все символы строки в числовые коды
			for (int j = 1; j < str_data.length() - 1; j++) {
				arguments.push_back(str_data[j]);
			}
			// Добавляем нуль-терминаторный символ
			arguments.push_back('\0');

			i++;
		}
		// Иначе...
		else {
			// Пробуем извлечь число
			int num;
			i = extract_number(tokens, i, num);
			arguments.push_back(num);
		}

		// Если токены ещё остались, то следующим токеном обяазан быть разеделитель - запятая
		if (i < tokens.size()) {
			i = check_comma(tokens, i);
		}
	}

	return i;
}

/*!
Извлекает инструкцию "data" с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] data_instr Считанная инструкция "data"
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_data_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& data_instr) const {
	int i = pos;

	// Проверяем, что первый токен в строке это "data"
	i = check_command(tokens, i, TOKEN_TYPE::DATA, "data");

	// Выделяем имя области памяти
	std::string name;
	i = extract_name(tokens, i, name);

	// Выделяем данные из команды
	std::vector<int> data;
	i = extract_data_argument_list(tokens, i, data);

	data_instr = std::make_shared<DataInstr>(name, data);

	return i;
}

/*!
Извлекает инструкцию псевдо-ассемблера с заданной позиции
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] instr Считанная инструкция
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
void MnemonicTranslator::extract_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& instr) const {
	int i = pos;

	if (tokens.size() == 0 || i < 0 || i >= tokens.size()) {
		throw SyntaxError("Ожидалась инструкция");
	}

	switch (tokens[i].type) {
	case TOKEN_TYPE::ADD:
		i = extract_add_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::SUB:
		i = extract_sub_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::AND:
		i = extract_and_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::OR:
		i = extract_or_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::XOR:
		i = extract_xor_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::NOT:
		i = extract_not_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::SHR:
		i = extract_shr_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::SHL:
		i = extract_shl_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::SET:
		i = extract_set_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::LD:
		i = extract_ld_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::ST:
		i = extract_st_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::LDI:
		i = extract_ldi_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::STI:
		i = extract_sti_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::JMP:
		i = extract_jmp_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::JEQ:
		i = extract_jeq_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::JGT:
		i = extract_jgt_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::CALL:
		i = extract_call_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::RET:
		i = extract_ret_instr(tokens, i, instr);
		break;
	case TOKEN_TYPE::DATA:
		i = extract_data_instr(tokens, i, instr);
		break;
	default:
		throw SyntaxError("Ожидалась инструкция, получено \"" + tokens[i].text + "\"");
	}

	if (i != tokens.size()) {
		throw SyntaxError("На одной строке может быть лишь одна инструкция");
	}
}

/*!
Извлекает имена меток, записанные перед инструкцией
\param[in] tokens Токены
\param[in] pos Заданная позиция
\param[out] label_names Список считанных имен меток
\return Позиция токена, следующего за инструкцией
\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
*/
int MnemonicTranslator::extract_labels(const std::vector<Token>& tokens, int pos, std::vector<std::string>& label_names) const {
	int i = pos;

	while (i < tokens.size() && tokens[i].type == TOKEN_TYPE::NAME) {
		label_names.push_back(tokens[i].text);

		i++;
		if (i == tokens.size() || tokens[i].type != TOKEN_TYPE::COLON) {
			throw SyntaxError("После метки \"" + label_names.at(label_names.size() - 1) + "\" должно идти двоеточие");
		}

		i++;
	}

	return i;
}

/*!
Переводит текстовые мнемоники на языке псевдо-ассемблера во внутреннее представление
\param[in] input_file Входной поток мнемоник
\param[out] instrs Считанные инструкции
\param[out] instr Считанные метки
\param[out] tokenizer_errors Ошибки, возникшие во время токенезации мнемоник
\param[out] syntax_errors Ошибки, возникшие во время синтаксического разбора инструкций
\return Флаг, указывающий, возникли ли ошибки во время перевода мнемоник
*/
bool MnemonicTranslator::translate(std::ifstream& input_file, std::vector<std::shared_ptr<Instr>>& instrs, std::map<std::string, int>& labels, std::vector<TokenizerError>& tokenizer_errors, std::vector<SyntaxError>& syntax_errors) const {
	// Токенайзер псевдо-ассемблера
	Tokenizer tokenizer;

	// Текущая строка из входного потока
	std::string line;

	// Метки, ссылающиеся на текущую инструкцию
	std::vector<std::string> found_label_names;

	int current_line_number = 0;

	// Пока во входном файле есть строки
	while (std::getline(input_file, line)) {
		try {
			current_line_number++;

			// Конвертировать текущую строку в поток токенов
			std::vector<Token> tokens;
			tokenizer.tokenize(line, tokens);

			// Извлечь метки, написанные вначале строки
			std::vector<std::string> label_names_buf;
			int instr_start_pos = extract_labels(tokens, 0, label_names_buf);

			// Если метки были найдены, проверяем, были ли среди них дубликата
			if (label_names_buf.size() > 0) {
				for (const std::string& label_name : label_names_buf) {
					if (std::find(found_label_names.begin(), found_label_names.end(), label_name) != found_label_names.end()) {
						throw SyntaxError("Метка \"" + label_name + "\" объявляется более одного раза");
					}

					found_label_names.push_back(label_name);
				}
			}

			// Если токенов кроме меток в строке не было, перейти к следующей строчке
			if (instr_start_pos == tokens.size()) {
				continue;
			}

			// Извлечь инструкцию, написанную в строке
			std::shared_ptr<Instr> instr = nullptr;
			extract_instr(tokens, instr_start_pos, instr);

			// Если на строке была инструкция
			if (instr != nullptr) {
				// Задать номер строки, на которой расположена инструкция
				instr->set_line_number(current_line_number);

				// Поместить её в список считанных инструкция
				instrs.push_back(instr);
				int instr_address = instrs.size() - 1;

				// Для каждой найденной раннее метки
				for (const std::string& label : found_label_names) {
					// Сохранить метку и адрес, на которой она указывает
					labels[label] = instr_address;
				}

				// Очистить список ранее найденных меток
				found_label_names.clear();
			}
		}
		catch (TokenizerError& err) {
			err.change_error_message("Строка " + std::to_string(current_line_number) + ": " + err.what());
			tokenizer_errors.push_back(err);
		}
		catch (SyntaxError& err) {
			err.change_error_message("Строка " + std::to_string(current_line_number) + ": " + err.what());
			syntax_errors.push_back(err);
		}
	}	

	return tokenizer_errors.size() == 0 && syntax_errors.size() == 0;
}