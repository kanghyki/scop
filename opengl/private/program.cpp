#include "logger.h"
#include "program.h"

Program::Program() : id_(0)
{
}

Program::~Program()
{
    if (id_)
    {
        glDeleteProgram(id_);
    }
}

ProgramPtr Program::Create(const std::vector<ShaderPtr> &shaders)
{
    auto program = ProgramPtr(new Program());
    if (!program->Link(shaders))
    {
        return nullptr;
    }

    return program;
}

void Program::Use() const
{
    active_texture_count_ = 0;
    glUseProgram(id_);
}

bool Program::Link(const std::vector<ShaderPtr> &shaders)
{
    id_ = glCreateProgram();
    for (auto &shader : shaders)
    {
        glAttachShader(id_, shader->id());
    }
    glLinkProgram(id_);

    int success = 0;
    glGetProgramiv(id_, GL_LINK_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetProgramInfoLog(id_, 512, nullptr, info_log);
        logger::error << "PROGRAM::LINK::FAILED_TO_LINK_SHADERS" << logger::endl;
        logger::error << info_log << logger::endl;
    }

    return success;
}

uint32_t Program::GetUniformLocation(const std::string &name) const
{
    return glGetUniformLocation(id_, name.c_str());
}

void Program::SetUniform(const std::string &name, int value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform1i(loc, value);
}

void Program::SetUniform(const std::string &name, float value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform1f(loc, value);
}

void Program::SetUniform(const std::string &name, const ftm::vec2 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform2fv(loc, 1, ftm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const ftm::vec3 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform3fv(loc, 1, ftm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const ftm::vec4 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniform4fv(loc, 1, ftm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const ftm::mat4 &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, ftm::value_ptr(value));
}

void Program::SetUniform(const std::string &name, const std::vector<ftm::mat4> &value) const
{
    uint32_t loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, value.size(), GL_FALSE, ftm::value_ptr(*(value.data())));
}
