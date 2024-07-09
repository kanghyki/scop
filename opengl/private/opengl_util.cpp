#include "logger.h"
#include "opengl_util.h"

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    logger::info << "Framebuffer size changed : " << width << ", " << height << logger::endl;
    glViewport(0, 0, width, height);
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
        logger::error << "failed to initialize glfw: " << desc << logger::endl;
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
        logger::error << "failed to create glfw window" << logger::endl;
        Terminate();
        return false;
    }
    glfwMakeContextCurrent(glfw_window_);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        logger::error << "failed to initialize GLAD" << logger::endl;
        Terminate();
        return false;
    }
    glViewport(0, 0, WINDOW_INIT_WIDTH, WINDOW_INIT_HEIGHT);

    glfwSetFramebufferSizeCallback(glfw_window_, framebuffer_size_callback);

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

void OpenGLUtil::PollEvent()
{
    glfwPollEvents();
}

void OpenGLUtil::Terminate()
{
    glfwTerminate();
}
