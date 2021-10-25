/// \brief Monospace font definition
/// \note Uses CP-1252 https://en.wikipedia.org/wiki/Windows-1252

#include "oled-driver/Font.hpp"

// ***** glyphs *****
// clang-format off
Font::Glyph missingGlyph = {5, {0x7f, 0x7f, 0x7f, 0x7f, 0x7f}};

// digits
Font::Glyph dg[] =
{
    {5, {0x3e, 0x51, 0x49, 0x45, 0x3e}},    // 0
    {5, {0x00, 0x42, 0x7f, 0x40, 0x00}},    // 1
    {5, {0x42, 0x61, 0x51, 0x49, 0x46}},    // 2
    {5, {0x21, 0x41, 0x45, 0x4b, 0x31}},    // 3
    {5, {0x18, 0x14, 0x12, 0x7f, 0x10}},    // 4
    {5, {0x2f, 0x49, 0x49, 0x49, 0x31}},    // 5
    {5, {0x3c, 0x4a, 0x49, 0x49, 0x30}},    // 6
    {5, {0x01, 0x71, 0x09, 0x05, 0x03}},    // 7
    {5, {0x36, 0x49, 0x49, 0x49, 0x36}},    // 8
    {5, {0x06, 0x49, 0x49, 0x29, 0x1e}}     // 9
};

// lowercase letters
Font::Glyph lc[] =
{
    {5, {0x20, 0x54, 0x54, 0x54, 0x78}},    // a
    {5, {0x7f, 0x50, 0x48, 0x48, 0x30}},    // b
    {5, {0x38, 0x44, 0x44, 0x44, 0x20}},    // c
    {5, {0x30, 0x48, 0x48, 0x50, 0x7f}},    // d
    {5, {0x38, 0x54, 0x54, 0x54, 0x18}},    // e
    {5, {0x08, 0x7e, 0x09, 0x01, 0x02}},    // f
    {5, {0x0c, 0x52, 0x52, 0x52, 0x3e}},    // g
    {5, {0x7f, 0x08, 0x04, 0x04, 0x78}},    // h
    {5, {0x00, 0x44, 0x7d, 0x40, 0x00}},    // i
    {5, {0x20, 0x40, 0x44, 0x3d, 0x00}},    // j
    {5, {0x00, 0x7f, 0x10, 0x28, 0x44}},    // k
    {5, {0x00, 0x41, 0x7f, 0x40, 0x00}},    // l
    {5, {0x7c, 0x04, 0x18, 0x04, 0x78}},    // m
    {5, {0x7c, 0x04, 0x04, 0x04, 0x78}},    // n
    {5, {0x38, 0x44, 0x44, 0x44, 0x38}},    // o
    {5, {0x7c, 0x14, 0x14, 0x14, 0x08}},    // p
    {5, {0x08, 0x14, 0x14, 0x18, 0x7c}},    // q
    {5, {0x7c, 0x08, 0x04, 0x04, 0x08}},    // r
    {5, {0x48, 0x54, 0x54, 0x54, 0x20}},    // s
    {5, {0x02, 0x3f, 0x42, 0x40, 0x20}},    // t
    {5, {0x3c, 0x40, 0x40, 0x20, 0x7c}},    // u
    {5, {0x1c, 0x20, 0x40, 0x20, 0x1c}},    // v
    {5, {0x3c, 0x40, 0x20, 0x40, 0x3c}},    // w
    {5, {0x44, 0x28, 0x10, 0x28, 0x44}},    // x
    {5, {0x0c, 0x50, 0x50, 0x50, 0x3c}},    // y
    {5, {0x44, 0x64, 0x54, 0x4c, 0x44}}     // z
};

// Umlaute lowercase
Font::Glyph ullc[] =
{
    {5, {0x20, 0x55, 0x54, 0x55, 0x78}},    // ä
    {5, {0x38, 0x45, 0x44, 0x45, 0x38}},    // ö
    {5, {0x3c, 0x41, 0x40, 0x21, 0x7c}}     // ü
};

