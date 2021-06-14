#pragma once

#include <SFML/Window.hpp>

#include "gl/VertexArrayObject.h"
#include "gl/ShaderProgram.h"

#include "util/DeltaTime.h"

class Application final
{
private:
	sf::ContextSettings contextSettings;
	sf::Window window;

	bool isRunning{ true };

	DeltaTime delta;
	uint8_t frameRate;

	std::vector<VertexArrayObject> vaos;
	std::vector<ShaderProgram> shaders;

	float uniformPosOffset{};
	float uniformColorOffset{};

private:
	void init();
	void terminate();

	void handleInput();
	void update();
	void render();

	void pollWindowEvents(const sf::Event& event);

public:
	Application(const std::string& windowName, const sf::VideoMode& videoMode, const uint8_t fps, const uint8_t sfWindowStyle, const sf::ContextSettings& contextSettings);

	void applicationLoop();

};