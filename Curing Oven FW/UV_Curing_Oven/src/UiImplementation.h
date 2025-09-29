#include "Ui.h"
#include "Definitions.h"

#ifndef UI_IMPLEMENTATION_H
#define UI_IMPLEMENTATION_H

/// @brief Crete Ui objects and initialise Ui to start
/// @param selectedSettings pointer to the settings struct to be edited by the menu
/// @param systemFunctions pointer to an array of functions to call from the menu
/// @return Ui pointer to be used to update.
Ui* CreateUi(SelectedSettings* selectedSettings, void(**systemFunctions)());

/// @brief React to user imputs and draw the screen 
/// @attention Run as often as possible
/// @param uiObj screen object to update
void UiUpdate(Ui* uiObj);

#endif /*UI_IMPLEMENTATION_H*/