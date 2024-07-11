#include "input_manager.h"
#include "logger.h"
#include "opengl_util.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    logger::info << "::FRAMEBUFFER_SIZE_CALLBACK:: " << width << "x" << height << logger::endl;
    glViewport(0, 0, width, height);
}

void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
#ifdef EDITOR
    ImGui_ImplGlfw_KeyCallback(window, key, scancode, action, mods);
#endif
    InputManager::GetInstance()->UpdateKey(key, action);
}

OpenGLUtil *OpenGLUtil::instance_ = nullptr;

OpenGLUtil *OpenGLUtil::GetInstance()
{
    if (!instance_)
    {
        instance_ = new OpenGLUtil();
    }
    return instance_;
}

OpenGLUtil ::OpenGLUtil() : glfw_window_(nullptr)
{
}

OpenGLUtil ::~OpenGLUtil()
{
}

bool OpenGLUtil::Init()
{
    if (!glfwInit())
    {
        const char *desc = nullptr;
        glfwGetError(&desc);
        logger::error << "OPENGL_UTIL::INIT::FAILED_TO_INIT_GLFW: " << desc << logger::endl;
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifndef __OSX__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    glfw_window_ = glfwCreateWindow(WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT, WINDOW_NAME, nullptr, nullptr);
    if (!glfw_window_)
    {
        logger::error << "OPENGL_UTIL::INIT::FAILED_TO_CREATE_WINDOW" << logger::endl;
        Terminate();
        return false;
    }
    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger::error << "OPENGL_UTIL::INIT::FAILED_TO_INIT_GLAD" << logger::endl;
        Terminate();
        return false;
    }
    glViewport(0, 0, WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT);

    glfwSetFramebufferSizeCallback(glfw_window_, framebuffer_size_callback);
    glfwSetKeyCallback(glfw_window_, KeyCallback);

    return true;
}

bool OpenGLUtil::IsWindowClose()
{
    return glfwWindowShouldClose(glfw_window_);
}

void OpenGLUtil::SwapBuffer()
{
    glfwSwapBuffers(glfw_window_);
}

void OpenGLUtil::Terminate()
{
    glfwTerminate();
}
