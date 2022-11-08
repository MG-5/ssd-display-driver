#include "oled-driver/Renderer.hpp"
#include <algorithm>
#include <cstring>

//--------------------------------------------------------------------------------------------------
void Renderer::clearAll()
{
    std::memset(vram, 0, VramSize);
}

//--------------------------------------------------------------------------------------------------
void Renderer::clearPage(size_t page)
{
    if (page >= ScreenPages)
        return;

    std::memset(vram + page * ScreenWidth, 0, ScreenWidth);
}

//--------------------------------------------------------------------------------------------------
void Renderer::clearPages(uint32_t pageMask)
{
    for (size_t i = 0; i < ScreenPages; ++i)
    {
        if (pageMask & 0x1)
            clearPage(i);

        pageMask >>= 1;
    }
}

//--------------------------------------------------------------------------------------------------
void Renderer::clearArea(const Area &area)
{
    if (area.pos.x >= ScreenWidth || area.pos.y > ScreenPages)
        return;

    size_t actualWidth = std::min(area.width, ScreenWidth - area.pos.x);
    size_t actualHeight = std::min(area.height, ScreenPages - area.pos.y);

    for (size_t y = area.pos.y; y < area.pos.y + actualHeight; ++y)
    {
        for (size_t x = area.pos.x; x < area.pos.x + actualWidth; ++x)
            vram[y * ScreenWidth + x] = 0;
    }
}

//--------------------------------------------------------------------------------------------------
void Renderer::render()
{
    target.submitImage(vram, VramSize);
}

//--------------------------------------------------------------------------------------------------
void Renderer::invertPage(size_t page)
{
    if (page >= ScreenPages)
        return;

    for (size_t i = page * ScreenWidth; i < (page + 1) * ScreenWidth; ++i)
        vram[i] = ~vram[i];
}

//--------------------------------------------------------------------------------------------------
void Renderer::invertPage(size_t page, size_t fromColumn, size_t toColumn)
{
    if (page >= ScreenPages || fromColumn > toColumn)
        return;

    fromColumn = std::min(fromColumn, ScreenWidth - 1);
    toColumn = std::min(toColumn, ScreenWidth - 1);

    for (size_t i = page * ScreenWidth + fromColumn; i <= page * ScreenWidth + toColumn; ++i)
        vram[i] = ~vram[i];
}

//--------------------------------------------------------------------------------------------------
void Renderer::invertPages(size_t fromPage, size_t toPage, size_t fromColumn, size_t toColumn)
{
    if (fromPage >= ScreenPages || toPage >= ScreenPages || fromPage > toPage ||
        fromColumn > toColumn)
        return;

    for (auto i = fromPage; i <= toPage; i++)
        invertPage(i, fromColumn, toColumn);
}

//--------------------------------------------------------------------------------------------------
void Renderer::invertPages(size_t fromPage, size_t toPage)
{
    if (fromPage >= ScreenPages || toPage >= ScreenPages || fromPage > toPage)
        return;

    for (auto i = fromPage; i <= toPage; i++)
        invertPage(i);
}

//--------------------------------------------------------------------------------------------------
void Renderer::drawHorizontalLine(size_t page, size_t row)
{
    drawHorizontalLine(page, row, 0, ScreenWidth - 1);
}

//--------------------------------------------------------------------------------------------------
void Renderer::drawHorizontalLine(size_t page, size_t row, size_t fromColumn, size_t toColumn)
{
    if (page >= ScreenPages || fromColumn > toColumn || row >= 8)
        return;

    fromColumn = std::min(fromColumn, ScreenWidth - 1);
    toColumn = std::min(toColumn, ScreenWidth - 1);
    uint8_t pattern = 1 << row;

    for (size_t i = page * ScreenWidth + fromColumn; i < page * ScreenWidth + toColumn + 1; ++i)
        vram[i] |= pattern;
}

//--------------------------------------------------------------------------------------------------
void Renderer::drawVerticalLine(size_t column, size_t pageStart, size_t pageEnd, size_t fromRow,
                                size_t toRow)
{
    if (column >= ScreenWidth || pageStart > pageEnd)
        return;

    pageStart = std::min(pageStart, ScreenPages - 1);
    pageEnd = std::min(pageEnd, ScreenPages - 1);

    fromRow = std::min(fromRow, PixelPerPage - 1);
    toRow = std::min(toRow, PixelPerPage - 1);

    size_t regionIndex = (column >> 5) & 0b11;
    uint8_t pattern = 0;

    if (pageStart == pageEnd)
    {
        for (size_t i = fromRow; i <= toRow; i++)
            pattern |= 1 << i;

        vram[pageStart * ScreenWidth + column] = pattern;
        return;
    }

    for (size_t i = fromRow; i <= 7; i++)
        pattern |= 1 << i;

    vram[pageStart * ScreenWidth + column] = pattern;

    for (size_t i = pageStart + 1; i < pageEnd; ++i)
    {
        vram[i * ScreenWidth + column] = 0b11111111;
    }

    pattern = 0;
    for (size_t i = 0; i <= toRow; i++)
        pattern |= 1 << i;

    vram[pageEnd * ScreenWidth + column] = pattern;
}

//--------------------------------------------------------------------------------------------------
void Renderer::drawImage(const Renderer::Position &screenPosition, const Image &image,
                         Alignment align)
{
    Position currentPos{getAdjustedPosition(screenPosition.x, align, image.getWidth()),
                        screenPosition.y};

    size_t actualHeight =
        std::min(static_cast<size_t>(image.getHeight()), ScreenPages - currentPos.y);
    size_t actualWidth =
        std::min(static_cast<size_t>(image.getWidth()), ScreenWidth - currentPos.x);

    for (size_t y = 0; y < actualHeight; ++y)
    {
        for (size_t x = 0; x < actualWidth + 0; ++x)
        {
            const size_t NewX = currentPos.x + x;
            if (NewX >= ScreenWidth)
                continue;

            vram[(currentPos.y + y) * ScreenWidth + NewX] =
                image.getData()[y * image.getWidth() + x];
        }
    }
}

