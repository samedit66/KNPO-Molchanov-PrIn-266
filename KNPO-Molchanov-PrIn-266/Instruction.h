#pragma once

#include <string>
#include <vector>

#include "ProgramState.h"

/*!
Базовый абстрактный класс для каждой инструкции псевдо-ассемблера
*/
class Instr
{
private:
	/// Номер строки, на которой находится инструкция
	int line_number = 0;

public:
	/*
	Выполняет инструкцию псевдо-ассемблера
	\param[in|out] state Состояние программы
	*/
	virtual void execute(ProgramState& state) const = 0;

	/*
	Устанавливает номер строки, на которой расположена инструкция
	\param[in] new_line_number Номер строки
	*/
	void set_line_number(int new_line_number);

	/*
	Возрвращает, на которой расположена инструкция
	\return Номер строки
	*/
	int get_line_number() const;
};

/*!
Класс регистрового варианта инструкции "add" псевдо-ассемблера
*/
class AddRegInstr : public Instr {
private:
	/// Регистр приемник данных
	REGISTER dest;

	/// Регистр источник данных
	REGISTER src;

public:
	AddRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
}; 

/*!
Класс числового варианта инструкции "add" псевдо-ассемблера
*/
class AddImmInstr : public Instr {
private:
	/// Регистр приемник данных
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	AddImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс регистрового варианта инструкции "sub" псевдо-ассемблера
*/
class SubRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	SubRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "add" псевдо-ассемблера
*/
class SubImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	SubImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс регистрового варианта инструкции "and" псевдо-ассемблера
*/
class AndRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	AndRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "and" псевдо-ассемблера
*/
class AndImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	AndImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс регистрового варианта инструкции "or" псевдо-ассемблера
*/
class OrRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	OrRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "or" псевдо-ассемблера
*/
class OrImmInstr : public Instr {
private:
	/// Регистр прмиеник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	OrImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};


/*!
Класс регистрового варианта инструкции "xor" псевдо-ассемблера
*/
class XorRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	XorRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "xor" псевдо-ассемблера
*/
class XorImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	XorImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс инструкции "not" псевдо-ассемблера
*/
class NotInstr : public Instr {
private:
	/// Регистр приемник-источник
	REGISTER reg;

public:
	NotInstr(REGISTER reg);

	void execute(ProgramState& state) const override;

	REGISTER get_reg();
};

/*!
Класс регистрового варианта инструкции "shr" псевдо-ассемблера
*/
class ShrRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	ShrRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "shr" псевдо-ассемблера
*/
class ShrImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	ShrImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс регистрового варианта инструкции "shl" псевдо-ассемблера
*/
class ShlRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	ShlRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "shl" псевдо-ассемблера
*/
class ShlImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	ShlImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс регистрового варианта инструкции "set" псевдо-ассемблера
*/
class SetRegInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	SetRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс числового варианта инструкции "set" псевдо-ассемблера
*/
class SetImmInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Непосредственный числовой аргумент
	int imm_value;

public:
	SetImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
Класс варианта инструкции "set" с именем ячейки памяти псевдо-ассемблера
*/
class SetNameInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Имя ячейки памяти
	std::string var_name;

public:
	SetNameInstr(REGISTER dest, const std::string& var_name);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	std::string get_var_name() const;
};

/*!
Класс инструкции "ld" псевдо-ассемблера
*/
class LdInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Имя ячейки памяти
	std::string var_name;

public:
	LdInstr(REGISTER dest, const std::string& var_name);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	std::string get_var_name() const;
};

/*!
Класс инструкции "st" псевдо-ассемблера
*/
class StInstr : public Instr {
private:
	/// Имя ячейки памяти
	std::string var_name;

	/// Регистр источник
	REGISTER src;

public:
	StInstr(const std::string& var_name, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_src() const;

	std::string get_var_name() const;
};

/*!
Класс инструкции "ldi" псевдо-ассемблера
*/
class LdiInstr : public Instr {
private:
	/// Регистр приемник
	REGISTER dest;

	/// Регистр, содержащий адрес ячейки памяти
	REGISTER src;

public:
	LdiInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс инструкции "sti" псевдо-ассемблера
*/
class StiInstr : public Instr {
private:
	/// Регистр, содержащий адрес ячейки памяти
	REGISTER dest;

	/// Регистр источник
	REGISTER src;

public:
	StiInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
Класс инструкции "jmp" псевдо-ассемблера
*/
class JmpInstr : public Instr {
private:
	/// Имя метки
	std::string label_name;

public:
	JmpInstr(const std::string& label_name);

	void execute(ProgramState& state) const override;

	std::string get_label_name();
};

/*!
Класс инструкции "jeq" псевдо-ассемблера
*/
class JeqInstr : public Instr {
private:
	/// Имя метки
	std::string label_name;

	/// Регистр источник первого операнда
	REGISTER src1;

	/// Регистр источник второго операнда
	REGISTER src2;

public:
	JeqInstr(const std::string& label_name, REGISTER src1, REGISTER src2);

	void execute(ProgramState& state) const override;

	REGISTER get_src1() const;

	REGISTER get_src2() const;

	std::string get_label_name();
};


/*!
Класс инструкции "jgt" псевдо-ассемблера
*/
class JgtInstr : public Instr {
private:
	/// Имя метки
	std::string label_name;

	/// Регистр источник первого операнда
	REGISTER src1;

	/// Регистр источник второго операнда
	REGISTER src2;

public:
	JgtInstr(const std::string& label_name, REGISTER src1, REGISTER src2);

	void execute(ProgramState& state) const override;

	REGISTER get_src1() const;

	REGISTER get_src2() const;

	std::string get_label_name();
};

/*!
Класс инструкции "call" псевдо-ассемблера
*/
class CallInstr : public Instr {
private:
	/// Имя подпрограммы
	std::string subroutine_name;

public:
	CallInstr(const std::string& subroutine_name);

	void execute(ProgramState& state) const override;

	std::string get_subroutine_name();
};

/*!
Класс инструкции "ret" псевдо-ассемблера
*/
class RetInstr : public Instr {
public:
	RetInstr();

	void execute(ProgramState& state) const override;
};

/*!
Класс инструкции "data" псевдо-ассемблера
*/
class DataInstr : public Instr {
private:
	std::string data_label_name;
	std::vector<int> data;

public:
	DataInstr(const std::string& data_label_name, const std::vector<int> data);

	void execute(ProgramState& state) const override;

	std::string get_data_label_name();

	std::vector<int> get_data();
};