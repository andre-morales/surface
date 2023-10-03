#pragma once
#include "Pointers.h"
#include <string>
#include <typeindex>
#include <unordered_map>

namespace Glass {
	class GUI;
	class Element;

	class LookAndFeel {
	public:
		GUI& gui;
		std::unordered_map<std::type_index, unique<Element>> prototypes;

		LookAndFeel(GUI& g);
		
		/* Reads a set of prototypes from a XML file */
		void readPrototypes(std::string_view filePath);

		/* Creates a set of elements defined in an XML file,
		using the stored prototypes */
		std::vector<unique<Element>> readFromFile(std::string_view filePath);

		/* Constructs the given type of element */
		template <class T>
		unique<T> make() {
			auto elem = make(typeid(T));
			return { elem };
		}

		/* Constructs an element by the name as it was registered */
		unique<Element> make(std::string_view elemTypeName);

		/* Constructs an element given its type info index */
		unique<Element> make(std::type_index type);
	};
}

