/*
string.hpp - v1.0

Usage:

	Include this file into any C++ file where you need access to the functionalities provided.
	There are no linkage problems since this file only containes template definitions.

C++ Standard:

	This file is written for C++20 and above.

*/

#pragma once


#include <string>
#include <string_view>
#include <vector>


namespace xstd
{
	/// @brief Template type to shorten the std::basic_string template type name.
	template<class T>
	using string = std::basic_string<T, std::char_traits<T>, std::allocator<T>>;

	/// @brief Template type to shorten the std::basic_string_view template type name.
	template<class T>
	using string_view = std::basic_string_view<T>;


	/// @brief Removes leading and trailing characters from a string.
	/// @param str - The string to trim.
	/// @param chars - The set of the characters to trim.
	/// @tparam T - The character type to used.
	/// @return The trimmed string.
	template<class T>
	[[nodiscard]] string<T> trim(const string<T>& str, string_view<T> chars) noexcept
	{
		size_t first = str.find_first_not_of(chars);
		if (first == string<T>::npos) {
			return string<T>();
		}
		size_t last = str.find_last_not_of(chars);
		return str.substr(first, (last - first + 1));
	}


	/// @brief Removes leading and trailing characters from a string.
	/// @param str - The string to trim.
	/// @param chars - The set of the characters to trim.
	/// @tparam T - The character type to used.
	/// @return The trimmed string.
	template<class T>
	[[nodiscard]] string<T> trim(const string<T>& str, const T* chars) noexcept
	{
		size_t first = str.find_first_not_of(chars);
		if (first == string<T>::npos) {
			return string<T>();
		}
		size_t last = str.find_last_not_of(chars);
		return str.substr(first, (last - first + 1));
	}


	/// @brief Remove leading characters from a string.
	/// @param str - The string to trim.
	/// @param chars - The set of the characters to trim.
	/// @tparam T - The character type used.
	/// @return The trimmed string.
	template<class T>
	[[nodiscard]] string<T> trim_left(const string<T>& str, string_view<T> chars) noexcept
	{
		size_t first = str.find_first_not_of(chars);
		if (first == string<T>::npos) {
			return string<T>();
		}
		return str.substr(first);
	}


	/// @brief Remove trailing characters from a string.
	/// @param str - The string to trim.
	/// @param chars - The set of the characters to trim.
	/// @tparam T - The character type the string uses.
	/// @return The trimmed string.
	template<class T>
	[[nodiscard]] string<T> trim_right(const string<T>& str, string_view<T> chars) noexcept
	{
		size_t last = str.find_last_not_of(chars);
		if (last == string<T>::npos) {
			return string<T>();
		}
		return str.substr(0, last + 1);
	}


	/// @brief Split a string into a pair of strings by any characters in the given set. The
	/// separators in front are ignored. Multiple separators in the middle are ignored. The
	/// separators at the end will be part of the last string.
	/// @param str - The string to split.
	/// @param chars - The separator characters.
	/// @tparam T - The character type used.
	/// @return The pair of strings. One or more strings can be empty.
	template<class T>
	[[nodiscard]] std::pair<string<T>, string<T>> split_one_by_any(
		const string<T>& str,
		string_view<T> chars)
	{
		if (chars.empty()) {
			return { str, string<T>() };
		}
		if (str.empty()) {
			return { string<T>(), string<T>() };
		}

		size_t first_char_pos = 0;
		size_t sep_pos2 = str.find_first_of(chars);
		if (sep_pos2 == string<T>::npos) {
			return { str, string<T>() };
		}

		while (sep_pos2 == first_char_pos) {
			first_char_pos = sep_pos2 + 1;
			sep_pos2 = str.find_first_of(chars, first_char_pos);
		}
		if (sep_pos2 == string<T>::npos) {
			return { str.substr(first_char_pos), string<T>() };
		}

		size_t first_word_len = sep_pos2 - first_char_pos;
		while (str.find_first_of(chars, sep_pos2 + 1) == sep_pos2 + 1) {
			++sep_pos2;
		}

		if (sep_pos2 == string<T>::npos) {
			return { str.substr(first_char_pos), string<T>() };
		}
		return { str.substr(first_char_pos, first_word_len), str.substr(sep_pos2 + 1) };
	}


