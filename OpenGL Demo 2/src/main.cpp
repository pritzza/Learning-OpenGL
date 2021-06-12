#include "Application.h"

int main()
{
	Application app{ "OpenGL", sf::VideoMode(800, 600, 32), 60, sf::Style::Default, sf::ContextSettings(24, 8, 4, 3, 3, sf::ContextSettings::Core) };

	app.applicationLoop();

	return 0;
}