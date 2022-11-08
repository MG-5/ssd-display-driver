#pragma once

#include "SSDInterface.hpp"
#include "display-renderer/IRenderTarget.hpp"

/// Display driver interface for the SSD1305 OLED controller IC.
class SSD1305 : public IRenderTarget
{
public:
    enum class AddressingMode
    {
        /// The column address pointer is automatically increased by 1.
        /// If the column address pointer reaches the column end address,
        /// it is reset to column start address and the page address pointer
        /// remains unchanged.
        Page = 0b10,

        /// The column address pointer is automatically increased by 1.
        /// If the column address pointer reaches the column end address,
        /// it is reset to the column start address and the page address
        /// pointer is increased by 1.
        Horizontal = 0b0,

        /// The page address pointer is automatically increased by 1.
        /// If the page address pointer reaches the page end address,
        /// it is reset to the page start address and the column address
        /// pointer is increased by 1.
        Vertical = 0b1
    };

    enum class DisplayState
    {
        On,     //!< Display is enabled on full brightness.
        Dimmed, //!< Display has the brightness and contrast settings of the dimmed state.
        Off     //!< Display is disabled.
    };

    enum class ColorMode
    {
        Monochrome, //!< The entire displays shows monochrome pixels only.
        AreaColor   //!< Some areas of the display can be colored individually.
    };

    enum class PowerMode
    {
        Normal,  //!< Normal power consumption.
        LowPower //!< Reduced power consumption.
    };

    /// COM output direction mode
    enum class ComMode
    {
        Normal, //!< Scan from COM0 to COM[N-1], where N is the multiplex ratio.
        Remap   //!< Scan from COM[N-1] to COM0, where N is the multiplex ratio.
    };

    enum class VcomhLevel
    {
        x0_43, //!< 0.43 * Vcc
        x0_77, //!< 0.77 * Vcc
        x0_83  //!< 0.83 * Vcc
    };

    explicit SSD1305(SSDInterface &interface) : di(interface){};

    void setColumnStartAddress(uint8_t addr);

    void setMemoryAddressingMode(AddressingMode mode);

    /// Sets column start and end address and sets the column address pointer to the column start
    /// address.
    /// \param addrStart Column start address, from 0 to 131. Is 0 on reset.
    /// \param addrEnd   Column end address, from 0 to 131. Is 131 on reset.
    void setColumnAddress(uint8_t addrStart, uint8_t addrEnd);

    /// Sets page start and end address and sets the page address pointer to the page start address.
    /// \param addrStart Page start address, from 0 o 7. Is 0 on reset.
    /// \param addrEnd   Page end address, from 0 to 7. Is 7 on reset.
    void setPageAddress(uint8_t addrStart, uint8_t addrEnd);

    /// Sets the first line of the VRAM to be displayed.
    ///
    /// This function defines the mapping of the VRAM rows to the display lines.
    /// If \p line equals n, VRAM row n is mapped to COM0.
    /// \param line The index of the row that is mapped to COM0, from 0 to 63.
    void setDisplayStartLine(uint8_t line);

    /// Sets the contrast of the display (BANK0) in 256 steps from 0x0 to 0xff.
    ///
    /// The segment output current increases as the \p contrast value increased.
    /// Formula: Iseg = \p contrast/256 * Iref * scale
    /// with Iref = 10 uA and scale = 32
    /// \param contrast The display contrast, from 0 to 255.
    void setContrastControl(uint8_t contrast);

    /// Set the brightness of the display for the area color banks.
    ///
    /// The segment output current increases as the \p brightness value increases. Does not affect
    /// the contrast of BANK0, which is set using setContrastControl(uint8_t).
    /// \param brightness Display brightness, from 0 to 255.
    void setBrightness(uint8_t brightness);

    /// Sets the color intensities (current drive pulse width) for Colors A, B, C and BANK0.
    /// Color D is fixed to 64 DCLKs.
    /// \param bank0  BANK0 color intensity, from 32 to 64 DCLKs.
    /// \param colorA Color A intensity, from 32 to 64 DCLKs.
    /// \param colorB Color B intensity, from 32 to 64 DCLKs.
    /// \param colorC Color C intensity, from 32 to 64 DCLKs.
    void setLUT(uint8_t bank0, uint8_t colorA, uint8_t colorB, uint8_t colorC);

    // TODO: bank color