// uppercase letters
Font::Glyph uc[] =
{
    {5, {0x7e, 0x11, 0x11, 0x11, 0x7e}},    // A
    {5, {0x7f, 0x49, 0x49, 0x49, 0x36}},    // B
    {5, {0x3e, 0x41, 0x41, 0x41, 0x22}},    // C
    {5, {0x7f, 0x41, 0x41, 0x22, 0x1c}},    // D
    {5, {0x7f, 0x49, 0x49, 0x49, 0x41}},    // E
    {5, {0x7f, 0x09, 0x09, 0x09, 0x01}},    // F
    {5, {0x3e, 0x41, 0x49, 0x49, 0x7a}},    // G
    {5, {0x7f, 0x08, 0x08, 0x08, 0x7f}},    // H
    {5, {0x00, 0x41, 0x7f, 0x41, 0x00}},    // I
    {5, {0x20, 0x40, 0x41, 0x3f, 0x01}},    // J
    {5, {0x7f, 0x08, 0x14, 0x22, 0x41}},    // K
    {5, {0x7f, 0x40, 0x40, 0x40, 0x40}},    // L
    {5, {0x7f, 0x02, 0x0c, 0x02, 0x7f}},    // M
    {5, {0x7f, 0x04, 0x08, 0x10, 0x7f}},    // N
    {5, {0x3e, 0x41, 0x41, 0x41, 0x3e}},    // O
    {5, {0x7f, 0x09, 0x09, 0x09, 0x06}},    // P
    {5, {0x3e, 0x41, 0x51, 0x21, 0x5e}},    // Q
    {5, {0x7f, 0x09, 0x19, 0x29, 0x46}},    // R
    {5, {0x46, 0x49, 0x49, 0x49, 0x31}},    // S
    {5, {0x01, 0x01, 0x7f, 0x01, 0x01}},    // T
    {5, {0x3f, 0x40, 0x40, 0x40, 0x3f}},    // U
    {5, {0x1f, 0x20, 0x40, 0x20, 0x1f}},    // V
    {5, {0x3f, 0x40, 0x38, 0x40, 0x3f}},    // W
    {5, {0x63, 0x14, 0x08, 0x14, 0x63}},    // X
    {5, {0x07, 0x08, 0x70, 0x08, 0x07}},    // Y
    {5, {0x61, 0x51, 0x49, 0x45, 0x43}}     // Z
};

// Umlaute uppercase
Font::Glyph uluc[] =
{
    {5, {0x78, 0x15, 0x14, 0x15, 0x78}},    // Ä
    {5, {0x38, 0x45, 0x44, 0x45, 0x38}},    // Ö
    {5, {0x3c, 0x41, 0x40, 0x41, 0x3c}}     // Ü
};

// special characters
Font::Glyph sc[] =
{
    {5, {0x00, 0x00, 0x00, 0x00, 0x00}},    // 032_space
    {5, {0x00, 0x00, 0x4f, 0x00, 0x00}},    // 033_excl
    {5, {0x00, 0x07, 0x00, 0x07, 0x00}},    // 034_quot
    {5, {0x14, 0x7f, 0x14, 0x7f, 0x14}},    // 035_hash
    {5, {0x24, 0x2a, 0x7f, 0x2a, 0x12}},    // 036_dsig
    {5, {0x23, 0x13, 0x08, 0x64, 0x62}},    // 037_perc
    {5, {0x36, 0x49, 0x55, 0x22, 0x50}},    // 038_amp
    {5, {0x00, 0x05, 0x03, 0x00, 0x00}},    // 039_apos
    {5, {0x00, 0x1c, 0x22, 0x41, 0x00}},    // 040_rbrl
    {5, {0x00, 0x41, 0x22, 0x1c, 0x00}},    // 041_rbrr
    {5, {0x14, 0x08, 0x3e, 0x08, 0x14}},    // 042_star
    {5, {0x08, 0x08, 0x3e, 0x08, 0x08}},    // 043_plus
    {5, {0x00, 0x50, 0x30, 0x00, 0x00}},    // 044_comm
    {5, {0x08, 0x08, 0x08, 0x08, 0x08}},    // 045_min
    {5, {0x00, 0x60, 0x60, 0x00, 0x00}},    // 046_dot
    {5, {0x20, 0x10, 0x08, 0x04, 0x02}},    // 047_slsh
    {5, {0x00, 0x36, 0x36, 0x00, 0x00}},    // 058_coln
    {5, {0x00, 0x56, 0x36, 0x00, 0x00}},    // 059_smcl
    {5, {0x08, 0x14, 0x22, 0x41, 0x00}},    // 060_abrl
    {5, {0x14, 0x14, 0x14, 0x14, 0x14}},    // 061_equ
    {5, {0x41, 0x22, 0x14, 0x08, 0x00}},    // 062_abrr
    {5, {0x02, 0x01, 0x51, 0x09, 0x06}},    // 063_quest
    {5, {0x32, 0x49, 0x79, 0x41, 0x3e}},    // 064_at
    {5, {0x00, 0x7f, 0x41, 0x41, 0x00}},    // 091_sbrl
    {5, {0x00, 0x41, 0x41, 0x7f, 0x00}},    // 093_sbrr
    {5, {0x04, 0x02, 0x01, 0x02, 0x04}},    // 094_circ
    {5, {0x40, 0x40, 0x40, 0x40, 0x40}},    // 095_uscr
    {5, {0x00, 0x01, 0x02, 0x04, 0x00}},    // 096_acc
    {5, {0x00, 0x08, 0x36, 0x41, 0x00}},    // 123_cbrl
    {5, {0x00, 0x00, 0x7f, 0x00, 0x00}},    // 124_vert
    {5, {0x00, 0x41, 0x36, 0x08, 0x00}},    // 125_cbrr
    {5, {0x08, 0x1c, 0x2a, 0x08, 0x08}},    // arrowl
    {5, {0x08, 0x08, 0x2a, 0x1c, 0x08}},    // arrowr
    {5, {0x15, 0x16, 0x7c, 0x16, 0x15}},    // yen
    {5, {0xE, 0x11, 0x11, 0x11, 0xE}}       // degree symbol
};

