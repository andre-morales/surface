#include "GLContext.h"
#include "Texture.h"
#include <unordered_map>
#include "GL/glew.h"
#include "Loggy.h"

static Loggy::Logger print{ "GL" };

using namespace std;

namespace Glow {
	GLContext::GLContext(Window& w) : window(w) {}

	GLContext::~GLContext() {}

	Texture& GLContext::addTexture(Texture&& texture) {
		// Automatic name
		std::string name = std::string("_t") + std::to_string(nameCounter);
		nameCounter++;
		
		return addTexture(name, std::move(texture));
	}

	Texture& GLContext::addTexture(std::string_view name, Texture&& texture) {
		// Create new texture based on the moved one
		auto ptr = new Texture(std::move(texture));

		print("Registered texture [", name, "]: ");

		// Create texture
		textures.emplace(name, ptr);
		return *ptr;
	}

	Texture* GLContext::getTexture(std::string_view name) {
		auto t = textures.find(std::string(name));
		if (t != textures.end()) {
			return t->second.get();
		}
		return nullptr;
	}

	// -- OpenGL Functions --
	void GLContext::enableFog() {
		if (!fog) { fog = true; glEnable(GL_FOG); }
	}
	void GLContext::disableFog() {
		if (fog) { fog = false; glDisable(GL_FOG); }
	}

	void GLContext::enableTexture2d() {
		if (!texture2d) { texture2d = true; glEnable(GL_TEXTURE_2D); }
	}
	void GLContext::disableTexture2d() {
		if (texture2d) { texture2d = false; glDisable(GL_TEXTURE_2D); }
	}

	void GLContext::enableAlphaTesting() {
		if (!alphaTesting) { alphaTesting = true; glEnable(GL_ALPHA_TEST); }
	}
	void GLContext::disableAlphaTesting() {
		if (alphaTesting) { alphaTesting = false; glDisable(GL_ALPHA_TEST); }
	}

	void GLContext::enableDepthTesting() {
		if (!depthTesting) { depthTesting = true; glEnable(GL_DEPTH_TEST); }
	}
	void GLContext::disableDepthTesting() {
		if (depthTesting) { depthTesting = false; glDisable(GL_DEPTH_TEST); }
	}

	void GLContext::enableBlending() {
		if (!blending) { blending = true; glEnable(GL_BLEND); }
	}
	void GLContext::disableBlending() {
		if (blending) { blending = false; glDisable(GL_BLEND); }
	}
	void GLContext::setBlending(uint32 sfactor, uint32 dfactor) {
		if (blendingSource != sfactor || blendingDestination != dfactor) {
			blendingSource = sfactor;
			blendingDestination = dfactor;
			glBlendFunc(sfactor, dfactor);
		}
	}

	void GLContext::setShadingModel(uint32 sm) {
		if (shadingModel != sm) {
			shadingModel = sm;
			glShadeModel(sm);
		}
	}

	// Built-in Lighting
	void GLContext::setLighting(bool lighting) {
		if (lighting) enableLighting();
		else disableLighting();
	}
	void GLContext::enableLighting() {
		if (!lighting) { lighting = true; glEnable(GL_LIGHTING); }
	}
	void GLContext::disableLighting() {
		if (lighting) { lighting = false; glDisable(GL_LIGHTING); }
	}

	// Face Culling
	void GLContext::setFaceCulling(bool faceCulling) {
		if (faceCulling) enableFaceCulling();
		else disableFaceCulling();
	}
	void GLContext::enableFaceCulling() {
		if (!faceCulling) { faceCulling = true; glEnable(GL_CULL_FACE); }
	}
	void GLContext::disableFaceCulling() {
		if (faceCulling) { faceCulling = false; glDisable(GL_CULL_FACE); }
	}

	void GLContext::enableVertsArray() {
		if (!vertsArray) { vertsArray = true; glEnableClientState(GL_VERTEX_ARRAY); }
	}
	void GLContext::disableVertsArray() {
		if (vertsArray) { vertsArray = false; glDisableClientState(GL_VERTEX_ARRAY); }
	}

	void GLContext::enableUVsArray() {
		if (!uvsArray) { uvsArray = true; glEnableClientState(GL_TEXTURE_COORD_ARRAY); }
	}
	void GLContext::disableUVsArray() {
		if (uvsArray) { uvsArray = false; glDisableClientState(GL_TEXTURE_COORD_ARRAY); }
	}

	// Normals Array
	void GLContext::setNormalsArray(bool normalsArray) {
		if (normalsArray) enableNormalsArray();
		else disableNormalsArray();
	}
	void GLContext::enableNormalsArray() {
		if (!normalsArray) { normalsArray = true; glEnableClientState(GL_NORMAL_ARRAY); }
	}
	void GLContext::disableNormalsArray() {
		if (normalsArray) { normalsArray = false; glDisableClientState(GL_NORMAL_ARRAY); }
	}

	void GLContext::setColorsArray(bool colorsArray) {
		if (colorsArray) enableColorsArray();
		else disableColorsArray();
	}
	void GLContext::enableColorsArray() {
		if (!colorsArray) { colorsArray = true; glEnableClientState(GL_COLOR_ARRAY); }
	}
	void GLContext::disableColorsArray() {
		if (colorsArray) { colorsArray = false; glDisableClientState(GL_COLOR_ARRAY); }
	}

	void GLContext::enableColorMaterial() {
		if (!colorMaterial) { colorMaterial = true; glEnable(GL_COLOR_MATERIAL); }
	}
	void GLContext::disableColorMaterial() {
		if (colorMaterial) { colorMaterial = false; glDisable(GL_COLOR_MATERIAL); }
	}


	// OpenGL 2.0
	void GLContext::bindShader(uint32 shader) {
		if (shaderId != shader) {
			shaderId = shader;
			glUseProgram(shader);
		}
	}

	// OpenGL 3.0
	void GLContext::bindFramebuffer(uint32 framebuffer) {
		if (frameBufferId != framebuffer) {
			frameBufferId = framebuffer;
			glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		}
	}

}