//--------------------------------------------------------------------------------------------------
void Renderer::setFont(const Font &font)
{
    this->font = &font;
}

//--------------------------------------------------------------------------------------------------
size_t Renderer::put(const Renderer::Position &screenPosition, char character, int characterScale)
{
    if (characterScale > 4 || characterScale < 0)
        return 0;

    auto &g = font->getGlyph(character);

    if (screenPosition.x >= ScreenWidth || screenPosition.y >= ScreenPages)
    {
        return characterScale * g.width;
    }

    if (characterScale == 1)
    {
        size_t actualWidth = std::min(static_cast<size_t>(g.width), ScreenWidth - screenPosition.x);

        for (size_t x = 0; x < actualWidth; ++x)
            vram[screenPosition.y * ScreenWidth + x + screenPosition.x] = g.data[x];

        return actualWidth;
    }

    size_t originPage = screenPosition.y;
    size_t originColumn = screenPosition.x;

    for (size_t i = 0; i < g.width; ++i)
    {
        uint8_t gdata = g.data[i];
        uint32_t bits = 0;

        // Expand data bits according to the scale
        for (size_t b = 0; b < 8; ++b)
        {
            for (int j = 0; j < characterScale; ++j)
            {
                bits <<= 1;
                bits |= (gdata & 0x80) ? 1 : 0;
            }

            gdata <<= 1;
        }

        for (int j = 0; j < characterScale; ++j)
        {
            uint32_t data = bits;

            for (int k = 0; k < characterScale; ++k)
            {
                if ((originPage + k) < ScreenPages &&
                    (originColumn + i * characterScale + j) < ScreenWidth)
                    vram[(originPage + k) * ScreenWidth + originColumn + i * characterScale + j] =
                        data & 0xff;

                data >>= 8;
            }
        }
    }

    return characterScale * g.width;
}

//--------------------------------------------------------------------------------------------------
size_t Renderer::print(const Renderer::Position &screenPosition, const char *string,
                       Alignment align, int textScale)
{
    auto width = getLineWidth(string, textScale);
    Position currentPos{getAdjustedPosition(screenPosition.x, align, width), screenPosition.y};
    auto width2 = width;

    char currentChar;
    while ((currentChar = *string++))
    {
        if (currentChar == CtrlLineFeed)
        {
            width = getLineWidth(string, textScale);
            currentPos.x = getAdjustedPosition(screenPosition.x, align, width);
            currentPos.y += textScale;
        }
        else if (currentChar == CtrlCarriageReturn)
        {
            currentPos.x = getAdjustedPosition(screenPosition.x, align, width);
        }
        else
        {
            if (currentChar == (char)195) // UTF-8 start byte
            {
                char nextChar = *string++;

                if (nextChar == (char)132)
                    currentChar = (char)196; // Ä

                else if (nextChar == (char)150)
                    currentChar = (char)214; // Ö

                else if (nextChar == (char)156)
                    currentChar = (char)220; // Ü

                else if (nextChar == (char)164)
                    currentChar = (char)228; // ä

                else if (nextChar == (char)169)
                    currentChar = (char)101; // é

                else if (nextChar == (char)182)
                    currentChar = (char)246; // ö

                else if (nextChar == (char)188)
                    currentChar = (char)252; // ü
            }

            currentPos.x += put(currentPos, currentChar, textScale);

            // Only insert space if there are more characters to follow
            char nextChar = *string;
            if (nextChar && nextChar != CtrlCarriageReturn && nextChar != CtrlLineFeed)
                currentPos.x += textScale;
        }
    }

    return std::max(width, width2);
}

//--------------------------------------------------------------------------------------------------
size_t Renderer::getLineWidth(const char *string, int textScale)
{
    size_t width = 0;
    size_t currentWidth = 0;
    char currentChar;

    while ((currentChar = *string++) && currentChar != CtrlLineFeed)
    {
        if (currentChar == CtrlCarriageReturn)
        {
            width = std::max(currentWidth, width);
            currentWidth = 0;
        }
        else
        {
            if (currentChar == (char)195) // UTF-8 start byte
            {
                char nextChar = *string++;

                if (nextChar == (char)132)
                    currentChar = (char)196; // Ä

                else if (nextChar == (char)150)
                    currentChar = (char)214; // Ö

                else if (nextChar == (char)156)
                    currentChar = (char)220; // Ü

                else if (nextChar == (char)164)
                    currentChar = (char)228; // ä

                else if (nextChar == (char)169)
                    currentChar = (char)101; // é

                else if (nextChar == (char)182)
                    currentChar = (char)246; // ö

                else if (nextChar == (char)188)
                    currentChar = (char)252; // ü
            }

            currentWidth += font->getGlyph(currentChar).width * textScale;

            // Only insert space if there are more characters to follow
            char nextChar = *string;
            if (nextChar && nextChar != CtrlCarriageReturn && nextChar != CtrlLineFeed)
                currentWidth += textScale;
        }
    }

    return std::max(currentWidth, width);
}

//--------------------------------------------------------------------------------------------------
size_t Renderer::getAdjustedPosition(size_t x, Alignment align, size_t width)
{
    switch (align)
    {
    case Alignment::Left:
        return x;

    case Alignment::Center:
        return x - width / 2;

    case Alignment::Right:
        return x - width;

    default:
        return 0;
    }
}