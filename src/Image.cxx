#include "oled-driver/Image.hpp"

//--------------------------------------------------------------------------------------------------
Image &Image::operator=(Image &&other)
{
    if (this != &other)
    {
        width = other.width;
        height = other.height;
        data = other.data;

        other.width = 0;
        other.height = 0;
        other.data = nullptr;
    }

    return *this;
}

//--------------------------------------------------------------------------------------------------
Image::Image(uint8_t width, uint8_t height, const uint8_t *data)
    : width(width), height(height), data(data)
{
}

//--------------------------------------------------------------------------------------------------
const uint8_t *Image::getData() const
{
    return data;
}