#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "util/Math.h"

/// todo find a better place for these

// arguments for glVertexAttribPointer
struct VertexAttributeInfo
{
    GLuint location;
    GLuint numElements;
    GLuint elementDataType;
    GLboolean shouldNormalize;
};

struct Position
{
    static constexpr VertexAttributeInfo META
    {
        0, 3, GL_FLOAT, GL_FALSE
    };

    GLfloat x, y, z;
};

struct Color
{
    static constexpr VertexAttributeInfo META
    {
        1, 4, GL_FLOAT, GL_FALSE
    };

    GLfloat r, g, b, a;
};

struct TextureCoordinate2D
{
    static constexpr VertexAttributeInfo META
    {
        2, 2, GL_FLOAT, GL_FALSE
    };

    GLfloat s, t;
};


// compile time sanity check to make sure no attirbutes have the same location
static_assert(
    Position::META.location != Color::META.location &&
    Color::META.location != TextureCoordinate2D::META.location
    );

struct Vertex
{
    Position pos;
    Color color;
    TextureCoordinate2D texCoord;
};

// todo: find a way to make this more flexible/dynamic
constexpr size_t VERTEX_POS_OFFSET{ offsetof(Vertex, Vertex::pos) };
constexpr size_t VERTEX_COLOR_OFFSET{ offsetof(Vertex, Vertex::color) };
constexpr size_t VERTEX_TEXCOORD_OFFSET{ offsetof(Vertex, Vertex::texCoord) };

// cube
static const std::vector<Position> cubePosVertices = {

    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },

    { -0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },

    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f, -0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },

    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },

    { -0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f, -0.5f },
    {  0.5f, -0.5f,  0.5f },
    {  0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f,  0.5f },
    { -0.5f, -0.5f, -0.5f },

    { -0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f, -0.5f },
    {  0.5f,  0.5f,  0.5f },
    {  0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f,  0.5f },
    { -0.5f,  0.5f, -0.5f },
};

static const std::vector<TextureCoordinate2D> cubeTexCoords = {

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 0.0f, 0.0f },
    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 1.0f, 0.0f },
    { 1.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 1.0f },
    { 0.0f, 0.0f },
    { 1.0f, 0.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f },

    { 0.0f, 1.0f },
    { 1.0f, 1.0f },
    { 1.0f, 0.0f },
    { 1.0f, 0.0f },
    { 0.0f, 0.0f },
    { 0.0f, 1.0f }
};

Application::Application(
	const std::string_view& name, 
	int windowWidth, 
	int windowHeight
)
	:
	window{ 
		name, 
		windowWidth, 
		windowHeight, 
		Application::OPENGL_MAJOR_VERSION, 
		Application::OPENGL_MINOR_VERSION,
		Application::OPENGL_PROFILE
	}
{
	if (!Application::isInstantiated)
	{
		std::cerr << "Error! Tried to create Application before "
			<< "instantiating libraries... How are you even seeing this?\n";

		assert(false);
	}


	// todo: turns our glad can only be loaded after creating context
	initGLAD();
	initOpenGL();
}

void Application::start()
{
	isRunning = true;

	initializeObjects();
	applicationLoop();
}

void Application::stop()
{
	isRunning = false;
	// terminate()
}

