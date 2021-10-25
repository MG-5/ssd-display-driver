#pragma once

#include <cstddef>
#include <cstdint>

/// Interface for receiving a rendered image.
class IRenderTarget
{
public:
    /// Submits an entire image to the render target.
    /// \param image  Pointer to a byte array containing the image pixels sequentially.
    /// \param length The number of pixels submitted.
    virtual void submitImage(const uint8_t *image, size_t length) = 0;

    /// Updates only a limited region of the image.
    /// \param x      The X coordinate of the region, in pixels.
    /// \param width  The Width of the region, in pixels.
    /// \param y      The Y coordinate of the region, in pages (8 pixels).
    /// \param data   Pointer to byte array containing the region data.
    /// \param length The number of pixels the region consists of.
    virtual void updateRegion(size_t x, size_t width, size_t y, const uint8_t *data,
                              uint8_t length) = 0;
};