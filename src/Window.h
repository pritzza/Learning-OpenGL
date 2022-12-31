#pragma once

#include <GLFW/glfw3.h>

#include <string>

class Window
{
public:
	Window(
		const std::string_view& name,
		int startingWidth,
		int startinHeight
	);

public:
	void swapBuffers();

	void pollEvents();

	bool shouldClose() const;

	// getters
	GLFWwindow* const getWindowPtr() const { return window; }
	int getWidth() const	{ return width;  }
	int getHeight() const	{ return height; }


private:
	GLFWwindow* const window;
	int width;
	int height;

private:
	void setSize(unsigned w, unsigned h);

	void setCallbacks();

	// call backs
	static void frameBufferResizeCallback(
		GLFWwindow* window, 
		int width, 
		int height
	);
};