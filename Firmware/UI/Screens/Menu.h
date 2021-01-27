#ifndef MENU_H
#define MENU_H

#include <cstring>

#include "IMenu.h"
#include "../Painter/Painter.h"
#include "../Widgets/MenuItem.h"

#include "../Widgets/PopUpParameterMenu.h"
#include "../Widgets/CheckboxMenuItem.h"
#include "../Screens/ParameterListScreen.h"
#include "../Screens/NumericValueScreen.h"
#include "../Widgets/PopUpMenuItem.h"
#include "../Widgets/NumericMenuItem.h"
#include "../Widgets/ParameterListMenuItem.h"

#include "../ButtonDefinitions.h"

#include "../../GlobalSettings.h"

#include "../../../Bootloader/Periphery/USB/IUSBDevice.h"
#include "../../../Bootloader/Periphery/ILI9341/ILI9341Device.h"

#include "../../CentralDB.h"

//#include <Helpers.h>

class Menu : public IMenu
{
    // this array of pointers to menuItems allows a menu of up to 64 entries to be filled by inherited class
    MenuItem* _menuItem[64] = {nullptr};

  protected:
    CentralDB* _db;

    const char* _label;
    const char* _menuBreadcrumbs;

    uint8_t _menuItemsCount;
    uint8_t _menuSelectionIndex;

    uint8_t _maxVisibleItems;

    uint8_t _parameterSelectionIndex;
    uint8_t _parameterMenuActive;
    uint8_t _menuOffset;

    // Color Defintions
    uint16_t _menuBackgroundColor;
    uint16_t _menuItemColor;
    uint16_t _menuDimmedItemColor;
    uint16_t _menuDisabledItemColor;
    uint16_t _menuSelectedItemColor;
    uint16_t _menuHightlightedItemColor;
    uint16_t _menuTextColor;
    uint16_t _menuDisabledTextColor;
    uint16_t _menuSelectedTextColor;

    PopUpParameterMenu _popUpParameterMenu;
    int8_t _popUpParameterMenuActive;

    ParameterListScreen _parameterListScreen;
    int8_t _parameterListMenuActive;

    NumericValueScreen _numericValueScreen;
    int8_t _numericValueMenuActive;
    bool _selectionButtonBeingPressed;

  protected:
    void Draw(IPainter* painter) override;

    void DrawHeader(IPainter* painter);

    void DrawMenuItems(IPainter* painter);

    void DrawScrollIndicator(IPainter* painter);

    void UnselectAllMenuItems();

    void UnhighlightAllMenuItems();

    void Update(Button button, int8_t knob, IMenuSystem* menuSystem) override;

    void SelectionIncrease(IMenuSystem* menuSystem);

    void SelectionDecrease(IMenuSystem* menuSystem);

    void SelectionPressDown(IMenuSystem* menuSystem);

    void SelectionPress(IMenuSystem* menuSystem);

  public:
    // TODO: Add assignment of menu system to IMenu
    explicit Menu(IUSBDevice* cdcDevice, CentralDB* centralDB);

    void AddMenuItem(MenuItem* newMenuItem);

    void SetMenuItem(uint8_t index, MenuItem* newMenuItem);

    void SetLabel(const char* value);

    const char* GetLabel();

    int8_t GetMenuItemsCount();
};

#endif // MENU_H
