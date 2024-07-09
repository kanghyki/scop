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
        return false;
    }

    std::string &code = result.value();
    Preprocess(code, filename);
    const char *pSource = code.c_str();
    int32_t codeLength = code.length();

    id_ = glCreateShader(shader_type);
    glShaderSource(id_, 1, &pSource, &codeLength);
    glCompileShader(id_);

    int success = 0;
    glGetShaderiv(id_, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(id_, 512, nullptr, infoLog);
        logger::error << "ERROR::SHADER::COMPILATION_FAILED: " << filename << logger::endl;
        logger::error << infoLog << logger::endl;
    }

    return success;
}

void Shader::Preprocess(std::string &code, const std::string &filename) const
{
    const std::string &path = filename.substr(0, filename.find_last_of('/') + 1);
    size_t pos;

    while ((pos = code.find("#include")) != std::string::npos)
    {
        size_t end = code.find('\n', pos);
        if (end == std::string::npos)
        {
            break;
        }

        const std::string &include = code.substr(pos, end - pos);

        size_t f = include.find('"');
        size_t e = include.find_last_not_of('"');
        auto result = utility::LoadTextFile(path + include.substr(f + 1, e - f));
        if (!result.has_value())
        {
            break;
        }
        code.replace(pos, end - pos, result.value());
    }
}
