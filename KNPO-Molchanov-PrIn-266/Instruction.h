#pragma once

#include <string>
#include <vector>

#include "ProgramState.h"

/*!
������� ����������� ����� ��� ������ ���������� ������-����������
*/
class Instr
{
private:
	/// ����� ������, �� ������� ��������� ����������
	int line_number = 0;

public:
	/*
	��������� ���������� ������-����������
	\param[in|out] state ��������� ���������
	*/
	virtual void execute(ProgramState& state) const = 0;

	/*
	������������� ����� ������, �� ������� ����������� ����������
	\param[in] new_line_number ����� ������
	*/
	void set_line_number(int new_line_number);

	/*
	�����������, �� ������� ����������� ����������
	\return ����� ������
	*/
	int get_line_number() const;
};

/*!
����� ������������ �������� ���������� "add" ������-����������
*/
class AddRegInstr : public Instr {
private:
	/// ������� �������� ������
	REGISTER dest;

	/// ������� �������� ������
	REGISTER src;

public:
	AddRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
}; 

/*!
����� ��������� �������� ���������� "add" ������-����������
*/
class AddImmInstr : public Instr {
private:
	/// ������� �������� ������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	AddImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ������������ �������� ���������� "sub" ������-����������
*/
class SubRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	SubRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "add" ������-����������
*/
class SubImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	SubImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ������������ �������� ���������� "and" ������-����������
*/
class AndRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	AndRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "and" ������-����������
*/
class AndImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	AndImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ������������ �������� ���������� "or" ������-����������
*/
class OrRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	OrRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "or" ������-����������
*/
class OrImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	OrImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};


/*!
����� ������������ �������� ���������� "xor" ������-����������
*/
class XorRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	XorRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "xor" ������-����������
*/
class XorImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	XorImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ���������� "not" ������-����������
*/
class NotInstr : public Instr {
private:
	/// ������� ��������-��������
	REGISTER reg;

public:
	NotInstr(REGISTER reg);

	void execute(ProgramState& state) const override;

	REGISTER get_reg();
};

/*!
����� ������������ �������� ���������� "shr" ������-����������
*/
class ShrRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	ShrRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "shr" ������-����������
*/
class ShrImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	ShrImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ������������ �������� ���������� "shl" ������-����������
*/
class ShlRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	ShlRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "shl" ������-����������
*/
class ShlImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	ShlImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� ������������ �������� ���������� "set" ������-����������
*/
class SetRegInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	SetRegInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ��������� �������� ���������� "set" ������-����������
*/
class SetImmInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ���������������� �������� ��������
	int imm_value;

public:
	SetImmInstr(REGISTER dest, int imm_value);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	int get_imm_value() const;
};

/*!
����� �������� ���������� "set" � ������ ������ ������ ������-����������
*/
class SetNameInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ��� ������ ������
	std::string var_name;

public:
	SetNameInstr(REGISTER dest, const std::string& var_name);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	std::string get_var_name() const;
};

/*!
����� ���������� "ld" ������-����������
*/
class LdInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// ��� ������ ������
	std::string var_name;

public:
	LdInstr(REGISTER dest, const std::string& var_name);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	std::string get_var_name() const;
};

/*!
����� ���������� "st" ������-����������
*/
class StInstr : public Instr {
private:
	/// ��� ������ ������
	std::string var_name;

	/// ������� ��������
	REGISTER src;

public:
	StInstr(const std::string& var_name, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_src() const;

	std::string get_var_name() const;
};

/*!
����� ���������� "ldi" ������-����������
*/
class LdiInstr : public Instr {
private:
	/// ������� ��������
	REGISTER dest;

	/// �������, ���������� ����� ������ ������
	REGISTER src;

public:
	LdiInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ���������� "sti" ������-����������
*/
class StiInstr : public Instr {
private:
	/// �������, ���������� ����� ������ ������
	REGISTER dest;

	/// ������� ��������
	REGISTER src;

public:
	StiInstr(REGISTER dest, REGISTER src);

	void execute(ProgramState& state) const override;

	REGISTER get_dest() const;

	REGISTER get_src() const;
};

/*!
����� ���������� "jmp" ������-����������
*/
class JmpInstr : public Instr {
private:
	/// ��� �����
	std::string label_name;

public:
	JmpInstr(const std::string& label_name);

	void execute(ProgramState& state) const override;

	std::string get_label_name();
};

/*!
����� ���������� "jeq" ������-����������
*/
class JeqInstr : public Instr {
private:
	/// ��� �����
	std::string label_name;

	/// ������� �������� ������� ��������
	REGISTER src1;

	/// ������� �������� ������� ��������
	REGISTER src2;

public:
	JeqInstr(const std::string& label_name, REGISTER src1, REGISTER src2);

	void execute(ProgramState& state) const override;

	REGISTER get_src1() const;

	REGISTER get_src2() const;

	std::string get_label_name();
};


/*!
����� ���������� "jgt" ������-����������
*/
class JgtInstr : public Instr {
private:
	/// ��� �����
	std::string label_name;

	/// ������� �������� ������� ��������
	REGISTER src1;

	/// ������� �������� ������� ��������
	REGISTER src2;

public:
	JgtInstr(const std::string& label_name, REGISTER src1, REGISTER src2);

	void execute(ProgramState& state) const override;

	REGISTER get_src1() const;

	REGISTER get_src2() const;

	std::string get_label_name();
};

/*!
����� ���������� "call" ������-����������
*/
class CallInstr : public Instr {
private:
	/// ��� ������������
	std::string subroutine_name;

public:
	CallInstr(const std::string& subroutine_name);

	void execute(ProgramState& state) const override;

	std::string get_subroutine_name();
};

/*!
����� ���������� "ret" ������-����������
*/
class RetInstr : public Instr {
public:
	RetInstr();

	void execute(ProgramState& state) const override;
};

/*!
����� ���������� "data" ������-����������
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