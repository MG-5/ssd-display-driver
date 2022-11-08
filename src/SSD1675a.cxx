#include "ssd-display-driver/SSD1675a.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>

constexpr auto Width = 152;
constexpr auto Height = 296;

// Waveforms

// 00 = VSS  =   0V
// 01 = VSH1 =  15V
// 10 = VSL  = -15V
// 11 = VSH2 =   5V

// https://forums.pimoroni.com/t/my-inky-phat-clock-refresh-speed-question/6955/5

namespace ssd1675a_lut
{
constexpr auto lutSize = 70;

// clang-format off
//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize>  BlackWhite = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00100010, 0b00010001, 0b00010000, 0b00000000, 0b00010000, 0b00000000, 0b00000000, // lut0 (black)
    0b00010001, 0b10001000, 0b10000000, 0b10000000, 0b10000000, 0b00000000, 0b00000000, // lut1 (white)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut2 (red)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut3 (red)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)

    // Duration       | Repeats
    // A   B   C   D  |
       4, 24,  4, 22,   1, // Phase 0
      10, 10, 10, 10,   2, // Phase 1
       0,  0,  0,  0,   0, // Phase 2
       0,  0,  0,  0,   0, // Phase 3
       0,  0,  0,  0,   0, // Phase 4
       0,  0,  0,  0,   0, // Phase 5
       0,  0,  0,  0,   0  // Phase 6
};

//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize>  Delta = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut0 (none)
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut1 (b->w)
    0b01000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut2 (w->b)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut3 (none)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)

    // Duration       | Repeats
    // A   B   C   D  |
      24,  0,  0,  0,   1, // Phase 0
       0,  0,  0,  0,   0, // Phase 1
       0,  0,  0,  0,   0, // Phase 2
       0,  0,  0,  0,   0, // Phase 3
       0,  0,  0,  0,   0, // Phase 4
       0,  0,  0,  0,   0, // Phase 5
       0,  0,  0,  0,   0  // Phase 6
};

//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize>  Default = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00100010, 0b00010001, 0b00010000, 0b00000000, 0b00010000, 0b00000000, 0b00000000, // lut0 (black)
    0b00010001, 0b10001000, 0b10000000, 0b10000000, 0b10000000, 0b00000000, 0b00000000, // lut1 (white)
    0b01101010, 0b10011011, 0b10011011, 0b10011011, 0b10011011, 0b00000000, 0b00000000, // lut2 (red)
    0b01101010, 0b10011011, 0b10011011, 0b10011011, 0b10011011, 0b00000000, 0b00000000, // lut3 (red)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)

    // Duration      | Repeats
    // A   B   C   D |
       4, 24,  4, 22,   1, // Phase 0
      10, 10, 10, 10,   2, // Phase 1
       0,  0,  0,  0,   0, // Phase 2
       0,  0,  0,  0,   0, // Phase 3
       4,  4,  8, 60,   7, // Phase 4
       0,  0,  0,  0,   0, // Phase 5
       0,  0,  0,  0,   0  // Phase 6
};
// clang-format on
} // namespace ssd1675a_lut

//--------------------------------------------------------------------------------------------------
namespace command
{
// clang-format off
constexpr auto DriverOutput 				= 0x01;
constexpr auto GateDrivingVoltage 			= 0x03;
constexpr auto SourceDrivingVoltage 		= 0x04;
constexpr auto DeepSleep 					= 0x10;
constexpr auto DataEntryMode	 			= 0x11;
constexpr auto SoftwareReset 				= 0x12;
constexpr auto TemperatureSensorControl		= 0x18;
constexpr auto WriteTemperatureSensor		= 0x1A;
constexpr auto ReadTemperatureSensor		= 0x1B;
constexpr auto WriteExtTemperatureSensor    = 0x1C;
constexpr auto MasterActivation			 	= 0x20;
constexpr auto DisplayUpdateControl1 		= 0x21;
constexpr auto DisplayUpdateControl2 		= 0x22;
constexpr auto WriteBWRam 					= 0x24;
constexpr auto WriteRedRam			 	    = 0x26;
constexpr auto WriteVcomRegister 			= 0x2C;
constexpr auto WriteLUTRegister 			= 0x32;
constexpr auto DummyLinePeriod 				= 0x3A;
constexpr auto GateLineWidth 				= 0x3B;
constexpr auto BorderWaveform 				= 0x3C;
constexpr auto ReadRamOption				= 0x41;
constexpr auto RamXStartEndPos				= 0x44;
constexpr auto RamYStartEndPos				= 0x45;
constexpr auto AutoWriteRedRam 				= 0x46;
constexpr auto AutoWriteBWRam 				= 0x47;
constexpr auto RamXCounter 					= 0x4E;
constexpr auto RamYCounter 					= 0x4F;
constexpr auto AnalogBlock 					= 0x74;
constexpr auto DigitalBlock 				= 0x7E;
constexpr auto Nop							= 0x7F;

// clang-format on
} // namespace command

