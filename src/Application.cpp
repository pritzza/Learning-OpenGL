#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "ModelMatrix.h"

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

struct Vertex
{
    Position pos;
    Color color;
    TextureCoordinate2D texCoord;
};

// compile time sanity check to make sure no attirbutes have the same location
static_assert(
    Position::META.location != Color::META.location &&
    Color::META.location != TextureCoordinate2D::META.location
    );

// todo: find a way to make this more flexible/dynamic
constexpr size_t VERTEX_POS_OFFSET{ offsetof(Vertex, Vertex::pos) };
constexpr size_t VERTEX_COLOR_OFFSET{ offsetof(Vertex, Vertex::color) };
constexpr size_t VERTEX_TEXCOORD_OFFSET{ offsetof(Vertex, Vertex::texCoord) };

// quad
static const std::vector<Vertex> quadVertices = {
    //{ Position            }, { Color                  }, { TexCoord }
    { {  0.5f,  0.5f,  0.0f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { 1.0f, 1.0f } },  // top right
    { {  0.5f, -0.5f,  0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },  // bottom right
    { { -0.5f, -0.5f,  0.0f }, { 0.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f } },  // bottom left
    { { -0.5f,  0.5f,  0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 1.0f } },  // top left 
};

static const std::vector<Position> quadPosVertices = {
    {  0.5f,  0.5f,  0.0f },  // top right
    {  0.5f, -0.5f,  0.0f },  // bottom right
    { -0.5f, -0.5f,  0.0f },  // bottom left
    { -0.5f,  0.5f,  0.0f },  // top left 
};

static const std::vector<Color> quadColors = {
    { 1.0f, 0.0f, 1.0f, 1.0f },  // top right
    { 0.0f, 1.0f, 0.0f, 1.0f },  // bottom right
    { 0.0f, 1.0f, 1.0f, 1.0f },  // bottom left
    { 1.0f, 1.0f, 0.0f, 1.0f },  // top left 
};

static const std::vector<TextureCoordinate2D> quadTexCoords = {
    { 1.0f, 1.0f },  // top right
    { 1.0f, 0.0f },  // bottom right
    { 0.0f, 0.0f },  // bottom left
    { 0.0f, 1.0f },  // top left 
};

static const std::vector<GLuint> quadIndices = {
    0, 1, 3,   // first triangle
    1, 2, 3,   // second triangle
};

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

// triangle
static const std::vector<Vertex> triVertices = {
    //{ Position            }, { Color                  }, { TexCoord }
    { { -0.5f, -0.5f,  0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.0f, 0.0f } },  // bot left
    { {  0.5f, -0.5f,  0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 1.0f, 0.0f } },  // bot right 
    { {  0.0f,  0.5f,  0.0f }, { 1.0f, 1.0f, 0.0f, 1.0f }, { 0.5f, 1.0f } },  // top
};

static const std::vector<GLuint> triIndices = {
    0, 1, 2,   // first triangle
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


	// todo: turns our glad can only be loaded after creating ontext?
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

    // cube
	gl->cube.defineAttribute(cubePosVertices);
	gl->cube.defineAttribute(cubeTexCoords);

    // quad
    gl->quad.defineAttribute(quadPosVertices);
    gl->quad.defineAttribute(quadTexCoords);

    // shader stuff
    constexpr std::string_view VERTEX_SHADER_SOURCE_PATH{
        "res/shaders/Vertex.glsl"
    };
    constexpr std::string_view FRAGMENT_SHADER_SOURCE_PATH{
        "res/shaders/Fragment.glsl"
    };

    const std::vector<std::string_view> SHADER_UNIFORM_NAMES = {
        UNIFORM_TIME,
        UNIFORM_TEXTURE0,
        UNIFORM_TEXTURE1,
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };

    gl->shaderProgram.init(VERTEX_SHADER_SOURCE_PATH, 
        FRAGMENT_SHADER_SOURCE_PATH, 
        SHADER_UNIFORM_NAMES);

    /// texture stuff
    // texture 0
    constexpr std::string_view texture0Path{ "res/textures/pipe.jpg" };
    gl->texture0.init(texture0Path, GL_TEXTURE_2D);
    gl->texture0.configure(
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR
    );

    // texture 1
    constexpr std::string_view texture1Path{ "res/textures/log.png" };
    gl->texture1.init(texture1Path, GL_TEXTURE_2D);
    gl->texture1.configure(
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR
    );

    // create perspective projection matrix
    const float FOV{ glm::radians(45.f) };
    const float ASPECT_RATIO{ (float)window.getWidth() / window.getHeight() };
    const float NEAR_PLANE{ 0.1f };
    const float FAR_PLANE{ 100.0f };

    camera.setProjectionMatrix(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

    camera.setPosition({ -7.f, 0.f, 0.f });
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



    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ////// update per frame

    camera.update();

    glUseProgram(gl->shaderProgram.get());
    gl->shaderProgram.setUniformMat4(
        UNIFORM_PERSPECTIVE_MAT,
        camera.getProjectionMatrix()
    );

    gl->shaderProgram.setUniformMat4(UNIFORM_VIEW_MAT, camera.getViewMatrix());

    //// render
    const int numCubes{ 300 };
    for (int i = 0; i < numCubes; ++i)
    {
        ///// update per object
        /// update per model uniforms

        /// update model matrices

        ModelMatrix transform;

        transform.translate({
            std::cosf(glfwGetTime() + i),
            std::sinf(glfwGetTime() + i),
            i - (numCubes / 2.f)
            });

        transform.rotate(glfwGetTime(), Math::Axis::Z);

        // actually setting uniforms
        // (program must be active to update uniforms)
        glUseProgram(gl->shaderProgram.get());

        transform.update();
        gl->shaderProgram.setUniformMat4(UNIFORM_MODEL_MAT, transform.getMatrix());

        gl->shaderProgram.setUniformf(UNIFORM_TIME, glfwGetTime() + i);

        //// render object prep

        // bind vao
        glBindVertexArray(gl->cube.getHandle());

        // bind shader program
        glUseProgram(gl->shaderProgram.get());

        // bind correct textures to correct texture units
        // update sampler uniforms
        // (depends on rendering architecture)
        gl->texture0.apply(GL_TEXTURE0);
        gl->texture1.apply(GL_TEXTURE1);

        gl->shaderProgram.setUniformi(UNIFORM_TEXTURE0, 0);
        gl->shaderProgram.setUniformi(UNIFORM_TEXTURE1, 1);

        /// actually rendering object
        glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());


        /// rendering clean up

        // (optional) unbind shader program
        glUseProgram(0);

        // (optional) unbind vao
        glBindVertexArray(0);
    }

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
