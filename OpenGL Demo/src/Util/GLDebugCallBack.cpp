#include "GLDebugCallBack.h"

#include <glad 3.3/glad.h>

#include <iostream>

void checkError(const char* where)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error;
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        }
        std::cout << error << ": " << where << '\n';
    }

}

/*
namespace {
    void GLAPIENTRY glDebugCallback(GLenum source, GLenum type, GLuint, GLenum severity,
        GLsizei, const char* message, const void*)
    {
        const char* severity_str = "?";
        switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:
            severity_str = "high";
            break;

        case GL_DEBUG_SEVERITY_MEDIUM:
            severity_str = "medium";
            break;

        case GL_DEBUG_SEVERITY_LOW:
            severity_str = "low";
            break;

        case GL_DEBUG_SEVERITY_NOTIFICATION:
            return;
        }

        const char* src = "?";
        switch (source) {
        case GL_DEBUG_SOURCE_API:
            src = "API";
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            src = "window system";
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            src = "shader compiler";
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            src = "third party";
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            src = "app";
            break;
        case GL_DEBUG_SOURCE_OTHER:
            src = "other";
            break;
        }

        const char* type_str = "?";
        switch (type) {
        case GL_DEBUG_TYPE_ERROR:
            type_str = "error";
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            type_str = "deprecated behavior";
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            type_str = "undefined behavior";
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            type_str = "portability";
            break;
        case GL_DEBUG_TYPE_MARKER:
            type_str = "marker";
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            type_str = "push group";
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            type_str = "pop group";
            break;
        case GL_DEBUG_TYPE_OTHER:
            type_str = "other";
            break;
        }

        std::cerr << "OpenGL Message\nType: " << type_str
            << "\nSeverity: " << severity_str << "\nSource: " << src
            << "\nMessage: " << message << "\n\n";
    }
} // namespace

void initGLDebug()
{
#ifndef __APPLE__
#ifndef NDEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // disable if in release
#endif
    glDebugMessageCallback(glDebugCallback, nullptr);

    // this disables messages printed for successfully compiled shaders
    glDebugMessageControl(GL_DEBUG_SOURCE_SHADER_COMPILER, GL_DEBUG_TYPE_OTHER,
        GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
}
*/