#include "Renderer.h"
#include "Client/Client.h"
#include "Client/ClientImpl.h"
#include "Client/Session.h"
#include "Glass/GUI.h"
#include "Glow/Window.h"
#include "Glow/GLContext.h"
#include "Glow/Texture.h"
#include "Glow/Inputs.h"
#include "Glow/Shader.h"
#include "GL/glew.h"
#include "Loggy.h"
#include "Geometry.h"
#include "Client/Chunk.h"
#include "Physics\Collisions.h"

#define RADS (3.14159265359/180.0)

using namespace Glow;

static Loggy::Logger print {"Render"};

Shader* shader;

Renderer::Renderer() {}

void Renderer::init() {
	Client& client = Client::get();
	Glow::Window& window = *client.window;

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glAlphaFunc(GL_GEQUAL, 0.5);


	{
		float lmambient[] = { 0.2, 0.2, 0.2, 1 };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmambient);

		float position[] = { 1, 1, 0.2, 0 };
		glLightfv(GL_LIGHT0, GL_POSITION, position);

		float ambient[] = { 0.4, 0.4, 0.4, 1 };
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

	gl.enableDepthTesting();
	//gl.enableBlending();
	//gl.setBlending(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//gl.enableFaceCulling();
	glMatrixMode(GL_PROJECTION);
	camera.aspect = client.getWindow()->getAspect();

	camera.makeProjection();
	camera.makeRotatedView();
	camera.makeModelView();
	glLoadIdentity();
	glLoadMatrixf(camera.projection.mat);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(camera.modelView.mat);

	{
		//float pos[] = { 1, 1, 0.2, 0 };
		//glLightfv(GL_LIGHT0, GL_POSITION, pos);
	}

	glColor4f(1, 1, 1, 1);
	gl.getTexture("grass")->bind();

	for (auto& chunk : session.chunks) {
		glPushMatrix();
		glTranslatef(chunk->cx * 16, 0, chunk->cz * 16);
		auto& verts = chunk->vertices;
		auto& uvs = chunk->uvs;
		auto& normals = chunk->normals;

		if (verts.size() > 0) {
			gl.enableNormalsArray();
			gl.enableVertsArray();
			gl.enableUVsArray();
			//gl.enableLighting();
			glUseProgram(shader->glId);
			glEnable(GL_LIGHT0);

			glVertexPointer(3, GL_FLOAT, 0, &verts[0]);
			glTexCoordPointer(2, GL_FLOAT, 0, &uvs[0]);
			glNormalPointer(GL_FLOAT, 0, &normals[0]);
			glDrawArrays(GL_TRIANGLES, 0, verts.size());

			gl.disableNormalsArray();
			gl.disableUVsArray();
			gl.disableVertsArray();
			//gl.disableLighting();
			glUseProgram(0);
		}
		
		{
			gl.enableVertsArray();

			auto direction = camera.getLookDirection();
			Vector3f hit_;
			Vector3f* hitTriangle_;

			bool didHit = session.cameraRaycast(&hit_, &hitTriangle_);
			if (didHit) {
				glColor3f(0.5, 1, 0);

				Vector3f v0 = hitTriangle_[0];
				Vector3f v1 = hitTriangle_[1];
				Vector3f v2 = hitTriangle_[2];

				v0 -= direction * 0.01;
				v1 -= direction * 0.01;
				v2 -= direction * 0.01;
				Vector3f tv[]{ v0, v1, v2 };

				glLineWidth(3);
				glVertexPointer(3, GL_FLOAT, 0, tv);
				glDrawArrays(GL_LINE_LOOP, 0, 3);

				Vector3f selectedBlock = *session.getLookingBlock();
				selectedBlock += direction * -0.1;
				glColor3f(1.0, 0.5, 0.3);
				dbgRenderCubeAt(gl, selectedBlock, 1, true);

				Vector3f placeBlock = *session.getLookingBlockForPlace();
				placeBlock += direction * -0.1;
				glColor3f(0.2, 1.0, 0.6);
				dbgRenderCubeAt(gl, placeBlock, 1, true);

				glColor3f(0.3, 0.3, 1);
				dbgRenderCubeAt(gl, hit_, 0.05, false);
			}

			gl.enableDepthTesting();
			gl.disableVertsArray();
		}

		glPopMatrix();
	}

	/*gl.getTexture("_t0")->bind();
	if (window.getKey(Glow::InputKey::F1)) {
		for (int x = 0; x < 16; x++) {
			for (int y = 0; y < 16; y++) {
				for (int z = 0; z < 16; z++) {
					int b = (*session.blocks)[x][y][z];
					if (b != 0) {
						glPushMatrix();
						glTranslatef(x, y, z);

						glVertexPointer(3, GL_FLOAT, 0, Geometry::Cube::verts_tris);
						glTexCoordPointer(2, GL_FLOAT, 0, Geometry::Cube::uvs_tris);
						glDrawArrays(GL_QUADS, 0, 24);

						glPopMatrix();
					}
				}
			}
		}
	}*/

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	gl.disableDepthTesting();
	gl.disableFaceCulling();
}