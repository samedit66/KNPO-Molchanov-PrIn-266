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

	// ���� ����� �������� �� � ���������� ������� ���������...
	if (token.type != TOKEN_TYPE::DECIMAL_NUMBER) {
		// ...���������� ������� ������� ���������
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
����������� ������
\param[in] m ��������� �� ������
*/
SyntaxError::SyntaxError(const std::string& m) {
	msg = m;
}

/*
���������� ��������� �� ������
\return ��������� �� ������
*/
std::string SyntaxError::what() const {
	return msg;
}

/*!
������ ��������� �� ������
\param[in] m ����� ��������� �� ������
*/
void SyntaxError::change_error_message(const std::string& m) {
	msg = m;
}

/*!
���������, ��� ����� �� �������� ������� �������� ��������� ���������, �����
������� ����������, � �������������� ����������, ����� ����� ������� ��������
\param[in] tokens ������
\param[in] pos �������� �������
\param[in] command_type ��������� ��� �������
\param[in] command_name ��� ������� ��� ����������, � ������ ���� ������� �� ���� �������
\return ������� ������, ���������� �� ������� �������
\throw SyntaxError � ������, ���� �������� ������ �� ��������� �� �������� ������
*/
int MnemonicTranslator::check_command(std::vector<Token> tokens, int pos, TOKEN_TYPE command_type, const std::string& command_name) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("��������� ������� \"" + command_name + "\"");
	}
	else if (tokens[pos].type != command_type) {
		throw SyntaxError("��������� ������� \"" + command_name + "\", �������� \"" + tokens[pos].text + "\"");
	}

	return pos + 1;
}

/*!
���������, ��� ����� �� �������� ������� �������� �������, ����� ������� ����������
� ����������, ��� ��������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\return ������� ������, ���������� �� ������� �������
\throw SyntaxError � ������, ���� ������� �� ��������� �� �������� �������
*/
int MnemonicTranslator::check_comma(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("��������� �������");
	}
	else if (tokens[pos].type != TOKEN_TYPE::COMMA) {
		throw SyntaxError("��������� �������, �������� \"" + tokens[pos].text + "\"");
	}

	return pos + 1;
}

/*!
��������, ��� �� �������� ������� ��������� ����� ��������, ����� ������� ����������
� ����������, ��� �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\return ������� ������, ���������� �� ������� �������
\throw SyntaxError � ������, ���� �������� �� ��������� �� �������� �������
*/
void MnemonicTranslator::expect_register(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("�������� �������");
	}
	else if (!is_register_token(tokens[pos].type)) {
		throw SyntaxError("�������� �������, �������� \"" + tokens[pos].text + "\"");
	}
}

/*!
��������, ��� �� �������� ������� ��������� ����� �����, ����� ������� ����������
� ����������, ��� ��������� ���
\param[in] tokens ������
\param[in] pos �������� �������
\return ������� ������, ���������� �� ������� �������
\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
*/
void MnemonicTranslator::expect_name(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("��������� ���");
	}
	else if (tokens[pos].type != TOKEN_TYPE::NAME) {
		throw SyntaxError("��������� ���, �������� \"" + tokens[pos].text + "\"");
	}
}

/*!
��������, ��� �� �������� ������� ��������� ����� �����, ����� ������� ����������
� ����������, ��� ��������� �����
\param[in] tokens ������
\param[in] pos �������� �������
\return ������� ������, ���������� �� ������� �������
\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
*/
void MnemonicTranslator::expect_number(const std::vector<Token>& tokens, int pos) const {
	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("��������� �����");
	}
	else if (!is_number_token(tokens[pos].type)) {
		throw SyntaxError("��������� �����, �������� \"" + tokens[pos].text + "\"");
	}
}