	/// @brief Split a string into a pair of strings by a separator. The separators in front are
	/// ignored. Multiple separators in the middle are ignored. The separators at the end will be
	/// part of the last string. The split is done by the whole separator string.
	/// @param str - The string to split.
	/// @param sep - The separator string.
	/// @tparam T - The character type used.
	/// @return The pair of strings. One or more strings can be empty.
	template<class T>
	[[nodiscard]] std::pair<string<T>, string<T>> split_one_by_all(
		const string<T>& str,
		string_view<T> sep)
	{
		static constexpr auto jump_over_sep = [](
			const string<T>& str,
			string_view<T> sep,
			size_t start) -> size_t
		{
			const size_t sep_len = sep.size();
			size_t sep_pos = str.find(sep, start);
			while (sep_pos == start) {
				start += sep_len;
				sep_pos = str.find(sep, start);
			}
			return start;
		};

		if (sep.empty()) {
			return { str, string<T>() };
		}
		if (sep.size() == 1) {
			return split_one_by_any<T>(str, sep.substr(0, 1));
		}

		size_t start = jump_over_sep(str, sep, 0);
		if (start == string<T>::npos || start == str.size()) {
			return { string<T>(), string<T>() };
		}

		size_t found = str.find(sep, start + 1);
		if (found == string<T>::npos) {
			return { str.substr(start), string<T>() };
		}

		size_t first_word_len = found - start;

		size_t start2 = jump_over_sep(str, sep, found + sep.size());
		if (start2 == string<T>::npos) {
			return { str.substr(start, first_word_len), string<T>() };
		}
		return { str.substr(start, first_word_len), str.substr(start2) };
	}


	/// @brief Split a string into a vector of strings by any of the characters given. All the
	/// separators are ignored.
	/// @param str - The string to split.
	/// @param chars - The separator character.
	/// @tparam T - The character type used.
	/// @return The vector of strings. None of the strings will be empty.
	template<class T>
	[[nodiscard]] std::vector<string<T>> split_by_any(const string<T>& str, string_view<T> chars)
	{
		if (str.empty()) {
			return { };
		}
		if (chars.empty()) {
			return { str };
		}

		size_t start = str.find_first_not_of(chars);
		if (start == string<T>::npos) {
			return { };
		}
		size_t found = str.find_first_of(chars, start + 1);
		if (found == string<T>::npos) {
			return { str.substr(start) };
		}

		std::vector<string<T>> result;
		while (found != string<T>::npos) {
			result.emplace_back(str.begin() + start, str.begin() + found);
			start = str.find_first_not_of(chars, found + 1);
			found = str.find_first_of(chars, start);
		}

		if (start < str.size()) {
			result.emplace_back(str.begin() + start, str.end());
		}
		return result;
	}


	/// @brief Split a string into a vector of strings by a separator. All the separators are
	/// ignored. The split is done by the whole separator string.
	/// @param str - The string to split.
	/// @param sep - The separator string.
	/// @return The vector of strings. None of the strings will be empty.
	template<class T>
	[[nodiscard]] std::vector<string<T>> split_by_all(const string<T>& str, string_view<T> sep)
	{
		static constexpr auto jump_over_sep = [](
			const string<T>& str,
			string_view<T>& sep,
			size_t start) -> size_t
		{
			const size_t sep_len = sep.size();
			size_t sep_pos = str.find(sep, start);
			while (sep_pos == start) {
				start += sep_len;
				sep_pos = str.find(sep, start);
			}
			return start;
		};

		
		if (str.empty()) {
			return { };
		}
		if (sep.empty()) {
			return { str };
		}
		if (sep.size() == 1) {
			return split_by_any(str, sep.substr(0, 1));
		}

		if (str.find(sep) == std::string::npos) {
			return { str };
		}

		size_t start = jump_over_sep(str, sep, 0);
		if (start == string<T>::npos || start == str.size()) {
			return { };
		}

		size_t found = str.find(sep, start + 1);
		if (found == string<T>::npos) {
			return { str.substr(start) };
		}

		std::vector<string<T>> result;
		while (found != string<T>::npos) {
			result.emplace_back(str.begin() + start, str.begin() + found);
			start = jump_over_sep(str, sep, found);
			found = str.find(sep, start);
		}

		if (start < str.size()) {
			result.emplace_back(str.begin() + start, str.end());
		}
		return result;
	}
}
