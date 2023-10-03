#pragma once
#include "Elements/ElementType.h"
#include "Layouts/LayoutType.h"
#include "Glass/Concepts.h"
#include "GlassException.h"
#include "Math/NamedTypes.h"
#include "Pointers.h"
#include <functional>
#include <string_view>
#include <typeindex>

namespace Glow {
	class Window;
}

namespace Glass {
	class Element;
	class ElementType;
	class Paint;
	class Font;
	class Layout;

	class GUI {
	public:
		Glow::Window& window;

		GUI(Glow::Window&);
		~GUI();
		
		/* Invokes the GUI painting system. Requires a valid graphics system context. */
		void paint();
		
		/* Returns all the elements that have the given screen point in their bounds area.
		The first element in the container is the bottom-most component of the GUI tree,
		and as such, the first element will always be the GUI root. */
		void getElementsInPoint(std::vector<Element*>& stack, Math::Pointf point);

		/* Returns a live reference to a container with all the elements being hovered
		by the mouse, from bottom to top. */
		const std::vector<Element*>& getHoveredElements();

		/* Constructs the given type of element */
		template <ELEMENT E>
		unique<E> make() {
			unique<Element> elem = make(typeid(E));
			E* ptr = (E*) elem.release();
			return unique<E>{ptr};
		}

		/* Constructs an element by the name as it was registered */
		unique<Element> make(const std::string& elemClassName);

		/* Constructs an element given its type info index */
		unique<Element> make(std::type_index type);
		
		/* Registers a type of element with the given name. It is given a default new constructor and no styler by default. */
		template <ELEMENT E>
		void registerElementType(std::string_view typeName) {
			std::type_index index = typeid(E);

			ElementType type{ *this, typeName, index };

			type.constructor = [](auto& type) {
				return new E(type);
			};

			registerElementType(std::move(type));
		}

		/* Registers a type of element with the given name, type info, and constructor function. */
		void registerElementType(ElementType&& elemType);
		
		/* Returns a reference to the Element Class information structure given its registered name */
		ElementType& getElementType(std::string_view typeName);

		/* Returns a reference to the Element Class information struct given its type info index */
		ElementType& getElementType(std::type_index typeIndex);

		/* Returns a reference to the Element Class information structure given a type */
		template <ELEMENT E>
		ElementType& getElementType() {
			return getElementType(typeid(E));
		}

		/* Registers a named paint */
		void addPaint(std::string_view paintName, shared<Paint> paint);

		/* Returns a named paint */
		shared<Paint> getPaint(std::string_view paintName);

		/* Registers a named font */
		void addFont(std::string_view fontName, Font&& font);

		/* Returns a pointer to the named font, or nullptr if no such font exists. */
		Font* getFont(std::string_view fontName);

		template <LAYOUT L>
		void registerLayoutType(std::string_view typeName) {
			std::type_index index = typeid(L);

			LayoutType type{ typeName, index };

			type.constructor = [](auto& type, auto& element) {
				return new L(type, element);
			};

			registerLayoutType(std::move(type));
		}

		void registerLayoutType(LayoutType&& layoutType);

		LayoutType& getLayoutType(std::string_view layoutName) const;
		LayoutType& getLayoutType(std::type_index type) const;
		Element* getElementById(std::string_view id);
		Element* find(std::string_view selector);

		/* Returns the GUI root element. */
		const unique<Element>& getRoot() const;
		
	private:
		friend class Element;

		unique<Element> root;
		std::vector<Element*> hoveredElements, newHoveredElements;
		std::unordered_map<std::string, shared<Paint>> paints;
		std::unordered_map<std::string, unique<Font>> fonts;

		std::unordered_map<std::string, std::type_index> elementTypeNames;
		std::unordered_map<std::string, std::type_index> layoutTypeNames;
		std::unordered_map<std::string, Element*> elementIds;

		void* resizeListener;
		void* mouseListener;
		void* mouseMoveListener;

		void* const elementTypes;
		void* const layoutTypes;

		GUI(GUI&&) = delete;
		GUI(const GUI&) = delete;

		void resize(float width, float height);
		void registerStandardElements();
		void registerStandardLayouts();
		void setupEvents();
		void destroyElement(Element& element);
	};
}