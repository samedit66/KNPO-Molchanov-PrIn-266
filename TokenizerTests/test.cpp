#include <pair>
#include <vector>
#include <string>

#include "pch.h"
#include "../KNPO-Molchanov-PrIn-266/Tokenizer.h"


TEST(TokenizerTest, NoTokens) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "";

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 0);
}

TEST(TokenizerTest, CommentWithReservedWords) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "; add r0, r1, 0xff";

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 0);
}

TEST(TokenizerTest, String) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "\"string\"";
	Token expected_token = { TOKEN_TYPE::STRING, "\"string\"", 0, 7 };

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0].type, expected_token.type);
	EXPECT_EQ(tokens[0].text, expected_token.text);
	EXPECT_EQ(tokens[0].start_index, expected_token.start_index);
	EXPECT_EQ(tokens[0].end_index, expected_token.end_index);
}

TEST(TokenizerTest, Char) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'a'";
	Token expected_token = { TOKEN_TYPE::CHAR, "'a'", 0, 2 };

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0].type, expected_token.type);
	EXPECT_EQ(tokens[0].text, expected_token.text);
	EXPECT_EQ(tokens[0].start_index, expected_token.start_index);
	EXPECT_EQ(tokens[0].end_index, expected_token.end_index);
}

TEST(TokenizerTest, Numbers) {
	Tokenizer tokenizer;

	std::vector<std::pair<std::string, Token>> test_data = {
		{ "0xffa21", { TOKEN_TYPE::HEX_NUMBER, "0xffa21", 0, 6 } },
		{ "0o1235", { TOKEN_TYPE::OCTAL_NUMBER, "0o1235", 0, 5 } },
		{ "102132", { TOKEN_TYPE::DECIMAL_NUMBER, "102132", 0, 5 } },
		{ "0b10101", { TOKEN_TYPE::BINARY_NUMBER, "0b10101", 0, 6 } },
	};

	for (const auto& test : test_data) {
		std::vector<Token> tokens;

		tokenizer.tokenize(test.first, tokens);

		ASSERT_EQ(tokens.size(), 1);
		EXPECT_EQ(tokens[0].type, test.second.type);
		EXPECT_EQ(tokens[0].text, test.second.text);
		EXPECT_EQ(tokens[0].start_index, test.second.start_index);
		EXPECT_EQ(tokens[0].end_index, test.second.end_index);
	}
}

TEST(TokenizerTest, ReservedWords) {
	Tokenizer tokenizer;

	std::vector<std::pair<std::string, Token>> test_data = {
		{ "add", { TOKEN_TYPE::ADD, "add", 0, 2 } },
		{ "sub", { TOKEN_TYPE::SUB, "sub", 0, 2 } },
		{ "and", { TOKEN_TYPE::AND, "and", 0, 2 } },
		{ "or", { TOKEN_TYPE::OR, "or", 0, 1 } },
		{ "xor", { TOKEN_TYPE::XOR, "xor", 0, 2 } },
		{ "not", { TOKEN_TYPE::NOT, "not", 0, 2 } },
		{ "shr", { TOKEN_TYPE::SHR, "shr", 0, 2 } },
		{ "shl", { TOKEN_TYPE::SHL, "shl", 0, 2 } },
		{ "set", { TOKEN_TYPE::SET, "set", 0, 2 } },
		{ "ld", { TOKEN_TYPE::LD, "ld", 0, 1 } },
		{ "st", { TOKEN_TYPE::ST, "st", 0, 1 } },
		{ "ldi", { TOKEN_TYPE::LDI, "ldi", 0, 2 } },
		{ "sti", { TOKEN_TYPE::STI, "sti", 0, 2 } },
		{ "jmp", { TOKEN_TYPE::JMP, "jmp", 0, 2 } },
		{ "jeq", { TOKEN_TYPE::JEQ, "jeq", 0, 2 } },
		{ "jgt", { TOKEN_TYPE::JGT, "jgt", 0, 2 } },
		{ "call", { TOKEN_TYPE::CALL, "call", 0, 3 } },
		{ "ret", { TOKEN_TYPE::RET, "ret", 0, 2 } },
		{ "data", { TOKEN_TYPE::DATA, "data", 0, 3 } },
		{ "r0", { TOKEN_TYPE::R0, "r0", 0, 1  } },
		{ "r1", { TOKEN_TYPE::R1, "r1", 0, 1  } },
		{ "r2", { TOKEN_TYPE::R2, "r2", 0, 1  } },
		{ "r3", { TOKEN_TYPE::R3, "r3", 0, 1  } },
		{ "r4", { TOKEN_TYPE::R4, "r4", 0, 1  } },
		{ "r5", { TOKEN_TYPE::R5, "r5", 0, 1  } },
		{ "r6", { TOKEN_TYPE::R6, "r6", 0, 1  } },
		{ "r7", { TOKEN_TYPE::R7, "r7", 0, 1  } },
	};

	for (const auto& test : test_data) {
		std::vector<Token> tokens;

		tokenizer.tokenize(test.first, tokens);

		ASSERT_EQ(tokens.size(), 1);
		EXPECT_EQ(tokens[0].type, test.second.type);
		EXPECT_EQ(tokens[0].text, test.second.text);
		EXPECT_EQ(tokens[0].start_index, test.second.start_index);
		EXPECT_EQ(tokens[0].end_index, test.second.end_index);
	}
}

