#pragma once

#include <glad/glad.h>  // remember to include glad or glad debug dir in proj
#include <GLFW/glfw3.h>

#include "gfx/Camera.h"
#include "gfx/ModelMatrix.h"
#include "gfx/Window.h"

#include "gfx/gl/ShaderProgram.h"
#include "gfx/gl/Texture.h"
#include "gfx/gl/VAO.h"

#include "gfx/Lighting.h"

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
		Light light;

		VAO objectCube;
		ModelMatrix objectTransform;
		MappedMaterial objectMaterial;

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
	static constexpr std::string_view UNIFORM_LIGHT_COLOR{ "lightColor" };	// keep for light source shader

	// from Lighting: Basic Lighting
	static constexpr std::string_view UNIFORM_VIEW_POSITION{ "viewPosition" };

	// from Lighting: Materials
	static constexpr std::string_view UNIFORM_MATERIAL{ "material" };
	static constexpr std::string_view UNIFORM_MATERIAL_AMBIENT{ "material.ambientColor" };
	static constexpr std::string_view UNIFORM_MATERIAL_DIFFUSE{ "material.diffuseColor" };
	static constexpr std::string_view UNIFORM_MATERIAL_SPECULAR{ "material.specularColor" };
	static constexpr std::string_view UNIFORM_MATERIAL_SHININESS{ "material.shininess" };

	static constexpr std::string_view UNIFORM_LIGHT{ "light" };
	static constexpr std::string_view UNIFORM_LIGHT_POSITION{ "light.position" };
	static constexpr std::string_view UNIFORM_LIGHT_IS_POINT{ "light.isPoint" };
	static constexpr std::string_view UNIFORM_LIGHT_AMBIENT{ "light.ambientColor" };
	static constexpr std::string_view UNIFORM_LIGHT_DIFFUSE{ "light.diffuseColor" };
	static constexpr std::string_view UNIFORM_LIGHT_SPECULAR{ "light.specularColor" };
	static constexpr std::string_view UNIFORM_LIGHT_K{ "light.k" };
	static constexpr std::string_view UNIFORM_LIGHT_INNER_CONE{ "light.innerCone" };
	static constexpr std::string_view UNIFORM_LIGHT_OUTTER_CONE{ "light.outterCone" };
	static constexpr std::string_view UNIFORM_LIGHT_DIRECTION{ "light.direction" };

	static constexpr std::string_view UNIFORM_MAPPED_MATERIAL_DIFFUSE{ "mappedMaterial.diffuse" };
	static constexpr std::string_view UNIFORM_MAPPED_MATERIAL_SPECULAR{ "mappedMaterial.specular" };
	static constexpr std::string_view UNIFORM_MAPPED_MATERIAL_SHININESS{ "mappedMaterial.shininess" };

	// texture unit stuff
	static constexpr int DIFFUSE_MAP_TEXTURE_UNIT{ 0 };
	static constexpr int SPECULAR_MAP_TEXTURE_UNIT{ 1 };


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