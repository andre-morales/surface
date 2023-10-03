#include "SheetReader.h"
#include "Elements\Element.h"
#include "SheetElementAttribs.h"
#include "GUI.h"
#include "Loggy.h"
#include "TinyXML2\tinyxml2.h"

using namespace tinyxml2;

Loggy::Logger print{"Glass"};

namespace Glass {
	SheetReader::SheetReader(GUI& g, CreatorFn fn) :
		gui(g), creatorFn(fn) {}

	std::vector<unique<Element>> SheetReader::read(const std::string& filePath) {
		std::vector<unique<Element>> comps;

		XMLDocument doc;
		doc.LoadFile(filePath.c_str());
		
		XMLElement* child = doc.FirstChildElement();
		while (child != nullptr) {
			auto&& element = enumElement(child);
			comps.emplace_back(std::move(element));

			child = child->NextSiblingElement();
		}

		return comps;
	}

	unique<Element> SheetReader::enumElement(const void* e) {
		// Bringing back type
		const XMLElement* xmlElem = (const XMLElement*) e;

		// Create component itself
		auto elemType = xmlElem->Name();
		auto element = creatorFn(elemType);

		// Set element's attributes
		SheetElementAttribs attrs { gui, xmlElem };
		element->setSheetAttribs(attrs);

		// Enumerate children
		const XMLElement* child = xmlElem->FirstChildElement();
		while (child != nullptr) {
			// Enum child and add it
			auto childComponent = enumElement(child);
			element->add(std::move(childComponent));

			child = child->NextSiblingElement();
		}

		return element;
	}
}