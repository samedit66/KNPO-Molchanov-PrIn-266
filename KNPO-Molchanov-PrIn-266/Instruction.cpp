#include <string>
#include <vector>
#include <iostream>

#include "Instruction.h"
#include "ProgramState.h"


/*
Устанавливает номер строки, на которой расположена инструкция
\param[in] new_line_number Номер строки
*/
void Instr::set_line_number(int new_line_number) {
	line_number = new_line_number;
}

/*
Возрвращает, на которой расположена инструкция
\return Номер строки
*/
int Instr::get_line_number() const {
	return line_number;
}


AddRegInstr::AddRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void AddRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) + state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER AddRegInstr::get_dest() const {
	return dest;
}

REGISTER AddRegInstr::get_src() const {
	return src;
}


AddImmInstr::AddImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void AddImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) + imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER AddImmInstr::get_dest() const {
	return dest;
}

int AddImmInstr::get_imm_value() const {
	return imm_value;
}


SubRegInstr::SubRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void SubRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) - state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER SubRegInstr::get_dest() const {
	return dest;
}

REGISTER SubRegInstr::get_src() const {
	return src;
}


SubImmInstr::SubImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void SubImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) - imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER SubImmInstr::get_dest() const {
	return dest;
}

int SubImmInstr::get_imm_value() const {
	return imm_value;
}


AndRegInstr::AndRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void AndRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) & state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER AndRegInstr::get_dest() const {
	return dest;
}

REGISTER AndRegInstr::get_src() const {
	return src;
}


AndImmInstr::AndImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void AndImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) & imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER AndImmInstr::get_dest() const {
	return dest;
}

int AndImmInstr::get_imm_value() const {
	return imm_value;
}


OrRegInstr::OrRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void OrRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) | state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER OrRegInstr::get_dest() const {
	return dest;
}

REGISTER OrRegInstr::get_src() const {
	return src;
}


OrImmInstr::OrImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void OrImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) | imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER OrImmInstr::get_dest() const {
	return dest;
}

int OrImmInstr::get_imm_value() const {
	return imm_value;
}


XorRegInstr::XorRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void XorRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) ^ state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER XorRegInstr::get_dest() const {
	return dest;
}

REGISTER XorRegInstr::get_src() const {
	return src;
}


XorImmInstr::XorImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void XorImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) ^ imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER XorImmInstr::get_dest() const {
	return dest;
}

int XorImmInstr::get_imm_value() const {
	return imm_value;
}


NotInstr::NotInstr(REGISTER reg) : reg{ reg } {
}

void NotInstr::execute(ProgramState& state) const {
	int result = ~state.get_register_value(reg);
	state.set_register_value(reg, result);
	state.inc_pc();
}

REGISTER NotInstr::get_reg() {
	return reg;
}


ShrRegInstr::ShrRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void ShrRegInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) >> state.get_register_value(src);
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER ShrRegInstr::get_dest() const {
	return dest;
}

REGISTER ShrRegInstr::get_src() const {
	return src;
}