// clang-format on

// ***** glyph table *****
// clang-format off
Font::GlyphTable gt =
{
    // control characters
    &missingGlyph,  // 0
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,  // 16
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    // special characters
    sc+0,           // 32
    sc+1,
    sc+2,
    sc+3,
    sc+4,
    sc+5,
    sc+6,
    sc+7,
    sc+8,
    sc+9,
    sc+10,
    sc+11,
    sc+12,
    sc+13,
    sc+14,
    sc+15,

    // digits
    dg+0,           // 48
    dg+1,
    dg+2,
    dg+3,
    dg+4,
    dg+5,
    dg+6,
    dg+7,
    dg+8,
    dg+9,

    // special characters
    sc+16,
    sc+17,
    sc+18,
    sc+19,
    sc+20,
    sc+21,
    sc+22,          // 64

    // uppercase letters
    uc+0,
    uc+1,
    uc+2,
    uc+3,
    uc+4,
    uc+5,
    uc+6,
    uc+7,
    uc+8,
    uc+9,
    uc+10,
    uc+11,
    uc+12,
    uc+13,
    uc+14,
    uc+15,          // 80
    uc+16,
    uc+17,
    uc+18,
    uc+19,
    uc+20,
    uc+21,
    uc+22,
    uc+23,
    uc+24,
    uc+25,

    // special characters
    sc+23,
    &missingGlyph,
    sc+24,
    sc+25,
    sc+26,
    sc+27,          // 96

    // lowercase letters
    lc+0,
    lc+1,
    lc+2,
    lc+3,
    lc+4,
    lc+5,
    lc+6,
    lc+7,
    lc+8,
    lc+9,
    lc+10,
    lc+11,
    lc+12,
    lc+13,
    lc+14,
    lc+15,          // 112
    lc+16,
    lc+17,
    lc+18,
    lc+19,
    lc+20,
    lc+21,
    lc+22,
    lc+23,
    lc+24,
    lc+25,

    // special characters
    sc+28,
    sc+29,
    sc+30,
    &missingGlyph,
    &missingGlyph,

    // ----- ASCII limit -----

    &missingGlyph,  // 128
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    &missingGlyph,  // 144
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    &missingGlyph,  // 160
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    sc+34,          // 176 - degree
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,  
    &missingGlyph,

    &missingGlyph, // 192
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    uluc+0,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    &missingGlyph,  // 208
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    uluc+1,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    uluc+2,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    &missingGlyph,  // 224
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    ullc+0,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,

    &missingGlyph,  // 240
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    ullc+1,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph,
    ullc+2,
    &missingGlyph,
    &missingGlyph,
    &missingGlyph
};
// clang-format on

// ***** font *****
Font fontMono(gt);
