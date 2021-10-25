#pragma once

#include <cstdint>

class Image
{
public:
    /// Construct a new Image object and leaves it uninitialized.
    Image() = default;

    //! Move assignment operator
    Image &operator=(Image &&other);

    /// Constructs a new Image object.
    /// \param width     Width of the image, in px.
    /// \param height    Height of the image, in pages. One page equals 8 px.
    /// \param data      Pointer to the image bits. The memory this pointer points to must be at
    /// least width * height bytes in size.
    Image(uint8_t width, uint8_t height, const uint8_t *data);

    /// Returns the image width in pixels.
    constexpr uint8_t getWidth() const
    {
        return width;
    }

    /// Returns the image height in pages (one page equals eight pixels).
    constexpr uint8_t getHeight() const
    {
        return height;
    }

    /// Returns a pointer to the image's bitmap data.
    const uint8_t *getData() const;

private:
    uint8_t width = 0;
    uint8_t height = 0;
    const uint8_t *data = nullptr;
};