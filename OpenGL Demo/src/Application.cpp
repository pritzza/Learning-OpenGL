#include "Application.h"

#include <glad 3.3/glad.h>

#include "glShaders/Shader.h"
#include "Util/GLDebugCallBack.h"

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
	
	this->demoShader = std::make_unique<ShaderProgram>("Baby's First Shader Program");
	
	demoShader->add(fragmentShader);
	demoShader->add(vertexShader);
	checkError("Application::addShaders");

	demoShader->link();
	checkError("Application::linkShader");

	renderer.setShaderProgram(*demoShader);
	checkError("Application::setShader");

	//Vertex vertices[3] =
	//{
	//	{  0.0f,  0.5f, 0.0f },
	//	{ -0.5f, -0.5f, 0.0f },
	//	{  0.5f, -0.5f, 0.0f },
	//};

	std::vector<float> vertices =
	{
	  0.0f,  0.5f, 0.0f,
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	};

	std::vector<unsigned int> indices =
	{
		0, 1, 2
	};
	checkError("Application::init");
	this->demoModel = std::make_unique<Model>(this->loader);

	demoModel->setVertices(this->loader, vertices);
	demoModel->setIndices(this->loader, indices);
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

		renderer.clear();

		renderer.draw(*demoModel);

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