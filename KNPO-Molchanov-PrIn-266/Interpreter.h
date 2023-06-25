#pragma once

#include <iostream>

#include "Instruction.h"

/*!
������������� ������-����������
*/
class Interpreter {
public:
	/*!
	��������� ������������� ���������� �� ����� ������-����������
	\param[in] input_file ������� ����
	*/
	void interpret(std::ifstream& input_file);
};