#include "Application.h"

#include <glad/glad.h>

#include <iostream>
#include <string_view>

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
	while (this->isRunning)
	{
		dt.start();

		sf::Event event;

		while (this->window.pollEvent(event))
			pollWindowEvents(event);

		// update

		this->shader.setUniform("posOffset", sin(dt.getTotalTime()), cos(dt.getTotalTime()), 0.f);

		// render
		renderer.clear();

		renderer.render(model);

		this->window.display();

		dt.wait(60);
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