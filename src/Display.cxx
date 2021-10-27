#include "oled-driver/Display.hpp"

namespace command
{
// clang-format off
constexpr auto SetLowerColumnStartAddress   = 0x00;
constexpr auto SetUpperColumnStartAddress   = 0x10;
constexpr auto SetMemoryAddressingMode      = 0x20;
constexpr auto SetColumnAddress             = 0x21;
constexpr auto SetPageAddress               = 0x22;
constexpr auto SetDisplayStartLine          = 0x40;
constexpr auto SetContrastControl           = 0x81;
constexpr auto SetBrightness                = 0x82;
constexpr auto ChargePumpSetting            = 0x8D;
constexpr auto SetLut                       = 0x91;
constexpr auto SetBankColor1To16            = 0x92;
constexpr auto SetBankColor17To32           = 0x93;
constexpr auto SetSegmentRemap              = 0xA0;
constexpr auto EntireDisplayOn              = 0xA4;
constexpr auto SetNormalInverseDisplay      = 0xA6;
constexpr auto SetMuxRatio                  = 0xA8;
constexpr auto DimModeSetting               = 0xAB;
constexpr auto MasterConfig                 = 0xAD;
constexpr auto SetDisplayOn                 = 0xAF;
constexpr auto SetDisplayOff                = 0xAE;
constexpr auto SetDisplayDimmed             = 0xAC;
constexpr auto SetPageStartAddress          = 0xB0;
constexpr auto SetComOutputDirection        = 0xC0;
constexpr auto SetDisplayOffset             = 0xD3;
constexpr auto SetDisplayClockDivider       = 0xD5;
constexpr auto SetAreaColorMode             = 0xD8;
constexpr auto SetPrechargingPeriod         = 0xD9;
constexpr auto SetComPinsConfig             = 0xDA;
constexpr auto SetVcomhDeselectLevel        = 0xDB;
constexpr auto EnterReadWriteModify         = 0xE0;
constexpr auto Nop                          = 0xE3;
constexpr auto ExitReadWriteModify          = 0xEE;
// clang-format on
} // namespace command

//--------------------------------------------------------------------------------------------------
Display::Display(SSD1306Interface &interface) : di{interface}
{
}

//--------------------------------------------------------------------------------------------------
void Display::setColumnStartAddress(uint8_t addr)
{
    columnStartAddress = addr;
    resetColumnStartAddress();
}

//--------------------------------------------------------------------------------------------------
void Display::resetColumnStartAddress()
{
    di.writeCommand(command::SetLowerColumnStartAddress | (columnStartAddress & 0xf));
    di.writeCommand(command::SetUpperColumnStartAddress | (columnStartAddress >> 4));
}

//--------------------------------------------------------------------------------------------------
void Display::setMemoryAddressingMode(Display::AddressingMode mode)
{
    di.writeCommand(command::SetMemoryAddressingMode);
    di.writeCommand(static_cast<uint8_t>(mode) & 0b11);
}

//--------------------------------------------------------------------------------------------------
void Display::setColumnAddress(uint8_t addrStart, uint8_t addrEnd)
{
    di.writeCommand(command::SetColumnAddress);
    di.writeCommand(addrStart);
    di.writeCommand(addrEnd);
}

//--------------------------------------------------------------------------------------------------
void Display::setPageAddress(uint8_t addrStart, uint8_t addrEnd)
{
    di.writeCommand(command::SetPageAddress);
    di.writeCommand(addrStart);
    di.writeCommand(addrEnd);
}

//--------------------------------------------------------------------------------------------------
void Display::setDisplayStartLine(uint8_t line)
{
    line &= 0x3f;
    di.writeCommand(command::SetDisplayStartLine | line);
}

//--------------------------------------------------------------------------------------------------
void Display::setContrastControl(uint8_t contrast)
{
    di.writeCommand(command::SetContrastControl);
    di.writeCommand(contrast);
}

//--------------------------------------------------------------------------------------------------
void Display::setBrightness(uint8_t brightness)
{
    di.writeCommand(command::SetBrightness);
    di.writeCommand(brightness);
}

//--------------------------------------------------------------------------------------------------
void Display::setLUT(uint8_t bank0, uint8_t colorA, uint8_t colorB, uint8_t colorC)
{
    di.writeCommand(command::SetLut);
    di.writeCommand(bank0);
    di.writeCommand(colorA);
    di.writeCommand(colorB);
    di.writeCommand(colorC);
}

