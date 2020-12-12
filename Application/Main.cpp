#include "pch.h"
#include "sdl.h"
#include "Engine/Engine.h"
#include "Engine/Graphics/Renderer.h"
#include "Engine/Graphics/Program.h"
#include "Engine/Graphics/Texture.h"
#include "Engine/Graphics/VertexArray.h"
#include "Engine/Graphics/VertexIndexArray.h"
#include <glad\glad.h>

int main(int argc, char** argv) {
	sn::Engine engine;
	engine.Startup();

	static float vertices[] = {
		//front
		-1.0f, -1.0f,  1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f,  1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f,  1.0f, 1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 0.0f, 1.0f,
		//back
		-1.0f, -1.0f, -1.0f, 1.0f, 0.0f,
		 1.0f, -1.0f, -1.0f, 0.0f, 0.0f,
		 1.0f,  1.0f, -1.0f, 0.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f, 1.0f,
	};

	static GLushort indices[] = {
		//front
		0, 1, 2,
		2, 3, 0,
		//right
		1, 5, 6,
		6, 2, 1,
		//back
		7, 6, 5,
		5, 4, 7,
		//left
		4, 0, 3,
		3, 7, 4,
		//bottom
		4, 5, 1,
		1, 0, 4,
		//top
		3, 2, 6,
		6, 7, 3
	};

	sn::Program program;
	program.CreateShaderFromFile("shaders\\basic.vert", GL_VERTEX_SHADER);
	program.CreateShaderFromFile("shaders\\basic.frag", GL_FRAGMENT_SHADER);
	program.Link();
	program.Use();

	//create vertex buffers
	sn::VertexIndexArray vertexIndexArray;
	vertexIndexArray.Create("vertex");
	vertexIndexArray.CreateBuffer(sizeof(vertices), sizeof(vertices) / (sizeof(float) * 5), vertices);
	vertexIndexArray.SetAttribute(0, 3, 5 * sizeof(float), 0);
	vertexIndexArray.SetAttribute(1, 2, 5 * sizeof(float), 3 * sizeof(float));
	vertexIndexArray.CreateIndexBuffer(GL_UNSIGNED_SHORT, sizeof(indices) / sizeof(GLushort), indices);

	//set position pipeline (vertex attribute)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	glEnableVertexAttribArray(0);

	//set uv pipeline (vertex attribute)
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//create index buffers
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//uniform
	glm::mat4 model = glm::mat4(1.0f);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.01f, 1000.0f);

	glm::vec3 eye{ 0, 0, 5 };
	glm::mat4 view = glm::lookAt(eye, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	sn::Texture texture;
	texture.CreateTexture("textures\\llama.jpg");

	bool quit = false;
	while (!quit) {
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type) {
		case SDL_QUIT:
			quit = true;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_ESCAPE) {
				quit = true;
			}
		}
		SDL_PumpEvents();
		engine.Update();

		float angle = 0;
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_E) == sn::InputSystem::eButtonState::HELD) {
			angle = 2.0f;
		}
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_Q) == sn::InputSystem::eButtonState::HELD) {
			angle = -2.0f;
		}

		model = glm::rotate(model, angle * engine.GetTimer().DeltaTime(), glm::vec3(0, 1, 0));

		//camera movement
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_A) == sn::InputSystem::eButtonState::HELD) {
			eye.x -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_D) == sn::InputSystem::eButtonState::HELD) {
			eye.x += 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_S) == sn::InputSystem::eButtonState::HELD) {
			eye.y -= 4 * engine.GetTimer().DeltaTime();
		}
		if (engine.GetSystem<sn::InputSystem>()->GetButtonState(SDL_SCANCODE_W) == sn::InputSystem::eButtonState::HELD) {
			eye.y += 4 * engine.GetTimer().DeltaTime();
		}
		view = glm::lookAt(eye, eye + glm::vec3{ 0, 0, -1 }, glm::vec3{ 0, 1, 0 });

		glm::mat4 mvp = projection * view * model;
		program.SetUniform("transform", mvp);

		engine.GetSystem<sn::Renderer>()->BeginFrame();

		//glDrawArrays(GL_TRIANGLES, 0, 3);
		//GLsizei numElements = sizeof(indices) / sizeof(GLushort);
		//glDrawElements(GL_TRIANGLES, numElements, GL_UNSIGNED_SHORT, 0);
		vertexIndexArray.Draw();

		engine.GetSystem<sn::Renderer>()->EndFrame();
	}
	engine.Shutdown();

	return 0;
}