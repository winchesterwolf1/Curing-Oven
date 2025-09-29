#include "Ui.h"
#include "Definitions.h"

#ifndef UI_IMPLEMENTATION_H
#define UI_IMPLEMENTATION_H

Ui* CreateUi(SelectedSettings* selectedSettings, void(**systemFunctions)());

void UiUpdate(Ui* uiObj);

#endif /*UI_IMPLEMENTATION_H*/