//--------------------------------------------------------------------------------------------------
void Display::setSegmentRemap(bool remap)
{
    di.writeCommand(command::SetSegmentRemap | (remap ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void Display::setEntireDisplayOn(bool on)
{
    di.writeCommand(command::EntireDisplayOn | (on ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void Display::setInverseDisplay(bool inverse)
{
    di.writeCommand(command::SetNormalInverseDisplay | (inverse ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void Display::setMultiplexRatio(uint8_t ratio)
{
    di.writeCommand(command::SetMuxRatio);
    di.writeCommand(ratio);
}

//--------------------------------------------------------------------------------------------------
void Display::setDimMode(uint8_t contrast, uint8_t brightness)
{
    di.writeCommand(command::DimModeSetting);
    di.writeCommand(0); // reserved
    di.writeCommand(contrast);
    di.writeCommand(brightness);
}

//--------------------------------------------------------------------------------------------------
void Display::setDisplayState(Display::DisplayState state)
{
    uint8_t cmd = command::Nop;

    switch (state)
    {
    case Display::DisplayState::On:
        cmd = command::SetDisplayOn;
        break;

    case Display::DisplayState::Off:
        cmd = command::SetDisplayOff;
        break;

    case Display::DisplayState::Dimmed:
        cmd = command::SetDisplayDimmed;
        break;

    default:
        return;
    }

    di.writeCommand(cmd);
}

//--------------------------------------------------------------------------------------------------
void Display::setPageStartAddress(uint8_t addr)
{
    pageStartAddress = addr;
}

//--------------------------------------------------------------------------------------------------
void Display::resetPageStartAddress()
{
    di.writeCommand(command::SetPageStartAddress | (pageStartAddress & 0b111));
}

//--------------------------------------------------------------------------------------------------
void Display::setComOutputMode(Display::ComMode mode)
{
    uint8_t arg = 0;

    if (mode == Display::ComMode::Remap)
        arg |= 0b1000;

    di.writeCommand(command::SetComOutputDirection | arg);
}

//--------------------------------------------------------------------------------------------------
void Display::setDisplayOffset(uint8_t offset)
{
    di.writeCommand(command::SetDisplayOffset);
    di.writeCommand(offset);
}

//--------------------------------------------------------------------------------------------------
void Display::setDisplayClockDivide(uint8_t ratio, uint8_t fOsc)
{
    ratio &= 0xf;
    fOsc &= 0xf;

    di.writeCommand(command::SetDisplayClockDivider);
    di.writeCommand(ratio | (fOsc << 4));
}

//--------------------------------------------------------------------------------------------------
void Display::setAreaColorModeAndPowerMode(ColorMode color, PowerMode power)
{
    uint8_t arg = 0;

    if (color == Display::ColorMode::AreaColor)
        arg |= 0b11 << 4;

    if (power == Display::PowerMode::LowPower)
        arg |= 0b101;

    di.writeCommand(command::SetAreaColorMode);
    di.writeCommand(arg);
}

//--------------------------------------------------------------------------------------------------
void Display::setPrechargingPeriod(uint8_t phase1, uint8_t phase2)
{
    di.writeCommand(command::SetPrechargingPeriod);
    di.writeCommand(phase1 | (phase2 << 4));
}

//--------------------------------------------------------------------------------------------------
void Display::setComPinConfig(bool alternative, bool lrRemap)
{
    uint8_t arg = 0b10;

    if (alternative)
        arg |= 1 << 4;

    if (lrRemap)
        arg |= 1 << 5;

    di.writeCommand(command::SetComPinsConfig);
    di.writeCommand(arg);
}

//--------------------------------------------------------------------------------------------------
void Display::setVcomhDeselectLevel(VcomhLevel level)
{
    uint8_t arg = 0;

    switch (level)
    {
    case Display::VcomhLevel::x0_43:
        arg = 0;
        break;

    case Display::VcomhLevel::x0_77:
        arg = 0b1101;
        break;

    case Display::VcomhLevel::x0_83:
        arg = 0b1111;
        break;
    }

    arg <<= 2;

    di.writeCommand(command::SetVcomhDeselectLevel);
    di.writeCommand(arg);
}

//--------------------------------------------------------------------------------------------------
void Display::enterReadWriteModify()
{
    di.writeCommand(command::EnterReadWriteModify);
}

//--------------------------------------------------------------------------------------------------
void Display::exitReadWriteModify()
{
    di.writeCommand(command::ExitReadWriteModify);
}

//--------------------------------------------------------------------------------------------------
void Display::nop()
{
    di.writeCommand(command::Nop);
}

//--------------------------------------------------------------------------------------------------
void Display::setChargePump(bool enable)
{
    di.writeCommand(command::ChargePumpSetting);
    di.writeCommand(enable ? 0x14 : 0x10);
}

//--------------------------------------------------------------------------------------------------
void Display::draw(uint8_t data)
{
    di.writeData(data);
}

//--------------------------------------------------------------------------------------------------
void Display::draw(const uint8_t *data, size_t length)
{
    di.writeData(data, length);
}

//--------------------------------------------------------------------------------------------------
void Display::submitImage(const uint8_t *image, size_t length)
{
    resetPageStartAddress();
    resetColumnStartAddress();

    draw(image, length);
}