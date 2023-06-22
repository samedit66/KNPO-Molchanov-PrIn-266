#include <regex>
#include <vector>
#include <set>
#include <regex>

#include "Tokenizer.h"


/*!
����������� ������ �� ���������� ������
\param[in] m ��������� �� ������
*/
NoTokenFoundError::NoTokenFoundError(const std::string& m) {
	msg = m;
}

/*
���������� ��������� �� ������
\return ��������� �� ������
*/
std::string NoTokenFoundError::what() const {
	return msg;
}


/*!
����������� �������-������
\param[in] t ��� ������
\param[in\ r ������, ����������� �����
*/
Tokenizer::TokenRegex::TokenRegex(TOKEN_TYPE t, std::string r) {
	type = t;
	matching_regex = r;
}

/*!
���������� ��� ������ ��� �������
\return ��� ������ ��� �������
*/
TOKEN_TYPE Tokenizer::TokenRegex::get_type() {
	return type;
}

/*!
��������� ������� ������ � ���������-������
\param[in] start �������� ������ ������
\param[in] end �������� ����� ������
\param[out] m ������, � ������� �������� ������ � ���������� �������������
\return ����, ����������� �� ���������� ���������� ������������� 
*/
bool Tokenizer::TokenRegex::match(std::string::const_iterator start, std::string::const_iterator end, std::smatch& m) {
	return std::regex_search(start, end, m, matching_regex);
}


/*!
����������� �����������, ��������������� ������ ����� ������-����������
*/
Tokenizer::Tokenizer() {
	add_token(TOKEN_TYPE::ADD, "add\\b");
	add_token(TOKEN_TYPE::SUB, "sub\\b");
	add_token(TOKEN_TYPE::AND, "and\\b");
	add_token(TOKEN_TYPE::OR, "or\\b");
	add_token(TOKEN_TYPE::XOR, "xor\\b");
	add_token(TOKEN_TYPE::NOT, "not\\b");
	add_token(TOKEN_TYPE::SHR, "shr\\b");
	add_token(TOKEN_TYPE::SHL, "shl\\b");
	add_token(TOKEN_TYPE::SET, "set\\b");
	add_token(TOKEN_TYPE::LD, "ld\\b");
	add_token(TOKEN_TYPE::ST, "st\\b");
	add_token(TOKEN_TYPE::LDI, "ldi\\b");
	add_token(TOKEN_TYPE::STI, "sti\\b");
	add_token(TOKEN_TYPE::JMP, "jmp\\b");
	add_token(TOKEN_TYPE::JEQ, "jeq\\b");
	add_token(TOKEN_TYPE::JGT, "jgt\\b");
	add_token(TOKEN_TYPE::CALL, "call\\b");
	add_token(TOKEN_TYPE::RET, "ret\\b");
	add_token(TOKEN_TYPE::DATA, "data\\b");

	add_token(TOKEN_TYPE::R0, "r0\\b");
	add_token(TOKEN_TYPE::R1, "r1\\b");
	add_token(TOKEN_TYPE::R2, "r2\\b");
	add_token(TOKEN_TYPE::R3, "r3\\b");
	add_token(TOKEN_TYPE::R4, "r4\\b");
	add_token(TOKEN_TYPE::R5, "r5\\b");
	add_token(TOKEN_TYPE::R6, "r6\\b");
	add_token(TOKEN_TYPE::R7, "r7\\b");

	add_token(TOKEN_TYPE::NAME, "([a-zA-Z_]\\w*)");
	add_token(TOKEN_TYPE::HEX_NUMBER, "0(?:[xX])([0-9A-Fa-f]+)");
	add_token(TOKEN_TYPE::OCTAL_NUMBER, "0(?:[oO])([0-7]+)");
	add_token(TOKEN_TYPE::BINARY_NUMBER, "0(?:[bB])([01]+)");
	add_token(TOKEN_TYPE::DECIMAL_NUMBER, "[0-9]+");
	add_token(TOKEN_TYPE::STRING, R"(\"(.+)?\")");
	add_token(TOKEN_TYPE::CHAR, R"(^'(\\n|\\t|\\'|\\\\|.)')");

	add_token(TOKEN_TYPE::PLUS, "\\+");
	add_token(TOKEN_TYPE::MINUS, "-");
	add_token(TOKEN_TYPE::COMMA, ",");
	add_token(TOKEN_TYPE::COLON, ":");

	add_token(TOKEN_TYPE::SPACE, "\\s+");
	ignore(TOKEN_TYPE::SPACE);

	add_token(TOKEN_TYPE::COMMENT, ";.*");
	ignore(TOKEN_TYPE::COMMENT);
}

/*!
��������� ����� ������ �������������� ������� ���� ������
\param[in] type ��� ������
\param[in] matching_regex ������ ��� ������
*/
void Tokenizer::add_token(TOKEN_TYPE type, const std::string& matching_regex) {
	std::string s = "^" + matching_regex; // ������������� ���������, ��� ������ ������ ��������� � ������ ������
	token_regexes.push_back(TokenRegex{ type, s });
}

/*!
��������� ��� ������, ������� ������� ������������ � �������� �������
\param[in] type ��� ������
*/
void Tokenizer::ignore(TOKEN_TYPE type) {
	ignored_tokens.insert(type);
}

/*!
\brief �������� ��������� ����� �� ������-���������

�������� ��������� ����� �� ������-���������. � ������, �����
����� �� ����� ������������, �� ����������� � �������� ������
\param[in] start ������ ��������� ������
\param[in] end ����� ��������� ������
\param[out] tokens ��������� ������
\return ��������, ����������� �� ������, ��������� ����� ����� ���������� ������� ���������� ������
\throw NoTokenFoundError � ������, ���� �� ���� ������� ���������������� ������� ��� ���������� ������
*/
std::string::const_iterator Tokenizer::extract_token(std::string::const_iterator start, std::string::const_iterator end, std::vector<Token>& tokens) {
	std::smatch matched_token;
	TOKEN_TYPE type = TOKEN_TYPE::UNSPECIFIED;

	// ��� ������� ��������� ������� ������...
	for (TokenRegex token_regex : token_regexes) {
		// ���� ������ ������ ��� ������...
		if (token_regex.match(start, end, matched_token)) {
			// ...������� ��� ���
			type = token_regex.get_type();
			// ...���������� ����
			break;
		}
	}

	// ���� ����� �� ��� ���������, �� �������� ���������� � ���,
	// ��� ���������� ����� �� ��� ������
	if (type == TOKEN_TYPE::UNSPECIFIED) {
		throw NoTokenFoundError("�������������� ������");
	}

	// ���� ��������� ����� �� ����� ������������, ��������� ���
	// � ������ ��������� �������
	if (ignored_tokens.find(type) == ignored_tokens.end()) {
		tokens.push_back(Token{ type, matched_token.str() });
	}

	return matched_token.suffix().first;
}

/*!
\brief �������� ������ ����� ������-���������� �� ������

�������� ������ ����� ������-���������� �� ������, �������� �� �� ������� ������
\param[in] str ������� ������
\param[out] tokens ��������� ������
*/
void Tokenizer::tokenize(const std::string& str, std::vector<Token>& tokens) {
	std::string::const_iterator start = str.begin();
	std::string::const_iterator end = str.end();

	// ���� �� �������� ��� ������...
	while (start < end) {
		// ...������� ��������� �����
		start = extract_token(start, end, tokens);
	}
}