#pragma once

#include <vector>
#include <map>
#include <string>

#include "Tokenizer.h"
#include "Instruction.h"

/*!
�����, ����������� ������, ����������� � ������, ����
���������� ���� �������� �����������
*/
class SyntaxError {
private:
	/// ��������� �� ������
	std::string msg;

public:
	/*!
	����������� ������
	\param[in] m ��������� �� ������
	*/
	SyntaxError(const std::string& m);

	/*
	���������� ��������� �� ������
	\return ��������� �� ������
	*/
	std::string what() const;

	/*!
	������ ��������� �� ������
	\param[in] m ����� ��������� �� ������
	*/
	void change_error_message(const std::string& m);
};

/*
���������� ��������� �������� ������-���������� �� ���������� ������������� 
*/
class MnemonicTranslator
{
private:
	/*!
	���������, ��� ����� �� �������� ������� �������� ��������� ���������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[in] command_type ��������� ��� �������
	\param[in] command_name ��� ������� ��� ����������, � ������ ���� ������� �� ���� �������
	\return ������� ������, ���������� �� ������� �������
	\throw SyntaxError � ������, ���� �������� ������ �� ��������� �� �������� ������
	*/
	int check_command(std::vector<Token> tokens, int pos, TOKEN_TYPE command_type, const std::string& command_name) const;

	/*!
	���������, ��� ����� �� �������� ������� �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\return ������� ������, ���������� �� ������� �������
	\throw SyntaxError � ������, ���� ������� �� ��������� �� �������� �������
	*/
	int check_comma(const std::vector<Token>& tokens, int pos) const;

	/*!
	��������, ��� �� �������� ������� ��������� ����� ��������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\return ������� ������, ���������� �� ������� �������
	\throw SyntaxError � ������, ���� �������� �� ��������� �� �������� �������
	*/
	void expect_register(const std::vector<Token>& tokens, int pos) const;

	/*!
	��������, ��� �� �������� ������� ��������� ����� �����
	\param[in] tokens ������
	\param[in] pos �������� �������
	\return ������� ������, ���������� �� ������� �������
	\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
	*/
	void expect_name(const std::vector<Token>& tokens, int pos) const;

	/*!
	��������, ��� �� �������� ������� ��������� ����� �����
	\param[in] tokens ������
	\param[in] pos �������� �������
	\return ������� ������, ���������� �� ������� �������
	\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
	*/
	void expect_number(const std::vector<Token>& tokens, int pos) const;

	/*!
	��������� �������� ����� � �������� ������� � ������������ ��� � �����
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] number ����������������� �����
	\return ������� ������, ���������� �������� �������
	\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
	*/
	int extract_number(std::vector<Token> tokens, int pos, int& number) const;

	/*!
	��������� ����� �������� � �������� ������� � ���������� �������, ������� �� ����������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] reg ��������� �������
	\return ������� ������, ���������� ������� ��������
	\throw SyntaxError � ������, ���� �������� �� ��������� �� �������� �������
	*/
	int extract_register(std::vector<Token> tokens, int pos, REGISTER& reg) const;

	/*!
	��������� ����� ����� � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] var_name ��������� ���
	\return ������� ������, ���������� ������� �����
	\throw SyntaxError � ������, ���� ����� �� ��������� �� �������� �������
	*/
	int extract_name(std::vector<Token> tokens, int pos, std::string& var_name) const;

	/*!
	��������� ���������� "add" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] add_instr ��������� ���������� "add"
	\return ������� ������, ���������� �� ����������� 
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_add_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& add_instr) const;

	/*!
	��������� ���������� "sub" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] sub_instr ��������� ���������� "sub"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_sub_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sub_instr) const;

	/*!
	��������� ���������� "and" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] and_instr ��������� ���������� "and"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_and_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& and_instr) const;

	/*!
	��������� ���������� "or" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] or_instr ��������� ���������� "or"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_or_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& or_instr) const;

	/*!
	��������� ���������� "xor" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] or_instr ��������� ���������� "xor"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_xor_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& xor_instr) const;

	/*!
	��������� ���������� "not" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] not_instr ��������� ���������� "not"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_not_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& not_instr) const;

	/*!
	��������� ���������� "shr" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] shr_instr ��������� ���������� "shr"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_shr_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shr_instr) const;

	/*!
	��������� ���������� "shl" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] shl_instr ��������� ���������� "shl"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_shl_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shl_instr) const;

	/*!
	��������� ���������� "set" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] set_instr ��������� ���������� "set"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_set_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& set_instr) const;

	/*!
	��������� ���������� "ld" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] ld_instr ��������� ���������� "ld"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_ld_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ld_instr) const;

	/*!
	��������� ���������� "st" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] st_instr ��������� ���������� "st"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_st_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& st_instr) const;

	/*!
	��������� ���������� "ldi" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] ldi_instr ��������� ���������� "ldi"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_ldi_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ldi_instr) const;

	/*!
	��������� ���������� "sti" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] sti_instr ��������� ���������� "sti"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_sti_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sti_instr) const;

	/*!
	��������� ���������� "jmp" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] jmp_instr ��������� ���������� "jmp"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_jmp_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jmp_instr) const;

	/*!
	��������� ���������� "jeq" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] jeq_instr ��������� ���������� "jeq"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_jeq_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jeq_instr) const;

	/*!
	��������� ���������� "jgt" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] jgt_instr ��������� ���������� "jgt"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_jgt_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jgt_instr) const;

	/*!
	��������� ���������� "call" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] call_instr ��������� ���������� "call"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_call_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& call_instr) const;

	/*!
	��������� ���������� "ret" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] ret_instr ��������� ���������� "ret"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_ret_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ret_instr) const;

	/*!
	��������� ������ ���������� ��� ���������� "data" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] arguments ������ ��������� ����������
	\return ������� ������, ���������� �� ������ ����������
	\throw SyntaxError � ������, ���� ������ ���������� ������� �����������
	*/
	int extract_data_argument_list(const std::vector<Token>& tokens, int pos, std::vector<int>& arguments) const;

	/*!
	��������� ���������� "data" � �������� �������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] data_instr ��������� ���������� "data"
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_data_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& data_instr) const;

	/*!
	��������� ���������� ������-���������� � �������� ������� 
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] instr ��������� ����������
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	void extract_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& instr) const;

	/*!
	��������� ����� �����, ���������� ����� �����������
	\param[in] tokens ������
	\param[in] pos �������� �������
	\param[out] label_names ������ ��������� ���� �����
	\return ������� ������, ���������� �� �����������
	\throw SyntaxError � ������, ���� ���������� �������� ������������� �����������
	*/
	int extract_labels(const std::vector<Token>& tokens, int pos, std::vector<std::string>& label_names) const;

public:

	/*!
	��������� ��������� ��������� �� ����� ������-���������� �� ���������� �������������
	\param[in] input_file ������� ����� ��������
	\param[out] instrs ��������� ����������
	\param[out] instr ��������� �����
	\param[out] tokenizer_errors ������, ��������� �� ����� ����������� ��������
	\param[out] syntax_errors ������, ��������� �� ����� ��������������� ������� ����������
	\return ����, �����������, �������� �� ������ �� ����� �������� ��������
	*/
	bool translate(std::ifstream& input_file, std::vector<std::shared_ptr<Instr>>& instrs, std::map<std::string, int>& labels, std::vector<TokenizerError>& tokenizer_errors, std::vector<SyntaxError>& syntax_errors) const;
};