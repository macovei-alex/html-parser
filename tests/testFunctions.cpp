#include "testFunctions.hpp"

#include "lib/fileUtils.hpp"
#include "lib/htmlParser.hpp"

#include <format>
#include <sstream>
#include <string_view>
#include <filesystem>



#define HP_TEST_COLOR_RESET		"\033[0m"
#define HP_TEST_COLOR_GREEN		"\033[32m"
#define HP_TEST_COLOR_RED		"\033[31m"
#define HP_TEST_COLOR_YELLOW	"\033[33m"



std::ostream& operator<<(std::ostream& os, const std::vector<hp::_private::Token>& tokens) {
	os << '[';
	for (const auto& token : tokens) {
		os << std::format("({}:{}:{}:{} -> \"{}\")\n",
			getTokenName(token.type),
			token.contentPos,
			token.line,
			token.linePos,
			token.value);
	}
	os << ']';
	return os;
}

template<typename T>
bool operator==(const std::vector<T>& lhs, const std::vector<T>& rhs) {
	if (lhs.size() != rhs.size()) {
		return false;
	}
	for (size_t i = 0; i < lhs.size(); ++i) {
		if (lhs[i] != rhs[i]) {
			return false;
		}
	}
	return true;
}


void hp::test::testTokenize(int testId, std::ostream& os) {
	const std::string testFile = std::format("resources/test-tokenize-{}.html", testId);
	const std::string expectFile = std::format("resources/expect-tokenize-{}.txt", testId);

	if (!std::filesystem::exists(testFile)) {
		os << "Test file missing from location: ( " << testFile << " )\n";
		return;
	}
	if (!std::filesystem::exists(expectFile)) {
		os << "Expect file missing from location: ( " << expectFile << " )\n";
		return;
	}

	std::string content = hp::readContents(testFile);
	auto tokens = hp::_private::tokenize(content);

	std::string expectedContent = hp::readContents(expectFile);
	auto expectedTokens = hp::test::_private::parseExpectContent(expectedContent);

	if (tokens == expectedTokens) {
		std::cout << HP_TEST_COLOR_GREEN "Test " << testId << " passed\n" HP_TEST_COLOR_RESET;
	}
	else {
		const ptrdiff_t missmatchPos = std::distance(
			expectedTokens.begin(),
			std::mismatch(expectedTokens.begin(), expectedTokens.end(), tokens.begin()).first
		);

		std::cerr << HP_TEST_COLOR_RED "Test " << testId << " failed:\n" HP_TEST_COLOR_YELLOW
			<< "  Expected:\n" << expectedTokens << "\n"
			<< "  Actual:\n" << tokens << "\n" HP_TEST_COLOR_RESET;

		std::cerr << "First difference at index: " << missmatchPos << '\n';
		std::cout << std::format("(({}:{}:{}:{} -> {}))\n",
			getTokenName(tokens[missmatchPos].type),
			tokens[missmatchPos].contentPos,
			tokens[missmatchPos].line,
			tokens[missmatchPos].linePos,
			tokens[missmatchPos].value);
		std::cout << std::format("(({}:{}:{}:{} -> {}))\n",
			getTokenName(expectedTokens[missmatchPos].type),
			expectedTokens[missmatchPos].contentPos,
			expectedTokens[missmatchPos].line,
			expectedTokens[missmatchPos].linePos,
			expectedTokens[missmatchPos].value);
	}
}


std::vector<hp::_private::Token> hp::test::_private::parseExpectContent(std::string& content) {
	using namespace hp::_private;

	static const std::unordered_map<std::string, TokenType> tokenTypeMap = {
		{ "Unknown", TokenType::Unknown },
		{ "TagStartOpen", TokenType::TagStartOpen },
		{ "TagEndOpen", TokenType::TagEndOpen },
		{ "TagClose", TokenType::TagClose },
		{ "TagName", TokenType::TagName },
		{ "AttributeName", TokenType::AttributeName },
		{ "AttributeValue", TokenType::AttributeValue },
		{ "Text", TokenType::Text },
		{ "Comment", TokenType::Comment },
		{ "WhiteSpace", TokenType::WhiteSpace },
	};

	while (content[content.size() - 1] != ')') {
		content.pop_back();
	}

	std::vector<Token> tokens;
	size_t pos = 0;

	while (pos < content.size()) {
		if (content[pos] != '(') {
			throw std::runtime_error(std::format("Invalid expect file format: expected ( '(' ) at position ( {} )", pos));
		}
		++pos;

		size_t aux = pos;
		while (content[pos] != ':') {
			++pos;
		}

		std::string tokenName = content.substr(aux, pos - aux);

		++pos;
		aux = pos;
		while (content[pos] != ':') {
			++pos;
		}
		std::string contentPos = content.substr(aux, pos - aux);
		size_t contentPosVal = std::stoul(contentPos);
		++pos;

		aux = pos;
		while (content[pos] != ':') {
			++pos;
		}
		std::string line = content.substr(aux, pos - aux);
		size_t lineVal = std::stoul(line);
		++pos;

		aux = pos;
		while (content[pos] != ' ') {
			++pos;
		}
		std::string linePos = content.substr(aux, pos - aux);
		size_t linePosVal = std::stoul(linePos);

		if (content.substr(pos, 4) != " -> ") {
			throw std::runtime_error(std::format("Expected ( \" -> \" ) at position ( {} ), but got ( {} ) instead", pos, content.substr(pos, 4)));
		}

		pos += 5;

		aux = pos;
		while (content[pos] != '\"' || content[pos + 1] != ')') {
			++pos;
		}
		std::string tokenValue = content.substr(aux, pos - aux);

		pos += 2;

		auto it = tokenTypeMap.find(tokenName);
		if (it != tokenTypeMap.end()) {
			tokens.emplace_back(tokenTypeMap.at(tokenName), std::move(tokenValue), contentPosVal, lineVal, linePosVal);
		}
		else {
			throw std::runtime_error(std::format("Invalid token type name: ( {} )", tokenName));
		}

		++pos;
	}

	return tokens;
}
