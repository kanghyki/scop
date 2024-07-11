#include "logger.h"
#include "shader.h"
#include "utility.h"

Shader::Shader() : id_(0)
{
}

Shader::~Shader()
{
    if (id_)
    {
        glDeleteShader(id_);
    }
}

ShaderPtr Shader::CreateFromFile(const std::string &filename, GLenum shader_type)
{
    auto shader = ShaderPtr(new Shader());
    if (!shader->LoadFile(filename, shader_type))
    {
        return nullptr;
    }

    return shader;
}

bool Shader::LoadFile(const std::string &filename, GLenum shader_type)
{
    auto result = utility::LoadTextFile(filename);
    if (!result.has_value())
    {
        logger::error << "SHADER::LOAD_FILE::FAILED_TO_LOAD_FILE: " << filename << logger::endl;
        return false;
    }

    std::string &code = result.value();
    const char *src_ptr = code.c_str();
    int32_t code_length = code.length();

    id_ = glCreateShader(shader_type);
    glShaderSource(id_, 1, &src_ptr, &code_length);
    glCompileShader(id_);

    int success = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info_log[512];
        glGetShaderInfoLog(id_, 512, nullptr, info_log);
        logger::error << "SHADER::LOAD_FILE::FAILED_TO_COMPILE_SHADER: " << filename << logger::endl;
        logger::error << info_log << logger::endl;
    }

    return success;
}
