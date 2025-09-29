#include <U8g2lib.h>
#ifndef SCREEN_H
#define SCREEN_H

/*** Macros to aid in drawing screens ***/
// Macro used to get the item height used by child classes
#define FONT_HEIGHT (Screen::GetFontAscent() - Screen::GetFontDescent())
// Number of pixels which make up the banner
#define BANNER_BORDER_THICKNESS 1
// Number of pixels between the drawn decent of the banner and the border
#define BANNER_PADDING 1
// Calculated banner height based on given parameters
#define BANNER_HEIGHT (Screen::GetFontAscent() - Screen::GetFontDescent() + BANNER_BORDER_THICKNESS + BANNER_PADDING)

// The limit of the number of characters able to be displayed on the screen (font specific)
#define SCREEN_CHARACTER_LIMIT 13


class Screen
{
    private:
        /// @brief Pointer to banner text to allow for changable banners
        char* _bannerTextPtr;

        /// @brief Private Method used to draw the screen banner
        void DrawBanner();

        /// @brief pointer to the OLED object in the child class to draw to.
        U8G2** _oled;

    protected:

        /// @brief Method used by child classes to draw the screen data
        virtual void DrawData();

        /// @brief Get font ascent for aid in drawing UI
        /// @return number of pixels above the datum the font gets drawn including the datum
        int GetFontAscent();

        /// @brief Get font decent for aid in drawing UI
        /// @return number of pixels below the datum the font gets drawn
        int GetFontDescent();

    public:
        /// @brief Constructor for a screen page
        /// @param bannerText Pointer to banner text to be placed at the top of the screen
        Screen(U8G2** oled, char* bannerTextPtr);

        /// @brief Draw the OLED screen over SPI
        void DrawScreen();

        /// @brief method to call when a CW rotation occurs while on this screen
        virtual void OnCwRotation();

        /// @brief method to call when an ACW rotation occurs while on this screen
        virtual void OnAcwRotation();

        /// @brief method to call when a button click occurs while on this screen
        virtual void OnButtonClick();
};

#endif /*SCREEN_H*/