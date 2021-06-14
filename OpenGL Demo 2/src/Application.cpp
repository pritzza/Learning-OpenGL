#include "Application.h"

#include <glad/glad.h>

#include "util/FileLoader.h"

#include "gl/Vertex.h"

#include <iostream>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const uint8_t fps, const uint8_t sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	contextSettings{ contextSettings },
	window{ videoMode, windowName, sfWindowStyle, contextSettings },
	frameRate{ fps }
{
	this->init();
}

void Application::init()
{
	// set up sfml window
	this->window.setVerticalSyncEnabled(true);
	this->window.setActive(true);

	if (!gladLoadGL())
	{
		std::cout << "couldnt load glad";
		return;
	}

	std::cout << glGetString(GL_VERSION) << '\n';

	int nrAttributes;
	glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
	std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

	shaders.emplace_back();
	shaders[0].makeProgram("Data/Shaders/Vertex.glsl", "Data/Shaders/Fragment.glsl");
	shaders[0].use();

	shaders[0].registerUniform(UniformName::MainFragmentColorOffset);
	shaders[0].registerUniform(UniformName::MainVertexPosOffset);

	//const std::vector<float> vertices = {
	//	 0.5f,  0.5f, 0.0f,  // top right
	//	 0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};
	
	const std::vector<ColoredVertex> vertices = {
	//	x, y, z, r, g, b, a
		{  0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f },  // top right
		{  0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, 1.0f },  // bottom right
		{ -0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, 1.0f },  // bottom left
		{ -0.5f,  0.5f, 0.0f,  0.0f, 0.5f, 0.5f, 1.0f }   // top left 
	};

	const std::vector<uint32_t> indices = {  // note that we start from 0!
		0, 1, 3,  // first triangle
		1, 2, 3   // second triangle
	};

	//FileLoader fl;
	//const std::vector<float> vertices = fl.getVertices("Data/Objs/Teapot.obj");
	//const std::vector<uint32_t> indices = fl.getIndices("Data/Objs/Teapot.obj");

	std::cout << vertices.size() << " vertices and " << indices.size() << " indices\n";


	vaos.emplace_back();

	vaos[0].makeVAO<ColoredVertex>(vertices, indices);

	// draw mesh in wire frame
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::terminate()
{
	this->isRunning = false;
}

void Application::handleInput()
{
}

void Application::update()
{
	sf::Event event;
 
	while (this->window.pollEvent(event))
		pollWindowEvents(event);

	// non boilerplate:

	uniformColorOffset = (sin(delta.getTotal() / 2.f) + 0.5f);
	uniformPosOffset = (cos(delta.getTotal()) * 2.f) + 3.f;

	shaders[0].setUniform(UniformName::MainFragmentColorOffset, uniformColorOffset);
	shaders[0].setUniform(UniformName::MainVertexPosOffset, uniformPosOffset);
}

void Application::render()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	vaos[0].draw();

	this->window.display();
}

void Application::applicationLoop()
{
	while (isRunning)
	{
		this->handleInput();
		this->update();
		this->render();
	
		this->delta.wait(this->frameRate);
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