#include "Application.h"

#include <glad/glad.h>

#include "Entity.h"

#include <iostream>

#include <math.h>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	contextSettings{ contextSettings },
	window{ videoMode, windowName, sfWindowStyle, contextSettings }
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
		 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f, 0.0f, 1.0f   // top left 
	};

	const std::vector<GLuint> indices = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	model.init(vertices, indices, "guy.png");
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
		dt.start();
		
		const float dtSin = sin(dt.getTotalTime());
		const float dtCos = cos(dt.getTotalTime());

		sf::Event event;

		while (this->window.pollEvent(event))
			pollWindowEvents(event);

		// update

		entity.move(0, 0, 0);
		entity.rotate(1, 1, 1);
		entity.scale(1);

		// render
		renderer.clear();

		renderer.render(shader, entity);

		this->window.display();

		dt.wait(60);	// 60 fps
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