#pragma once

#include "SSD1305.hpp"
#include "SSDInterface.hpp"

/// Display driver interface for the SSD1306 OLED controller IC.
/// It is compatible to SSD1305, only the charge pump is added.
class SSD1306 : public SSD1305
{
public:
    explicit SSD1306(SSDInterface &interface) : SSD1305(interface){};

    void setChargePump(bool enable);
};