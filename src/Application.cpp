#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "gfx/VertexAttributes.h"

#include "util/Math.h"

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

static const std::vector<Normal> cubeNormals = {
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },
   { 0.0f,  0.0f, -1.0f },

   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },
   { 0.0f,  0.0f, 1.0f, },

   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },
   {-1.0f,  0.0f,  0.0f },

   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },
   { 1.0f,  0.0f,  0.0f },

   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },
   { 0.0f, -1.0f,  0.0f },

   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
   { 0.0f,  1.0f,  0.0f },
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
    gl->objectCube.defineAttribute(cubeNormals);

    //// shader stuff

    // object shader
    constexpr std::string_view OBJECT_VERTEX_SHADER_PATH{
        "res/shaders/Lighting/ObjectVertex.glsl"
    };
    constexpr std::string_view OBJECT_FRAGMENT_SHADER_PATH{
        "res/shaders/Lighting/ObjectFragment.glsl"
    };

    const std::vector<std::string_view> OBJECT_PROGRAM_UNIFORM_NAMES = {
        
        // material
        UNIFORM_MATERIAL_SHININESS,
        UNIFORM_MATERIAL_AMBIENT,
        UNIFORM_MATERIAL_DIFFUSE,
        UNIFORM_MATERIAL_SPECULAR,

        // light
        UNIFORM_LIGHT_POSITION,
        UNIFORM_LIGHT_AMBIENT,
        UNIFORM_LIGHT_DIFFUSE,
        UNIFORM_LIGHT_SPECULAR,

        // for specular lighting
        UNIFORM_VIEW_POSITION,

        // camera transform
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };

    gl->objectProgram.init(
        OBJECT_VERTEX_SHADER_PATH,
        OBJECT_FRAGMENT_SHADER_PATH,
        OBJECT_PROGRAM_UNIFORM_NAMES
    );


    /// light source shader
    constexpr std::string_view LIGHT_SOURCE_VERTEX_SHADER_PATH{
        "res/shaders/Lighting/LightSourceVertex.glsl"
    };
    constexpr std::string_view LIGHT_SOURCE_FRAGMENT_SHADER_PATH{
        "res/shaders/Lighting/LightSourceFragment.glsl"
    };

    const std::vector<std::string_view> LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES = {

        // color of light source model
        UNIFORM_LIGHT_COLOR,

        // for camera transform
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };

    gl->lightSourceProgram.init(
        LIGHT_SOURCE_VERTEX_SHADER_PATH,
        LIGHT_SOURCE_FRAGMENT_SHADER_PATH,
        LIGHT_SOURCE_PROGRAM_UNIFORM_NAMES
    );


    // create perspective projection matrix
    const float FOV{ glm::radians(45.f) };
    const float ASPECT_RATIO{ (float)window.getWidth() / window.getHeight() };
    const float NEAR_PLANE{ 0.1f };
    const float FAR_PLANE{ 100.0f };

    camera.setProjectionMatrix(FOV, ASPECT_RATIO, NEAR_PLANE, FAR_PLANE);

    camera.setPosition({ 0.0f, 0.f, -10.f });
    camera.setYaw(glm::radians(90.f));

    gl->objectTransform.scale(0.8f);

    gl->lightSourceTransform.setPosition({ 0.f, 0.f, 0.f });

    gl->light.position = gl->lightSourceTransform.getPosition();
    gl->light.ambientColor = glm::vec3{ 0.1f };
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
   
    gl->objectTransform.rotate({
        deltaTime/1.f,
        deltaTime/2.f,
        deltaTime/3.f
    });

    //gl->objectTransform.update();
    gl->lightSourceTransform.update();

    gl->light.diffuseColor = glm::vec3{ 
        1.0f,                          // r
        (sin(currentTime)/2) + .5 ,    // g
        (cos(currentTime)/2) + .5      // b
    };
    
    gl->objectMaterial.diffuseColor = glm::vec3{ 
        1.0f,           // r
        0.0f,           // g
        1.0f            // b
    };

    //// pre rendering frame
    // clear color buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /// rendering light source

    // activate shader
    gl->lightSourceProgram.use();

    /// update all uniforms (CAMERA TRANSFORM and light color)
    // camera transform
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

    // lighting stuff
    gl->lightSourceProgram.setUniformVec3f(
        UNIFORM_LIGHT_COLOR,
        gl->light.diffuseColor
    );

    // bind light source VAO
    gl->lightSourceCube.bind();

    // draw light source
    glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());


    /// rendering object

    // activate object shader
    gl->objectProgram.use();

    const int cubesLength{ 10 };
    for (int i = 0; i < cubesLength * cubesLength; ++i)
    {
        const float xPos = (i % cubesLength) - (cubesLength / 2.f);
        const float yPos = (i / cubesLength) - (cubesLength / 2.f);
        const float zPos = sqrt( pow(xPos, 2) + pow(yPos, 2)) + cubesLength/2.f;
        
        gl->objectTransform.setPosition({ xPos, yPos, zPos });

        gl->objectTransform.update();


        /// update all uniforms:
        /// camera transform, view position, light, and object material
        
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

        // lighting stuff (view position, light, object material)
        gl->objectProgram.setUniformVec3f(
            UNIFORM_VIEW_POSITION,
            camera.getPosition()
        );

        gl->objectProgram.setUniformLight(
            UNIFORM_LIGHT_POSITION,
            UNIFORM_LIGHT_AMBIENT,
            UNIFORM_LIGHT_DIFFUSE,
            UNIFORM_LIGHT_SPECULAR,
            gl->light
        );

        gl->objectProgram.setUniformMaterial(
            UNIFORM_MATERIAL_SHININESS,
            UNIFORM_MATERIAL_AMBIENT,
            UNIFORM_MATERIAL_DIFFUSE,
            UNIFORM_MATERIAL_SPECULAR,
            gl->objectMaterial
        );

        // bind object VAO
        gl->objectCube.bind();

        // draw object
        glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());
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
