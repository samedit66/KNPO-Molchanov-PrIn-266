#pragma once

#include <array>
#include <stack>
#include <vector>
#include <string>
#include <map>


const int MEMORY_SIZE = 2048;
const int REGISTER_COUNT = 8;
const int MAX_CALL_STACK_DEPTH = 64;

enum class REGISTER {
	R0,
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	R7,
};

class RuntimeError {
private:
	std::string msg;

public:
	RuntimeError(const std::string& m);

	std::string what() const;
};

class ProgramState
{
private:
	/// ��������
	std::array<int, REGISTER_COUNT> registers{};

	/// ������ ������
	std::array<int, MEMORY_SIZE> memory{};

	/// ������� ����� ��� ����������
	std::map<std::string, int> labels;

	/// ������� ����� ��� ������
	std::map<std::string, int> data_labels;

	/// ���� ��� ������� �����������
	std::stack<int> call_stack;

	/// ������ ������� ����������
	int pc{};

	/// ������ ������ ��������� ������ ������
	int memory_alloc_index{};

	/// ���������� ����������
	int instr_count{};

	/*!
	���������, ����� �� �������������� ����� � �������� ����������� ������ ������
	\param[in] address ����� ��� ��������
	*/
	void check_memory_address(int address);

	/*!
	���������, ����� �� �������������� ��� � �������� ����������� ����� �����
	\param[in] label_name ��� ��� ��������
	*/
	void check_label_name(const std::string& label_name);
	
	/*!
	���������, ����� �� �������������� ��� � �������� ����������� ����� ������ ������
	\param[in] label_name ��� ��� ��������
	*/
	void check_data_label_name(const std::string& data_label_name);

	/*!
	���������, ����� �� �������������� ����� � �������� ����������� ������ ����������
	\param[in] address ����� ��� ��������
	*/
	void check_instr_address(int address);

public:
	ProgramState(int instr_count);

	/*!
	���������� ����, "��������" �� ��� �������������
	\return ����, "��������" �� ��� �������������
	*/
	bool is_running() const;

	/*!
	��������� ������ �� ������
	\param[out] str ����������� ������
	\param[in] address ����� � ������, � �������� ����� ������ ����������
	*/
	void extract_string(std::string& str, int address);

	/*!
	���������� �������� ��������
	\param[in] r �������
	\return �������� ��������
	*/
	int get_register_value(REGISTER r) const;

	/*!
	������������� �������� ��������
	\param[in] r �������
	\param[in] value ��������
	*/
	void set_register_value(REGISTER r, int value);

	/*!
	���������� �������� �� ������ �� �����
	\param[in] name ���
	\return �������� �� ������ �� �����
	*/
	int get_address_of_data_label(const std::string& name);

	/*!
	���������� �������� �� ������ �� �����
	\param[in] name ���
	\return �������� �� ������ �� �����
	*/
	int get_memory_value_by_name(const std::string& name);

	/*!
	������������� �������� � ������ �� �����
	\param[in] name ���
	\param[in] value ��������
	*/
	void set_memory_value_by_name(const std::string& name, int value);

	/*!
	���������� �������� �� ������ �� ������
	\param[in] value �����
	\return �������� �� ������ �� ������
	*/
	int get_memory_value(int address);

	/*!
	������������� �������� � ������ �� ������
	\param[in] address �����
	\param[in] value ��������
	*/
	void set_memory_value(int address, int value);

	/*!
	���������� ����� ����� �� �����
	\param[in] label_name ��� �����
	\return ����� �����
	*/
	int get_label_address(const std::string& label_name);

	/*!
	��������� ����� �����
	\param[in] label_name ��� �����
	\param[in] address ����� �����
	*/
	void add_label(const std::string& label_name, int address);

	/*!
	���������� ������ ������� ����������
	\return ������ ������� ����������
	*/
	int get_pc() const;

	/*!
	������������� ������ ������� ����������
	\param[in] address ������
	*/
	void set_pc(int address);

	/*!
	��������� � ������� ��������� ����������
	*/
	void inc_pc();

	/*!
	�������� ���������� ��� ������������ ������������� ������������
	\param[in] label_name ��� ������������
	\throw RuntimeError � ������, ���� ���� ������� ������� ����������
	*/
	void call_subroutine(const std::string& subroutione_name);

	/*!
	���������� ���������� ������������
	\throw RuntimeError � ������, ���� ���� ������� ���������� ��������� ������������ ��� �����-���� ������������
	*/
	void return_from_subroutine();

	/*!
	�������� ������ ��� ������
	\param[in] data_label_name ����� ������ ������
	\throw RuntimeError � ������, ���� ��� ������ ������ ��� ����������, ��� ���� �� ������� ������
	*/
	void allocate_memory(const std::string& data_label_name, const std::vector<int>& data);
};