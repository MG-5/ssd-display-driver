#pragma once

#include <array>
#include <cstddef>
#include <cstdint>

/// Font that contains glyph for rendering strings.
class Font
{
public:
    static constexpr size_t NumberOfGlyphs = 256;  //! Number of glyphs stored in every Font
    static constexpr size_t MaximumGlyphWidth = 5; //! Maximum glyph width in pixels

public:
    /// Visual representation of a single character.
    struct Glyph
    {
        /// Definines the effective width of the glyph.
        ///
        /// For non-monospaced fonts, the glyph can vary from character to
        /// character. The width defines how many bytes (columns) from the
        /// data array are to be drawn.
        /// Must be smaller than MaximumGlyphWidth.
        const uint8_t width;

        /// Byte array containing the glyph pixel data.
        ///
        /// Each glyph is 8 pixels tall.
        /// and up to MaximumGlyphWidth pixels wide. Each byte represents 8 pixels in
        /// a column; the least significant bit is the top row pixel, the most
        /// significant the bottom row pixel.
        uint8_t data[MaximumGlyphWidth]{0};
    };

    /// Array of NumberOfGlyphs Glyph pointers.
    ///
    /// Each entry must point to a valid Glyph object; nullptr is not allowed.
    /// If a glyph for a character should not be provided by the font, it should
    /// instead point to a "missing character" glyph.
    using GlyphTable = std::array<Glyph *, NumberOfGlyphs>;

    /// Creates a new font object.
    /// \param tab Reference to a GlyphTable object containing the glyph pointers
    explicit Font(const GlyphTable &tab) : glyphs{tab}
    {
    }

    [[nodiscard]] Glyph &getGlyph(uint8_t character) const
    {
        return *glyphs[character];
    }

private:
    const GlyphTable &glyphs;
};