ShrImmInstr::ShrImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void ShrImmInstr::execute(ProgramState& state) const {
	int result = state.get_register_value(dest) >> imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER ShrImmInstr::get_dest() const {
	return dest;
}

int ShrImmInstr::get_imm_value() const {
	return imm_value;
}


ShlRegInstr::ShlRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void ShlRegInstr::execute(ProgramState& state) const {
	int shift_count = state.get_register_value(src);
	if (shift_count < 0) {
		throw RuntimeError("Количество сдвигов не может быть отрицательным");
	}
	int result = state.get_register_value(dest) << shift_count;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER ShlRegInstr::get_dest() const {
	return dest;
}

REGISTER ShlRegInstr::get_src() const {
	return src;
}


ShlImmInstr::ShlImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void ShlImmInstr::execute(ProgramState& state) const {
	if (imm_value < 0) {
		throw RuntimeError("Количество сдвигов не может быть отрицательным");
	}
	int result = state.get_register_value(dest) << imm_value;
	state.set_register_value(dest, result);
	state.inc_pc();
}

REGISTER ShlImmInstr::get_dest() const {
	return dest;
}

int ShlImmInstr::get_imm_value() const {
	return imm_value;
}


SetRegInstr::SetRegInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void SetRegInstr::execute(ProgramState& state) const {
	state.set_register_value(dest, state.get_register_value(src));
	state.inc_pc();
}

REGISTER SetRegInstr::get_dest() const {
	return dest;
}

REGISTER SetRegInstr::get_src() const {
	return src;
}


SetImmInstr::SetImmInstr(REGISTER dest, int imm_value) : dest{ dest }, imm_value{ imm_value } {
}

void SetImmInstr::execute(ProgramState& state) const {
	state.set_register_value(dest, imm_value);
	state.inc_pc();
}

REGISTER SetImmInstr::get_dest() const {
	return dest;
}

int SetImmInstr::get_imm_value() const {
	return imm_value;
}


SetNameInstr::SetNameInstr(REGISTER dest, const std::string& var_name) : dest{ dest }, var_name{ var_name } {
}

void SetNameInstr::execute(ProgramState& state) const {
	int value = state.get_memory_value_by_name(var_name);
	state.set_register_value(dest, value);
	state.inc_pc();
}

REGISTER SetNameInstr::get_dest() const {
	return dest;
}

std::string SetNameInstr::get_var_name() const {
	return var_name;
}


LdInstr::LdInstr(REGISTER dest, const std::string& var_name) : dest{ dest }, var_name{ var_name } {
}

void LdInstr::execute(ProgramState& state) const {
	int address = state.get_address_of_data_label(var_name);
	state.set_register_value(dest, address);
	state.inc_pc();
}

REGISTER LdInstr::get_dest() const {
	return dest;
}

std::string LdInstr::get_var_name() const {
	return var_name;
}


StInstr::StInstr(const std::string& var_name, REGISTER src) : var_name{ var_name }, src{ src } {
}

void StInstr::execute(ProgramState& state) const {
	int value = state.get_register_value(src);
	state.set_memory_value_by_name(var_name, value);
	state.inc_pc();
}

REGISTER StInstr::get_src() const {
	return src;
}

std::string StInstr::get_var_name() const {
	return var_name;
}


LdiInstr::LdiInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void LdiInstr::execute(ProgramState& state) const {
	int address = state.get_register_value(src);
	int value = state.get_memory_value(address);
	state.set_register_value(dest, value);
	state.inc_pc();
}

REGISTER LdiInstr::get_dest() const {
	return dest;
}

REGISTER LdiInstr::get_src() const {
	return src;
}

StiInstr::StiInstr(REGISTER dest, REGISTER src) : dest{ dest }, src{ src } {
}

void StiInstr::execute(ProgramState& state) const {
	int address = state.get_register_value(dest);
	int value = state.get_register_value(src);
	state.set_memory_value(address, value);
	state.inc_pc();
}

REGISTER StiInstr::get_dest() const {
	return dest;
}

REGISTER StiInstr::get_src() const {
	return src;
}


JmpInstr::JmpInstr(const std::string& label_name) : label_name{ label_name } {
}

void JmpInstr::execute(ProgramState& state) const {
	int address = state.get_label_address(label_name);
	state.set_pc(address);
}

std::string JmpInstr::get_label_name() {
	return label_name;
}


JeqInstr::JeqInstr(const std::string& label_name, REGISTER src1, REGISTER src2) : label_name{ label_name }, src1{ src1 }, src2{ src2 } {
}

void JeqInstr::execute(ProgramState& state) const {
	int a = state.get_register_value(src1);
	int b = state.get_register_value(src2);

	if (a == b) {
		int address = state.get_label_address(label_name);
		state.set_pc(address);
	}
	else {
		state.inc_pc();
	}
}

REGISTER JeqInstr::get_src1() const {
	return src1;
}

REGISTER JeqInstr::get_src2() const {
	return src2;
}

std::string JeqInstr::get_label_name() {
	return label_name;
}


JgtInstr::JgtInstr(const std::string& label_name, REGISTER src1, REGISTER src2) : label_name{ label_name }, src1{ src1 }, src2{ src2 } {
}

void JgtInstr::execute(ProgramState& state) const {
	int a = state.get_register_value(src1);
	int b = state.get_register_value(src2);

	if (a > b) {
		int address = state.get_label_address(label_name);
		state.set_pc(address);
	}
	else {
		state.inc_pc();
	}
}

REGISTER JgtInstr::get_src1() const {
	return src1;
}

REGISTER JgtInstr::get_src2() const {
	return src2;
}

std::string JgtInstr::get_label_name() {
	return label_name;
}


CallInstr::CallInstr(const std::string& subroutine_name) : subroutine_name{ subroutine_name } {
}

void CallInstr::execute(ProgramState& state) const {
	state.call_subroutine(subroutine_name);
}

std::string CallInstr::get_subroutine_name() {
	return subroutine_name;
}


RetInstr::RetInstr() {
}

void RetInstr::execute(ProgramState& state) const {
	state.return_from_subroutine();
}


DataInstr::DataInstr(const std::string& data_label_name, const std::vector<int> data) : data_label_name{ data_label_name }, data{ data } {
}

void DataInstr::execute(ProgramState& state) const {
	state.allocate_memory(data_label_name, data);
	state.inc_pc();
}

std::string DataInstr::get_data_label_name() {
	return data_label_name;
}

std::vector<int> DataInstr::get_data() {
	return data;
}