#pragma once
#include "Pointers.h"
#include "Types.h"
#include <string>
#include <unordered_map>

namespace Glow {
	class Window;
	class Texture;

	class GLContext {
	public:
		Window& window;

		GLContext(Window& w);
		~GLContext();

		/* Registers an anonymous texture */
		Texture& addTexture(Texture&& texture);
		
		/* Register a named texture */
		Texture& addTexture(std::string_view name, Texture&& texture);

		/* Returns a pointer to the given named texture, or nullptr if no texture with such name
		was found. */
		Texture* getTexture(std::string_view name);

		// -- OpenGL Functions --
		void enableFog();
		void disableFog();

		void enableTexture2d();
		void disableTexture2d();

		void enableAlphaTesting();
		void disableAlphaTesting();

		void enableDepthTesting();
		void disableDepthTesting();

		void enableBlending();
		void disableBlending();
		void setBlending(uint32, uint32);

		void setShadingModel(uint32);

		bool lighting = false;
		void setLighting(bool);
		void enableLighting();
		void disableLighting();

		void setFaceCulling(bool);
		void enableFaceCulling();
		void disableFaceCulling();

		bool colorsArray = false;
		void enableVertsArray();
		void disableVertsArray();

		void enableUVsArray();
		void disableUVsArray();

		void setNormalsArray(bool);
		void enableNormalsArray();
		void disableNormalsArray();

		void setColorsArray(bool);
		void enableColorsArray();
		void disableColorsArray();

		void enableColorMaterial();
		void disableColorMaterial();

		// OpenGL 2.0
		uint32 shaderId;
		void bindShader(uint32);

		// OpenGL 3.0
		uint32 frameBufferId;
		void bindFramebuffer(uint32);
	private:
		bool fog, texture2d, depthTesting, alphaTesting, blending, faceCulling;
		bool vertsArray, uvsArray, normalsArray;
		uint32 blendingSource, blendingDestination, shadingModel;
		uint32 texture2dId, arrayBufferId;
		bool colorMaterial;

		uint32_t nameCounter = 0;
		std::unordered_map<std::string, unique<Texture>> textures;

		GLContext(const GLContext&) = delete;
		GLContext(GLContext&&) = delete;
	};
}