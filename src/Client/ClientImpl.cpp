#include "ClientImpl.h"
#include "Client.h"
#include "Glow/Window.h"
#include "Glow/GLContext.h"
#include "Glow/Texture.h"
#include "Glass/GUI.h"
#include "Glass/Paints/TexturePaint.h"
#include "Glass/Font.h"

using namespace Glow;
using namespace Glass;

ClientImpl::ClientImpl(Client& c) : This(c) {}

void ClientImpl::mkUIResources() {
	Glow::Window& window = *This.window;
	Glass::GUI& gui = *This.gui;
	Glow::GLContext& ctx = *gui.window.getContext();

	// Common paints
	{
		auto& tex = ctx.addTexture({ "res/ui_button.png" });
		auto tp = mkShared<TexturePaint>(tex);
		tp->corners = { 2. / 64, 2. / 64, 2. / 64, 2. / 64 };
		gui.addPaint("button", tp);
	}
	{
		auto& tex = ctx.addTexture({ "res/ui_button_hover.png" });
		auto p = mkShared<TexturePaint>(tex);
		p->corners = { 2. / 64, 2. / 64, 2. / 64, 2. / 64 };
		gui.addPaint("button-hover", p);
	}
	{
		auto& tex = ctx.addTexture({ "res/ui_button_pressed.png" });
		auto p = mkShared<TexturePaint>(tex);
		p->corners = { 2. / 64, 2. / 64, 2. / 64, 2. / 64 };
		gui.addPaint("button-pressed", p);
	}
	{
		auto& tex = ctx.addTexture({ "res/gui_panel2.png" });
		gui.addPaint("panel2", mkShared<TexturePaint>(tex));
	}

	{
		Texture tex{ "res/image.png" };
		tex.setFiltering(Texture::Filtering::NEAR, Texture::Filtering::NEAR);
		ctx.addTexture("test", std::move(tex));
	}
	{
		Texture tex{ "res/testatlas.png" };
		tex.setFiltering(Texture::Filtering::NEAR, Texture::Filtering::NEAR);
		tex.setWrapping(Texture::Wrapping::REPEAT, Texture::Wrapping::REPEAT);
		ctx.addTexture("grass", std::move(tex));
	}

	// Pixel font
	Texture pixelTexture{ "res/font.png" };
	pixelTexture.setFiltering(Texture::Filtering::NEAR, Texture::Filtering::NEAR);
	ctx.addTexture("pixel_font", std::move(pixelTexture));
	gui.addFont("pixel", Font("pixel", "pixel_font", { 7, 9 }));
}