#pragma once

#include <cstddef>
#include <cstdint>

/// Hardware interface to a SSD1305/6 display controller.
class SSDInterface
{
public:
    /// Writes a single command byte to the display driver.
    /// \param cmd The command byte to be written.
    virtual void writeCommand(uint8_t cmd) = 0;

    /// Writes a single data byte to the display driver's GDDRAM.
    /// This method writes a single byte to the frame buffer in the display
    /// driver's GDDRAM. The write position is then updated according to the
    /// selected addressing mode.
    /// \param data The eight data bytes representing eight monochrome pixels.
    virtual void writeData(uint8_t data) = 0;

    /// Writes multiple bytes to the display driver's GDDRAM.
    /// This method writes multiple bytes to the frame buffer in the display
    /// driver's GDDRAM. The write position is then updated according to the
    /// selected addressing mode.
    /// \param data   Pointer to the pixel data to be written.
    /// \param length The number of data bytes to be written.
    virtual void writeData(const uint8_t *data, size_t length) = 0;

    /// Only needed for SSD1375a/SSD1680, which has a busy pin.
    /// For other devices simply stubs this function.
    /// Its clever to use FreeRTOS delay, if a RTOS is used.
    virtual void waitUntilIdle() = 0;
};