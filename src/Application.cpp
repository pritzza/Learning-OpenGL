#include "Application.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <cassert>
#include <iostream>

#include <glm/trigonometric.hpp>

#include "gfx/VertexAttributes.h"

#include "util/Math.h"

#include "gfx/Materials.h"



#include <array>
#include <iostream>
#include <optional>
#include <string>

enum class Axis
{
    X,
    Y,
    Z
};

char toChar(Axis axis)
{
    switch (axis)
    {
    case Axis::X:   return 'x';
    case Axis::Y:   return 'y';
    case Axis::Z:   return 'z';
    }

    return '?';
}

struct Vector
{

    float x{};
    float y{};
    float z{};

    float getComponent(Axis a) const
    {
        switch (a)
        {
        case Axis::X:   return x;
        case Axis::Y:   return y;
        case Axis::Z:   return z;
        }

        return 0;
    }

    Vector operator+(const Vector& other) const
    {
        return Vector{
            x + other.x,
            y + other.y,
            z + other.z,
        };
    }

    Vector operator-(const Vector& other) const
    {
        return Vector{
            x - other.x,
            y - other.y,
            z - other.z,
        };
    }

    Vector operator*(float scalar) const
    {
        return Vector{
            x * scalar,
            y * scalar,
            z * scalar,
        };
    }
};

struct Line
{
    Vector start;
    Vector end;
};

struct Plane
{
    Axis axis;
    int value{};
};

enum class Face
{
    Front,
    Back,
    Left,
    Right,
    Top,
    Bottom,
    INVALID
};

Axis toAxis(Face f)
{
    switch (f)
    {
    case Face::Left:    return Axis::X;
    case Face::Right:   return Axis::X;

    case Face::Top:     return Axis::Y;
    case Face::Bottom:  return Axis::Y;

    case Face::Front:   return Axis::Z;
    case Face::Back:    return Axis::Z;
    }
}

const std::string toString(Face f)
{
    switch (f)
    {
    case Face::Front:   return "Front";
    case Face::Back:    return "Back";
    case Face::Left:    return "Left";
    case Face::Right:   return "Right";
    case Face::Top:     return "Top";
    case Face::Bottom:  return "Bottom";
    }

    return "INVALID";
}

Face toFace(Axis planeAxis, float component)
{
    switch (planeAxis)
    {
    case Axis::X:
        if (component > 0)
            return Face::Left;
        if (component < 0)
            return Face::Right;

    case Axis::Y:
        if (component > 0)
            return Face::Bottom;
        if (component < 0)
            return Face::Top;

    case Axis::Z:
        if (component > 0)
            return Face::Front;
        if (component < 0)
            return Face::Back;
    }
    return Face::INVALID;
}

// origin of our cubes are their front bottom left corner, when cubes faces the camera
// when intersecting say the back of a cube, the z coordinate of the
// actual point of intersection will have a z coordinate 1 greater
// than the coordinate identifier of the actual cube it hits
// this gives that difference between the point of intersection and the actual cube
Vector getOffset(Face intersectedFace)
{
    Vector offset{ 0, 0, 0 };

    switch (intersectedFace)
    {
    //case Face::Right:   offset.x = -1;  break;
    //case Face::Top:     offset.y = -1;  break;
    //case Face::Back:    offset.z = -1;  break;
    }

    return offset;
}

struct CubeIntersection
{
    Vector cubeCoords;
    Vector intersectionPosition;
    Face intersectedFace;
    float t;
};

// m*t + b
struct LinearEquation
{
    float m{};  // coefficient/slope

    float b{};  // constant term/intercept

    float valueAt(float t) const
    {
        return m * t + b;
    }
};

Vector getPoint(const Line& l, float t)
{
    //                  coefficient (m),     intercept (b)
    LinearEquation eqX{ l.end.x - l.start.x, l.start.x };
    LinearEquation eqY{ l.end.y - l.start.y, l.start.y };
    LinearEquation eqZ{ l.end.z - l.start.z, l.start.z };

    return Vector{
        eqX.valueAt(t),
        eqY.valueAt(t),
        eqZ.valueAt(t),
    };
}

std::optional<float> intersect(const Line& l, const Plane& p)
{
    LinearEquation eq;

    switch (p.axis)
    {
        //                                 coefficient (m),     intercept (b)
    case Axis::X: eq = LinearEquation{ l.end.x - l.start.x, l.start.x }; break;
    case Axis::Y: eq = LinearEquation{ l.end.y - l.start.y, l.start.y }; break;
    case Axis::Z: eq = LinearEquation{ l.end.z - l.start.z, l.start.z }; break;
    }

    std::optional<float> t{ std::nullopt };

    if (eq.m != 0)
        t = (p.value - eq.b) / eq.m;

    return t;
}

std::optional<CubeIntersection> processIntersection(
    const Plane& plane,
    const Vector& rayDirection,
    const Line& ray
)
{
    const std::optional<float> t{ intersect(ray, plane) };

    const bool isPlaneParallel{ t.has_value() };

    if (isPlaneParallel)
    {
        const float tVal{ t.value() };

        const bool isHit{ 0 <= tVal && tVal <= 1 };

        if (isHit)
        {
            // component of the ray direction that is the same as the plane's axis
            float component{ rayDirection.getComponent(plane.axis) };

            // the face that got hit
            Face cubeFace{ toFace(plane.axis, component) };

            // intersection point
            const Vector ip{ getPoint(ray, tVal) };

            Vector faceOffset{ getOffset(cubeFace) };

            static constexpr double SMALL_NUMBER{ 0.00001 };

            // intersected cube coordinates
            const Vector ic{
                (int)(ip.x + faceOffset.x + SMALL_NUMBER),
                (int)(ip.y + faceOffset.y + SMALL_NUMBER),
                (int)(ip.z + faceOffset.z + SMALL_NUMBER),
            };

            return CubeIntersection{ ic, ip, cubeFace, tVal };
        }
    }

    return std::nullopt;
}

