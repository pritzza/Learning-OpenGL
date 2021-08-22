#include "Application.h"

#include <glad/glad.h>

#include "Entity.h"

#include <iostream>

#include <math.h>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	contextSettings{ contextSettings },
	window{ videoMode, windowName, sfWindowStyle, contextSettings },
	renderer{ window.getSize().x, window.getSize().y }
{
	this->init();
}

void Application::init()
{
	window.setVerticalSyncEnabled(true);
	window.setActive(true);

	if (!gladLoadGL())
		this->terminate();

	glEnable(GL_DEPTH_TEST);

	this->shader.init("Vertex.glsl", "Fragment.glsl");
	this->shader.use();

	this->shader.setUniform("text", 0);

	const std::vector<GLfloat> vertices = {
		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f,  0.5f, -0.5f,

		-0.5f,  0.5f,  0.5f,
		-0.5f, -0.5f,  0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		 0.5f,  0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f,
		 0.5f,  0.5f,  0.5f,

		-0.5f,  0.5f, -0.5f,
		-0.5f, -0.5f, -0.5f,
		-0.5f, -0.5f,  0.5f,
		-0.5f,  0.5f,  0.5f,

		-0.5f,	0.5f,  0.5f,
		-0.5f,	0.5f, -0.5f,
		 0.5f,	0.5f, -0.5f,
		 0.5f,	0.5f,  0.5f,

		-0.5f, -0.5f,  0.5f,
		-0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f, -0.5f,
		 0.5f, -0.5f,  0.5f
	};

	const std::vector<GLfloat> colors = {
		 0.5f, 0.5f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.0f, 1.0f,
		 0.5f, 0.5f, 0.0f, 1.0f
	};

	const std::vector<GLfloat> uvs = {
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f,
		0.f, 0.f,
		0.f, 1.f,
		1.f, 1.f,
		1.f, 0.f
	};

	const std::vector<GLuint> indices = {  // note that we start from 0!
		0,	1,	3,
		3,	1,	2,
		4,	5,	7,
		7,	5,	6,
		8,	9,	11,
		11,	9,	10,
		12,	13,	15,
		15,	13,	14,
		16,	17,	19,
		19,	17,	18,
		20,	21, 23,
		23,	21, 22
	};

	model.init(vertices, colors, uvs, indices, "guy.png");
}

void Application::terminate()
{
	this->isRunning = false;
}

void Application::applicationLoop()
{
	Entity entity{ this->model };

	while (this->isRunning)
	{
		delta.start();

		const float dtSin = sin(delta.getTotalTime());
		const float dtCos = cos(delta.getTotalTime());
		const float dt = delta.getDT();

		sf::Event event;

		while (this->window.pollEvent(event))
			pollWindowEvents(event);

		// input
		Camera& camera{ renderer.getCamera() };

		static const sf::Vector2i DISPLAY_RESOLUTION{ 1920, 1080 };
		const sf::Vector2i mousePos{ sf::Mouse::getPosition() - (DISPLAY_RESOLUTION / 2) };

		static constexpr float SENSITIVITY{ 5.f };

		camera.setRotation(mousePos.y / SENSITIVITY, mousePos.x / SENSITIVITY, 0);

		// update

		entity.setPos(0, 0, -2);

		//entity.move(0, 0, -.1f);
		entity.rotate(1, 1, 1);
		//entity.scale(1);

		// render

		renderer.clear();

		renderer.render(shader, entity);

		this->window.display();

		delta.wait(Application::FPS);	// 60 fps
	}
}

void Application::pollWindowEvents(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::Closed:		this->terminate();										break;
	case sf::Event::Resized:	glViewport(0, 0, event.size.width, event.size.height);	break;
	}
}