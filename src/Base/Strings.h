#pragma once
#include <string_view>
#include <iostream>

namespace str {
	template <class... Args>
	constexpr std::string concat(Args&&... args) {
		std::string str;
		(str.append(std::move(args)), ...);
		return str;
	}

	constexpr std::string_view trimStart(std::string_view str) {
		auto len = str.length();
		if (len == 0) return str;

		size_t begin = 0;
		for (; begin < len; ++begin) {
			char c = str.at(begin);
			if (c != ' ') {
				break;
			}
		}

		return str.substr(begin, len - begin);
	}

	constexpr std::string_view trimEnd(std::string_view str) {
		auto len = str.length();
		if (len == 0) return str;

		size_t end = len;
		for (; end > 0; --end) {
			char c = str.at(end - 1);
			if (c != ' ') {
				break;
			}
		}

		return str.substr(0, end);
	}

	constexpr std::string_view trim(std::string_view str) {
		return trimStart(trimEnd(str));
	}

	constexpr std::vector<std::string_view> split(std::string_view str, std::string_view delim) {
		std::vector<std::string_view> vec;
		
		size_t back = 0;
		size_t front = str.find(delim, back);
		if (front == std::string::npos) {
			vec.emplace_back(str);
			return vec;
		}

		while (front != std::string::npos) {
			vec.emplace_back(str.substr(back, front - back));
			
			back = front + 1;
			if (back >= str.length()) break;

			front = str.find(delim, back);
		}

		vec.emplace_back(str.substr(back, str.length() - back));
		return vec;
	}
}