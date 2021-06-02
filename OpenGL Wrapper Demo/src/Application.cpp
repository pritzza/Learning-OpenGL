#include "Application.h"

#include <glad 3.3/glad.h>

#include "glObjects/Vertex.h"
#include "glObjects/VertexBufferObject.h"
#include "glObjects/VertexArrayObject.h"
#include "glShaders/Shader.h"
#include "glShaders/ShaderProgram.h"
#include "glShaders/ShaderStatusChecker.h"

#include <iostream>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	m_contextSettings{contextSettings},
	m_window{videoMode, windowName, sfWindowStyle, contextSettings}
{
	this->m_window.setVerticalSyncEnabled(true);

	// activate the window
	this->m_window.setActive(true);

	this->init();
}

void Application::init()
{
	gladLoadGL();

	std::cout << glGetString(GL_VERSION) << '\n';

	Shader vertexShader("Vertex", GL_VERTEX_SHADER,
		R"DELIM(

		#version 330 core
		layout (location = 0) in vec3 aPos;

		void main()
		{
		   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
		}

		)DELIM"
	);

	Shader fragmentShader("Fragment", GL_FRAGMENT_SHADER,
		R"DELIM(

		#version 330 core
		out vec4 FragColor;

		void main()
		{
		    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); 
		}

		)DELIM"
	);

	ShaderProgram shader("Baby's First Shader Program");

	shader.add(fragmentShader);
	shader.add(vertexShader);
	shader.link();
	shader.use();

	/*
	Vertex vertices[3] =
	{
		{  0.0f,  0.5f, 0.0f },
		{ -0.5f, -0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f },
	};
	*/

	float vertices[] = {
	   -0.5f, -0.5f, 0.0f, // left  
		0.5f, -0.5f, 0.0f, // right 
		0.0f,  0.5f, 0.0f  // top   
	};

	VertexArrayObject vao{ 1 };
	vao.bind();

	VertexBufferObject vbo{ 1 };
	vbo.bind(GL_ARRAY_BUFFER);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(0);
}

void Application::terminate()
{
	this->m_isRunning = false;
}

void Application::applicationLoop()
{
	while (this->m_isRunning)
	{
		sf::Event event;

		while (this->m_window.pollEvent(event))
			pollWindowEvents(event);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);

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