void Application::initializeObjects()
{
    // allocate gl objects now that everything is loaded
	gl = std::make_unique<OpenGLStuff>();

    gl->lightSourceCube.defineAttribute(cubePosVertices);

    gl->objectCube.defineAttribute(cubePosVertices);

    // shader stuff
    constexpr std::string_view ALL_OBJECTS_VERTEX_SHADER_PATH{
        "res/shaders/AllObjectsVertex.glsl"
    };
    constexpr std::string_view LIGHT_SOURCE_FRAGMENT_SHADER_PATH{
        "res/shaders/LightSourceFragment.glsl"
    };
    constexpr std::string_view OBJECT_FRAGMENT_SHADER_PATH{
        "res/shaders/ObjectFragment.glsl"
    };

    const std::vector<std::string_view> LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES = {
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };

    const std::vector<std::string_view> OBJECT_PROGRAM_UNIFORM_NAMES = {
        UNIFORM_OBJECT_COLOR,
        UNIFORM_LIGHT_COLOR,
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };

    gl->lightSourceProgram.init(
        ALL_OBJECTS_VERTEX_SHADER_PATH,
        LIGHT_SOURCE_FRAGMENT_SHADER_PATH,
        LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES
    );

    gl->objectProgram.init(
        ALL_OBJECTS_VERTEX_SHADER_PATH,
        OBJECT_FRAGMENT_SHADER_PATH,
        OBJECT_PROGRAM_UNIFORM_NAMES
    );

    // create perspective projection matrix
    const float FOV{ glm::radians(45.f) };
    const float ASPECT_RATIO{ (float)window.getWidth() / window.getHeight() };
    const float NEAR_PLANE{ 0.1f };
    const float FAR_PLANE{ 100.0f };

    camera.setProjectionMatrix(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

    camera.setPosition({ -5.0f, 0.f, 0.f });

    gl->objectTransform.setPosition({ 2.f, 0.f, 0.f });
    gl->lightSourceTransform.setPosition({ 1.f, 3.f, 3.f });

    gl->lightSourceColor = glm::vec3{ 0.1f, 1.0f, 0.2f };
    gl->objectColor = glm::vec3{ 1.f, 0.8f, 0.7f };
}

void Application::applicationLoop()
{
	while (isRunning)
	{
        deltaTime = glfwGetTime() - currentTime;
        currentTime = glfwGetTime();

		handleInput();
		update();
		display();
	}
}

void Application::handleInput()
{
    // handle input
    window.pollEvents();

    GLFWwindow* const glfwWindow{ window.getWindowPtr() };

    if (glfwGetKey(glfwWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(glfwWindow, true);

    if (glfwGetKey(glfwWindow, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Backward, deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Forward, deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Left, deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveRelative(Camera::Direction::Right, deltaTime);

    if (glfwGetKey(glfwWindow, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.moveAbsolute(Math::J_HAT, deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        camera.moveAbsolute(-Math::J_HAT, deltaTime);

    if (glfwGetKey(glfwWindow, GLFW_KEY_UP) == GLFW_PRESS)
        camera.setPitch(camera.getPitch() + deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_DOWN) == GLFW_PRESS)
        camera.setPitch(camera.getPitch() - deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_LEFT) == GLFW_PRESS)
        camera.setYaw(camera.getYaw() - deltaTime);
    if (glfwGetKey(glfwWindow, GLFW_KEY_RIGHT) == GLFW_PRESS)
        camera.setYaw(camera.getYaw() + deltaTime);
}

void Application::update()
{
    if (window.shouldClose())
        stop();


    camera.update();
   
    gl->objectTransform.update();
    gl->lightSourceTransform.update();


    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// rendering light source

    // activate shader
    gl->lightSourceProgram.use();

    // update all uniforms (just CAMERA TRANSFORM DUMMY [its 3 matrices!!!])
    gl->lightSourceProgram.setUniformMat4(
        UNIFORM_MODEL_MAT,
        gl->lightSourceTransform.getMatrix()
    );

    gl->lightSourceProgram.setUniformMat4(
        UNIFORM_VIEW_MAT,
        camera.getViewMatrix()
    );

    gl->lightSourceProgram.setUniformMat4(
        UNIFORM_PERSPECTIVE_MAT,
        camera.getProjectionMatrix()
    );

    // bind light source VAO
    gl->lightSourceCube.bind();

    // draw light source
    glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());


    /// rendering object

    // activate object shader
    gl->objectProgram.use();

    /// update all uniforms (CAMERA TRANSFORM, obj color, and light source color)
    // camera transform
    gl->objectProgram.setUniformMat4(
        UNIFORM_MODEL_MAT,
        gl->objectTransform.getMatrix()
    );
    gl->objectProgram.setUniformMat4(
        UNIFORM_VIEW_MAT,
        camera.getViewMatrix()
    );

    gl->objectProgram.setUniformMat4(
        UNIFORM_PERSPECTIVE_MAT,
        camera.getProjectionMatrix()
    );

    // lighting stuff
    gl->objectProgram.setUniformVec3f(
        UNIFORM_LIGHT_COLOR,
        gl->lightSourceColor
    );

    gl->objectProgram.setUniformVec3f(
        UNIFORM_OBJECT_COLOR,
        gl->objectColor
    );

    // bind object VAO
    gl->objectCube.bind();

    // draw object
    glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());
}

void Application::display()
{
    window.swapBuffers();
}


///// static methods
void Application::instantiateLibraries()
{
	initGLFW();

	initSTB_Image();

	Application::isInstantiated = true;
}

void Application::uninstantiateLibraries()
{
	// pretty sure this frees GL context
	glfwTerminate();

	Application::isInstantiated = false;
}

void Application::initGLFW()
{
	glfwInit();
}

void Application::initGLAD()
{
	// load opengl functions with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		std::cerr << "Error! Failed to initialize GLAD.\n";
}

void Application::initOpenGL()
{
	// set clear color of color buffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	// must include this line or else program crashes upon buffering texture data
	// https://stackoverflow.com/questions/9950546/c-opengl-glteximage2d-access-violation
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}

void Application::initSTB_Image()
{
	// so textures aren't displayed upsidedown
	stbi_set_flip_vertically_on_load(true);
}
