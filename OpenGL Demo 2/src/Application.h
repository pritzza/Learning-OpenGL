#pragma once

#include <SFML/Window.hpp>

#include "gl/VertexArrayObject.h"
#include "gl/ShaderProgram.h"

class Application
{
private:
	sf::ContextSettings contextSettings;
	sf::Window window;

	bool isRunning{ true };

	sf::Clock frameTimer;
	uint8_t fps;

	std::vector<VertexArrayObject> vaos;
	std::vector<ShaderProgram> shaders;

	float fragColorOffsetUniform{};

public:
	Application(const std::string& windowName, const sf::VideoMode& videoMode, const uint8_t fps, const uint8_t sfWindowStyle, const sf::ContextSettings& contextSettings);

	void init();
	void terminate();

	void applicationLoop();

	void pollWindowEvents(const sf::Event& event);
};