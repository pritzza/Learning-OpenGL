#pragma once

#include "Window.h"

class Application
{
public:
	static void instantiateLibraries();

public:
	void start();
	void stop();

public:
	Window window;

	bool isRunning{ false };

private:
	void update();
	void handleInput();
	void display();

	// static methods for library initilization
private:
	static void initGLFW();
	static void initSTB_Image();
	static void initGLAD();
	static void initOpenGL();
};