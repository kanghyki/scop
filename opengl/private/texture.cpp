#include "texture.h"

/*
 * BaseTexture
 */

BaseTexture::BaseTexture(uint32_t texture_type) : id_(0), texture_type_(texture_type)
{
    glGenTextures(1, &id_);
}

BaseTexture::~BaseTexture()
{
    if (id_)
    {
        glDeleteTextures(1, &id_);
    }
}

void BaseTexture::Bind() const
{
    glBindTexture(texture_type_, id_);
}

void BaseTexture::SetFilter(uint32_t min_filter, uint32_t mag_filter) const
{
    glTexParameteri(texture_type_, GL_TEXTURE_MIN_FILTER, min_filter);
    glTexParameteri(texture_type_, GL_TEXTURE_MAG_FILTER, mag_filter);
}

void BaseTexture::SetWrap(uint32_t s_wrap, uint32_t t_wrap, uint32_t r_wrap) const
{
    glTexParameteri(texture_type_, GL_TEXTURE_WRAP_S, s_wrap);
    glTexParameteri(texture_type_, GL_TEXTURE_WRAP_T, t_wrap);
    if (r_wrap != GL_NONE)
    {
        glTexParameteri(texture_type_, GL_TEXTURE_WRAP_R, r_wrap);
    }
}

void BaseTexture::SetBorderColor(const ftm::vec4 &color) const
{
    glTexParameterfv(texture_type_, GL_TEXTURE_BORDER_COLOR, ftm::value_ptr(color));
}

uint32_t BaseTexture::format() const
{
    switch (internal_format_)
    {
    case GL_DEPTH_COMPONENT32F:
    case GL_DEPTH_COMPONENT:
        return GL_DEPTH_COMPONENT;
    case GL_RED:
    case GL_R:
    case GL_R16F:
    case GL_R32F:
        return GL_RED;
    case GL_RG:
    case GL_RG16F:
    case GL_RG32F:
        return GL_RG;
    case GL_RGB:
    case GL_RGB16F:
    case GL_RGB32F:
        return GL_RGB;
    case GL_RGBA:
    case GL_RGBA16F:
    case GL_RGBA32F:
        return GL_RGBA;
    }
    return -1;
}

uint32_t BaseTexture::data_type() const
{
    switch (internal_format_)
    {
    case GL_R16F:
    case GL_R32F:
    case GL_RG16F:
    case GL_RG32F:
    case GL_RGB16F:
    case GL_RGB32F:
    case GL_RGBA16F:
    case GL_RGBA32F:
    case GL_DEPTH_COMPONENT:
    case GL_DEPTH_COMPONENT32F:
        return GL_FLOAT;
    case GL_RED:
    case GL_R:
    case GL_RG:
    case GL_RGB:
    case GL_RGBA:
        return GL_UNSIGNED_BYTE;
    }
    return -1;
}

/*
 * Texture
 */

Texture::Texture() : BaseTexture(GL_TEXTURE_2D)
{
}

Texture::~Texture()
{
}

TexturePtr Texture::Load(const std::string &filename)
{
    auto image = Image::Load(filename);
    if (!image)
    {
        return nullptr;
    }
    auto texture = TexturePtr(new Texture());
    texture->Bind();
    texture->SetFilter(GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    texture->SetWrap(GL_CLAMP_TO_EDGE, GL_CLAMP_TO_EDGE);
    texture->SetTextureFromImage(image.get());

    return texture;
}

void Texture::SetTextureFromImage(const Image *image)
{
    width_ = image->width();
    height_ = image->height();
    GLenum iformat = GL_RGBA;
    switch (image->channel_count())
    {
    case 1:
        iformat = GL_RED;
        break;
    case 2:
        iformat = GL_RG;
        break;
    case 3:
        iformat = GL_RGB;
        break;
    default:
        break;
    }
    internal_format_ = iformat;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format_, width_, height_, 0, format(), data_type(), image->data());
    glGenerateMipmap(GL_TEXTURE_2D);
}
