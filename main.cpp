#include "logger.h"
#include "opengl_util.h"

int main(int argc, char **argv)
{
    OpenGLUtil *og = OpenGLUtil::GetInstance();

    logger::info << "Initialize OpenGL" << logger::endl;
    if (!og->Init())
    {
        return 1;
    }

    logger::info << "Render loop" << logger::endl;
    while (!og->IsWindowClose())
    {
        // TODO: render ...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        og->SwapBuffer();
        og->PollEvent();
    }
    og->Terminate();

    return 0;
}
