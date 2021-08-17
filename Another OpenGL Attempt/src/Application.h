#pragma once

#include <SFML/Window.hpp>

#include "gl/Renderer.h"
#include "gl/ShaderProgram.h"
#include "gl/Model.h"

#include "util/DeltaTime.h"

class Application
{
private:
	sf::ContextSettings contextSettings;
	sf::Window window;

	bool isRunning{ true };

	DeltaTime dt;

	Renderer renderer;

	ShaderProgram shader;
	Model model;

public:
	Application(const std::string& windowName, const sf::VideoMode& videoMode, const unsigned int sfWindowStyle, const sf::ContextSettings& contextSettings);

	void init();
	void terminate();

	void applicationLoop();

	void pollWindowEvents(const sf::Event& event);
};