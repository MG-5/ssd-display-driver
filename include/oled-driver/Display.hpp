#pragma once

#include <cstddef>

#include "IRenderTarget.hpp"
#include "SSD1306.hpp"
#include "SSD1306Interface.hpp"

/// Implementation for the physical OLED display.
class Display : public SSD1306, public IRenderTarget
{
public:
    explicit Display(SSD1306Interface &interface);

    void setColumnStartAddress(uint8_t addr) override;
    void resetColumnStartAddress();
    void setMemoryAddressingMode(AddressingMode mode) override;
    void setColumnAddress(uint8_t addrStart, uint8_t addrEnd) override;
    void setPageAddress(uint8_t addrStart, uint8_t addrEnd) override;
    void setDisplayStartLine(uint8_t line) override;
    void setContrastControl(uint8_t contrast) override;
    void setBrightness(uint8_t brightness) override;
    void setLUT(uint8_t bank0, uint8_t colorA, uint8_t colorB, uint8_t colorC) override;
    void setSegmentRemap(bool remap) override;
    void setEntireDisplayOn(bool on) override;
    void setInverseDisplay(bool inverse) override;
    void setMultiplexRatio(uint8_t ratio) override;
    void setDimMode(uint8_t contrast, uint8_t brightness) override;
    void setDisplayState(DisplayState state) override;
    void setPageStartAddress(uint8_t addr) override;
    void resetPageStartAddress();
    void setComOutputMode(ComMode mode) override;
    void setDisplayOffset(uint8_t offset) override;
    void setDisplayClockDivide(uint8_t divideRatio, uint8_t fOsc) override;
    void setAreaColorModeAndPowerMode(ColorMode color, PowerMode power) override;
    void setPrechargingPeriod(uint8_t phase1, uint8_t phase2) override;
    void setComPinConfig(bool alternative, bool leftRightRemap) override;
    void setVcomhDeselectLevel(VcomhLevel level) override;
    void enterReadWriteModify() override;
    void exitReadWriteModify() override;
    void nop() override;

    /// Only for SSD1306
    void setChargePump(bool enable) override;

    void draw(uint8_t data) override;
    void draw(const uint8_t *data, size_t length) override;

    void submitImage(const uint8_t *image, size_t length) override;
    void updateRegion(size_t x, size_t width, size_t y, const uint8_t *data, uint8_t length)
    {
        // STUB
        (void)x;
        (void)width;
        (void)y;
        (void)data;
        (void)length;
    }

private:
    SSD1306Interface &di;

    uint8_t columnStartAddress = 0;
    uint8_t pageStartAddress = 0;
};