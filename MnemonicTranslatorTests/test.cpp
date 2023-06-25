#include "pch.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <typeinfo>

#include "../KNPO-Molchanov-PrIn-266/MnemonicTranslator.h"


TEST(MnemonicTranslatorTest, AddRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "add r0, r1" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	AddRegInstr* instr = dynamic_cast<AddRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr->get_src(), REGISTER::R1);
}

TEST(MnemonicTranslatorTest, AddImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "add r0, -0xff" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	AddImmInstr* instr = dynamic_cast<AddImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr->get_imm_value(), -255);
}

TEST(MnemonicTranslatorTest, SubRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "sub r1, r0" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	SubRegInstr* instr = dynamic_cast<SubRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R0);
}

TEST(MnemonicTranslatorTest, SubImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "sub r1, 0o312" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	SubImmInstr* instr = dynamic_cast<SubImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_imm_value(), 202);
}

TEST(MnemonicTranslatorTest, AndRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "and r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	AndRegInstr* instr = dynamic_cast<AndRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, AndImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "and r4, 142" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	AndImmInstr* instr = dynamic_cast<AndImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), 142);
}

TEST(MnemonicTranslatorTest, OrRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "or r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	OrRegInstr* instr = dynamic_cast<OrRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, OrImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "or r4, -34" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	OrImmInstr* instr = dynamic_cast<OrImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), -34);
}

TEST(MnemonicTranslatorTest, XorRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "xor r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	XorRegInstr* instr = dynamic_cast<XorRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, XorImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "xor r4, 0b01" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	XorImmInstr* instr = dynamic_cast<XorImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), 1);
}

TEST(MnemonicTranslatorTest, ShrRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "shr r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ShrRegInstr* instr = dynamic_cast<ShrRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, ShrImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "shr r4, 2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ShrImmInstr* instr = dynamic_cast<ShrImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), 2);
}

TEST(MnemonicTranslatorTest, ShlRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "shl r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ShlRegInstr* instr = dynamic_cast<ShlRegInstr* > (instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, ShlImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "shl r4, 2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ShlImmInstr* instr = dynamic_cast<ShlImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), 2);
}

TEST(MnemonicTranslatorTest, SetRegInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "set r1, r2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	SetRegInstr* instr = dynamic_cast<SetRegInstr*> (instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R2);
}

TEST(MnemonicTranslatorTest, SetImmInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "set r4, 2" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	SetImmInstr* instr = dynamic_cast<SetImmInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R4);
	EXPECT_EQ(instr->get_imm_value(), 2);
}

TEST(MnemonicTranslatorTest, SetNameInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "set r0, array_size" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	SetNameInstr* instr = dynamic_cast<SetNameInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr->get_var_name(), "array_size");
}

TEST(MnemonicTranslatorTest, LdInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "ld r0, str" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	LdInstr* instr = dynamic_cast<LdInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr->get_var_name(), "str");
}

TEST(MnemonicTranslatorTest, StInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "st str, r5" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	StInstr* instr = dynamic_cast<StInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_var_name(), "str");
	EXPECT_EQ(instr->get_src(), REGISTER::R5);
}

TEST(MnemonicTranslatorTest, LdiInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "ldi r0, r1" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	LdiInstr* instr = dynamic_cast<LdiInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr->get_src(), REGISTER::R1);
}

TEST(MnemonicTranslatorTest, JmpInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "jmp loop" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	JmpInstr* instr = dynamic_cast<JmpInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_label_name(), "loop");
}

TEST(MnemonicTranslatorTest, JeqInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "jeq loop, r0, r1" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	JeqInstr* instr = dynamic_cast<JeqInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_src1(), REGISTER::R0);
	EXPECT_EQ(instr->get_src2(), REGISTER::R1);
	EXPECT_EQ(instr->get_label_name(), "loop");
}

TEST(MnemonicTranslatorTest, JgtInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "jgt loop, r0, r1" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	JgtInstr* instr = dynamic_cast<JgtInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_src1(), REGISTER::R0);
	EXPECT_EQ(instr->get_src2(), REGISTER::R1);
	EXPECT_EQ(instr->get_label_name(), "loop");
}

TEST(MnemonicTranslatorTest, CallInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "call factorial" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	CallInstr* instr = dynamic_cast<CallInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_subroutine_name(), "factorial");
}

TEST(MnemonicTranslatorTest, RetInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "ret" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	RetInstr* instr = dynamic_cast<RetInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
}

