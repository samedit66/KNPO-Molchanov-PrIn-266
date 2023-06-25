#include "pch.h"

#include "../KNPO-Molchanov-PrIn-266/Instruction.h"
#include "../KNPO-Molchanov-PrIn-266/ProgramState.h"


TEST(InstructionTests, AddRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 2);
	state.set_register_value(REGISTER::R1, 3);

	AddRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 5);
}

TEST(InstructionTests, AddImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 2);

	AddImmInstr instr{ REGISTER::R0, 5 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 7);
}

TEST(InstructionTests, SubRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 2);
	state.set_register_value(REGISTER::R1, 3);

	SubRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), -1);
}

TEST(InstructionTests, SubImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 10);

	SubImmInstr instr{ REGISTER::R0, 3 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 7);
}

TEST(InstructionTests, AndRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.set_register_value(REGISTER::R1, 3);

	AndRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 0);
}

TEST(InstructionTests, AndImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 2);

	AndImmInstr instr{ REGISTER::R0, 1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 0);
}

TEST(InstructionTests, OrRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.set_register_value(REGISTER::R1, 3);

	OrRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 3);
}

TEST(InstructionTests, OrImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 4);

	OrImmInstr instr{ REGISTER::R0, 1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 5);
}

TEST(InstructionTests, XorRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 3);
	state.set_register_value(REGISTER::R1, 7);

	XorRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 4);
}

TEST(InstructionTests, XorImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 5);

	XorImmInstr instr{ REGISTER::R0, 2 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 7);
}

TEST(InstructionTests, NotInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 5);

	NotInstr instr{ REGISTER::R0 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), -6);
}

TEST(InstructionTests, ShrRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 16);
	state.set_register_value(REGISTER::R1, 2);

	ShrRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 4);
}

TEST(InstructionTests, ShrImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 4);

	ShrImmInstr instr{ REGISTER::R0, 2 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 1);
}

TEST(InstructionTests, ShlRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 4);
	state.set_register_value(REGISTER::R1, 2);

	ShlRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 16);
}

TEST(InstructionTests, ShlImmInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 16);

	ShlImmInstr instr{ REGISTER::R0, 1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 32);
}

TEST(InstructionTests, SetImmInstruction) {
	ProgramState state(1);

	SetImmInstr instr{ REGISTER::R0, 19 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 19);
}

TEST(InstructionTests, SetRegInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.set_register_value(REGISTER::R1, 20);

	SetRegInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 20);
}

TEST(InstructionTests, SetNameInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.allocate_memory("age", {17});

	SetNameInstr instr{ REGISTER::R0, "age" };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 17);
}

TEST(InstructionTests, LdInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.allocate_memory("age", { 17 });

	LdInstr instr{ REGISTER::R0, "age" };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), state.get_address_of_data_label("age"));
}

TEST(InstructionTests, StInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.allocate_memory("age", { 17 });

	StInstr instr{ "age", REGISTER::R0 };
	instr.execute(state);

	ASSERT_EQ(state.get_memory_value_by_name("age"), 17);
}

TEST(InstructionTests, LdiInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.set_register_value(REGISTER::R1, 0);
	state.allocate_memory("age", { 18 });

	LdiInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_register_value(REGISTER::R0), 18);
}

TEST(InstructionTests, StiInstruction) {
	ProgramState state(1);
	state.set_register_value(REGISTER::R0, 0);
	state.set_register_value(REGISTER::R1, 12);

	StiInstr instr{ REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_memory_value(0), 12);
}

TEST(InstructionTests, JmpInstruction) {
	ProgramState state(1);
	state.add_label("loop", 12);

	JmpInstr instr{ "loop" };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 12);
}

TEST(InstructionTests, JeqInstructionJumps) {
	ProgramState state(1);
	state.add_label("loop", 12);

	state.set_register_value(REGISTER::R0, 10);
	state.set_register_value(REGISTER::R1, 10);

	JeqInstr instr{ "loop", REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 12);
}

TEST(InstructionTests, JeqInstructionDoesntJump) {
	ProgramState state(1);
	state.add_label("loop", 12);

	state.set_register_value(REGISTER::R0, 9);
	state.set_register_value(REGISTER::R1, 10);

	JeqInstr instr{ "loop", REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 1);
}

TEST(InstructionTests, JgtInstructionJumps) {
	ProgramState state(1);
	state.add_label("loop", 15);

	state.set_register_value(REGISTER::R0, 14);
	state.set_register_value(REGISTER::R1, 10);

	JgtInstr instr{ "loop", REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 15);
}

TEST(InstructionTests, JgtInstructionDoesntJump) {
	ProgramState state(1);
	state.add_label("loop", 15);

	state.set_register_value(REGISTER::R0, 10);
	state.set_register_value(REGISTER::R1, 14);

	JgtInstr instr{ "loop", REGISTER::R0, REGISTER::R1 };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 1);
}

TEST(InstructionTests, CallInstruction) {
	ProgramState state(1);
	state.add_label("func", 15);

	CallInstr instr{ "func" };
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 15);
}

TEST(InstructionTests, RetInstruction) {
	ProgramState state(1);
	state.add_label("func", 5);
	state.call_subroutine("func");

	RetInstr instr;
	instr.execute(state);

	ASSERT_EQ(state.get_pc(), 1);
}