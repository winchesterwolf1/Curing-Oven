#include "Menu.h"

/*** Macros to aid in drawing buttons ***/
// Horizontal Padding on each button
#define ITM_PADDING_H 5
// Vertical patting for each button
#define ITM_PADDING_V 2
// Calculated Item Height from given parameters
#define ITM_HEIGHT (FONT_HEIGHT + ITM_PADDING_V * 2 + 1)
// Calculated First button Y value calculated from given parameters
#define FIRST_BUTTON_Y (BANNER_HEIGHT + Screen::GetFontAscent() + ITM_PADDING_V)

// Max number of items on the screen
#define MAX_ITEMS_ON_SCREEN 3

// The style of button when item is selected
#define SELECTED_ITEM_STYLE (U8G2_BTN_INV | U8G2_BTN_BW1)
// The style of button when item is not selected
#define DEFAULT_ITEM_STYLE (U8G2_BTN_BW1)
// Macro function used to which button is slected while drawing the menu buttons
#define GET_ITEM_STYLE(index) ((_viewStartIndex + index)==_selectedIndex ? SELECTED_ITEM_STYLE : DEFAULT_ITEM_STYLE)


/// @brief Constructor. Simply initialise base class
Menu::Menu() : Screen(&_oled, _bannerText)
{}

/// @brief Sets the info and references on the screen
void Menu::Set(U8G2* oled, const char* bannerText, unsigned int numMenuItems, MenuItem* menuItems)
{
    _oled = oled;
    SetBannerText(bannerText);
    _menuItems = menuItems;
    _numMenuItems = numMenuItems;
    ResetSelection();
}

/// @brief Data on the screen is designed as the value of the output parameter in the centre with an OK button at the bottom of the screen
void Menu::DrawData()
{
    //Menu Options
    for(int i = 0; i < MAX_ITEMS_ON_SCREEN; i++)
    {
        _oled->drawButtonUTF8(ITM_PADDING_H, FIRST_BUTTON_Y + ITM_HEIGHT*i, 
            GET_ITEM_STYLE(i), _oled->getDisplayWidth()-(ITM_PADDING_H*2),  
            ITM_PADDING_H,  ITM_PADDING_V, _menuItems[_viewStartIndex+i].ItemText );
    }
}

/// @brief update indicies to 0
void Menu::ResetSelection()
{
    _selectedIndex = 0;
    _viewStartIndex = 0;
}

/// @brief copy new banner text into internal refernece
void Menu::SetBannerText(const char* bannerText)
{
    strncpy(_bannerText, bannerText, SCREEN_CHARACTER_LIMIT);
}

/// @brief increment selected index, and move visibile index if required
void Menu::OnCwRotation()
{
    if(_selectedIndex < (_numMenuItems-1))
    {
        _selectedIndex++;
    }

    if(_selectedIndex >= MAX_ITEMS_ON_SCREEN+_viewStartIndex && (_viewStartIndex+MAX_ITEMS_ON_SCREEN)<(_numMenuItems))
    {
        _viewStartIndex++;
    }
}

/// @brief decrement selected index, and move visible index if required
void Menu::OnAcwRotation()
{
    if(_selectedIndex > 0)
    {
        _selectedIndex--;
    }

    if(_selectedIndex < _viewStartIndex && _viewStartIndex>0)
    {
        _viewStartIndex--;
    } 
}

/// @brief run selected buttons action
void Menu::OnButtonClick()
{
    _menuItems[_selectedIndex].executeAction();
}
