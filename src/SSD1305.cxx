#include "ssd-display-driver/SSD1305.hpp"

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
void SSD1305::setColumnStartAddress(uint8_t addr)
{
    columnStartAddress = addr;
    resetColumnStartAddress();
}

//--------------------------------------------------------------------------------------------------
void SSD1305::resetColumnStartAddress()
{
    di.writeCommand(command::SetLowerColumnStartAddress | (columnStartAddress & 0xf));
    di.writeCommand(command::SetUpperColumnStartAddress | (columnStartAddress >> 4));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setMemoryAddressingMode(SSD1305::AddressingMode mode)
{
    di.writeCommand(command::SetMemoryAddressingMode);
    di.writeCommand(static_cast<uint8_t>(mode) & 0b11);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setColumnAddress(uint8_t addrStart, uint8_t addrEnd)
{
    di.writeCommand(command::SetColumnAddress);
    di.writeCommand(addrStart);
    di.writeCommand(addrEnd);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setPageAddress(uint8_t addrStart, uint8_t addrEnd)
{
    di.writeCommand(command::SetPageAddress);
    di.writeCommand(addrStart);
    di.writeCommand(addrEnd);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setDisplayStartLine(uint8_t line)
{
    line &= 0x3f;
    di.writeCommand(command::SetDisplayStartLine | line);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setContrastControl(uint8_t contrast)
{
    di.writeCommand(command::SetContrastControl);
    di.writeCommand(contrast);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setBrightness(uint8_t brightness)
{
    di.writeCommand(command::SetBrightness);
    di.writeCommand(brightness);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setLUT(uint8_t bank0, uint8_t colorA, uint8_t colorB, uint8_t colorC)
{
    di.writeCommand(command::SetLut);
    di.writeCommand(bank0);
    di.writeCommand(colorA);
    di.writeCommand(colorB);
    di.writeCommand(colorC);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setSegmentRemap(bool remap)
{
    di.writeCommand(command::SetSegmentRemap | (remap ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setEntireDisplayOn(bool on)
{
    di.writeCommand(command::EntireDisplayOn | (on ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setInverseDisplay(bool inverse)
{
    di.writeCommand(command::SetNormalInverseDisplay | (inverse ? 1 : 0));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setMultiplexRatio(uint8_t ratio)
{
    di.writeCommand(command::SetMuxRatio);
    di.writeCommand(ratio);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setDimMode(uint8_t contrast, uint8_t brightness)
{
    di.writeCommand(command::DimModeSetting);
    di.writeCommand(0); // reserved
    di.writeCommand(contrast);
    di.writeCommand(brightness);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setDisplayState(SSD1305::DisplayState state)
{
    uint8_t cmd = command::Nop;

    switch (state)
    {
    case SSD1305::DisplayState::On:
        cmd = command::SetDisplayOn;
        break;

    case SSD1305::DisplayState::Off:
        cmd = command::SetDisplayOff;
        break;

    case SSD1305::DisplayState::Dimmed:
        cmd = command::SetDisplayDimmed;
        break;

    default:
        return;
    }

    di.writeCommand(cmd);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setPageStartAddress(uint8_t addr)
{
    pageStartAddress = addr;
}

//--------------------------------------------------------------------------------------------------
void SSD1305::resetPageStartAddress()
{
    di.writeCommand(command::SetPageStartAddress | (pageStartAddress & 0b111));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setComOutputMode(SSD1305::ComMode mode)
{
    uint8_t arg = 0;

    if (mode == SSD1305::ComMode::Remap)
        arg |= 0b1000;

    di.writeCommand(command::SetComOutputDirection | arg);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setDisplayOffset(uint8_t offset)
{
    di.writeCommand(command::SetDisplayOffset);
    di.writeCommand(offset);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setDisplayClockDivide(uint8_t ratio, uint8_t fOsc)
{
    ratio &= 0xf;
    fOsc &= 0xf;

    di.writeCommand(command::SetDisplayClockDivider);
    di.writeCommand(ratio | (fOsc << 4));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setAreaColorModeAndPowerMode(ColorMode color, PowerMode power)
{
    uint8_t arg = 0;

    if (color == SSD1305::ColorMode::AreaColor)
        arg |= 0b11 << 4;

    if (power == SSD1305::PowerMode::LowPower)
        arg |= 0b101;

    di.writeCommand(command::SetAreaColorMode);
    di.writeCommand(arg);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setPrechargingPeriod(uint8_t phase1, uint8_t phase2)
{
    di.writeCommand(command::SetPrechargingPeriod);
    di.writeCommand(phase1 | (phase2 << 4));
}

//--------------------------------------------------------------------------------------------------
void SSD1305::setComPinConfig(bool alternative, bool lrRemap)
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
void SSD1305::setVcomhDeselectLevel(VcomhLevel level)
{
    uint8_t arg = 0;

    switch (level)
    {
    case SSD1305::VcomhLevel::x0_43:
        arg = 0;
        break;

    case SSD1305::VcomhLevel::x0_77:
        arg = 0b1101;
        break;

    case SSD1305::VcomhLevel::x0_83:
        arg = 0b1111;
        break;
    }

    arg <<= 2;

    di.writeCommand(command::SetVcomhDeselectLevel);
    di.writeCommand(arg);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::enterReadWriteModify()
{
    di.writeCommand(command::EnterReadWriteModify);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::exitReadWriteModify()
{
    di.writeCommand(command::ExitReadWriteModify);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::nop()
{
    di.writeCommand(command::Nop);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::draw(uint8_t data)
{
    di.writeData(data);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::draw(const uint8_t *data, size_t length)
{
    di.writeData(data, length);
}

//--------------------------------------------------------------------------------------------------
void SSD1305::submitImage(const uint8_t *image, size_t length)
{
    resetPageStartAddress();
    resetColumnStartAddress();

    draw(image, length);
}