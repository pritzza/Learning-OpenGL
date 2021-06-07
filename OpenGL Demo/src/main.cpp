#include "Application.h"

#include <glad 3.3/glad.h>

int main()
{
	Application app{ "OpenGL", sf::VideoMode(800, 600, 32), sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3, sf::ContextSettings::Core) };

	app.applicationLoop();

	return 0;
}