    /// Sets the segment remapping.
    /// The default mapping is column address 0 to SEG0. In remapping (inverted)
    /// mode, column address 131 is mapped to SEG0.
    /// \param remap True if the segment mapping is inverted, false otherwise.
    void setSegmentRemap(bool remap);

    /// Indicates wether the display shows VRAM content or is entirely ON.
    /// \param on If true, the entire display is ON, regardless of the VRAM content.
    void setEntireDisplayOn(bool on);

    void setInverseDisplay(bool inverse);

    /// Sets the multiplex ration.
    /// \param ratio Multiplex ratio, from 16 to 63 (default).
    void setMultiplexRatio(uint8_t ratio);

    /// Configures brightness and contrast for the display in dim mode.
    /// Refer to setContrastControl(uint8_t) and setBrightness(uint8_t) for details.
    /// \param contrast   Contrast for BANK0, from 0 to 255.
    /// \param brightness Brightness for color bank, from 0 to 255.
    void setDimMode(uint8_t contrast, uint8_t brightness);

    void setDisplayState(DisplayState state);

    /// Sets the page start address in page addressing mode.
    /// \param addr Page start address, from 0 to 7.
    void setPageStartAddress(uint8_t addr);

    /// Sets the scan direction of COM output.
    void setComOutputMode(ComMode mode);

    /// Specifies the mapping of the display start line to one of COM0 to COM63.
    /// \param offset See SSD1305 datasheet for details.
    void setDisplayOffset(uint8_t offset);

    /// Sets display clock divide ratio and oscillator frequency.
    /// \param divideRatio Divide ratio to generate DLCK (display clock) from CLK.
    /// Valid values in the range of 1 (reset value) to 16.
    /// \param fOsc Oscillator frequency that is source of CLK
    /// if CLS is pulled high. For details see SSD1305 datasheet.
    void setDisplayClockDivide(uint8_t divideRatio, uint8_t fOsc);

    /// Enables/disables area color mode and power saving.
    void setAreaColorModeAndPowerMode(ColorMode color, PowerMode power);

    /// Sets the duration of the pre-charching period, counted in numbers of DLCKS (2 on reset).
    /// \param phase1 Phase 1 period, from 1 to 15 DLCKs.
    /// \param phase2 Phase 2 period, from 1 to 15 DLCKs.
    void setPrechargingPeriod(uint8_t phase1, uint8_t phase2);

    /// Set the Com Pin Config object
    /// \param alternative    Alternative COM pin configuration if true, sequential otherwise.
    /// \param leftRightRemap Remap left/right if true.
    void setComPinConfig(bool alternative, bool leftRightRemap);

    /// Adjusts the Vcomh regulator output.
    void setVcomhDeselectLevel(VcomhLevel level);

    /// Enters the read write modify mode.
    /// In RWM mode, the RAM address pointer will not be increased when reading data.
    /// Writing will automatically increase the RAM address pointer by one.
    void enterReadWriteModify();

    /// Leaves read write modify mode.
    /// When leaving RWM mode, the RAM address pointer returns back to the original
    /// location before the call to enterReadWriteModify().
    void exitReadWriteModify();

    /// No operation command.
    void nop();

    /// Only for SSD1306
    void setChargePump(bool enable);

    // TODO: Scrolling

    /// Draws 8 data bits onto the display.
    ///
    /// The 8 monochrome pixels are drawn vertically at the current position of the current page.
    /// The LSB will be the top pixel, the MSB the bottom pixel.
    /// The current position will be updated afterwads according to the selected addressing mode.
    /// \param[in] data Byte representing the 8 pixels to be drawn.
    void draw(uint8_t data);

    /// Draws data bytes onto the display.
    ///
    /// For each byte 8 monochrome pixels are drawn vertically at the current position
    /// of the current page.
    /// The LSB will be the top pixel, the MSB the bottom pixel.
    /// The current position will be updated afterwads according to the selected addressing mode.
    /// \param[in] data   Byte representing the pixels to be drawn.
    /// \param[in] length Length of the data (number of pixels) to be drawn.
    void draw(const uint8_t *data, size_t length);

    void submitImage(const uint8_t *image, size_t length) override;

    void resetColumnStartAddress();
    void resetPageStartAddress();

protected:
    SSDInterface &di;

    uint8_t columnStartAddress;
    uint8_t pageStartAddress;
};