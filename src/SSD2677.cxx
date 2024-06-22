#include "ssd-display-driver/SSD2677.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include <array>

//--------------------------------------------------------------------------
namespace ssd2677_lut
{
constexpr auto LutSize = 60;

constexpr std::array<uint8_t, lutSize> LutVcom = {
    0x0, 0xF, 0xF, 0x0, 0x0, 0x1, 0x0, 0xF, 0x1, 0xF, 0x1, 0x2, 0x0, 0xF,
    0xF, 0x0, 0x0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
    0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0,
};

constexpr std::array<uint8_t, lutSize> LutWW = {
    0x10, 0xF, 0xF, 0x0, 0x0, 0x1, 0x84, 0xF, 0x1, 0xF, 0x1, 0x2, 0x20, 0xF,
    0xF,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
};

constexpr std::array<uint8_t, lutSize> LutBW = {
    0x10, 0xF, 0xF, 0x0, 0x0, 0x1, 0x84, 0xF, 0x1, 0xF, 0x1, 0x2, 0x20, 0xF,
    0xF,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
};

constexpr std::array<uint8_t, lutSize> LutWB = {
    0x80, 0xF, 0xF, 0x0, 0x0, 0x3, 0x84, 0xF, 0x1, 0xF, 0x1, 0x4, 0x40, 0xF,
    0xF,  0x0, 0x0, 0x3, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
};

constexpr std::array<uint8_t, lutSize> LutBB = {
    0x80, 0xF, 0xF, 0x0, 0x0, 0x1, 0x84, 0xF, 0x1, 0xF, 0x1, 0x2, 0x40, 0xF,
    0xF,  0x0, 0x0, 0x1, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
    0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0, 0x0, 0x0, 0x0, 0x0, 0x0,  0x0,
};
} // namespace ssd2677_lut

//--------------------------------------------------------------------------
namespace command
{
constexpr auto PanelSetting = 0x00;
constexpr auto PowerSetting = 0x01;
constexpr auto PowerOff = 0x02;
constexpr auto PowerOn = 0x04;
constexpr auto BoosterSoftStart = 0x06;
constexpr auto DeepSleep = 0x07;
constexpr auto DataStartTransmission1 = 0x10;
constexpr auto DisplayRefresh = 0x12;
constexpr auto DataStartTransmission2 = 0x13;
constexpr auto DualSpi = 0x15;
constexpr auto LutVcom = 0x20;
constexpr auto LutWW = 0x21;
constexpr auto LutBW = 0x22;
constexpr auto LutWB = 0x23;
constexpr auto LutBB = 0x24;
constexpr auto VcomDataInterval = 0x50;
constexpr auto TconSetting = 0x60;
constexpr auto ResolutionSetting = 0x61;
constexpr auto GateSourceStart = 0x65;
constexpr auto VcomDcSetting = 0x82;

} // namespace command

//--------------------------------------------------------------------------
void SSD2677::init()
{
    setPower();
    setVcomDc();
    setBooster();
    powerOn();
    setPanel();
    setResolution();
    setVcomDataInterval();
    setGateSourceNonOverlapPeriod();
    setGateSourceStart();
    setLUT();
}

//--------------------------------------------------------------------------
void SSD2677::setPower()
{
    interface.writeCommand(command::PowerSetting);
    interface.writeData(0x17); // enable border LDO, internal DC/DC
    interface.writeData(0x17); // VCOM fast slew rate, VGH=12V, VGL= -12V
    interface.writeData(0x3F); // VDH= 15V
    interface.writeData(0x3F); //  VDL= -15V
    interface.writeData(0x11); //  VDHR = 5.8V
}

//--------------------------------------------------------------------------
void SSD2677::setVcomDc()
{
    interface.writeCommand(command::VcomDcSetting);
    interface.writeData(0x24); // VDCS= -1.1V
}

//--------------------------------------------------------------------------
void SSD2677::setBooster()
{
    interface.writeCommand(command::BoosterSoftStart);
    interface.writeData(0x27); // phase A: 10ms soft start period ,driving strength 5, 6.58µs
                               // minimum off time of GDR
    interface.writeData(0x27); // phase B: same as A
    interface.writeData(0x2F); // phase C1: driving strength 6, 6.58µs minimum off time of GDR
    interface.writeData(0x17); // phase C2: driving strength 3, 6.58µs minimum off time of GDR
}
//--------------------------------------------------------------------------
void SSD2677::setPanel()
{
    interface.writeCommand(command::PanelSetting);
    interface.writeData(0x3F); // LUT from Register, Pixel with Black/White,
                               // Scan up mode, Shift right, Booster on
}

//--------------------------------------------------------------------------
void SSD2677::setResolution()
{
    interface.writeCommand(command::ResolutionSetting);
    interface.writeData(0x03); // HRES
    interface.writeData(0x20); // HRES -> 800 horizontal
    interface.writeData(0x01); // VRES
    interface.writeData(0xE0); // VRES -> 480 vertical
}

//--------------------------------------------------------------------------
void SSD2677::setVcomDataInterval()
{
    interface.writeCommand(command::VcomDataInterval);
    interface.writeData(0x10); // LUTKW
    interface.writeData(0x00); // VCOM and Data Interval 17 hsync
}

//--------------------------------------------------------------------------
void SSD2677::setGateSourceNonOverlapPeriod()
{
    interface.writeCommand(command::TconSetting);
    interface.writeData(0x22); // Source To Gate= 20, Gate to Source= 12
}

//--------------------------------------------------------------------------
void SSD2677::setGateSourceStart()
{
    interface.writeCommand(command::GateSourceStart);
    interface.writeData(0x00); // Horizontal Display Start Positio
    interface.writeData(0x00);
    interface.writeData(0x00); // Vertical Display Start Position
    interface.writeData(0x00);
}

//--------------------------------------------------------------------------
void SSD2677::setLUT()
{
    interface.writeCommand(command::LutVcom);
    interface.writeData(reinterpret_cast<const uint8_t *>(LutVcom.data()), LutSize);

    interface.writeCommand(command::LutWW);
    interface.writeData(reinterpret_cast<const uint8_t *>(LutWW.data()), LutSize);

    interface.writeCommand(command::LutBW);
    interface.writeData(reinterpret_cast<const uint8_t *>(LutBW.data()), LutSize);

    interface.writeCommand(command::LutBB);
    interface.writeData(reinterpret_cast<const uint8_t *>(LutBB.data()), LutSize);
}

//--------------------------------------------------------------------------
void SSD2677::powerOn()
{
    interface.writeCommand(command::PowerOn);
    interface.delay(1);
    interface.waitUntilIdle();
}