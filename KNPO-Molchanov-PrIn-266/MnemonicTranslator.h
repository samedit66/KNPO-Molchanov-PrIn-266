#pragma once

#include <vector>
#include <map>
#include <string>

#include "Tokenizer.h"
#include "Instruction.h"

/*!
Класс, описывающий ошибку, возникающую в случае, если
инструкция была записана некорректно
*/
class SyntaxError {
private:
	/// Сообщение об ошибке
	std::string msg;

public:
	/*!
	Конструктор ошибки
	\param[in] m Сообщение об ошибке
	*/
	SyntaxError(const std::string& m);

	/*
	Возвращает сообщение об ошибке
	\return сообщение об ошибке
	*/
	std::string what() const;

	/*!
	Меняет сообщение об ошибке
	\param[in] m Новое сообщение об ошибке
	*/
	void change_error_message(const std::string& m);
};

/*
Транслятор текстовых мнемоник псевдо-ассмеблера во внутреннее представление 
*/
class MnemonicTranslator
{
private:
	/*!
	Проверяет, что токен на заданной позиции является требуемой командной
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[in] command_type Требуемый тип команды
	\param[in] command_name Имя команды для исключения, в случае если команды не было найдено
	\return Позиция токена, следующего за искомым токеном
	\throw SyntaxError В случае, если искомого токена не оказалось на заданной позици
	*/
	int check_command(std::vector<Token> tokens, int pos, TOKEN_TYPE command_type, const std::string& command_name) const;

	/*!
	Проверяет, что токен на заданной позиции является запятой
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\return Позиция токена, следующего за искомым токеном
	\throw SyntaxError В случае, если запятой не оказалось на заданной позиции
	*/
	int check_comma(const std::vector<Token>& tokens, int pos) const;

	/*!
	Проверят, что на заданной позиции находится токен регистра
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\return Позиция токена, следующего за искомым токеном
	\throw SyntaxError В случае, если регистра не оказалось на заданной позиции
	*/
	void expect_register(const std::vector<Token>& tokens, int pos) const;

	/*!
	Проверят, что на заданной позиции находится токен имени
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\return Позиция токена, следующего за искомым токеном
	\throw SyntaxError В случае, если имени не оказалось на заданной позиции
	*/
	void expect_name(const std::vector<Token>& tokens, int pos) const;

	/*!
	Проверят, что на заданной позиции находится токен числа
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\return Позиция токена, следующего за искомым токеном
	\throw SyntaxError В случае, если числа не оказалось на заданной позиции
	*/
	void expect_number(const std::vector<Token>& tokens, int pos) const;

	/*!
	Извлекает числовой токен с заданной позиции и конвертирует его в число
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] number Сконвертированное число
	\return Позиция токена, следующего числовым токеном
	\throw SyntaxError В случае, если числа не оказалось на заданной позиции
	*/
	int extract_number(std::vector<Token> tokens, int pos, int& number) const;

	/*!
	Извлекает токен регистра с заданной позиции и определяет регистр, который он обозначает
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] reg Считанный регистр
	\return Позиция токена, следующего токеном регистра
	\throw SyntaxError В случае, если регистра не оказалось на заданной позиции
	*/
	int extract_register(std::vector<Token> tokens, int pos, REGISTER& reg) const;

	/*!
	Извлекает токен имени с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] var_name Считанное имя
	\return Позиция токена, следующего токеном имени
	\throw SyntaxError В случае, если имени не оказалось на заданной позиции
	*/
	int extract_name(std::vector<Token> tokens, int pos, std::string& var_name) const;

