#pragma once
#include "Pointers.h"
#include <string>
#include <vector>
#include <functional>

namespace Glass {
	class GUI;
	class Element;

	class SheetReader {
	public:
		typedef std::function<unique<Element>(const std::string& name)> CreatorFn;

		GUI& gui;
		const CreatorFn creatorFn;

		SheetReader(GUI& gui, CreatorFn creatorFunction);

		std::vector<unique<Element>> read(const std::string& filePath);
	
	private:
		unique<Element> enumElement(const void* elem);
	};
}