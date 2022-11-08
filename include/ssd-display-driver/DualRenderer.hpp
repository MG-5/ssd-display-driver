#pragma once

#include "Renderer.hpp"

/// The original class Renderer is written for SSD1305/1306, which has a different VRAM structure.
/// There each byte represents a column with 8 pixel among each other.
/// But in SSD1675/1680 a byte represents 8 pixels side by side.
/// So at every render call we rearrange the VRAM and send both black and red content to
/// display interface.
class DualRenderer
{

public:
    DualRenderer(size_t screenWidth, size_t screenHeight, IRenderTarget &target)
        : ScreenWidth(screenWidth), ScreenHeight(screenHeight), target(target)
    {
        intermediateVram = new uint8_t[IntermediateVramSize];
    }

    ~DualRenderer()
    {
        delete[] intermediateVram;
    }

    const size_t ScreenWidth;
    const size_t ScreenHeight;

    // width must divided by 8 due a byte representing 8 pixels side by side
    const size_t IntermediateVramSize = (ScreenWidth / 8 * ScreenHeight);

    void dualRender()
    {
        rearrangeVram(black.vram);
        target.submitImage(intermediateVram, IntermediateVramSize | (1 << 24));
        //   bit 24 to trigger black

        rearrangeVram(red.vram);
        target.submitImage(intermediateVram, IntermediateVramSize | (1 << 26));
        //   bit 26 to trigger red

        target.submitImage(nullptr, 0);
        // length = 0 to trigger display update
    }

private:
    IRenderTarget &target;
    uint8_t *intermediateVram = nullptr;

    void rearrangeVram(uint8_t *vramToRead)
    {
        for (size_t widthIndex = 0; widthIndex < ScreenWidth; widthIndex++)
        {
            for (size_t page = 0; page < ScreenHeight / 8; page++)
            {
                uint8_t byteData = 0;

                size_t currentPosition = page * ScreenWidth + widthIndex;
                byteData = vramToRead[currentPosition];

                uint8_t reversedByteData = 0;
                for (size_t i = 0; i <= 3; i++)
                    reversedByteData |= (byteData & (1 << i)) << (7 - 2 * i);

                for (size_t i = 4; i <= 7; i++)
                    reversedByteData |= (byteData & (1 << i)) >> ((i - 3) * 2 - 1);

                // reverse bit order and shift them by 24 due uint32_t
                // return type byteData = __RBIT(byteData) >> 24; // only for ARM arch

                size_t dualRamIndex = page + widthIndex * ScreenHeight / 8;
                intermediateVram[dualRamIndex] = reversedByteData;
            }
        }
    }

public:
    // pages instead of raw height are needed
    Renderer black{ScreenWidth, ScreenHeight / 8, target};
    Renderer red{ScreenWidth, ScreenHeight / 8, target};
};