TEST(MnemonicTranslatorTest, MoreThanOneInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "ret add r0, r1" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_FALSE(result);
	EXPECT_EQ(instrs.size(), 0);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 1);
}

TEST(MnemonicTranslatorTest, UnfinishedInstruction) {
	std::ofstream output_file("test.asm");
	output_file << "add r0," << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_FALSE(result);
	EXPECT_EQ(instrs.size(), 0);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 1);
}

TEST(MnemonicTranslatorTest, UnallowedOperands) {
	std::ofstream output_file("test.asm");
	output_file << "add 100, r0" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_FALSE(result);
	EXPECT_EQ(instrs.size(), 0);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 1);
}

TEST(MnemonicTranslatorTest, InstructionWithLabel) {
	std::ofstream output_file("test.asm");
	output_file << "loop: add r1, r0" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ASSERT_EQ(labels.size(), 1);
	EXPECT_EQ(labels.count("loop"), 1);
	EXPECT_EQ(labels.at("loop"), 0);
	AddRegInstr* instr = dynamic_cast<AddRegInstr*>(instrs[0].get());
	ASSERT_NE(instr, nullptr);
	EXPECT_EQ(instr->get_dest(), REGISTER::R1);
	EXPECT_EQ(instr->get_src(), REGISTER::R0);
}

TEST(MnemonicTranslatorTest, OnlyLabel) {
	std::ofstream output_file("test.asm");
	output_file << "loop:" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 0);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	EXPECT_EQ(labels.size(), 0);
}

TEST(MnemonicTranslatorTest, SeveralLines) {
	std::ofstream output_file("test.asm");
	output_file << "loop:" << std::endl;
	output_file << "add r0, 1" << std::endl;
	output_file << "jgt loop, r0, r3" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_TRUE(result);
	EXPECT_EQ(instrs.size(), 2);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 0);
	ASSERT_EQ(labels.size(), 1);
	EXPECT_EQ(labels.count("loop"), 1);
	EXPECT_EQ(labels.at("loop"), 0);

	AddImmInstr* instr1 = dynamic_cast<AddImmInstr*>(instrs[0].get());
	ASSERT_NE(instr1, nullptr);
	EXPECT_EQ(instr1->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr1->get_imm_value(), 1);

	JgtInstr* instr2 = dynamic_cast<JgtInstr*>(instrs[1].get());
	ASSERT_NE(instr2, nullptr);
	EXPECT_EQ(instr2->get_label_name(), "loop");
	EXPECT_EQ(instr2->get_src1(), REGISTER::R0);
	EXPECT_EQ(instr2->get_src2(), REGISTER::R3);
}

TEST(MnemonicTranslatorTest, SeveralLabelsWithSameName) {
	std::ofstream output_file("test.asm");
	output_file << "loop:" << std::endl;
	output_file << "loop:" << std::endl;
	output_file << "loop:" << std::endl;
	output_file << "add r0, 12" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_FALSE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 2);
	ASSERT_EQ(labels.size(), 1);
	EXPECT_EQ(labels.count("loop"), 1);
	EXPECT_EQ(labels.at("loop"), 0);

	AddImmInstr* instr1 = dynamic_cast<AddImmInstr*>(instrs[0].get());
	ASSERT_NE(instr1, nullptr);
	EXPECT_EQ(instr1->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr1->get_imm_value(), 12);
}

TEST(MnemonicTranslatorTest, LabelWithoutColon) {
	std::ofstream output_file("test.asm");
	output_file << "loop" << std::endl;
	output_file << "add r0, 12" << std::endl;
	output_file.close();

	std::ifstream input_file("test.asm");
	std::vector<std::shared_ptr<Instr>> instrs;
	std::map<std::string, int> labels;
	std::vector<TokenizerError> tokenizer_errors;
	std::vector<SyntaxError> syntax_errors;

	MnemonicTranslator mn;
	bool result = mn.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors);

	ASSERT_FALSE(result);
	EXPECT_EQ(instrs.size(), 1);
	EXPECT_EQ(tokenizer_errors.size(), 0);
	EXPECT_EQ(syntax_errors.size(), 1);
	ASSERT_EQ(labels.size(), 0);

	AddImmInstr* instr1 = dynamic_cast<AddImmInstr*>(instrs[0].get());
	ASSERT_NE(instr1, nullptr);
	EXPECT_EQ(instr1->get_dest(), REGISTER::R0);
	EXPECT_EQ(instr1->get_imm_value(), 12);
}