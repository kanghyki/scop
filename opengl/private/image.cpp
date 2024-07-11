#include "image.h"
#include "image_loader.h"
#include "logger.h"

Image::Image()
{
}

Image::~Image()
{
    if (data_)
    {
        delete[] data_;
    }
}

ImagePtr Image::Load(const std::string &filepath, bool flip_vertical)
{
    auto image = ImagePtr(new Image());
    if (!image->LoadFile(filepath, flip_vertical))
    {
        return nullptr;
    }

    return image;
}

bool Image::LoadFile(const std::string &filepath, bool flip_vertical)
{
    auto dpos = filepath.find_last_of('.');
    if (dpos == std::string::npos || filepath.substr(dpos) != ".tga")
    {
        logger::warning << "IMAGE::LOAD_FILE::UNSUPPORTED_FILE_EXTENSION: " << filepath << logger::endl;
        return false;
    }
    else
    {
        TGAHeader header;
        if (!ImageLoader::LoadTGA(filepath, &data_, header))
        {
            logger::warning << "IMAGE::LOAD_FILE::FAILED_TO_LOAD_IMAGE: " << filepath << logger::endl;
            return false;
        }
        width_ = header.width_;
        height_ = header.height_;
        channel_count_ = header.bits_per_pixel_ / 8;
        byte_per_channel_ = 1;
        if (flip_vertical)
        {
            ImageLoader::FilpVertical(header, data_);
        }
    }

    return true;
}
