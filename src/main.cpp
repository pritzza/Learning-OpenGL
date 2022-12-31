#include <iostream>
#include <string>
#include <vector>

#include <cstddef>

#include <glad/glad.h>  // remember to include glad or glad debug dir in proj
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"

#include "Window.h"

#include "gl/ShaderProgram.h"
#include "gl/VBO.h"
#include "gl/VAO.h"
#include "gl/Texture.h"

#include "ModelMatrix.h"

#include "util/FileReader.h"
#include "util/Math.h"

Camera camera;

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
    Position::META.location != Color::META.location                 &&
    Color::META.location    != TextureCoordinate2D::META.location
);

// todo: find a way to make this more flexible/dynamic
constexpr size_t VERTEX_POS_OFFSET{      offsetof(Vertex, Vertex::pos     ) };
constexpr size_t VERTEX_COLOR_OFFSET{    offsetof(Vertex, Vertex::color   ) };
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

/*
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    glViewport(0, 0, windowWidth, windowHeight);
}

void handleInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
*/

int main()
{
    //// initilization

    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    Window window{ "Finally", 800, 600 };

    // load opengl functions with glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Error! Failed to initialize GLAD.\n";
        return -1;
    }

    // misc initilization

    //// stb_image library settings initialization
    // so textures aren't displayed upsidedown
    stbi_set_flip_vertically_on_load(true);

    ///// meat and bones OpenGL initilization
    
    //// one time stuff

    // set view port to window dimensions
    glViewport(0, 0, window.getWidth(), window.getHeight());

    // set clear color of color buffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    // must include this line or else program crashes upon buffering texture data
    // https://stackoverflow.com/questions/9950546/c-opengl-glteximage2d-access-violation
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    //// creating an object
    /// cube
    // create cube vao
    VAO cube;

    cube.defineAttribute(cubePosVertices);
    cube.defineAttribute(cubeTexCoords);

    VAO quad;

    quad.defineAttribute(quadPosVertices);
    quad.defineAttribute(quadTexCoords);

    constexpr std::string_view VERTEX_SHADER_SOURCE_PATH{ 
        "res/shaders/Vertex.glsl" 
    };
    constexpr std::string_view FRAGMENT_SHADER_SOURCE_PATH{ 
        "res/shaders/Fragment.glsl" 
    };

    // todo make this constexpr and readable
    constexpr std::string_view UNIFORM_TIME{ "time" };
    constexpr std::string_view UNIFORM_TEXTURE0{ "texture0" };
    constexpr std::string_view UNIFORM_TEXTURE1{ "texture1" };
    constexpr std::string_view UNIFORM_MODEL_MAT{ "model" };
    constexpr std::string_view UNIFORM_VIEW_MAT{ "view" };
    constexpr std::string_view UNIFORM_PERSPECTIVE_MAT{ "perspective" };

    const std::vector<std::string_view> SHADER_UNIFORM_NAMES = {
        UNIFORM_TIME,
        UNIFORM_TEXTURE0,
        UNIFORM_TEXTURE1,
        UNIFORM_MODEL_MAT,
        UNIFORM_VIEW_MAT,
        UNIFORM_PERSPECTIVE_MAT,
    };  
    
    ShaderProgram shaderProgram{ 
        VERTEX_SHADER_SOURCE_PATH,
        FRAGMENT_SHADER_SOURCE_PATH,
        SHADER_UNIFORM_NAMES
    };

    constexpr std::string_view texture0Path{ "res/textures/pipe.jpg" };
    constexpr std::string_view texture1Path{ "res/textures/log.png" };

    // generate textures
    Texture texture0{ texture0Path, GL_TEXTURE_2D };
    texture0.configure(
        GL_CLAMP_TO_EDGE,
        GL_CLAMP_TO_EDGE,
        GL_LINEAR_MIPMAP_LINEAR,
        GL_LINEAR
    );

    Texture texture1{ texture1Path, GL_TEXTURE_2D };
    texture1.configure(
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

    GLFWwindow* const glfwWindow{ window.getWindowPtr() };

    // program loop
    while (!window.shouldClose())
    {
        static double currentTime{ 0 };
        static double deltaTime{ 0 };

        deltaTime = glfwGetTime() - currentTime;
        currentTime = glfwGetTime();

        // handle input
        window.pollEvents();

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
            camera.moveAbsolute( Math::J_HAT, deltaTime);
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

        ////// RENDERING

        //// pre rendering frame
        // clear just color buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        ////// update per frame
            
        camera.update();

        glUseProgram(shaderProgram.get());
        shaderProgram.setUniformMat4(
            UNIFORM_PERSPECTIVE_MAT, 
            camera.getProjectionMatrix()
        );

        shaderProgram.setUniformMat4(UNIFORM_VIEW_MAT, camera.getViewMatrix());

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
            glUseProgram(shaderProgram.get());

            transform.update();
            shaderProgram.setUniformMat4(UNIFORM_MODEL_MAT, transform.getMatrix());
            
            shaderProgram.setUniformf(UNIFORM_TIME, glfwGetTime() + i);

            //// render object prep

            // bind vao
            glBindVertexArray(cube.getHandle());

            // bind shader program
            glUseProgram(shaderProgram.get());

            // bind correct textures to correct texture units
            // update sampler uniforms
            // (depends on rendering architecture)
            texture0.apply(GL_TEXTURE0);
            texture1.apply(GL_TEXTURE1);

            shaderProgram.setUniformi(UNIFORM_TEXTURE0, 0);
            shaderProgram.setUniformi(UNIFORM_TEXTURE1, 1);

            /// actually rendering object
            glDrawArrays(GL_TRIANGLES, 0, cubePosVertices.size());


            /// rendering clean up
            
            // (optional) unbind shader program
            glUseProgram(0);

            // (optional) unbind vao
            glBindVertexArray(0);
        }

        window.swapBuffers();
    }

    //// free resources

    // pretty sure this frees GL context, should be last
    glfwTerminate();

	return 0;
}