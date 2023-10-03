#include "GUI.h"
#include "Elements/ElementType.h"
#include "Elements/Element.h"
#include "Elements/Button.h"
#include "Elements/Text.h"
#include "Layouts/StackLayout.h"
#include "Layouts/SplitLayout.h"
#include "Events/MouseButtonEvent.h"
#include "Events/MouseMoveEvent.h"
#include "Events/MouseOverEvent.h"
#include "Glow/Window.h"
#include "Font.h"
#include "Exception.h"
#include "GL/glew.h"
#include <algorithm>
#include <iostream>
#include <ranges>

using namespace Glow;
using namespace std::string_literals;

typedef std::unordered_map<std::type_index, Glass::ElementType> ElementTypesMap;
typedef std::unordered_map<std::type_index, Glass::LayoutType> LayoutTypesMap;

namespace Glass {
	GUI::GUI(Glow::Window& w) :
		window(w),
		elementTypes(new ElementTypesMap()),
		layoutTypes(new LayoutTypesMap()) {

		registerStandardElements();
		registerStandardLayouts();

		root = { make<Element>() };
		root->setId("gui-root");
		root->setLayout<StackLayout>();

		setupEvents();

		resize((float)w.getWidth(), (float)w.getHeight());
	}

	GUI::~GUI() {
		window.removeResizeListener(resizeListener);
		window.removeMouseListener(mouseListener);
		window.removeMouseMoveListener(mouseMoveListener);

		root.release();

		delete ((ElementTypesMap*)elementTypes);
		delete ((LayoutTypesMap*)layoutTypes);
	}

	void GUI::registerStandardElements() {
		registerElementType<Element>("element");
		registerElementType<Button>("button");
		registerElementType<Text>("text");
	}

	void GUI::registerStandardLayouts() {
		registerLayoutType<StackLayout>("stack");
		registerLayoutType<SplitLayout>("split");
	}

	void GUI::setupEvents() {
		resizeListener = window.addResizeListener([&](int w, int h) {
			resize((float)w, (float)h);
		});

		mouseListener = window.addMouseListener([&](MouseButton btn, InputAction act, int mods) {
			MouseButtonEvent ev{ btn, act, mods };

			for (auto& el : hoveredElements) {
				el->fireMouseButton(ev);
			}
		});

		mouseMoveListener = window.addMouseMoveListener([&](float x, float y) {
			// oldVec refers to the current live vector of hovered elements
			// newVec refers to the spare vector used to store new elements
			auto& oldVec = hoveredElements;
			auto& newVec = newHoveredElements;

			getElementsInPoint(newVec, { x, y });

			/*for (auto& el : newVec) {
				std::cout << (" : ["s + el->id + "]");
			}
			printf("\n");*/

			// Find the difference between the old hovered elements and the new ones
			auto its = std::mismatch(oldVec.begin(), oldVec.end(), newVec.begin(), newVec.end());
			auto& [oldIt, newIt] = its;

			// For every old hovered element that isn't hovered anymore, fire mouse leave events
			MouseOverEvent leaveEvent{ true };
			while (oldIt != oldVec.end()) {
				Element* elem = *oldIt;
				elem->mouseInside = false;
				elem->fireMouseOver(leaveEvent);
				oldIt++;
			}

			// For every new hovered element that wasn't hovered before, fire mouse enter events
			MouseOverEvent enterEvent{ false };
			while (newIt != newVec.end()) {
				Element& elem = **(newIt++);
				elem.mouseInside = true;
				elem.fireMouseOver(enterEvent);
			}

			// Move the new elements to the live vector
			std::swap(hoveredElements, newHoveredElements);
			newHoveredElements.clear();

			// For every currently hovered element, fire the actual mouse move event
			MouseMoveEvent moveEvent{ x, y };
			for (Element* elem : hoveredElements) {
				elem->fireMouseMove(moveEvent);
			}
		});
	}

