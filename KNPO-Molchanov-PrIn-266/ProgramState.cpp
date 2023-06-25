#include <iostream>
#include <string>

#include "ProgramState.h"


RuntimeError::RuntimeError(const std::string& m) {
	msg = m;
}

std::string RuntimeError::what() const {
	return msg;
}


ProgramState::ProgramState(int n) {
	instr_count = n;
	set_pc(0);
}

/*!
���������, ����� �� �������������� ����� � �������� ����������� ������ ������
\param[in] address ����� ��� ��������
*/
void ProgramState::check_memory_address(int address) {
	if (address < 0 || address >= MEMORY_SIZE) {
		throw RuntimeError("������������ ����� \"" + std::to_string(address) + "\"");
	}
}

/*!
���������, ����� �� �������������� ��� � �������� ����������� ����� �����
\param[in] label_name ��� ��� ��������
*/
void ProgramState::check_label_name(const std::string& label_name) {
	if (labels.count(label_name) == 0) {
		throw RuntimeError("����������� ����� \"" + label_name + "\"");
	}
}

/*!
���������, ����� �� �������������� ��� � �������� ����������� ����� ������ ������
\param[in] label_name ��� ��� ��������
*/
void ProgramState::check_data_label_name(const std::string& data_label_name) {
	if (data_labels.count(data_label_name) == 0) {
		throw RuntimeError("����������� ��� ������ ������ \"" + data_label_name + "\"");
	}
}

/*!
���������, ����� �� �������������� ����� � �������� ����������� ������ ����������
\param[in] address ����� ��� ��������
*/
void ProgramState::check_instr_address(int address) {
	if (address < 0 || address >= instr_count) {
		throw RuntimeError("����������� ����� ���������� \"" + std::to_string(address) + "\"");
	}
}

/*!
���������� ����, "��������" �� ��� �������������
\return ����, "��������" �� ��� �������������
*/
bool ProgramState::is_running() const {
	return pc < instr_count;
}

/*!
���������� �������� ��������
\param[in] r �������
\return �������� ��������
*/
int ProgramState::get_register_value(REGISTER r) const {
	return registers.at((int)r);
}

/*!
������������� �������� ��������
\param[in] r �������
\param[in] value ��������
*/
void ProgramState::set_register_value(REGISTER r, int value) {
	registers[(int)r] = value;
}

/*!
���������� �������� �� ������ �� �����
\param[in] name ���
\return �������� �� ������ �� �����
*/
int ProgramState::get_address_of_data_label(const std::string& name) {
	check_data_label_name(name);
	return data_labels.at(name);
}

/*!
���������� �������� �� ������ �� �����
\param[in] name ���
\return �������� �� ������ �� �����
*/
int ProgramState::get_memory_value_by_name(const std::string& name) {
	check_data_label_name(name);
	return memory[data_labels.at(name)];
}

/*!
������������� �������� � ������ �� �����
\param[in] name ���
\param[in] value ��������
*/
void ProgramState::set_memory_value_by_name(const std::string& name, int value) {
	check_data_label_name(name);
	data_labels[name] = value;
}

/*!
���������� �������� �� ������ �� ������
\param[in] value �����
\return �������� �� ������ �� ������
*/
int ProgramState::get_memory_value(int address) {
	check_memory_address(address);
	return memory[address];
}

/*!
������������� �������� � ������ �� ������
\param[in] address �����
\param[in] value ��������
*/
void ProgramState::set_memory_value(int address, int value) {
	check_memory_address(address);
	memory[address] = value;
}

/*!
���������� ����� ����� �� �����
\param[in] label_name ��� �����
\return ����� �����
*/
int ProgramState::get_label_address(const std::string& label_name) {
	check_label_name(label_name);
	return labels.at(label_name);
}

/*!
��������� ����� �����
\param[in] label_name ��� �����
\param[in] address ����� �����
*/
void ProgramState::add_label(const std::string& label_name, int address) {
	labels[label_name] = address;
}

/*!
���������� ������ ������� ����������
\return ������ ������� ����������
*/
int ProgramState::get_pc() const {
	return pc;
}

/*!
������������� ������ ������� ����������
\param[in] address ������
*/
void ProgramState::set_pc(int address) {
	pc = address;
}

/*!
��������� � ������� ��������� ����������
*/
void ProgramState::inc_pc() {
	pc++;
}

/*!
�������� ���������� ��� ������������ ������������� ������������
\param[in] label_name ��� ������������
\throw RuntimeError � ������, ���� ���� ������� ������� ����������
*/
void ProgramState::call_subroutine(const std::string& subroutione_name) {
	if (call_stack.size() == MAX_CALL_STACK_DEPTH) {
		throw RuntimeError("������� ����� ����������� �������");
	}

	if (subroutione_name == "putc") {
		char r0_value = (char)get_register_value(REGISTER::R0);
		std::cout << r0_value << std::endl;
		inc_pc();
	}
	else if (subroutione_name == "puts") {
		int str_address = get_register_value(REGISTER::R0);

		char c = get_memory_value(str_address);
		while (str_address < MEMORY_SIZE && c != '\0') {
			std::cout << c;
			str_address++;
			c = get_memory_value(str_address);
		}
		
		if (c != '\0') {
			throw RuntimeError("�� ������ ����� ������");
		}

		std::cout << std::endl;
		inc_pc();
	}
	else if (subroutione_name == "getc") {
		char input;
		std::cin >> input;
		set_register_value(REGISTER::R0, input);
		inc_pc();
	}
	else {
		check_label_name(subroutione_name);
		int address = labels.at(subroutione_name);
		call_stack.push(get_pc() + 1);
		set_pc(address);
	}
}

/*!
���������� ���������� ������������
\throw RuntimeError � ������, ���� ���� ������� ���������� ��������� ������������ ��� �����-���� ������������
*/
void ProgramState::return_from_subroutine() {
	if (call_stack.size() == 0) {
		throw RuntimeError("������������� ������������ ����������");
	}

	set_pc(call_stack.top());
	call_stack.pop();
}

/*!
�������� ������ ��� ������
\param[in] data_label_name ����� ������ ������
\throw RuntimeError � ������, ���� ��� ������ ������ ��� ����������, ��� ���� �� ������� ������
*/
void ProgramState::allocate_memory(const std::string& data_label_name, const std::vector<int>& data) {
	if (data_labels.count(data_label_name) > 0) {
		throw RuntimeError("��� ���������� �� ����� ����������� \"" + data_label_name + "\"");
	}

	data_labels[data_label_name] = memory_alloc_index;

	int i = 0;
	while (memory_alloc_index < MEMORY_SIZE && i < data.size()) {
		set_memory_value(memory_alloc_index, data[i]);
		memory_alloc_index++;
		i++;
	}

	if (memory_alloc_index == MEMORY_SIZE && i != data.size()) {
		throw RuntimeError("�� ������� ������ ��� ������ ���� ��������");
	}
}