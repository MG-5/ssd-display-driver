#include "oled-driver/SSD1306.hpp"

namespace command
{
constexpr auto ChargePumpSetting = 0x8D;
}

//--------------------------------------------------------------------------------------------------
void SSD1306::setChargePump(bool enable)
{
    di.writeCommand(command::ChargePumpSetting);
    di.writeCommand(enable ? 0x14 : 0x10);
}