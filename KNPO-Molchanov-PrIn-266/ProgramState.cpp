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
Проверяет, может ли использоваться адрес в качестве допустимого адреса памяти
\param[in] address Адрес для проверки
*/
void ProgramState::check_memory_address(int address) {
	if (address < 0 || address >= MEMORY_SIZE) {
		throw RuntimeError("Недопустимый адрес \"" + std::to_string(address) + "\"");
	}
}

/*!
Проверяет, может ли использоваться имя в качестве допустимого имени метки
\param[in] label_name Имя для проверки
*/
void ProgramState::check_label_name(const std::string& label_name) {
	if (labels.count(label_name) == 0) {
		throw RuntimeError("Неизвестная метка \"" + label_name + "\"");
	}
}

/*!
Проверяет, может ли использоваться имя в качестве допустимого имени ячейки памяти
\param[in] label_name Имя для проверки
*/
void ProgramState::check_data_label_name(const std::string& data_label_name) {
	if (data_labels.count(data_label_name) == 0) {
		throw RuntimeError("Неизвестное имя ячейки памяти \"" + data_label_name + "\"");
	}
}

/*!
Проверяет, может ли использоваться адрес в качестве допустимого адреса инструкции
\param[in] address Адрес для проверки
*/
void ProgramState::check_instr_address(int address) {
	if (address < 0 || address >= instr_count) {
		throw RuntimeError("Некорретный адрес инструкции \"" + std::to_string(address) + "\"");
	}
}

/*!
Возвращает флаг, "работает" ли ещё интерпретатор
\return Флаг, "работает" ли ещё интерпретатор
*/
bool ProgramState::is_running() const {
	return pc < instr_count;
}

/*!
Возвращает значение регистра
\param[in] r Регистр
\return Значение регистра
*/
int ProgramState::get_register_value(REGISTER r) const {
	return registers.at((int)r);
}

/*!
Устанавливает значение регистра
\param[in] r Регистр
\param[in] value Значение
*/
void ProgramState::set_register_value(REGISTER r, int value) {
	registers[(int)r] = value;
}

/*!
Возвращает значение из памяти по имени
\param[in] name Имя
\return Значение из памяти по имени
*/
int ProgramState::get_address_of_data_label(const std::string& name) {
	check_data_label_name(name);
	return data_labels.at(name);
}

/*!
Возвращает значение из памяти по имени
\param[in] name Имя
\return Значение из памяти по имени
*/
int ProgramState::get_memory_value_by_name(const std::string& name) {
	check_data_label_name(name);
	return memory[data_labels.at(name)];
}

/*!
Устанавливает значение в памяти по имени
\param[in] name Имя
\param[in] value Значение
*/
void ProgramState::set_memory_value_by_name(const std::string& name, int value) {
	check_data_label_name(name);
	data_labels[name] = value;
}

/*!
Возвращает значение из памяти по адресу
\param[in] value Адрес
\return Значение из памяти по адресу
*/
int ProgramState::get_memory_value(int address) {
	check_memory_address(address);
	return memory[address];
}

/*!
Устанавливает значение в памяти по адресу
\param[in] address Адрес
\param[in] value Значение
*/
void ProgramState::set_memory_value(int address, int value) {
	check_memory_address(address);
	memory[address] = value;
}

/*!
Возвращает адрес метки по имеми
\param[in] label_name Имя метки
\return Адрес метки
*/
int ProgramState::get_label_address(const std::string& label_name) {
	check_label_name(label_name);
	return labels.at(label_name);
}

/*!
Добавляет новую метку
\param[in] label_name Имя метки
\param[in] address Адрес метки
*/
void ProgramState::add_label(const std::string& label_name, int address) {
	labels[label_name] = address;
}

/*!
Возвращает индекс текущей инструкции
\return Индекс текущей инструкции
*/
int ProgramState::get_pc() const {
	return pc;
}

/*!
Устанавливает индекс текущей инструкции
\param[in] address Индекс
*/
void ProgramState::set_pc(int address) {
	pc = address;
}

/*!
Переходит к индексу следующей инструкции
*/
void ProgramState::inc_pc() {
	pc++;
}

/*!
Вызывает встроенную или определенную пользователем подпрограмму
\param[in] label_name Имя подпрограммы
\throw RuntimeError В случае, если стек вызовов функции переполнен
*/
void ProgramState::call_subroutine(const std::string& subroutione_name) {
	if (call_stack.size() == MAX_CALL_STACK_DEPTH) {
		throw RuntimeError("Слишком много подпрограмм вызвано");
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
			throw RuntimeError("Не найден конец строки");
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
Прекращает выполнение подпрограммы
\throw RuntimeError В случае, если была попытка прекратить выполение подпрограммы вне какой-либо подпрограммы
*/
void ProgramState::return_from_subroutine() {
	if (call_stack.size() == 0) {
		throw RuntimeError("Выполняющиеся подпрограммы отсуствуют");
	}

	set_pc(call_stack.top());
	call_stack.pop();
}

/*!
Выделяет память для данных
\param[in] data_label_name Метка ячейки памяти
\throw RuntimeError В случае, если имя ячейки памяти уже определено, или если не хватает памяти
*/
void ProgramState::allocate_memory(const std::string& data_label_name, const std::vector<int>& data) {
	if (data_labels.count(data_label_name) > 0) {
		throw RuntimeError("Имя переменной не может повторяться \"" + data_label_name + "\"");
	}

	data_labels[data_label_name] = memory_alloc_index;

	int i = 0;
	while (memory_alloc_index < MEMORY_SIZE && i < data.size()) {
		set_memory_value(memory_alloc_index, data[i]);
		memory_alloc_index++;
		i++;
	}

	if (memory_alloc_index == MEMORY_SIZE && i != data.size()) {
		throw RuntimeError("Не хватает памяти для записи всех значений");
	}
}