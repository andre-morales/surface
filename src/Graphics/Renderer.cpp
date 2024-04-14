#include "Renderer.h"
#include "Client/Client.h"
#include "Client/ClientImpl.h"
#include "Client/Session.h"
#include "Client/World.h"
#include "Client/Player.h"
#include "Client/Chunk.h"
#include "Glass/GUI.h"
#include "Glow/Window.h"
#include "Glow/GLContext.h"
#include "Glow/Texture.h"
#include "Glow/Inputs.h"
#include "Glow/Shader.h"
#include "GL/glew.h"
#include "Physics/Engine.h"
#include "Geometry.h"
#include "Loggy.h"

#define RADS (3.14159265359/180.0)

using namespace Glow;

static Loggy::Logger print {"Render"};

Shader* shader;

Renderer::Renderer() {}

void Renderer::_recompileShader() {
	shader->readShaderFile("res/shaders/world.sh");
	shader->compile();
}

void Renderer::init() {
	Client& client = Client::get();
	Glow::Window& window = *client.window;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glAlphaFunc(GL_GEQUAL, 0.5);

	{
		float lmambient[] = { 1, 1, 1, 1 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmambient);

		float position[] = { 0.57, 0.57, 0.57, 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		float ambient[] = { 1, 1, 1, 1 };
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

		float diffuse[] = { 1, 1, 1, 1 };
		glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	}

	shader = new Shader();
	shader->readShaderFile("res/shaders/world.sh");
	shader->compile();
}

void Renderer::resize(int width, int height) {
	glViewport(0, 0, width, height);
}

void Renderer::render() {
	Client& client = Client::get();
	Glow::Window& window = *client.window;
	Glow::GLContext& gl = *window.getContext();
	Session* sessionPtr = client.getSession();

	glClearColor(1.0f, 0.4f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (sessionPtr) {
		renderSession();
	}

	client.gui->paint();

	auto error = glGetError();
	if (error != 0) {
		print("GL Error: ", error);
	}
}

void dbgRenderCubeAt(GLContext& gl, Vector3f position, float scale = 1, bool wireframe = true) {
	glPushMatrix();
	gl.disableTexture2d();

	glTranslatef(position.x, position.y, position.z);
	glScalef(scale, scale, scale);

	glLineWidth(3);
	
	if (wireframe) {
		glVertexPointer(3, GL_FLOAT, 0, Geometry::Cube::verts_lines);
		glDrawArrays(GL_LINES, 0, 24);
	} else {
		glVertexPointer(3, GL_FLOAT, 0, Geometry::Cube::verts_tris);
		glDrawArrays(GL_QUADS, 0, 24);
	}
	glPopMatrix();

	gl.enableTexture2d();
}

void Renderer::renderSession() {
	Client& client = Client::get();
	Glow::Window& window = *client.window;
	Glow::GLContext& gl = *window.getContext();
	Session& session = *client.getSession();
	Player& player = session.getPlayer();
	std::vector<Chunk*>& chunks = session.getWorld().chunks;

	gl.enableDepthTesting();
	gl.enableBlending();
	gl.setBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	gl.enableFaceCulling();
	glMatrixMode(GL_PROJECTION);

	camera.aspect = client.getWindow()->getAspect();
	camera.makeTransformMatrices();
	glLoadIdentity();
	glLoadMatrixf(camera.projectionMat.mat);

	// Render sky
	{
		gl.getTexture("sky")->bind();
		glDepthMask(GL_FALSE);

		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(camera.rotatedViewMat);
		
		gl.enableVertsArray();
		gl.enableUVsArray();
		glVertexPointer(3, GL_FLOAT, 0, Geometry::Skybox::verts);
		glTexCoordPointer(2, GL_FLOAT, 0, Geometry::Skybox::uvs);
		glDrawArrays(GL_QUADS, 0, 24);

		glDepthMask(GL_TRUE);
	}

	glLoadMatrixf(camera.modelViewMat.mat);
	{
		//float pos[] = { 1, 1, 0.2, 0 };
		//glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}

	glColor4f(1, 1, 1, 1);
	gl.getTexture("grass")->bind();

	// Render all chunk batches
	for (auto& chunk : chunks) {
		glPushMatrix();
		glTranslatef(chunk->cx * 16, 0, chunk->cz * 16);
		auto& verts = chunk->vertices;
		auto& uvs = chunk->uvs;
		auto& normals = chunk->normals;
		auto& colors = chunk->colors;
		if (verts.size() > 0) {
			gl.enableNormalsArray();
			gl.enableVertsArray();
			gl.enableUVsArray();
			gl.enableColorsArray();
			gl.enableLighting();
			glUseProgram(shader->glId);
			glEnable(GL_LIGHT0);

			glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0]);
			glNormalPointer(GL_FLOAT, 0, &normals[0]);
			glColorPointer(3, GL_FLOAT, 0, &colors[0]);
			glDrawArrays(GL_TRIANGLES, 0, (GLsizei)verts.size());

			gl.disableNormalsArray();
			gl.disableUVsArray();
			gl.disableVertsArray();
			gl.disableColorsArray();
			gl.disableLighting();
			glUseProgram(0);
		}
		
		glPopMatrix();
	}
	
	// Show wireframe view of the block that would be placed/broken and look-at position
	{
		gl.enableVertsArray();
		auto camDirection = camera.getLookingDirection();

		if (camera.isLookingAtTerrain) {
			// What block would be broken
			auto selectedBlock_ = camera.getLookingBlockPos();
			if (selectedBlock_) {
				Vector3f selectedBlock = *selectedBlock_;
				selectedBlock += camDirection * -0.1;
				glColor3f(1.0, 0.5, 0.3);
				dbgRenderCubeAt(gl, selectedBlock, 1, true);
			}
			
			// Where the next block would be placed
			auto placeBlock_ = camera.getLookingEmptyBlockPos();
			if (placeBlock_) {
				Vector3f placeBlock = *placeBlock_;
				placeBlock += camDirection * -0.1;
				glColor3f(0.2, 1.0, 0.6);
				dbgRenderCubeAt(gl, placeBlock, 1, true);
			}

			glColor3f(0.3, 0.3, 1);
			dbgRenderCubeAt(gl, camera.lookingTerrainPos, 0.05, false);
		}

		gl.enableTexture2d();
		gl.enableDepthTesting();
		gl.disableVertsArray();
	}

	if (debugOutlines) {
		Physics::drawDebug();
		drawDebugOutlines();
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	gl.disableDepthTesting();
	gl.disableFaceCulling();
}

void Renderer::drawDebugOutlines() {
	Client& client = Client::get();
	Glow::Window& window = *client.window;
	Glow::GLContext& gl = *window.getContext();
	Session& session = *client.getSession();
	Player& player = session.getPlayer();

	{
		gl.enableVertsArray();
		auto camDirection = camera.getLookingDirection();

		if (camera.isLookingAtTerrain) {

			// Draw currently looked at triangle
			auto& lookTriangle = camera.lookingTerrainTriangle;

			glColor3f(0.5, 1, 0);

			Vector3f tv[]{
				lookTriangle[0] - camDirection * 0.01,
				lookTriangle[1] - camDirection * 0.01,
				lookTriangle[2] - camDirection * 0.01
			};

			glLineWidth(3);
			glVertexPointer(3, GL_FLOAT, 0, tv);
			glDrawArrays(GL_LINE_LOOP, 0, 3);

			glColor3f(0.3, 0.3, 1);
			dbgRenderCubeAt(gl, camera.lookingTerrainPos, 0.05, false);
		}

		gl.enableTexture2d();
		gl.enableDepthTesting();
		gl.disableVertsArray();
	}
}

void Renderer::drawLine(const Vector3f& from, const Vector3f& to, const Vector3f& color) {
	glLineWidth(2);
	glColor3fv(color.vec);
	glBegin(GL_LINES);
	glVertex3fv(from.vec);
	glVertex3fv(to.vec);
	glEnd();
}

Renderer& Renderer::get() {
	return *Client::get().getRenderer();
}