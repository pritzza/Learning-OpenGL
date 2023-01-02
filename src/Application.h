#pragma once

#include <glad/glad.h>  // remember to include glad or glad debug dir in proj
#include <GLFW/glfw3.h>

#include "Camera.h"
#include "ModelMatrix.h"
#include "Window.h"

#include "gl/ShaderProgram.h"
#include "gl/Texture.h"
#include "gl/VAO.h"

#include <array>

class Application
{
public:
	static void instantiateLibraries();
	static void uninstantiateLibraries();

public:
	Application(
		const std::string_view& name,
		int windowWidth,
		int windowHeight
	);

	void start();
	void stop();

public:
	bool isRunning{ false };
	
	double currentTime{ 0.0 };
	double deltaTime{ 0.0 };

	Window window;
	Camera camera;

	struct OpenGLStuff
	{
		VAO lightSourceCube;
		ModelMatrix lightSourceTransform;
		glm::vec3 lightSourceColor;

		VAO objectCube;
		ModelMatrix objectTransform;
		glm::vec3 objectColor;

		ShaderProgram lightSourceProgram;
		ShaderProgram objectProgram;
	};

	std::unique_ptr<OpenGLStuff> gl;

private:
	static constexpr int OPENGL_MAJOR_VERSION{ 3 };
	static constexpr int OPENGL_MINOR_VERSION{ 3 };
	static constexpr int OPENGL_PROFILE{ GLFW_OPENGL_CORE_PROFILE };

	// todo: find better place for this
	/// uniform names lol
	// from goofing around
	static constexpr std::string_view UNIFORM_TIME{ "time" };

	// textures
	static constexpr std::string_view UNIFORM_TEXTURE0{ "texture0" };
	static constexpr std::string_view UNIFORM_TEXTURE1{ "texture1" };

	// camera transform
	static constexpr std::string_view UNIFORM_MODEL_MAT{ "model" };
	static constexpr std::string_view UNIFORM_VIEW_MAT{ "view" };
	static constexpr std::string_view UNIFORM_PERSPECTIVE_MAT{ "perspective" };

	// from Lighting: Colors
	static constexpr std::string_view UNIFORM_LIGHT_COLOR{ "lightColor" };
	static constexpr std::string_view UNIFORM_OBJECT_COLOR{ "objectColor" };

private:
	void initializeObjects();

	void applicationLoop();

	void handleInput();
	void update();
	void display();

	// static methods for library initilization and configureation
private:
	static inline bool isInstantiated{ false };

	static void initGLFW();
	static void initGLAD();
	static void initOpenGL();
	static void initSTB_Image();
};