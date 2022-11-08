#pragma once

#include <stdbool.h>
#include <stdint.h>

#include "IRenderTarget.hpp"
#include "SSDInterface.hpp"

class SSD1675a : public IRenderTarget
{
public:
    enum class RamOption : uint8_t
    {
        Normal = 0,
        Bypass = 0b100,
        Inverse = 0b1000
    };

    enum class LutSelection
    {
        None,
        Default,
        BlackWhite,
        Delta,
        Red
    };

    explicit SSD1675a(SSDInterface &interface) : interface(interface){};

    /// needs to be called before init
    void selectLut(LutSelection selection)
    {
        lutSelection = selection;
    }

    void init();

    void setDriverOutput(uint8_t value1, uint8_t value2, uint8_t value3);
    void setSourceDrivingVoltage(uint8_t value1, uint8_t value2, uint8_t value3);
    void deepSleep(uint8_t mode);
    void setDataEntryMode(uint8_t value);
    void softwareReset();
    void masterActivation();
    void setDisplayUpdateControl1(RamOption redRamOption, RamOption blackRamOption,
                                  bool outputMode);
    void setDisplayUpdateControl2(uint8_t value);
    void writeVcomRegister(uint8_t value);
    virtual void loadLut();
    void setBorderWaveform(uint8_t value);
    void setXStartEnd(uint8_t start, uint8_t end);
    void setYStartEnd(uint16_t start, uint16_t end);
    void setAddressCounter(uint8_t x, const uint16_t y);
    void nop();

    void setAnalogBlock(uint8_t value);
    void setDigitalBlock(uint8_t value);
    void setDummyLinePeriod(uint8_t value);
    void setGateLineWidth(uint8_t value);

    void draw(uint8_t data);
    void draw(const uint8_t *data, size_t length);
    void submitImage(const uint8_t *image, size_t length) override;

protected:
    SSDInterface &interface;

    LutSelection lutSelection = LutSelection::None;
};
