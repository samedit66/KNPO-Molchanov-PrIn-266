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
	Token expected_token = { TOKEN_TYPE::STRING, "\"string\"" };

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0].type, expected_token.type);
	EXPECT_EQ(tokens[0].text, expected_token.text);
}

TEST(TokenizerTest, Char) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'a'";
	Token expected_token = { TOKEN_TYPE::CHAR, "'a'" };

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), 1);
	EXPECT_EQ(tokens[0].type, expected_token.type);
	EXPECT_EQ(tokens[0].text, expected_token.text);
}

TEST(TokenizerTest, Numbers) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "0xffa21 0o1235 102132 0b101010111";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::HEX_NUMBER, "0xffa21" },
		{ TOKEN_TYPE::OCTAL_NUMBER, "0o1235" },
		{ TOKEN_TYPE::DECIMAL_NUMBER, "102132" },
		{ TOKEN_TYPE::BINARY_NUMBER, "0b101010111" }
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
	}
}

TEST(TokenizerTest, ReservedWords) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "add sub and or xor not shr shl set ld st ldi sti jmp jeq jgt call ret data r0 r1 r2 r3 r4 r5 r6 r7";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::ADD, "add" },
		{ TOKEN_TYPE::SUB, "sub" },
		{ TOKEN_TYPE::AND, "and" },
		{ TOKEN_TYPE::OR, "or" },
		{ TOKEN_TYPE::XOR, "xor" },
		{ TOKEN_TYPE::NOT, "not" },
		{ TOKEN_TYPE::SHR, "shr" },
		{ TOKEN_TYPE::SHL, "shl" },
		{ TOKEN_TYPE::SET, "set" },
		{ TOKEN_TYPE::LD, "ld" },
		{ TOKEN_TYPE::ST, "st" },
		{ TOKEN_TYPE::LDI, "ldi" },
		{ TOKEN_TYPE::STI, "sti" },
		{ TOKEN_TYPE::JMP, "jmp" },
		{ TOKEN_TYPE::JEQ, "jeq" },
		{ TOKEN_TYPE::JGT, "jgt" },
		{ TOKEN_TYPE::CALL, "call" },
		{ TOKEN_TYPE::RET, "ret" },
		{ TOKEN_TYPE::DATA, "data" },
		{ TOKEN_TYPE::R0, "r0" },
		{ TOKEN_TYPE::R1, "r1" },
		{ TOKEN_TYPE::R2, "r2" },
		{ TOKEN_TYPE::R3, "r3" },
		{ TOKEN_TYPE::R4, "r4" },
		{ TOKEN_TYPE::R5, "r5" },
		{ TOKEN_TYPE::R6, "r6" },
		{ TOKEN_TYPE::R7, "r7" },
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
	}
}

TEST(TokenizerTest, Names) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "hello world";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::NAME, "hello" },
		{ TOKEN_TYPE::NAME, "world" },
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
	}
}

TEST(TokenizerTest, SpecialSymbols) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "+ - , :";
	std::vector<Token> expected_tokens = {
		{ TOKEN_TYPE::PLUS, "+" },
		{ TOKEN_TYPE::MINUS, "-" },
		{ TOKEN_TYPE::COMMA, "," },
		{ TOKEN_TYPE::COLON, ":" },
	};

	tokenizer.tokenize(str, tokens);

	ASSERT_EQ(tokens.size(), expected_tokens.size());
	for (int i = 0; i < tokens.size(); i++) {
		EXPECT_EQ(tokens[i].type, expected_tokens[i].type);
		EXPECT_EQ(tokens[i].text, expected_tokens[i].text);
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

	ASSERT_THROW(tokenizer.tokenize(str, tokens), NoTokenFoundError);
}

TEST(TokenizerTest, UnclosedString) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "\"hello world";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), NoTokenFoundError);
}

TEST(TokenizerTest, UnclosedChar) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'a";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), NoTokenFoundError);
}

TEST(TokenizerTest, CharHoldsOnlyOneSymbol) {
	Tokenizer tokenizer;
	std::vector<Token> tokens;
	std::string str = "'aascasc'";

	ASSERT_THROW(tokenizer.tokenize(str, tokens), NoTokenFoundError);
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
