#include "ftm.h"
#include "input_manager.h"
#include "mesh.h"
#include "obj_loader.h"
#include "opengl_util.h"
#include "program.h"
#include "shader.h"
#include "texture.h"
#include "time_manager.h"

#include "logger.h"
#include "utility.h"
#include <fstream>
#include <map>

int main(int argc, char **argv)
{
    OpenGLUtil *og = OpenGLUtil::GetInstance();
    InputManager *im = InputManager::GetInstance();
    TimeManager *tm = TimeManager::GetInstance();

    if (!og->Init())
    {
        return 1;
    }
    im->Init();

    std::map<std::string, std::string> config;
    std::ifstream ifs;
    if (argc > 1)
    {
        ifs.open(argv[1]);
    }
    else
    {
        logger::info << "::MAIN::OPEN_DEFAULT_CONFIG" << logger::endl;
        ifs.open("default.scop");
    }
    if (!ifs.is_open())
    {
        logger::error << "::MAIN::FAILED_TO_OPEN_CONFIG_FILE" << logger::endl;
        return 1;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        auto sv = utility::Split(line, "=");
        if (sv.size() == 2)
        {
            config[sv[0]] = sv[1];
        }
    }

    ShaderPtr vs = Shader::CreateFromFile(config["VS"], GL_VERTEX_SHADER);
    ShaderPtr fs = Shader::CreateFromFile(config["FS"], GL_FRAGMENT_SHADER);
    ProgramPtr p = nullptr;
    if (!vs || !fs || !(p = Program::Create({vs, fs})))
    {
        return 1;
    }
    Object obj_src;
    if (!LoadObject(config["OBJ"], obj_src))
    {
        return 1;
    }
    MeshPtr mesh = Mesh::Create(obj_src.vertices_, obj_src.indices_);
    if (!mesh)
    {
        return 1;
    }
    TexturePtr tex = Texture::Load(config["TEX"]);
    if (!tex)
    {
        return 1;
    }
    // tex->SetWrap(GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER);

    ftm::vec3 cam_pos(0.0f, 0.0f, 3.5f);
    ftm::vec3 obj_pos(0.0f, 0.0f, 0.0f);
    float move_speed = 0.01f;
    float obj_rot = 0.0f;
    float rot_speed = 45.0f;
    bool is_tex = false;

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    //    glEnable(GL_BLEND);
    //    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.35f, 0.35f, 0.35f, 1.0f);

    tm->Reset();
    while (!og->IsWindowClose())
    {
        tm->Update();
        im->Update();
        if (KEY_TAB(Key::ESC))
        {
            break;
        }
        if (KEY_HOLD(Key::W))
        {
            obj_pos.z -= move_speed;
        }
        if (KEY_HOLD(Key::S))
        {
            obj_pos.z += move_speed;
        }
        if (KEY_HOLD(Key::A))
        {
            obj_pos.x -= move_speed;
        }
        if (KEY_HOLD(Key::D))
        {
            obj_pos.x += move_speed;
        }
        if (KEY_HOLD(Key::Q))
        {
            obj_pos.y += move_speed;
        }
        if (KEY_HOLD(Key::E))
        {
            obj_pos.y -= move_speed;
        }
        if (KEY_TAB(Key::SPACE))
        {
            is_tex = !is_tex;
        }
        if (KEY_HOLD(Key::LEFT_CTRL))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
        if (KEY_AWAY(Key::LEFT_CTRL))
        {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
        float dt = tm->delta_time();
        obj_rot += dt * rot_speed;
        if (obj_rot > 360.0f)
        {
            obj_rot -= 360.0f;
        }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        auto view = ftm::lookAt(cam_pos, cam_pos + ftm::vec3(0.0f, 0.0f, 1.0f), ftm::vec3(0.0f, 1.0f, 0.0f));
        auto proj = ftm::perspective(ftm::radians(45.0f), 800.0f / 600.0f, 0.1f, 2000.0f);
        p->Use();
        p->SetUniform("view", view);
        p->SetUniform("proj", proj);
        p->SetUniform("model", ftm::translate(ftm::mat4(1.0f), obj_pos) *
                                   ftm::rotate(ftm::mat4(1.0f), ftm::radians(obj_rot), ftm::vec3(0.0f, -1.0f, 0.0f)) *
                                   ftm::scale(ftm::mat4(1.0f), ftm::vec3(0.5f)));
        p->SetUniform("is_tex", is_tex);
        p->SetUniform("center", obj_src.center_);
        mesh->Draw(p.get());

        og->SwapBuffer();
    }
    og->Terminate();

    return 0;
}
