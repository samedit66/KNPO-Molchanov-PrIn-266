#include <vector>
#include <set>
#include <regex>

#include "Tokenizer.h"


/*!
Конструктор ошибки об отсутствии токена
\param[in] m Сообщение об ошибке
*/
TokenizerError::TokenizerError(const std::string& m) {
	msg = m;
}

/*
Возвращает сообщение об ошибке
\return сообщение об ошибке
*/
std::string TokenizerError::what() const {
	return msg;
}

/*!
Меняет сообщение об ошибке
\param[in] m Новое сообщение об ошибке
*/
void TokenizerError::change_error_message(const std::string& m) {
	msg = m;
}

/*!
Конструктор шаблона-токена
\param[in] t Тип токена
\param[in\ r Шаблон, описывающий токен
*/
Tokenizer::TokenRegex::TokenRegex(TOKEN_TYPE t, std::string r) {
	type = t;
	matching_regex = r;
}

/*!
Возвращает тип токена для шаблона
\return Тип токена для шаблона
*/
TOKEN_TYPE Tokenizer::TokenRegex::get_type() {
	return type;
}

/*!
Проверяет наличие токена в итераторе-строке
\param[in] start Итератор начала строки
\param[in] end Итератор конца строки
\param[out] m Объект, в котором хранятся данные о результате сопоставления
\return Флаг, указывающий об успешности результата сопоставления 
*/
bool Tokenizer::TokenRegex::match(const std::string& str, int start_index, std::smatch& m) {
	return std::regex_search(str.begin() + start_index, str.end(), m, matching_regex);
}


/*!
Конструктор токенайзера, устанавливающий токены языка псевдо-ассемблера
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
Добавляет новый шаблон соотвествующий данному типу токена
\param[in] type Тип токена
\param[in] matching_regex Шаблон для токена
*/
void Tokenizer::add_token(TOKEN_TYPE type, const std::string& matching_regex) {
	std::string s = "^" + matching_regex; // Дополнительно указываем, что шаблон должен совпадать с начала строки
	token_regexes.push_back(TokenRegex{ type, s });
}

/*!
Добавляет тип токена, который следует игнорировать в процессе разбора
\param[in] type Тип токена
*/
void Tokenizer::ignore(TOKEN_TYPE type) {
	ignored_tokens.insert(type);
}

/*!
\brief Выделяет следующий токен из строки-итератора

Выделяет следующий токен из строки-итератора. В случае, когда
токен не нужно игнорировать, он добавляется в выходной вектор
\param[in] start Начало итератора строки
\param[in] end Конец итератора строки
\param[out] tokens Считанные токены
\return Итератор, указывающий на символ, следующий сразу после последнего символа считанного токена
\throw NoTokenFoundError В случае, если не было найдено соответствующего шаблона для извлечения токена
*/
int Tokenizer::extract_token(const std::string& str, int start_index, std::vector<Token>& tokens) {
	std::smatch matched_token;
	TOKEN_TYPE type = TOKEN_TYPE::UNSPECIFIED;

	// Для каждого заданного шаблона токена...
	for (TokenRegex token_regex : token_regexes) {
		// Если шаблон токена был найден...
		if (token_regex.match(str, start_index, matched_token)) {
			// ...извлечь его тип
			type = token_regex.get_type();
			// ...прекратить цикл
			break;
		}
	}

	// Если токен не был определен, то выкинуть исключение о том,
	// что подходящий токен не был найден
	if (type == TOKEN_TYPE::UNSPECIFIED) {
		throw TokenizerError("Синтаксическая ошибка");
	}

	// Если найденный токен не нужно игнорировать, поместить его
	// в список найденных токенов
	if (ignored_tokens.find(type) == ignored_tokens.end()) {
		std::string text = matched_token[0].str();
		int start_index = matched_token[0].first - str.begin();
		int end_index = matched_token[0].second - str.begin() - 1;

		tokens.push_back(Token{ type, text, start_index, end_index });
	}

	return matched_token[0].second - str.begin();
}

/*!
\brief Выделяет токены языка псевдо-ассемблера из строки

Выделяет токены языка псевдо-ассмеблера из строки, добавляя их во входной вектор
\param[in] str Входная строка
\param[out] tokens Считанные токены
*/
void Tokenizer::tokenize(const std::string& str, std::vector<Token>& tokens) {
	int i = 0;

	// Пока не пройдена вся строка...
	while (i < str.size()) {
		// ...извлечь очередной токен
		i = extract_token(str, i, tokens);
	}
}