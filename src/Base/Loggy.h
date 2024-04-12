#pragma once

#include <iostream>

namespace Loggy {
	void init();

	template <class... Args>
	void printl(const std::string& pref, Args&&... args) {
		std::cout << "[" << pref << "] ";
		
		((std::cout << args), ...);

		std::cout << '\n';
	}

	class Logger {
	public:
		Logger();
		Logger(const std::string&);

		template <class... Args>
		Logger& operator()(Args&&... args) {
			printl(branch, args...);
			return *this;
		}

	private:
		const std::string branch;
	};

}