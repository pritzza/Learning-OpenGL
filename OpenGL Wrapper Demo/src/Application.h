#pragma once

#include <SFML/Window.hpp>

class Application
{
private:
	sf::ContextSettings m_contextSettings;
	sf::Window m_window;

	bool m_isRunning{ true };

public:
	Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings);

	Application(const Application& app) = delete;
	Application& operator= (const Application& app) = delete;

	void init();
	void terminate();

	void applicationLoop();

	void pollWindowEvents(const sf::Event& event);
	
};