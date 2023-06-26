#pragma once

#include <iostream>

#include "Instruction.h"

/*!
Интерпретатор псевдо-ассемблера
*/
class Interpreter {
public:
	/*!
	Выполняет интерпретацию инструкций на языке псевдо-ассемблера
	\param[in] input_file Входной файл
	*/
	void interpret(std::ifstream& input_file);
};