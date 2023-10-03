#pragma once
#include "Glass/GlassException.h"
#include "Glass/Concepts.h"
#include "Math/NamedTypes.h"
#include "Pointers.h"
#include <vector>
#include <string>
#include <typeindex>
#include <functional>

namespace Glass {
	class GUI;
	class ElementType;
	class SheetElementAttribs;
	class Layout;
	class Paint;
	class MouseButtonEvent;
	class MouseOverEvent;
	class MouseMoveEvent;

	class Element {
	public:
		typedef void parent_t;
		typedef std::function<void(const MouseButtonEvent&)> MouseButtonListener;
		typedef std::function<void(const MouseMoveEvent&)> MouseMoveListener;
		typedef std::function<void(const MouseOverEvent&)> MouseOverListener;

		ElementType& type;
		GUI& gui;

		virtual ~Element();

		// ---- Hierarchy and Layouting ----
		/* Sets this component's attributes using its sheet properties */
		virtual void setSheetAttribs(const SheetElementAttribs& attribs);

		/* Returns this element's sheet attributes */
		const SheetElementAttribs* getSheetAttribs() const;

		/* Adds a child element */
		void add(unique<Element>&& child);

		/* Returns a live reference to the children of this component */
		const std::vector<unique<Element>>& getChildren() const;

		template <LAYOUT L>
		void setLayout() {
			setLayout(typeid(L));
		}

		void setLayout(std::string_view layoutName);
		void setLayout(std::type_index layoutType);
		void relayout();

		/* Clones this component. */
		template <class T = Element>
		unique<T> clone() const {
			// Create component copy with just interface types
			Element* copyPtr = makeNew();
			unique<Element> copy { copyPtr };

			// Convert and clone into
			unique<T> castedCopy = (unique<T>) copy;
			cloneInto(*castedCopy);
			return castedCopy;
		}

		/* Clones this element's properties into another element */
		virtual void cloneInto(Element& target) const;

		// ---- Painting ----
		/**
		 * Called before the first paint this component will do and never called again.
		 * It is garanteed that this component will belong to valid GUI at the time of
		 * this call.
		 */
		virtual void initPainting();
		void paint(Vector2f anchor);
		void paintChildren(Vector2f anchor);
		virtual void paintSelf(Vector2f anchor);

		// ---- Events ----
		MouseButtonListener* onMouseButton(MouseButtonListener l);
		void removeMouseButton(void* ptr);
		void fireMouseButton(const MouseButtonEvent& ev);

		MouseMoveListener* onMouseMove(MouseMoveListener);
		void removeMouseMove(void* ptr);
		void fireMouseMove(const MouseMoveEvent& ev);
		
		MouseOverListener* onMouseOver(MouseOverListener l);
		void removeMouseOver(void* ptr);
		void fireMouseOver(const MouseOverEvent& ev);

		// ---- Properties ----
		void setId(std::string_view id);
		const std::string& getId() const;

		Element* getParent() const;

		void setPosition(float x, float y);
		void setSize(float w, float h);
		void setBounds(const Math::Rectf& bounds);
		Math::Rectf getBounds();
		void setVisible(bool visible);
		void setOpaque(bool opaque);
		Math::Pointf getScreenPos();

		shared<Paint> defaultBackground, hoverBackground, actionBackground;

	protected:
		Element* parent = nullptr;
		std::string id;
		unique<Layout> layout;
		std::vector<unique<Element>> children;
		Math::Rectf bounds;
		Paint* background = nullptr;
		bool visible = true;
		bool opaque = true;
		bool mouseInside = false;
		bool mouseHeld = false;
		void* mouseListener = nullptr;

		Element(ElementType& type);

		template <class T>
		static T& castRef(Element& c) {
			T* derived = dynamic_cast<T*>(&c);
			if (derived == nullptr) {
				auto msg = std::string() + "Reference cast failed! Derived class [" + typeid(T).name() + "] does not implement makeNew() method.";
				throw GlassException{msg};
			}
			return *derived;
		}

	private:
		friend class GUI;

		bool sheetCreation = false;
		const SheetElementAttribs* sheetAttribs;

		std::vector<MouseButtonListener*> mouseButtonListeners;
		std::vector<MouseOverListener*> mouseOverListeners;
		std::vector<MouseMoveListener*> mouseMoveListeners;

		Element(const Element&) = delete;
		Element(Element&&) = delete;

		Element* makeNew() const;
		void updateBackground();
	};
}