	/*!
	Извлекает инструкцию "add" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] add_instr Считанная инструкция "add"
	\return Позиция токена, следующего за инструкцией 
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_add_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& add_instr) const;

	/*!
	Извлекает инструкцию "sub" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] sub_instr Считанная инструкция "sub"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_sub_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sub_instr) const;

	/*!
	Извлекает инструкцию "and" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] and_instr Считанная инструкция "and"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_and_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& and_instr) const;

	/*!
	Извлекает инструкцию "or" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] or_instr Считанная инструкция "or"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_or_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& or_instr) const;

	/*!
	Извлекает инструкцию "xor" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] or_instr Считанная инструкция "xor"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_xor_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& xor_instr) const;

	/*!
	Извлекает инструкцию "not" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] not_instr Считанная инструкция "not"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_not_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& not_instr) const;

	/*!
	Извлекает инструкцию "shr" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] shr_instr Считанная инструкция "shr"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_shr_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shr_instr) const;

	/*!
	Извлекает инструкцию "shl" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] shl_instr Считанная инструкция "shl"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_shl_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& shl_instr) const;

	/*!
	Извлекает инструкцию "set" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] set_instr Считанная инструкция "set"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_set_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& set_instr) const;

	/*!
	Извлекает инструкцию "ld" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] ld_instr Считанная инструкция "ld"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_ld_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ld_instr) const;

	/*!
	Извлекает инструкцию "st" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] st_instr Считанная инструкция "st"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_st_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& st_instr) const;

	/*!
	Извлекает инструкцию "ldi" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] ldi_instr Считанная инструкция "ldi"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_ldi_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ldi_instr) const;

	/*!
	Извлекает инструкцию "sti" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] sti_instr Считанная инструкция "sti"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_sti_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& sti_instr) const;

	/*!
	Извлекает инструкцию "jmp" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] jmp_instr Считанная инструкция "jmp"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_jmp_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jmp_instr) const;

	/*!
	Извлекает инструкцию "jeq" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] jeq_instr Считанная инструкция "jeq"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_jeq_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jeq_instr) const;

	/*!
	Извлекает инструкцию "jgt" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] jgt_instr Считанная инструкция "jgt"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_jgt_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& jgt_instr) const;

	/*!
	Извлекает инструкцию "call" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] call_instr Считанная инструкция "call"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_call_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& call_instr) const;

	/*!
	Извлекает инструкцию "ret" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] ret_instr Считанная инструкция "ret"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_ret_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& ret_instr) const;

	/*!
	Извлекает список аргументов для инструкции "data" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] arguments Список считанных аргументов
	\return Позиция токена, следующего за список аргументов
	\throw SyntaxError В случае, если список аргументов записан неправильно
	*/
	int extract_data_argument_list(const std::vector<Token>& tokens, int pos, std::vector<int>& arguments) const;

	/*!
	Извлекает инструкцию "data" с заданной позиции
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] data_instr Считанная инструкция "data"
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_data_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& data_instr) const;

	/*!
	Извлекает инструкцию псевдо-ассемблера с заданной позиции 
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] instr Считанная инструкция
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	void extract_instr(const std::vector<Token>& tokens, int pos, std::shared_ptr<Instr>& instr) const;

	/*!
	Извлекает имена меток, записанные перед инструкцией
	\param[in] tokens Токены
	\param[in] pos Заданная позиция
	\param[out] label_names Список считанных имен меток
	\return Позиция токена, следующего за инструкцией
	\throw SyntaxError В случае, если инструкция записана синтаксически неправильно
	*/
	int extract_labels(const std::vector<Token>& tokens, int pos, std::vector<std::string>& label_names) const;

public:

	/*!
	Переводит текстовые мнемоники на языке псевдо-ассемблера во внутреннее представление
	\param[in] input_file Входной поток мнемоник
	\param[out] instrs Считанные инструкции
	\param[out] instr Считанные метки
	\param[out] tokenizer_errors Ошибки, возникшие во время токенезации мнемоник
	\param[out] syntax_errors Ошибки, возникшие во время синтаксического разбора инструкций
	\return Флаг, указывающий, возникли ли ошибки во время перевода мнемоник
	*/
	bool translate(std::ifstream& input_file, std::vector<std::shared_ptr<Instr>>& instrs, std::map<std::string, int>& labels, std::vector<TokenizerError>& tokenizer_errors, std::vector<SyntaxError>& syntax_errors) const;
};