//--------------------------------------------------------------------------------------------------
void SSD1675a::init()
{
    // readCalibration();

    interface.waitUntilIdle();
    softwareReset();
    interface.waitUntilIdle();
    vTaskDelay(20);

    setDataEntryMode(0b001);
    setDisplayUpdateControl1(RamOption::Normal, RamOption::Inverse, false);
    setDriverOutput(Height & 0xFF, (Height & 0x100) >> 8, 0x00);

    setBorderWaveform(0x80);

    if (lutSelection != LutSelection::None)
    {
        loadLut();
        interface.writeCommand(0x22); // Display Update Control 2
        interface.writeData(
            0xCF); // Enable clock signal, Analog, Display with DISPLAY Mode 2, Disable Analog, OSC
    }
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDriverOutput(uint8_t value1, uint8_t value2, uint8_t value3)
{
    interface.writeCommand(command::DriverOutput);
    interface.writeData(value1); // MUX gate lines
    interface.writeData(value2); // MUX gate lines
    interface.writeData(value3); // Gate scanning sequence and direction
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setSourceDrivingVoltage(uint8_t value1, uint8_t value2, uint8_t value3)
{
    interface.writeCommand(command::SourceDrivingVoltage);
    interface.writeData(value1);
    interface.writeData(value2);
    interface.writeData(value3);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::deepSleep(uint8_t mode)
{
    interface.writeCommand(command::DeepSleep);
    interface.writeData(mode);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDataEntryMode(uint8_t value)
{
    interface.writeCommand(command::DataEntryMode);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::softwareReset()
{
    interface.writeCommand(command::SoftwareReset);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::masterActivation()
{
    interface.writeCommand(command::MasterActivation);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDisplayUpdateControl1(RamOption redRamOption, RamOption blackRamOption,
                                        bool outputMode)
{
    uint8_t value = static_cast<uint8_t>(redRamOption) << 4;
    value |= static_cast<uint8_t>(blackRamOption);

    interface.writeCommand(command::DisplayUpdateControl1);
    interface.writeData(value);
    interface.writeData((outputMode & 0x1) << 7);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDisplayUpdateControl2(uint8_t value)
{
    interface.writeCommand(command::DisplayUpdateControl2);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::writeVcomRegister(uint8_t value)
{
    interface.writeCommand(command::WriteVcomRegister);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::loadLut()
{
    using namespace ssd1675a_lut;

    if (lutSelection == LutSelection::None)
        return;

    interface.writeCommand(command::WriteLUTRegister);

    switch (lutSelection)
    {
    case LutSelection::BlackWhite:
        interface.writeData(reinterpret_cast<const uint8_t *>(BlackWhite.data()), lutSize);
        break;

    case LutSelection::Delta:
        interface.writeData(reinterpret_cast<const uint8_t *>(Delta.data()), lutSize);
        break;

    case LutSelection::Red:
        // return ssd1675a_lut::Red;
        // break;
    case LutSelection::Default:
    default:
        interface.writeData(reinterpret_cast<const uint8_t *>(Default.data()), lutSize);
        break;
    }

    interface.waitUntilIdle();
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setBorderWaveform(uint8_t value)
{
    interface.writeCommand(command::BorderWaveform);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setXStartEnd(uint8_t start, uint8_t end)
{
    interface.writeCommand(command::RamXStartEndPos);
    interface.writeData(start);
    interface.writeData(end);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setYStartEnd(uint16_t start, uint16_t end)
{
    interface.writeCommand(command::RamYStartEndPos);
    interface.writeData(start & 0xFF);
    interface.writeData(start >> 8);
    interface.writeData(end & 0xFF);
    interface.writeData(end >> 8);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setAddressCounter(uint8_t x, const uint16_t y)
{
    interface.writeCommand(command::RamXCounter);
    interface.writeData(x);

    interface.writeCommand(command::RamYCounter);
    interface.writeData(y & 0xFF);
    interface.writeData(y >> 8);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::nop()
{
    interface.writeCommand(command::Nop);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setAnalogBlock(uint8_t value)
{
    interface.writeCommand(command::AnalogBlock);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDigitalBlock(uint8_t value)
{
    interface.writeCommand(command::DigitalBlock);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setDummyLinePeriod(uint8_t value)
{
    interface.writeCommand(command::DummyLinePeriod);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::setGateLineWidth(uint8_t value)
{
    interface.writeCommand(command::GateLineWidth);
    interface.writeData(value);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::draw(uint8_t data)
{
    interface.writeData(data);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::draw(const uint8_t *data, size_t length)
{
    interface.writeData(data, length);
}

//--------------------------------------------------------------------------------------------------
void SSD1675a::submitImage(const uint8_t *image, size_t length)
{
    if (length == 0)
    {
        masterActivation();
        interface.waitUntilIdle();
    }
    else if ((length >> 24) & 0x1)
    {
        // Bit 24 is set -> black ram
        interface.writeCommand(command::WriteBWRam);
        draw(image, length & 0xFFFF);
    }
    else if ((length >> 26) & 0x1)
    {
        // Bit 26 is set -> red ram
        interface.writeCommand(command::WriteRedRam);
        draw(image, length & 0xFFFF);
    }
}