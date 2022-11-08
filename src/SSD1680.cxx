#include "ssd-display-driver/SSD1680.hpp"
#include <array>

namespace command
{
constexpr auto WriteLUTRegister = 0x32;
}

// Waveforms

// 00 = VSS  =   0V
// 01 = VSH1 =  15V
// 10 = VSL  = -15V
// 11 = VSH2 =   5V

namespace ssd1680_lut
{
constexpr auto lutSize = 153;

// clang-format off

//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize> BlackWhite = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6     Phase 7     Phase 8     Phase 9    Phase 10    Phase 11
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00100010, 0b00010001, 0b00010000, 0b00000000, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut0 (black)
    0b00010001, 0b10001000, 0b10000000, 0b10000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut1 (white)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut2 (red)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut3 (red)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)

    // Duration               | Repeats
    // A   B  AB   C   D  CD  |
       4, 24,  0,  4, 22,  0,    1, // Phase 0
      10, 10,  0, 10, 10,  0,    2, // Phase 1
       0,  0,  0,  0,  0,  0,    0, // Phase 2
       0,  0,  0,  0,  0,  0,    0, // Phase 3
       0,  0,  0,  0,  0,  0,    0, // Phase 4
       0,  0,  0,  0,  0,  0,    0, // Phase 5
       0,  0,  0,  0,  0,  0,    0, // Phase 6
       0,  0,  0,  0,  0,  0,    0, // Phase 7
       0,  0,  0,  0,  0,  0,    0, // Phase 8
       0,  0,  0,  0,  0,  0,    0, // Phase 9
       0,  0,  0,  0,  0,  0,    0, // Phase 10
       0,  0,  0,  0,  0,  0,    0, // Phase 11

    //  Frame Rate (6 bytes) and Gate Scan Selection (3 bytes)
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0, 0, 0
};

//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize> Delta = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6     Phase 7     Phase 8     Phase 9    Phase 10    Phase 11
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut0 (none)
    0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut1 (b->w)
    0b01000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut2 (w->b)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut3 (none)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)

    // Duration               | Repeats
    // A   B  AB   C   D  CD  |
      24,  0,  0,  0,  0,  0,    1, // Phase 0
       0,  0,  0,  0,  0,  0,    0, // Phase 1
       0,  0,  0,  0,  0,  0,    0, // Phase 2
       0,  0,  0,  0,  0,  0,    0, // Phase 3
       0,  0,  0,  0,  0,  0,    0, // Phase 4
       0,  0,  0,  0,  0,  0,    0, // Phase 5
       0,  0,  0,  0,  0,  0,    0, // Phase 6
       0,  0,  0,  0,  0,  0,    0, // Phase 7
       0,  0,  0,  0,  0,  0,    0, // Phase 8
       0,  0,  0,  0,  0,  0,    0, // Phase 9
       0,  0,  0,  0,  0,  0,    0, // Phase 10
       0,  0,  0,  0,  0,  0,    0, // Phase 11

    //  Frame Rate (6 bytes) and Gate Scan Selection (3 bytes)
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0, 0, 0
};

//--------------------------------------------------------------------------------------------------
constexpr std::array<uint8_t,lutSize> Default = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6     Phase 7     Phase 8     Phase 9    Phase 10    Phase 11
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00100010, 0b00010001, 0b00010000, 0b00000000, 0b00010000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut0 (black)  
    0b00010001, 0b10001000, 0b10000000, 0b10000000, 0b10000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut1 (white)  
    0b01101010, 0b10011011, 0b10011011, 0b10011011, 0b10011011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut2 (red)    
    0b01101010, 0b10011011, 0b10011011, 0b10011011, 0b10011011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut3 (red)    
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut4 (vcom)   

    // Duration               | Repeats
    // A   B  AB   C   D  CD  |
       4, 24,  0,  4, 22,  0,    1, // Phase 0 
      10, 10,  0, 10, 10,  0,    2, // Phase 1 
       0,  0,  0,  0,  0,  0,    0, // Phase 2 
       0,  0,  0,  0,  0,  0,    0, // Phase 3 
       4,  4,  0,  8, 60,  0,    7, // Phase 4 
       0,  0,  0,  0,  0,  0,    0, // Phase 5 
       0,  0,  0,  0,  0,  0,    0, // Phase 6 
       0,  0,  0,  0,  0,  0,    0, // Phase 7 
       0,  0,  0,  0,  0,  0,    0, // Phase 8 
       0,  0,  0,  0,  0,  0,    0, // Phase 9 
       0,  0,  0,  0,  0,  0,    0, // Phase 10 
       0,  0,  0,  0,  0,  0,    0, // Phase 11 

    //  Frame Rate (6 bytes) and Gate Scan Selection (3 bytes) 
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0, 0, 0
};

constexpr std::array<uint8_t, lutSize> Red = {
    // Phase 0     Phase 1     Phase 2     Phase 3     Phase 4     Phase 5     Phase 6     Phase 7     Phase 8     Phase 9    Phase 10    Phase 11
    // A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D     A B C D
    0b00100001, 0b00010001, 0b00000000, 0b00010001, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut0 (black)
    0b00010010, 0b10001000, 0b00000000, 0b00100010, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // lut1 (white)
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // LUT2 - ignore
    0b00100001, 0b01000100, 0b10110111, 0b00110011, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // LUT3 - Red
    0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, // LUT4 - VCOM

    // Duration               | Repeats
    // A   B  AB   C   D  CD  |
       4, 11,  0,  4, 11,  0,    2, // 0 
       10, 10, 0, 10, 10,  0,    1, // 1 
       2,  14, 3,  1, 20,  2,    4, // 2      2,  8,  3,  1,  16 , 2,    4,
       4,  24, 0,  4, 24,  0,    1, // 3
       0,  0,  0,  0,  0,  0,    0, // 4
       0,  0,  0,  0,  0,  0,    0, // Phase 5 
       0,  0,  0,  0,  0,  0,    0, // Phase 6
       0,  0,  0,  0,  0,  0,    0, // Phase 7 
       0,  0,  0,  0,  0,  0,    0, // Phase 8 
       0,  0,  0,  0,  0,  0,    0, // Phase 9 
       0,  0,  0,  0,  0,  0,    0, // Phase 10 
       0,  0,  0,  0,  0,  0,    0, // Phase 11 

    //  Frame Rate (6 bytes) and Gate Scan Selection (3 bytes) 
    0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0, 0, 0
};
// clang-format on
} // namespace ssd1680_lut

//--------------------------------------------------------------------------------------------------
void SSD1680::loadLut()
{
    using namespace ssd1680_lut;

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
        interface.writeData(reinterpret_cast<const uint8_t *>(Red.data()), lutSize);
        break;

    case LutSelection::Default:
    default:
        interface.writeData(reinterpret_cast<const uint8_t *>(Default.data()), lutSize);
        break;
    }

    interface.waitUntilIdle();
}