#include "SSD1675a.hpp"

class SSD1680 : public SSD1675a
{
    // hiding these functions by making them private
    // due SSD1680 does not use them
private:
    using SSD1675a::setAnalogBlock;
    using SSD1675a::setDigitalBlock;
    using SSD1675a::setDummyLinePeriod;
    using SSD1675a::setGateLineWidth;

public:
    explicit SSD1680(SSDInterface &interface) : SSD1675a(interface){};

    void loadLut() override;
};