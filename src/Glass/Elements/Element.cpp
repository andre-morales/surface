#include "Element.h"
#include "Glow/Window.h"
#include "Math/vecs.h"
#include "Glass/GUI.h"
#include "Glass/Paints/Paint.h"
#include "Glass/Paints/ColorPaint.h"
#include "Glass/SheetElementAttribs.h"
#include "Glass/Layouts/Layout.h"
#include "Glass/Events/MouseButtonEvent.h"
#include "Glass/Events/MouseOverEvent.h"
#include "ElementType.h"
#include "Loggy.h"

static Loggy::Logger print {"GUI"};

using namespace Glow;

namespace Glass {
	Element::Element(ElementType& et) :
		type(et),
		gui(et.gui),
		bounds({ 0, 0, 32, 32 })
	{

		//defaultBackground = mkShared<ColorPaint>(Math::Colorf{0, 0, 0, 0});
		background = defaultBackground.get();

		onMouseOver([&](const MouseOverEvent& ev) {
			updateBackground();
		});

		onMouseButton([&](const MouseButtonEvent& ev) {
			if (ev.button != MouseButton::LEFT) return;

			if (ev.action == Glow::InputAction::PRESS) {
				mouseHeld = true;
				updateBackground();
			}
		});

		mouseListener = gui.window.addMouseListener([&](MouseButton btn, InputAction act, int mods) {
			if (btn != MouseButton::LEFT) return;

			if (act == InputAction::RELEASE) {
				mouseHeld = false;
				updateBackground();
			}
		});
		
	}

	Element::~Element() {
		gui.window.removeMouseListener(mouseListener);
		gui.destroyElement(*this);
	}

	// ---- Creation and Initialization ----
	void Element::add(unique<Element>&& child) {
		if (child->parent) {
			throw GlassException("Element already has a parent. Remove it first.");
		}
		child->parent = this;
		children.emplace_back(std::move(child));

		relayout();
	}

	const std::vector<unique<Element>>& Element::getChildren() const {
		return children;
	}

	void Element::relayout() {
		if (layout) layout->relayout();
	}

	Element* Element::makeNew() const {
		return type.constructor(type);
	}

	void Element::cloneInto(Element& c) const {
		c.defaultBackground = defaultBackground;
		c.hoverBackground = hoverBackground;
		c.actionBackground = actionBackground;
		c.background = background;
		c.opaque = opaque;
		c.visible = visible;
	}

	void Element::setSheetAttribs(const SheetElementAttribs& attr) {
		sheetAttribs = &attr;
		sheetCreation = true;

		for (const auto& [key, value] : attr.getAll()) {
			if (key == "id") {
				setId(value);
			}

			if (key == "layout") {
				setLayout(value);
			}

			if (key == "background") {
				defaultBackground = attr.asPaint(value).value();
				background = defaultBackground.get();
			}	

			if (key == "hover-background") {
				hoverBackground = attr.asPaint(value).value();				
			}

			if (key == "action-background") {
				actionBackground = attr.asPaint(value).value();
			}

			if (key == "bounds") {
				auto vec = attr.asFloatVector(value).value();
				if (vec.size() != 4) {
					throw Glass::GlassException("Improper attribute format.");
				}

				setBounds({ vec[0], vec[1], vec[2], vec[3] });
			}

			if (key == "bgcolor") {
				auto vec = attr.asFloatVector(value).value();
				if (vec.size() != 4) {
					throw Glass::GlassException("Improper attribute format.");
				}

				defaultBackground = mkShared<ColorPaint>(Math::Colorf{ vec[0], vec[1], vec[2], vec[3] });
				background = defaultBackground.get();
			}

			if (key == "visible") {
				if (value == "true") visible = true;
				if (value == "false") visible = false;
			}

			if (key == "opaque") {
				if (value == "true") opaque = true;
				if (value == "false") opaque = false;
			}
		}
		
		sheetAttribs = nullptr;
		sheetCreation = false;
	}

	const SheetElementAttribs* Element::getSheetAttribs() const {
		return sheetAttribs;
	}

