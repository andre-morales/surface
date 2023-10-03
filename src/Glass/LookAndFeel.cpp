#include "LookAndFeel.h"
#include "GUI.h"
#include "Glass/Elements/ElementType.h"
#include "Elements\Element.h"
#include "SheetReader.h"
#include <algorithm>
#include "Loggy.h"

static Loggy::Logger print{"LAF"};

namespace Glass {
	LookAndFeel::LookAndFeel(GUI& g) : gui(g) {}

	void LookAndFeel::readPrototypes(std::string_view filePath) {
		SheetReader reader{ gui, [&](const auto& name) {
			return gui.make(name);
		}};

		auto protos = reader.read(std::string(filePath));
		
		for (auto& proto : protos) {
			auto typeIndex = proto->type.type;
			prototypes.emplace(typeIndex, std::move(proto));
		}
	}

	std::vector<unique<Element>> LookAndFeel::readFromFile(std::string_view filePath) {
		SheetReader reader{ gui, [&](const auto& name) {
			return make(name);
		} };

		auto elements = reader.read(std::string(filePath));
		return elements;
	}

	unique<Element> LookAndFeel::make(std::type_index type) {
		// Ask GUI to create element
		auto element = gui.make(type);
		
		// Find a prototype, and if none got found, just return the raw element.
		auto it = prototypes.find(type);
		if (it == prototypes.end()) {
			return element;
		}

		// Otherwise, clone prototype into the new element
		print("Found prototype.");
		auto& proto = it->second;
		proto->cloneInto(*element);
		return element;
	}

	unique<Element> LookAndFeel::make(std::string_view typeName) {
		auto& elemType = gui.getElementType(typeName);
		return make(elemType.type);
	}
}