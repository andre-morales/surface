#include "SheetElementAttribs.h"
#include "GUI.h"
#include "Strings.h"
#include "TinyXML2\tinyxml2.h"
#include <vector>
#include <utility>

using namespace tinyxml2;

namespace Glass {
	SheetElementAttribs::SheetElementAttribs(GUI& g, const void* n) :
		gui(g), node(n) {}

	std::optional<std::string_view> SheetElementAttribs::get(std::string_view name) const {
		const XMLElement* element = (const XMLElement*)node;

		std::string str {name};
		const char* attr = element->Attribute(str.c_str());
		if (attr) {
			return { attr };
		}
		return {};
	}

	std::vector<std::pair<std::string_view, std::string_view>> SheetElementAttribs::getAll() const {
		const XMLElement* element = (const XMLElement*)node;

		std::vector<std::pair<std::string_view, std::string_view>> vec;
		
		const XMLAttribute* attr = element->FirstAttribute();
		while (attr) {
			vec.emplace_back(attr->Name(), attr->Value());
			attr = attr->Next();
		}
		return vec;
	}

	std::optional<std::vector<std::string_view>> SheetElementAttribs::getStrvVector(std::string_view name) const {
		auto attrOpt = get(name);
		if (!attrOpt) {
			return {};
		}

		return asStrvVector(*attrOpt);
	}

	std::optional<std::vector<float>> SheetElementAttribs::getFloatVector(std::string_view name) const {
		auto attrOpt = get(name);
		if (!attrOpt) {
			return {};
		}

		return asFloatVector(*attrOpt);
	}

	std::optional<shared<Paint>> SheetElementAttribs::getPaint(std::string_view name) const {
		auto attr = get(name);
		if (!attr) return {};

		return asPaint(*attr);
	}

	std::optional<float> SheetElementAttribs::getFloat(std::string_view name) const {
		auto attrOpt = get(name);
		if (!attrOpt) {
			return {};
		}

		float val = std::strtof(std::string(*attrOpt).c_str(), nullptr);
		return { val };
	}

	std::optional<std::vector<std::string_view>> SheetElementAttribs::asStrvVector(std::string_view value) const {
		auto attr = str::trim(value);

		// Sanity checks
		if (attr.length() < 2) return { };
		if (*(attr.begin()) != '[') return {};
		if (*(attr.end() - 1) != ']') return {};

		// Remove brackets
		attr = attr.substr(1, attr.length() - 2);

		// Split the attribute string into substrings
		auto splits = str::split(attr, ",");
		return splits;
	}

	std::optional<std::vector<float>> SheetElementAttribs::asFloatVector(std::string_view value) const {
		auto attrOpt = asStrvVector(value);
		if (!attrOpt) {
			return {};
		}

		std::vector<float> vect;
		for (const auto& split : *attrOpt) {
			std::string str {split};

			float val = std::strtof(str.c_str(), nullptr);
			vect.push_back(val);
		}

		return vect;
	}

	std::optional<shared<Paint>> SheetElementAttribs::asPaint(std::string_view value) const {
		auto paint = gui.getPaint(value);
		if (!paint) return {};

		return paint;
	}
	
}