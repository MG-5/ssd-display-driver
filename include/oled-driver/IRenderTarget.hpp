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
};