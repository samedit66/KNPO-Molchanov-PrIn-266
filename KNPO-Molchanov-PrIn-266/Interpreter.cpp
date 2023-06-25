#include <fstream>
#include <map>
#include <vector>

#include "Interpreter.h"
#include "MnemonicTranslator.h"
#include "Tokenizer.h"

/*!
Выполняет интерпретацию инструкций на языке псевдо-ассемблера
\param[in] input_file Входной файл
*/
void Interpreter::interpret(std::ifstream& input_file) {
	// Считанные инструкции
	std::vector<std::shared_ptr<Instr>> instrs;

	// Карта меток
	std::map<std::string, int> labels;

	// Ошибки, возникшие в процессе токенизации
	std::vector<TokenizerError> tokenizer_errors;

	// Синтаксические ошибки
	std::vector<SyntaxError> syntax_errors;

	// Переводим мнемоники во внутрнее представление
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
			std::cout << "Строка " + std::to_string(instrs.at(state.get_pc())->get_line_number()) + ": " + err.what() << std::endl;
		}
	}
	catch (RuntimeError& err) {
		std::cout << err.what() << std::endl;
	}
}