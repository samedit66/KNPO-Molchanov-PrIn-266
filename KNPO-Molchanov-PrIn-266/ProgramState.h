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

/*!
Класс, представляющий внутренней состояние программы
*/
class ProgramState
{
private:
	/// Регистры
	std::array<int, REGISTER_COUNT> registers{};

	/// Ячейки памяти
	std::array<int, MEMORY_SIZE> memory{};

	/// Таблица меток для инструкций
	std::map<std::string, int> labels;

	/// Таблица меток для данных
	std::map<std::string, int> data_labels;

	/// Стек для вызовов подпрограмм
	std::stack<int> call_stack;

	/// Индекс текущей инструкции
	int pc{};

	/// Индекс первой свободной ячейки памяти
	int memory_alloc_index{};

	/// Количество инструкций
	int instr_count{};

	/*!
	Проверяет, может ли использоваться адрес в качестве допустимого адреса памяти
	\param[in] address Адрес для проверки
	*/
	void check_memory_address(int address);

	/*!
	Проверяет, может ли использоваться имя в качестве допустимого имени метки
	\param[in] label_name Имя для проверки
	*/
	void check_label_name(const std::string& label_name);
	
	/*!
	Проверяет, может ли использоваться имя в качестве допустимого имени ячейки памяти
	\param[in] label_name Имя для проверки
	*/
	void check_data_label_name(const std::string& data_label_name);

	/*!
	Проверяет, может ли использоваться адрес в качестве допустимого адреса инструкции
	\param[in] address Адрес для проверки
	*/
	void check_instr_address(int address);

public:
	ProgramState(int instr_count);

	/*!
	Возвращает флаг, "работает" ли ещё интерпретатор
	\return Флаг, "работает" ли ещё интерпретатор
	*/
	bool is_running() const;

	/*!
	Извлекает строку из памяти
	\param[out] str Извлеченная строка
	\param[in] address Адрес в памяти, с которого нужно начать извлечение
	*/
	void extract_string(std::string& str, int address);

	/*!
	Возвращает значение регистра
	\param[in] r Регистр
	\return Значение регистра
	*/
	int get_register_value(REGISTER r) const;

	/*!
	Устанавливает значение регистра
	\param[in] r Регистр
	\param[in] value Значение
	*/
	void set_register_value(REGISTER r, int value);

	/*!
	Возвращает значение из памяти по имени
	\param[in] name Имя
	\return Значение из памяти по имени
	*/
	int get_address_of_data_label(const std::string& name);

	/*!
	Возвращает значение из памяти по имени
	\param[in] name Имя
	\return Значение из памяти по имени
	*/
	int get_memory_value_by_name(const std::string& name);

	/*!
	Устанавливает значение в памяти по имени
	\param[in] name Имя
	\param[in] value Значение
	*/
	void set_memory_value_by_name(const std::string& name, int value);

	/*!
	Возвращает значение из памяти по адресу
	\param[in] value Адрес
	\return Значение из памяти по адресу
	*/
	int get_memory_value(int address);

	/*!
	Устанавливает значение в памяти по адресу
	\param[in] address Адрес
	\param[in] value Значение
	*/
	void set_memory_value(int address, int value);

	/*!
	Возвращает адрес метки по имеми
	\param[in] label_name Имя метки
	\return Адрес метки
	*/
	int get_label_address(const std::string& label_name);

	/*!
	Добавляет новую метку
	\param[in] label_name Имя метки
	\param[in] address Адрес метки
	*/
	void add_label(const std::string& label_name, int address);

	/*!
	Возвращает индекс текущей инструкции
	\return Индекс текущей инструкции
	*/
	int get_pc() const;

	/*!
	Устанавливает индекс текущей инструкции
	\param[in] address Индекс
	*/
	void set_pc(int address);

	/*!
	Переходит к индексу следующей инструкции
	*/
	void inc_pc();

	/*!
	Вызывает встроенную или определенную пользователем подпрограмму
	\param[in] label_name Имя подпрограммы
	\throw RuntimeError В случае, если стек вызовов функции переполнен
	*/
	void call_subroutine(const std::string& subroutione_name);

	/*!
	Прекращает выполнение подпрограммы
	\throw RuntimeError В случае, если была попытка прекратить выполение подпрограммы вне какой-либо подпрограммы
	*/
	void return_from_subroutine();

	/*!
	Выделяет память для данных
	\param[in] data_label_name Метка ячейки памяти
	\throw RuntimeError В случае, если имя ячейки памяти уже определено, или если не хватает памяти
	*/
	void allocate_memory(const std::string& data_label_name, const std::vector<int>& data);
};