/*!
��������� �������� ����� � �������� ������� � ������������ ��� � �����
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] number ����������������� �����
\return ������� ������, ���������� �������� �������
\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
*/
int MnemonicTranslator::extract_number(std::vector<Token> tokens, int pos, int& number) const {
	int i = pos;
	if (tokens.size() == 0 || i < 0 || i >= tokens.size()) {
		throw SyntaxError("��������� �����");
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
��������� ����� �������� � �������� ������� � ���������� �������, ������� �� ����������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] reg ��������� �������
\return ������� ������, ���������� ������� ��������
\throw SyntaxError � ������, ���� �������� �� ��������� �� �������� �������
*/
int MnemonicTranslator::extract_register(std::vector<Token> tokens, int pos, REGISTER& reg) const {
	expect_register(tokens, pos);
	reg = reg_table[tokens[pos].type];
	return pos + 1;
}

/*!
��������� ����� ����� � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] var_name ��������� ���
\return ������� ������, ���������� ������� �����
\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
*/
int MnemonicTranslator::extract_name(std::vector<Token> tokens, int pos, std::string& var_name) const {
	if (tokens.size() == 0) {
		throw SyntaxError("��������� ��� �����");
	}
	else if (tokens[pos].type != TOKEN_TYPE::NAME) {
		throw SyntaxError("��������� ��� �����, �������� \"" + tokens[pos].text + "\"");
	}

	var_name = tokens[pos].text;

	return pos + 1;
}

/*!
��������� ���������� "add" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] add_instr ��������� ���������� "add"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_add_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& add_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "add"
	i = check_command(tokens, i, TOKEN_TYPE::ADD, "add");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "sub" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] sub_instr ��������� ���������� "sub"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_sub_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sub_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "sub"
	i = check_command(tokens, i, TOKEN_TYPE::SUB, "sub");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "and" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] and_instr ��������� ���������� "and"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_and_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& and_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "and"
	i = check_command(tokens, i, TOKEN_TYPE::AND, "and");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "or" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] or_instr ��������� ���������� "or"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_or_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& or_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "or"
	i = check_command(tokens, i, TOKEN_TYPE::OR, "or");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "xor" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] or_instr ��������� ���������� "xor"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_xor_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& xor_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "xor"
	i = check_command(tokens, i, TOKEN_TYPE::XOR, "xor");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "not" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] not_instr ��������� ���������� "not"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_not_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& not_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "not"
	i = check_command(tokens, i, TOKEN_TYPE::NOT, "not");

	// �������� ������������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	not_instr = std::make_shared<NotInstr>(dest);

	return i;
}

/*!
��������� ���������� "shr" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] shr_instr ��������� ���������� "shr"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_shr_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shr_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "shr"
	i = check_command(tokens, i, TOKEN_TYPE::SHR, "shr");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "shl" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] shl_instr ��������� ���������� "shl"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_shl_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shl_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "shr"
	i = check_command(tokens, i, TOKEN_TYPE::SHL, "shl");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� �����
	if (i == tokens.size()) {
		throw SyntaxError("��������� ����� ��� �������");
	}
	else if (!is_register_token(tokens[i].type)
		&& tokens[i].type != TOKEN_TYPE::MINUS
		&& tokens[i].type != TOKEN_TYPE::PLUS
		&& !is_number_token(tokens[i].type)) {
		throw SyntaxError("��������� ����� ��� �������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - ����� ��� �������
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
��������� ���������� "set" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] set_instr ��������� ���������� "set"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_set_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& set_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "set"
	i = check_command(tokens, i, TOKEN_TYPE::SET, "set");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// ���������, ��� ����� ������� ��� ������� ��� ��� ����������
	if (i == tokens.size()) {
		throw SyntaxError("�������� ������� ��� ��� ����������");
	}
	else if (!is_register_token(tokens[i].type) && tokens[i].type != TOKEN_TYPE::NAME && !is_number_token(tokens[i].type)) {
		throw SyntaxError("�������� ������� ��� ��� ����������, �������� \"" + tokens[i].text + "\"");
	}

	// �������� ������ �������� - �������, ��� ���������� ��� �����
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
��������� ���������� "ld" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] ld_instr ��������� ���������� "ld"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_ld_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ld_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "ld"
	i = check_command(tokens, i, TOKEN_TYPE::LD, "ld");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - ��� ����������
	std::string var_name;
	i = extract_name(tokens, i, var_name);

	ld_instr = std::make_shared<LdInstr>(dest, var_name);

	return i;
}

/*!
��������� ���������� "st" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] st_instr ��������� ���������� "st"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_st_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& st_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "st"
	i = check_command(tokens, i, TOKEN_TYPE::ST, "st");

	// �������� ������ �������� ������� - ��� ����������
	std::string var_name;
	i = extract_name(tokens, i, var_name);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	st_instr = std::make_shared<StInstr>(var_name, dest);

	return i;
}

/*!
��������� ���������� "ldi" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] ldi_instr ��������� ���������� "ldi"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_ldi_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ldi_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "ldi"
	i = check_command(tokens, i, TOKEN_TYPE::LDI, "ldi");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER src;
	i = extract_register(tokens, i, src);

	ldi_instr = std::make_shared<LdiInstr>(dest, src);

	return i;
}

/*!
��������� ���������� "sti" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] sti_instr ��������� ���������� "sti"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_sti_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sti_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "ldi"
	i = check_command(tokens, i, TOKEN_TYPE::STI, "sti");

	// �������� ������ �������� ������� - �������
	REGISTER dest;
	i = extract_register(tokens, i, dest);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER src;
	i = extract_register(tokens, i, src);

	sti_instr = std::make_shared<StiInstr>(dest, src);

	return i;
}

/*!
��������� ���������� "jmp" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] jmp_instr ��������� ���������� "jmp"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_jmp_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jmp_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "jmp"
	i = check_command(tokens, i, TOKEN_TYPE::JMP, "jmp");

	// �������� ������������ �������� ������� - ��� �����
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	jmp_instr = std::make_shared<JmpInstr>(label_name);

	return i;
}

/*!
��������� ���������� "jeq" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] jeq_instr ��������� ���������� "jeq"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_jeq_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jeq_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "jeq"
	i = check_command(tokens, i, TOKEN_TYPE::JEQ, "jeq");

	// �������� ������ �������� ������� - ��� �����
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER reg1;
	i = extract_register(tokens, i, reg1);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER reg2;
	i = extract_register(tokens, i, reg2);

	jeq_instr = std::make_shared<JeqInstr>(label_name, reg1, reg2);

	return i;
}

/*!
��������� ���������� "jgt" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] jgt_instr ��������� ���������� "jgt"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_jgt_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jgt_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "jgt"
	i = check_command(tokens, i, TOKEN_TYPE::JGT, "jgt");

	// �������� ������ �������� ������� - ��� �����
	std::string label_name;
	i = extract_name(tokens, i, label_name);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER reg1;
	i = extract_register(tokens, i, reg1);

	// ���������� ������� ����� ������� ���������
	i = check_comma(tokens, i);

	// �������� ������ �������� ������� - �������
	REGISTER reg2;
	i = extract_register(tokens, i, reg2);

	jgt_instr = std::make_shared<JgtInstr>(label_name, reg1, reg2);

	return i;
}

/*!
��������� ���������� "call" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] call_instr ��������� ���������� "call"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_call_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& call_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "call"
	i = check_command(tokens, i, TOKEN_TYPE::CALL, "call");

	// �������� ������������ �������� ������� - ��� ������������
	std::string subroutine_name;
	i = extract_name(tokens, i, subroutine_name);

	call_instr = std::make_shared<CallInstr>(subroutine_name);

	return i;
}

/*!
��������� ���������� "ret" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] ret_instr ��������� ���������� "ret"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_ret_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ret_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "ret"
	i = check_command(tokens, i, TOKEN_TYPE::RET, "ret");

	ret_instr = std::make_shared<RetInstr>();

	return i;
}

/*!
��������� ������ ���������� ��� ���������� "data" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] arguments ������ ��������� ����������
\return ������� ������, ���������� �� ������ ����������
\throw SyntaxError � ������, ���� ������ ���������� ������� �����������
*/
int MnemonicTranslator::extract_data_argument_list(const std::vector<Token>& tokens, int pos, std::vector<int>& arguments) const {
	int i = pos;

	if (tokens.size() == 0 || pos < 0 || pos >= tokens.size()) {
		throw SyntaxError("��������� ������");
	}

	while (i < tokens.size()) {
		// ���� �������� ��� ������...
		if (tokens[i].type == TOKEN_TYPE::STRING) {
			std::string str_data = tokens[i].text;

			// �������� unescaped ����������� ������������������
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

			// ��������� ��� ������� ������ � �������� ����
			for (int j = 1; j < str_data.length() - 1; j++) {
				arguments.push_back(str_data[j]);
			}
			// ��������� ����-������������� ������
			arguments.push_back('\0');

			i++;
		}
		// �����...
		else {
			// ������� ������� �����
			int num;
			i = extract_number(tokens, i, num);
			arguments.push_back(num);
		}

		// ���� ������ ��� ��������, �� ��������� ������� ������� ���� ������������ - �������
		if (i < tokens.size()) {
			i = check_comma(tokens, i);
		}
	}

	return i;
}

/*!
��������� ���������� "data" � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] data_instr ��������� ���������� "data"
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_data_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& data_instr) const {
	int i = pos;

	// ���������, ��� ������ ����� � ������ ��� "data"
	i = check_command(tokens, i, TOKEN_TYPE::DATA, "data");

	// �������� ��� ������� ������
	std::string name;
	i = extract_name(tokens, i, name);

	// �������� ������ �� �������
	std::vector<int> data;
	i = extract_data_argument_list(tokens, i, data);

	data_instr = std::make_shared<DataInstr>(name, data);

	return i;
}

/*!
��������� ���������� ������-���������� � �������� �������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] instr ��������� ����������
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
void MnemonicTranslator::extract_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& instr) const {
	int i = pos;

	if (tokens.size() == 0 || i < 0 || i >= tokens.size()) {
		throw SyntaxError("��������� ����������");
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
		throw SyntaxError("��������� ����������, �������� \"" + tokens[i].text + "\"");
	}

	if (i != tokens.size()) {
		throw SyntaxError("�� ����� ������ ����� ���� ���� ���� ����������");
	}
}

/*!
��������� ����� �����, ���������� ����� �����������
\param[in] tokens ������
\param[in] pos �������� �������
\param[out] label_names ������ ��������� ���� �����
\return ������� ������, ���������� �� �����������
\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
*/
int MnemonicTranslator::extract_labels(const std::vector<Token>& tokens, int pos, std::vector<std::string>& label_names) const {
	int i = pos;

	while (i < tokens.size() && tokens[i].type == TOKEN_TYPE::NAME) {
		label_names.push_back(tokens[i].text);

		i++;
		if (i == tokens.size() || tokens[i].type != TOKEN_TYPE::COLON) {
			throw SyntaxError("����� ����� \"" + label_names.at(label_names.size() - 1) + "\" ������ ���� ���������");
		}

		i++;
	}

	return i;
}

/*!
��������� ��������� ��������� �� ����� ������-���������� �� ���������� �������������
\param[in] input_file ������� ����� ��������
\param[out] instrs ��������� ����������
\param[out] instr ��������� �����
\param[out] tokenizer_errors ������, ��������� �� ����� ����������� ��������
\param[out] syntax_errors ������, ��������� �� ����� ��������������� ������� ����������
\return ����, �����������, �������� �� ������ �� ����� �������� ��������
*/
bool MnemonicTranslator::translate(std::ifstream& input_file, std::vector<std::shared_ptr<Instr>>& instrs, std::map<std::string, int>& labels, std::vector<TokenizerError>& tokenizer_errors, std::vector<SyntaxError>& syntax_errors) const {
	// ���������� ������-����������
	Tokenizer tokenizer;

	// ������� ������ �� �������� ������
	std::string line;

	// �����, ����������� �� ������� ����������
	std::vector<std::string> found_label_names;

	int current_line_number = 0;

	// ���� �� ������� ����� ���� ������
	while (std::getline(input_file, line)) {
		try {
			current_line_number++;

			// �������������� ������� ������ � ����� �������
			std::vector<Token> tokens;
			tokenizer.tokenize(line, tokens);

			// ������� �����, ���������� ������� ������
			std::vector<std::string> label_names_buf;
			int instr_start_pos = extract_labels(tokens, 0, label_names_buf);

			// ���� ����� ���� �������, ���������, ���� �� ����� ��� ���������
			if (label_names_buf.size() > 0) {
				for (const std::string& label_name : label_names_buf) {
					if (std::find(found_label_names.begin(), found_label_names.end(), label_name) != found_label_names.end()) {
						throw SyntaxError("����� \"" + label_name + "\" ����������� ����� ������ ����");
					}

					found_label_names.push_back(label_name);
				}
			}

			// ���� ������� ����� ����� � ������ �� ����, ������� � ��������� �������
			if (instr_start_pos == tokens.size()) {
				continue;
			}

			// ������� ����������, ���������� � ������
			std::shared_ptr<Instr> instr = nullptr;
			extract_instr(tokens, instr_start_pos, instr);

			// ���� �� ������ ���� ����������
			if (instr != nullptr) {
				// ������ ����� ������, �� ������� ����������� ����������
				instr->set_line_number(current_line_number);

				// ��������� � � ������ ��������� ����������
				instrs.push_back(instr);
				int instr_address = instrs.size() - 1;

				// ��� ������ ��������� ������ �����
				for (const std::string& label : found_label_names) {
					// ��������� ����� � �����, �� ������� ��� ���������
					labels[label] = instr_address;
				}

				// �������� ������ ����� ��������� �����
				found_label_names.clear();
			}
		}
		catch (TokenizerError& err) {
			err.change_error_message("������ " + std::to_string(current_line_number) + ": " + err.what());
			tokenizer_errors.push_back(err);
		}
		catch (SyntaxError& err) {
			err.change_error_message("������ " + std::to_string(current_line_number) + ": " + err.what());
			syntax_errors.push_back(err);
		}
	}	

	return tokenizer_errors.size() == 0 && syntax_errors.size() == 0;
}