TEST(TokenizerTest, SeveralNames) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "hello world";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::NAME, "hello", 0, 4 },
		{ TOKEN_TYPE::NAME, "world", 6, 10 },
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
		EXPECT_EQ(tokens[i].start_index, expected_tokens[i].start_index);
		EXPECT_EQ(tokens[i].end_index, expected_tokens[i].end_index);
	}
}

TEST(TokenizerTest, SpecialSymbols) {
	Tokenizer tokenizer;

	std::vector<std::pair<std::string, Token>> test_data = {
		{ "+", { TOKEN_TYPE::PLUS, "+", 0, 0 }  },
		{ "-", { TOKEN_TYPE::MINUS, "-", 0, 0 } },
		{ ",", { TOKEN_TYPE::COMMA, ",", 0, 0 } },
		{ ":", { TOKEN_TYPE::COLON, ":", 0, 0 } },
	};

	for (const auto& test : test_data) {
		std::vector<Token> tokens;

		tokenizer.tokenize(test.first, tokens);

		ASSERT_EQ(tokens.size(), 1);
		EXPECT_EQ(tokens[0].type, test.second.type);
		EXPECT_EQ(tokens[0].text, test.second.text);
		EXPECT_EQ(tokens[0].start_index, test.second.start_index);
		EXPECT_EQ(tokens[0].end_index, test.second.end_index);
	}
}

TEST(TokenizerTest, Spaces) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "              ";

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 0);
}

TEST(TokenizerTest, UnknownToken) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "!";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), TokenizerError);
}

TEST(TokenizerTest, UnclosedString) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "\"hello world";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), TokenizerError);
}

TEST(TokenizerTest, UnclosedChar) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'a";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), TokenizerError);
}

TEST(TokenizerTest, CharHoldsOnlyOneSymbol) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'aascasc'";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), TokenizerError);
}

TEST(TokenizerTest, NonReservedTokenStartsWithReservedWord) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "callmelater";
	Token expeceted_token = { TOKEN_TYPE::NAME, "callmelater" };

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0].type, expeceted_token.type);
	EXPECT_EQ(tokens[0].text, expeceted_token.text);
}

TEST(TokenizerTest, ComplexTest) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	const std::string str = "     add r0, r1, -0xffff; add r0 adfsdfg asdf as";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::ADD, "add" },
		{ TOKEN_TYPE::R0, "r0" },
		{ TOKEN_TYPE::COMMA, "," },
		{ TOKEN_TYPE::R1, "r1" },
		{ TOKEN_TYPE::COMMA, "," },
		{ TOKEN_TYPE::MINUS, "-" },
		{ TOKEN_TYPE::HEX_NUMBER, "0xffff" }
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
	}
}
