#pragma once
#include "Pointers.h"
#include <string_view>
#include <optional>
#include <vector>

namespace Glass {
	class GUI;
	class SheetReader;
	class Paint;

	class SheetElementAttribs {
	public:
		std::optional<std::string_view> get(std::string_view name) const;
		std::optional<float> getFloat(std::string_view name) const;
		std::vector<std::pair<std::string_view, std::string_view>> getAll() const;

		std::optional<std::vector<std::string_view>> getStrvVector(std::string_view name) const;
		std::optional<std::vector<float>> getFloatVector(std::string_view name) const;
		std::optional<shared<Paint>> getPaint(std::string_view name) const;

		std::optional<std::vector<std::string_view>> asStrvVector(std::string_view value) const;
		std::optional<std::vector<float>> asFloatVector(std::string_view value) const;
		std::optional<shared<Paint>> asPaint(std::string_view name) const;

	private:
		friend class SheetReader;

		GUI& gui;
		const void* const node;

		SheetElementAttribs(GUI& gui, const void* xmlNode);
		SheetElementAttribs(const SheetElementAttribs&) = delete;
		SheetElementAttribs(SheetElementAttribs&&) = delete;
	};
}