	void GUI::paint() {
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glScalef(1, -1, 1);
		glTranslatef(-1, -1, 0);
		glScalef(2.0f / window.getWidth(), 2.0f / window.getHeight(), 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		root->paint({ 0, 0 });
	}


	void GUI::getElementsInPoint(std::vector<Element*>& stack, Math::Pointf point) {
		Element* elem = root.get();
		// If the root is not visible or does not contain the point, return
		if (!elem->visible || !elem->bounds.contains(point)) return;

		while (elem != nullptr) {
			// Add current element to vect
			stack.push_back(elem);

			// Shift the point, so that it becomes relative to the element
			point -= Math::Pointf{ elem->bounds.x, elem->bounds.y };

			// Find next element
			const auto& children = elem->children;

			elem = nullptr;
			for (const auto& child : std::ranges::views::reverse(children)) {
				if (child->visible &&
					child->opaque &&
					child->bounds.contains(point)) {
					elem = child.get();
					break;
				}
			}
		}
	}

	const std::vector<Element*>& GUI::getHoveredElements() {
		return hoveredElements;
	}

	void GUI::destroyElement(Element& element) {
		// Unregister element's id if it has one
		if (!element.id.empty()) {
			elementIds.erase(element.id);
		}

		// If the element destroyed was being hovered, remove it and all of its children
		// (the elements after it) from the hovered elements vector
		Element* ptr = &element;
		auto it = std::find(hoveredElements.begin(), hoveredElements.end(), ptr);
		hoveredElements.erase(it, hoveredElements.end());
	}


	Element* GUI::find(std::string_view selector) {
		if (selector.at(0) == '#') {
			return getElementById(selector.substr(1));
		} else {
			throw GlassException("Invalid selector");
		}
	}

	void GUI::registerElementType(ElementType&& elemType) {
		auto& types = *(ElementTypesMap*)elementTypes;

		std::type_index typeIndex = elemType.type;
		elementTypeNames.emplace(elemType.name, typeIndex);
		types.emplace(typeIndex, std::move(elemType));
	}

	ElementType& GUI::getElementType(std::string_view typeName) {
		auto it = elementTypeNames.find(std::string(typeName));
		if (it == elementTypeNames.end()) {
			throw GlassException("Element class not found.");
		}

		auto type = it->second;
		return getElementType(type);
	}

	ElementType& GUI::getElementType(std::type_index typeIndex) {
		auto& classes = *(ElementTypesMap*)elementTypes;

		auto it = classes.find(typeIndex);
		if (it == classes.end()) {
			throw GlassException("Element class not found.");
		}

		return it->second;
	}

	void GUI::registerLayoutType(LayoutType&& type) {
		auto& types = *(LayoutTypesMap*)layoutTypes;

		std::type_index typeIndex = type.type;
		layoutTypeNames.emplace(type.name, typeIndex);
		types.emplace(typeIndex, std::move(type));
	}

	unique<Element> GUI::make(std::type_index type) {
		ElementType& ec = getElementType(type);

		ElementType::ConstructorFn constructor = ec.constructor;
		unique<Element> elem{ constructor(ec) };
		return elem;
	}

	unique<Element> GUI::make(const std::string& className) {
		auto it = elementTypeNames.find(className);
		if (it == elementTypeNames.end()) {
			throw GlassException("Element class not found.");
		}

		return make(it->second);
	}

	void GUI::addPaint(std::string_view name, shared<Paint> paint) {
		paints.emplace(name, paint);
	}

	shared<Paint> GUI::getPaint(std::string_view name) {
		auto it = paints.find(std::string(name));
		if (it == paints.end()) {
			return { nullptr };
		}
		return it->second;
	}

	void GUI::addFont(std::string_view name, Font&& font) {
		fonts.emplace(name, new Font(std::move(font)));
	}

	Font* GUI::getFont(std::string_view name) {
		auto it = fonts.find(std::string(name));
		if (it == fonts.end()) return nullptr;

		return it->second.get();
	}

	LayoutType& GUI::getLayoutType(std::string_view layoutName) const {
		auto it = layoutTypeNames.find(std::string(layoutName));
		if (it == layoutTypeNames.end()) {
			throw GlassException("Layout class ["s + std::string(layoutName) + "] not registered in GUI.");
		}

		auto type = it->second;
		return getLayoutType(type);
	}

	LayoutType& GUI::getLayoutType(std::type_index type) const {
		auto& types = *(LayoutTypesMap*)layoutTypes;

		auto it = types.find(type);
		if (it == types.end()) {
			throw GlassException("Layout class ["s + type.name() + "] not registered in GUI.");
		}

		return it->second;
	}

	Element* GUI::getElementById(std::string_view id) {
		auto it = elementIds.find(std::string{ id });
		if (it == elementIds.end()) return nullptr;

		return it->second;
	}

	void GUI::resize(float width, float height) {
		root->setBounds({ 0, 0, width, height });
	}

	const unique<Element>& GUI::getRoot() const {
		return root;
	}
}