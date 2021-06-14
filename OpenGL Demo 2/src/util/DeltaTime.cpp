#include "DeltaTime.h"

#include <SFML/System/Sleep.hpp>

void DeltaTime::wait(const uint8_t frameRate)
{
	this->clock.restart();

	this->processingTime = this->clock.getElapsedTime().asSeconds();	// processing time before wait
	sf::sleep(sf::seconds(1.f / frameRate - this->processingTime));

	this->deltaTime = this->clock.getElapsedTime().asSeconds();	// total time (use this for dt calculations)

	this->totalTime += deltaTime;
}