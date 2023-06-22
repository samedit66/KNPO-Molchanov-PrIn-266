#include <iostream>
#include <cstdio>
#include <string>
#include <vector>
#include <regex>

#include "Tokenizer.h"


int main()
{
	Tokenizer tokenizer;

	std::vector<Token> tokens;
	tokenizer.tokenize("", tokens);

	std::cout << tokens.size() << std::endl;

	for (const Token& token : tokens) {
		std::cout << token.text << std::endl;
	}

	return 0;
}