	void Element::setLayout(std::string_view layoutName) {
		auto& type = gui.getLayoutType(layoutName);

		Layout* newLayout = type.constructor(type, *this);
		layout.reset(newLayout);

		relayout();
	}

	void Element::setLayout(std::type_index layoutType) {
		auto& type = gui.getLayoutType(layoutType);

		Layout* newLayout = type.constructor(type, *this);
		layout.reset(newLayout);

		relayout();
	}

	// ---- Painting -----
	void Element::initPainting() {}

	void Element::paint(Vector2f anchor) {
		if (!visible) return;

		paintSelf(anchor);
		paintChildren(anchor);
	}

	void Element::paintChildren(Vector2f anchor) {
		for (const auto& it : children) {
			it->paint(anchor + Vector2f{it->bounds.x, it->bounds.y});
		}
	}

	void Element::paintSelf(Vector2f anchor) {
		if (!background) return;
		background->paint(Math::Rectf{anchor.x, anchor.y, bounds.w, bounds.h});
	}

	// ---- Basic properties ----
	void Element::setId(std::string_view id) {
		// Erase old ID
		gui.elementIds.erase(this->id);

		// Register new id
		this->id = id;
		gui.elementIds.emplace(id, this);
	}

	const std::string& Element::getId() const {
		return id;
	}

	Element* Element::getParent() const {
		return parent;
	}

	void Element::setPosition(float x, float y) {
		bounds.x = x;
		bounds.y = y;
	}

	void Element::setSize(float w, float h) {
		bounds.w = w;
		bounds.h = h;
		relayout();
	}

	void Element::setBounds(const Math::Rectf& b) {
		bounds = b;
		relayout();
	}

	Math::Rectf Element::getBounds() {
		return bounds;
	}

	void Element::setVisible(bool v) {
		visible = v;
	}

	void Element::setOpaque(bool v) {
		opaque = v;
	}

	Math::Pointf Element::getScreenPos() {
		if (!parent) {
			return { bounds.x, bounds.y };
		}

		return { parent->bounds.x + bounds.x, parent->bounds.y + bounds.y };
	}

	void Element::updateBackground() {
		if (mouseHeld && actionBackground) {
			background = actionBackground.get();
		} else if (mouseInside && hoverBackground) {
			background = hoverBackground.get();
		} else {
			background = defaultBackground.get();
		}
	}

	// ---- Events ----
	template <class T>
	T* vecAdd(std::vector<T*>& vec, T&& value) {
		T* ptr = new T(std::move(value));
		vec.push_back(ptr);
		return ptr;
	}

	template <class T>
	T* vecErase(std::vector<T*>& vec, void* value) {
		auto it = std::find(vec.begin(), vec.end(), (T*)value);
		
		if (it == vec.end()) return nullptr;

		auto ptr = *it;
		vec.erase(it);
		return ptr;
	}
	
	Element::MouseButtonListener* Element::onMouseButton(MouseButtonListener l) {
		return vecAdd(mouseButtonListeners, std::move(l));
	}

	void Element::removeMouseButton(void* ptr) {
		delete vecErase(mouseButtonListeners, ptr);
	}

	void Element::fireMouseButton(const MouseButtonEvent& ev) {
		for (const auto& l : mouseButtonListeners) {
			(*l)(ev);
		}
	}

	Element::MouseMoveListener* Element::onMouseMove(MouseMoveListener l) {
		return vecAdd(mouseMoveListeners, std::move(l));
	}

	void Element::removeMouseMove(void* ptr) {
		delete vecErase(mouseMoveListeners, ptr);
	}

	void Element::fireMouseMove(const MouseMoveEvent& ev) {
		for (const auto& l : mouseMoveListeners) {
			(*l)(ev);
		}
	}

	Element::MouseOverListener* Element::onMouseOver(MouseOverListener l) {
		return vecAdd(mouseOverListeners, std::move(l));
	}

	void Element::removeMouseOver(void* ptr) {
		delete vecErase(mouseOverListeners, ptr);
	}

	void Element::fireMouseOver(const MouseOverEvent& ev) {
		for (const auto& l : mouseOverListeners) {
			(*l)(ev);
		}
	}
}