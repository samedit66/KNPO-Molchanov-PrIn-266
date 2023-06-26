#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <fstream>

#include "Interpreter.h"


int main(int argc, char* argv[]) {
	if (argc != 2) {
		std::cerr << "������ �������������: " << argv[0] << " <����.asm>" << std::endl;
		return 1;
	}

	std::string file_name(argv[1]);
	if (file_name.substr(file_name.find_last_of(".") + 1) != "asm") {
		std::cerr << "������: ���� \"" << argv[1] << "\" ������ ����� ���������� \".asm\"" << std::endl;
		return 1;
	}

	std::ifstream input_file(file_name);
	if (!input_file.is_open()) {
		std::cerr << "������: ���� \"" << argv[1] << "\" �� ����� ���� ������" << std::endl;
		return 1;
	}

	Interpreter interp;
	interp.interpret(input_file);

	input_file.close();
	return 0;
}