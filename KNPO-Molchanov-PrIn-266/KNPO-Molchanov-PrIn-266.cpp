#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "Tokenizer.h"
#include "MnemonicTranslator.h"
#include "Interpreter.h"

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "������ �������������: " << argv[0] << " <����.asm>" << std::endl;
		return 1;
	}

	std::ifstream input_file(argv[1]);

	if (!input_file.is_open()) {
		std::cerr << "������: ���� \"" << argv[1] << "\" �� ����� ���� ������" << std::endl;
		return 1;
	}

	Interpreter interp;
	interp.interpret(input_file);

	input_file.close();
	return 0;
}