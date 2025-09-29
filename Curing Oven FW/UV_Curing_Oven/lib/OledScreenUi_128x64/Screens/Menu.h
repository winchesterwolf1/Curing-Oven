#include "Screen.h"
#include "MenuItem.h"
#ifndef MENU_H
#define MENU_H

/// @brief Class for drawing a menu and handling scrolling through the menu and selecting an item using the encoder knob
class Menu : public Screen
{
    private:
        /// @brief Text to display at the top of the screen
        char _bannerText[SCREEN_CHARACTER_LIMIT + 1] {0};

        /// @brief pointer to OLED screen to draw
        U8G2* _oled = nullptr;

        /// @brief Number of menu items stored in this menu
        unsigned int _numMenuItems;

        /// @brief Index of the currently selected menu item
        unsigned int _selectedIndex = 0;

        /// @brief Itemx of the top menu item visible in the screen
        unsigned int _viewStartIndex = 0;

        /// @brief A pointer to the list of menu items that make up this menu
        MenuItem* _menuItems;

        /// @brief Implementation of the draw data method ran by the base screen class when drawing the screen
        void DrawData();
        
    public:
        /// @brief Constructor
        Menu();

        /// @brief Sets the screen properties to draw
        /// @param oled oled to draw to 
        /// @param bannerText banner text of this screen
        /// @param numMenuItems number of menu items in this menu
        /// @param menuItems pointer to a list of menu items in this menu
        void Set(U8G2* oled, const char* bannerText, unsigned int numMenuItems, MenuItem* menuItems);

        /// @brief Resets the selected indicies of the menu
        void ResetSelection();

        /// @brief Sets the banner text at the top of the screen
        void SetBannerText(const char* bannerText);

        /// @brief method to call when a CW rotation occurs while on this screen
        void OnCwRotation();

        /// @brief method to call when an ACW rotation occurs while on this screen
        void OnAcwRotation();

        /// @brief method to call when a button click occurs while on this screen
        void OnButtonClick();
};


#endif /*MENU_H*/