void interpretIntersection(
    const std::optional<CubeIntersection>& intersection
)
{
    const bool intersectionOccured{ intersection.has_value() };

    if (intersectionOccured)
    {
        CubeIntersection i{ intersection.value() };

        std::cout << "Intersection on the " << toString(i.intersectedFace)
            << " face of cube ["
            << i.cubeCoords.x << ", "
            << i.cubeCoords.y << ", "
            << i.cubeCoords.z << "] at exactly ("
            << i.intersectionPosition.x << ", "
            << i.intersectionPosition.y << ", "
            << i.intersectionPosition.z << "), when t = "
            << i.t << '\n';
    }
}

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

    gl->lightSourceTransform.setPosition({ 0, -35.f, 0 });

    //gl->objectTransform.update();
    gl->lightSourceTransform.update();

    if (currentTime > 20)   // wait a little before light changes colors
    {
        gl->light.diffuseColor = glm::vec3{
            1.0f,                          // r
            (sin(currentTime) / 2) + .5 ,    // g
            (cos(currentTime) / 2) + .5      // b
        };
    }
    


    // initialize conditions
    auto dir = camera.getForwardDirection();
    auto pos = camera.getPosition();
    //Vector viewDir{ dir.x, dir.y, dir.z };
    //Vector viewDir{ 1.5f, 0.f, 6.5f };
    //double n{ .435534564 };
    double n{ currentTime };
    Vector viewDir{ std::sin(n), cos(n/2), std::cos(n) };
    Vector viewPos{ 0.5f, -3.f, 0.5f };

    const float castDistance{ 10.f };

    // create view ray
    Vector viewTarget{ viewDir * castDistance };

    Line viewRay{
        viewPos,
        viewTarget + viewPos,
    };

    static constexpr int NUM_PLANES_PER_AXIS{ 100 };
    static constexpr int NUM_AXIS{ 3 };
    static constexpr int NUM_PLANES{ NUM_PLANES_PER_AXIS * NUM_AXIS };

    std::array<Plane, NUM_PLANES> planes;

    // initialize planes
    for (int i = 0; i < NUM_PLANES; ++i)
    {
        const Axis planeAxis{ static_cast<Axis>(i / NUM_PLANES_PER_AXIS) };
        const int planeValue{ (i % NUM_PLANES_PER_AXIS) - (int)(NUM_PLANES_PER_AXIS / 2) };

        planes[i].axis = planeAxis;
        planes[i].value = planeValue;
    }

    std::vector<glm::vec3> cubeCoords;
    std::vector<float> cubeT;
    cubeT.push_back(1);
    cubeCoords.emplace_back(glm::vec3{
        viewRay.end.x,
        viewRay.end.y,
        viewRay.end.z + 1
        });

    // find intersection between view ray and planes
    for (const Plane& plane : planes)
    {
        std::optional<CubeIntersection> intersection{ processIntersection(
            plane, viewDir, viewRay
        ) };

        //interpretIntersection(intersection);

        if (intersection.has_value())
        {
            auto i{ intersection.value() };

            cubeCoords.emplace_back(glm::vec3{
                i.cubeCoords.x,
                i.cubeCoords.y,
                i.cubeCoords.z
                });

            //cubeCoords.emplace_back(glm::vec3{
            //    i.intersectionPosition.x,
            //    i.intersectionPosition.y,
            //    i.intersectionPosition.z
            //    });

            cubeT.push_back(i.t);
        }
    }
    std::cout << cubeCoords.size() << '\n';


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

    const int cubesLength{ 1 };
    for (int i = 0; i < cubesLength * cubesLength; ++i)
    {
        const float xPos = (i % cubesLength) - (cubesLength / 2.f);
        const float yPos = (i / cubesLength) - (cubesLength / 2.f);
        const float zPos = sqrt( pow(xPos, 2) + pow(yPos, 2)) + cubesLength/2.f;
        
        gl->objectTransform.setPosition({ xPos, yPos, zPos });

        gl->objectTransform.update();

        // setting object material based on index
        static constexpr int NUM_MATERIALS{ static_cast<int>(Materials::ID::SIZE) };
        const Materials::ID matID{ static_cast<Materials::ID>(i % NUM_MATERIALS) };
        gl->objectMaterial = Materials::MATERIALS.getVal(matID);


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

    for (int i = 0; i < cubeCoords.size(); ++i)
    {
        glm::vec3& cubeCoord = cubeCoords[i];
        gl->objectTransform.setPosition(cubeCoord);
        gl->objectTransform.setRotation({ 0.f, 0.f, 0.f });
        gl->objectTransform.update();

        gl->objectProgram.setUniformMat4(
            UNIFORM_MODEL_MAT,
            gl->objectTransform.getMatrix()
        );

        float t = cubeT[i];

        gl->objectMaterial.diffuseColor = glm::vec3{ t,t,t };
        gl->objectProgram.setUniformMaterial(
            UNIFORM_MATERIAL_SHININESS,
            UNIFORM_MATERIAL_AMBIENT,
            UNIFORM_MATERIAL_DIFFUSE,
            UNIFORM_MATERIAL_SPECULAR,
            gl->objectMaterial
        );

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
