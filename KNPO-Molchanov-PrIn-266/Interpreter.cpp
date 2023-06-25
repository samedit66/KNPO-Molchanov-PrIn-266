#include <fstream>
#include <map>
#include <vector>

#include "Interpreter.h"
#include "MnemonicTranslator.h"
#include "Tokenizer.h"

/*!
��������� ������������� ���������� �� ����� ������-����������
\param[in] input_file ������� ����
*/
void Interpreter::interpret(std::ifstream& input_file) {
	// ��������� ����������
	std::vector<std::shared_ptr<Instr>> instrs;

	// ����� �����
	std::map<std::string, int> labels;

	// ������, ��������� � �������� �����������
	std::vector<TokenizerError> tokenizer_errors;

	// �������������� ������
	std::vector<SyntaxError> syntax_errors;

	// ��������� ��������� �� �������� �������������
	MnemonicTranslator mnemonic_translator;
	if (!mnemonic_translator.translate(input_file, instrs, labels, tokenizer_errors, syntax_errors)) {
		for (const auto& err : tokenizer_errors) {
			std::cout << err.what() << std::endl;
		}
		for (const auto& err : syntax_errors) {
			std::cout << err.what() << std::endl;
		}

		return;
	}

	try {
		ProgramState state(instrs.size());

		for (const auto& l : labels) {
			state.add_label(l.first, l.second);
		}

		try {
			while (state.is_running()) {
				instrs.at(state.get_pc())->execute(state);
			}
		}
		catch (RuntimeError& err) {
			std::cout << "������ " + std::to_string(instrs.at(state.get_pc())->get_line_number()) + ": " + err.what() << std::endl;
		}
	}
	catch (RuntimeError& err) {
		std::cout << err.what() << std::endl;
	}
}