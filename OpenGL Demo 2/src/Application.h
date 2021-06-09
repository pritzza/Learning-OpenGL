#pragma once

#include <SFML/Window.hpp>

#include "gl/VertexArrayObject.h"

class Application
{
private:
	sf::ContextSettings m_contextSettings;
	sf::Window m_window;

	bool isRunning{ true };

	std::vector<VertexArrayObject> vaos;

public:
	Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings);

	void init();
	void terminate();

	void applicationLoop();

	void pollWindowEvents(const sf::Event& event);
};