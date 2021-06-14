#pragma once

#include <SFML/System/Clock.hpp>

#include <cstdint>

class DeltaTime final
{
private:
	sf::Clock clock;

	double deltaTime{};			// time between each frame
	double processingTime{};	// the time it takes to execute one gameloop excluding the wait/sleep

	double totalTime{};

public:
	void wait(const uint8_t frameRate);

	inline const double getDT() const		{ return this->deltaTime;		}
	inline const double getPT() const		{ return this->processingTime;  }
	inline const double getTotal() const	{ return this->totalTime;		}
};