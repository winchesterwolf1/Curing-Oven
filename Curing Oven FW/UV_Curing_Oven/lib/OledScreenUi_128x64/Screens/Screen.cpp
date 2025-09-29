#include "Screen.h"
#include <string.h>

/// @brief Constructor for the screen class
/// @param oled pointer to the pointer saved in the child class
/// @param bannerText text to display at the top of the screen
Screen::Screen(U8G2** oled, char* bannerText)
{
    _oled = oled;
    _bannerTextPtr = bannerText;
}

/// @brief Draw the header banner with banner text
void Screen::DrawBanner()
{
    // Draw the banner text centered on the top of the screen with a border underneath it.
    (*_oled)->setFont(u8g2_font_etl16thai_t);
    (*_oled)->drawStr((((*_oled)->getDisplayWidth()-(*_oled)->getStrWidth(_bannerTextPtr))/2),(*_oled)->getFontAscent(),_bannerTextPtr);
    (*_oled)->drawBox(0,BANNER_HEIGHT-BANNER_BORDER_THICKNESS,(*_oled)->getDisplayWidth(), BANNER_BORDER_THICKNESS);
}

/// @brief to be implemented by child class
void Screen::DrawData()
{
    // Not implemented here
}

/// @brief return the value stored in the OLED
int Screen::GetFontAscent()
{
    return (*_oled)->getFontAscent();
}

/// @brief return the value stored in the OLED
int Screen::GetFontDescent()
{
    return (*_oled)->getFontDescent();
}

/// @brief Draw the screen starting with the Banner, and then using the child implementation of the DrawData method
void Screen::DrawScreen()
{
    if((*_oled) != nullptr)
    {
        (*_oled)->firstPage();
        do {
            DrawBanner();
            DrawData();
        } while ( (*_oled)->nextPage() );
    }
}

/// @brief to be implemented by child class
void Screen::OnCwRotation()
{
    // Not implemented here
}

/// @brief to be implemented by child class
void Screen::OnAcwRotation()
{
    // Not implemented here
}

/// @brief to be implemented by child class
void Screen::OnButtonClick()
{
    // Not implemented here
}

