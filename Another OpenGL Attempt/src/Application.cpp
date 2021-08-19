#include "Application.h"

#include <glad/glad.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

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
	glm::mat4 translationMatrix
	{
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
	};
	
	glm::mat4 projection{ glm::perspective(glm::radians(45.f), (float)window.getSize().x / window.getSize().y, 0.1f, 100.0f) };

	glm::mat4 view{ glm::lookAt(
		glm::vec3(4,3,3),
		glm::vec3(0,0,0),
		glm::vec3(0,1,0)
		) };

	glm::mat4 modelMatrix{ 1.0f };

	glm::mat4 mvp{ projection * view * modelMatrix };

	//glm::mat4 myMatrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
	//glm::vec4 myVector(10.0f, 10.0f, 10.0f, 0.0f);
	//glm::vec4 transformedVector = myMatrix * myVector; // guess the result

	this->shader.setUniform("transformationMatrix", mvp);

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