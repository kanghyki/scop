#ifndef INCLUDED_OPENGL_UTIL_H
#define INCLUDED_OPENGL_UTIL_H

#include "opengl_inc.h"
#include <string>

class OpenGLUtil
{
  public:
    static OpenGLUtil *GetInstance();
    ~OpenGLUtil();

    bool Init();
    bool IsWindowClose();
    void PollEvent();
    void SwapBuffer();
    void Terminate();

    GLFWwindow *glfw_window()
    {
        return glfw_window_;
    }

  private:
    OpenGLUtil();
    OpenGLUtil(const OpenGLUtil &c);
    OpenGLUtil &operator=(const OpenGLUtil &c);

    static OpenGLUtil *instance_;

    /* GLFW */
    GLFWwindow *glfw_window_;
};

#endif
