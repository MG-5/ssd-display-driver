#pragma once

#include "Font.hpp"
#include "IRenderTarget.hpp"
#include "Image.hpp"
#include "fonts/dfi.hpp"

#include <cstdio>

class DualRenderer;

/// Draws visual elements onto a screen buffer.
class Renderer
{
public:
    static constexpr char CtrlCarriageReturn = '\r';
    static constexpr char CtrlLineFeed = '\n';

    /// A position on the screen buffer with the upper left corner as the origin.
    struct Position
    {
        size_t x = 0; //!< The X coordinate, in pixels.
        size_t y = 0; //!< The Y coordinate, in pages (8 pixels).
    };

    /// Represents an area on the screen.
    struct Area
    {
        Position pos;
        size_t width = 0;  //!< The width of the area, in pixels.
        size_t height = 0; //!< The height of the area, in pages (8 pixels).
    };

    /// Alignment of visual elements.
    enum class Alignment
    {
        Left,
        Center,
        Right
    };

    /// Creates a new Renderer instance.
    /// \param screenWidth  in pixels.
    /// \param screenPages height in pages (8 pixels).
    /// \param target Reference to an IRenderTarget object receiving the rendered image.
    Renderer(size_t screenWidth, size_t screenPages, IRenderTarget &target)
        : ScreenWidth(screenWidth), ScreenPages(screenPages), target(target)
    {
        vram = new uint8_t[VramSize];
    }

    ~Renderer()
    {
        delete[] vram;
    }

    /// Clears the entire screen buffer.
    void clearAll();

    /// Clears an entire screen buffer page.
    void clearPage(size_t page);

    /// Clears multiple pages at once.
    /// Each bit in the pageMask corresponds to a screen buffer page, i.e.
    /// the lowest bit (bit 0) references page 0, the next one (bit 1) references page 1 and so on.
    /// If a bit is set, the corresponding page will be erased.
    /// \param pageMask Bitmask defining which pages to erase.
    void clearPages(uint32_t pageMask);

    /// Clears an area of the screen buffer.
    /// \param area Reference to an Area object describing the area to be cleared.
    void clearArea(const Area &area);

    /// Submits the image to the render target.
    void render();

    void invertPage(size_t page);

    /// Partly inverts a single page. (column in pixels)
    void invertPage(size_t page, size_t fromColumn, size_t toColumn);

    void invertPages(size_t fromPage, size_t toPage);

    /// Partly inverts pages. (column in pixels)
    void invertPages(size_t fromPage, size_t toPage, size_t fromColumn, size_t toColumn);

    /// Draws a horizontal line spanning the full screen width.
    /// \param row Row number inside the page the line is to be drawn to, from 0 to 7.
    void drawHorizontalLine(size_t page, size_t row);

    /// Draws a horizontal line.
    /// \param row Row number inside the page the line is to be drawn to.
    void drawHorizontalLine(size_t page, size_t row, size_t fromColumn, size_t toColumn);

    /// Draws a vertical line.
    /// \param fromRow   Row the line starts inside pageStart.
    /// \param toRow     Row the line ends inside pageEnd.
    void drawVerticalLine(size_t column, size_t pageStart, size_t pageEnd, size_t fromRow = 0,
                          size_t toRow = 7);

    /// Draws an image onto the screen buffer.
    void drawImage(const Position &screenPosition, const Image &image,
                   Alignment align = Alignment::Left);

    /// Sets the font to be used for following text draw methods.
    /// \param font Reference to a Font object.
    void setFont(const Font &font);

    /// Puts a single character onto the screen buffer.
    /// \param characterScale Size multiplicator, valid from 1 to 4. Default is 1.
    /// \return Width of the drawn character, in pixels.
    size_t put(const Position &screenPosition, char character, int characterScale = 1);

    /// Writes a string onto the screen buffer.
    /// This method pays attention to carriage return "\\r" and line feed characters "\\n".
    /// \param textScale Size multiplicator, valid from 1 to 4. Default is 1.
    /// \return Width of the string written, in pixels.
    size_t print(const Position &screenPosition, const char *string,
                 Alignment align = Alignment::Left, int textScale = 1);

    /// Determines the line length of a string in pixels and return it.
    /// Stops at the first line feed character found. Also pays attention to carriage return.
    /// \param textScale Size multiplicator, valid from 1 to 4. Default is 1.
    size_t getLineWidth(const char *string, int textScale = 1);

    const size_t ScreenWidth = 0;
    const size_t ScreenPages = 0;
    static constexpr size_t PixelPerPage = 8;
    const size_t VramSize = ScreenWidth * ScreenPages;

protected:
    /// Computes the correct X coordinate for differenct string alignments.
    /// \param x     The original X coordinate.
    /// \param align Alignment method.
    /// \param width Width of the string, in pixels.
    /// \return The adjusted X coordinate.
    size_t getAdjustedPosition(size_t x, Alignment align, size_t width);

    /// Reference to the target receiving the rendered image
    IRenderTarget &target;

    /// Video RAM containing the screen pixels
    uint8_t *vram = nullptr;

    /// Font used for text rendering. Can be set using setFont().
    const Font *font = &fontDfi;

    friend DualRenderer;
};