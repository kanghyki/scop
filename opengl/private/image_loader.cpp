#include "image_loader.h"
#include <fstream>
#include <iostream>

ImageLoader::ImageLoader()
{
}

ImageLoader::~ImageLoader()
{
}

bool ImageLoader::LoadTGA(const std::string &in_filepath, uint8_t **out_data, TGAHeader &header)
{
    std::ifstream file(in_filepath);
    if (!file.is_open())
    {
        return false;
    }
    file.read(reinterpret_cast<char *>(&header.id_length_), sizeof(header.id_length_));
    file.read(reinterpret_cast<char *>(&header.color_map_type_), sizeof(header.color_map_type_));
    file.read(reinterpret_cast<char *>(&header.image_type_), sizeof(header.image_type_));
    file.read(reinterpret_cast<char *>(&header.color_map_origin_), sizeof(header.color_map_origin_));
    file.read(reinterpret_cast<char *>(&header.color_map_length_), sizeof(header.color_map_length_));
    file.read(reinterpret_cast<char *>(&header.color_map_depth_), sizeof(header.color_map_depth_));
    file.read(reinterpret_cast<char *>(&header.x_origin_), sizeof(header.x_origin_));
    file.read(reinterpret_cast<char *>(&header.y_origin_), sizeof(header.y_origin_));
    file.read(reinterpret_cast<char *>(&header.width_), sizeof(header.width_));
    file.read(reinterpret_cast<char *>(&header.height_), sizeof(header.height_));
    file.read(reinterpret_cast<char *>(&header.bits_per_pixel_), sizeof(header.bits_per_pixel_));
    file.read(reinterpret_cast<char *>(&header.image_descriptor_), sizeof(header.image_descriptor_));

    size_t size = header.width_ * header.height_ * (header.bits_per_pixel_ / 8);
    (*out_data) = new uint8_t[size];
    if (*out_data)
    {
        file.read(reinterpret_cast<char *>(*out_data), size);
    }
    file.close();

    if (*out_data)
    {
        return true;
    }
    return false;
}

bool ImageLoader::FilpVertical(TGAHeader &header, uint8_t *out_data)
{
    if (!out_data)
    {
        return false;
    }
    size_t stride = header.width_ * (header.bits_per_pixel_ / 8);
    uint8_t *temp = new uint8_t[stride];

    for (size_t i = 0; i < header.height_ / 2; ++i)
    {
        size_t itop = i * stride;
        size_t ibottom = (header.height_ - 1 - i) * stride;
        std::memcpy(temp, &out_data[itop], stride);
        std::memcpy(&out_data[itop], &out_data[ibottom], stride);
        std::memcpy(&out_data[ibottom], temp, stride);
    }
    delete[] temp;

    return true;
}
