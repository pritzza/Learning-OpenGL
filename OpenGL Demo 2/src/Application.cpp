#include "Application.h"

#include <glad/glad.h>

#include "gl/ShaderProgram.h"

#include <iostream>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	m_contextSettings{ contextSettings },
	m_window{ videoMode, windowName, sfWindowStyle, contextSettings }
{
	this->init();
}

void Application::init()
{
	// set up sfml window
	this->m_window.setVerticalSyncEnabled(true);
	this->m_window.setActive(true);

	gladLoadGL();

	std::cout << glGetString(GL_VERSION) << '\n';

	ShaderProgram shader;
	shader.makeProgram("Data/Shaders/Vertex.glsl", "Data/Shaders/Fragment.glsl");
	shader.use();

	const std::vector<float> vertices = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	const std::vector<uint32_t> indices = {  // note that we start from 0!
		0, 1, 3,  // first Triangle
		1, 2, 3   // second Triangle
	};

	vaos.emplace_back();

	vaos[0].makeVAO(vertices, indices);
}

void Application::terminate()
{
	this->isRunning = false;
}

void Application::applicationLoop()
{
	while (this->isRunning)
	{
		sf::Event event;

		while (this->m_window.pollEvent(event))
			pollWindowEvents(event);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		vaos[0].draw();

		this->m_window.display();
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