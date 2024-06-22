#pragma once

#include "SSDInterface.hpp"
#include "display-renderer/IRenderTarget.hpp"

class SSD2677 : public IRenderTarget
{
public:
    explicit SSD2677(SSDInterface &interface) : interface(interface){};

    void init();

    void setPower();
    void setVcomDc();
    void setBooster();
    void setPanel();
    void setResolution();
    void setVcomDataInterval();
    void setGateSourceNonOverlapPeriod();
    void setGateSourceStart();
    void setLUT();
    void setDisplayData1();
    void setDisplayData2();

    void powerOff();
    void powerOn();
    void deepSleep();
    void displayRefresh();

    // maybe not needed
    void setDualSpi();

    void submitImage(const uint8_t *image, size_t length) override;

protected:
    SSDInterface &interface;
};
