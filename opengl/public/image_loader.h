#ifndef INCLUDED_IMAGE_LOADER_H
#define INCLUDED_IMAGE_LOADER_H

#include <string>

struct TGAHeader;

class ImageLoader
{
  public:
    ~ImageLoader();

    static bool LoadTGA(const std::string &in_filepath, uint8_t **out_data, TGAHeader &header);
    static bool FilpVertical(TGAHeader &header, uint8_t *out_data);

  private:
    ImageLoader();
    ImageLoader(const ImageLoader &);
    ImageLoader &operator=(const ImageLoader &);
};

struct TGAHeader
{
    uint8_t id_length_;
    uint8_t color_map_type_;
    uint8_t image_type_;
    uint16_t color_map_origin_;
    uint16_t color_map_length_;
    uint8_t color_map_depth_;
    uint16_t x_origin_;
    uint16_t y_origin_;
    uint16_t width_;
    uint16_t height_;
    uint8_t bits_per_pixel_;
    uint8_t image_descriptor_;
};

#endif
