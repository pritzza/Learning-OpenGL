#include "Application.h"

#include <glad/glad.h>

#include "util/FileLoader.h"

#include <iostream>

Application::Application(const std::string& windowName, const sf::VideoMode& videoMode, const uint8_t fps, const uint8_t sfWindowStyle, const sf::ContextSettings& contextSettings)
	:
	contextSettings{ contextSettings },
	window{ videoMode, windowName, sfWindowStyle, contextSettings },
	fps{ fps }
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

	//const std::vector<float> vertices = {
	//	 0.5f,  0.5f, 0.0f,  // top right
	//	 0.5f, -0.5f, 0.0f,  // bottom right
	//	-0.5f, -0.5f, 0.0f,  // bottom left
	//	-0.5f,  0.5f, 0.0f   // top left 
	//};
	//
	//const std::vector<uint32_t> indices = {  // note that we start from 0!
	//	0, 1, 3,  // first Triangle
	//	1, 2, 3   // second Triangle
	//};

	FileLoader fl;
	const std::vector<float> vertices = fl.getVertices("Data/Objs/Teapot.obj");
	const std::vector<uint32_t> indices = fl.getIndices("Data/Objs/Teapot.obj");

	std::cout << vertices.size() << ", " << indices.size() << '\n';

	vaos.emplace_back();

	vaos[0].makeVAO(vertices, indices);

	// draw mesh in wire frame
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void Application::terminate()
{
	this->isRunning = false;
}

void Application::applicationLoop()
{
	double dt{};	// delta time; time between each frame
	double pt{};	// processing time; the time it takes to execute one gameloop excluding the wait/sleep

	while (isRunning)
	{
		frameTimer.restart();
		// dt boilerplate


		sf::Event event;

		while (this->window.pollEvent(event))
			pollWindowEvents(event);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		int fragColorOffsetUniformHandle{ glGetUniformLocation(this->shaders[0].getHandle(), "colorOffset")};
		int vertPosOffsetUniformHandle{ glGetUniformLocation(this->shaders[0].getHandle(), "posOffset") };

		fragColorOffsetUniform += 0; //dt/2.f;

		const float posOffset = 3.f;

		glUniform1f(fragColorOffsetUniformHandle, fragColorOffsetUniform);
		glUniform1f(vertPosOffsetUniformHandle, posOffset);

		vaos[0].draw();

		this->window.display();


		// dt boilerplate
		pt = frameTimer.getElapsedTime().asSeconds();	// processing time before wait
		sf::sleep(sf::seconds(1.f / this->fps - pt));

		dt = frameTimer.getElapsedTime().asSeconds();	// total time (use this for dt calculations)
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