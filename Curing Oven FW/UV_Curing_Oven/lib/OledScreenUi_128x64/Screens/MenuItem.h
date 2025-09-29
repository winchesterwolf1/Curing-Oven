#include "Screen.h"
#include "UiCallbackWrapper.h"
#ifndef MENU_ITEM_H
#define MENU_ITEM_H

/// @brief Class for an item in a Ui menu
class MenuItem
{
    public:
        /// @brief text to display on the item button
        char ItemText[SCREEN_CHARACTER_LIMIT + 1] {0};

        /// @brief Action to run if the item is selected
        UiCallback _action = nullptr;

        /// @brief context for the action to run.
        void* _actionObject = nullptr;

        /// @brief Default constructor
        /// @attention If using this constructor you must initialise this object using the Set method
        MenuItem()
        {}

        /// @brief Constructor for this class
        /// @param itemText text to display
        /// @param action action to run if item is selected
        /// @param actionObject context for the action to run in
        MenuItem(const char* itemText, UiCallback action, void* actionObject)
        {
            Set(itemText, action, actionObject);
        }

        /// @brief Set the values of the menu item to allow for a default construction
        /// @param itemText text to display
        /// @param action action to run if item is selected
        /// @param actionObject context for the action to run in
        void Set(const char* itemText, UiCallback action, void* actionObject)
        {
            strncpy(ItemText, itemText, SCREEN_CHARACTER_LIMIT);
            ItemText[SCREEN_CHARACTER_LIMIT] = '\0';
            _action = action;
            _actionObject = actionObject;
        }

        /// @brief Execute action if values have been setup correctly
        void executeAction()
        {
            if (_action != nullptr && _actionObject != nullptr) {
                _action(_actionObject);
            }
        }
};

#endif /*MENU_ITEM_H*/
