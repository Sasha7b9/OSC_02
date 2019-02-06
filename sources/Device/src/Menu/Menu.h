#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @defgroup Menu
 *  @{
 *  @defgroup Pages
 *  @{
 *  @}
 */

#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Menu
{
friend class Page;

public:
    /// �������������
    static void Init();

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void Update();
    /// ������� ���������� ��� �������, ������� � ���������� ������
    static void ButtonEvent(KeyEvent event);
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ��������/������ ����.
    static void Show(bool show);
    /// ���������� true, ���� ���� ������������ �� ������
    static bool IsShown();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi();
    /// ���������� ����� ��������� �������� ����
    static Control *OpenedItem();
    /// ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();

    static Control *ItemUnderButton(Key::E button);

    static void SetItemUnderButton(Key::E button, Control *control);

    static Page::Name::E GetNameOpenedPage();

    static PageBase *OpenedPage();
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Control *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();

    static void SetItemForHint(void *item);

    static void SaveSettings();
    /// ����� �������� ��� ���������� �������������� ������
    static void ReleaseFunctionalButton(Key::E key);
    /// ����� �������� ��� "�������" ������� �������������� ������
    static void LongFunctionalButton(Key::E key);
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static Control *itemUnderKey;
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    static Control *itemHint;

    /// ���������� ����
    static void Draw();
    /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    static int Y();

private:
    /// ��������� �������� ������� �� ������ �������� page
    static Control *LastOpened(Page *page);
    /// ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();
    /// �������, ������� �������� ����� ������ ��������� ����
    static void OnTimerStrNaviAutoHide();
    
    static void ProcessButtonForHint(Key::E button);
    
    static void ResetItemsUnderButton();
    /// ���������� true, ���� ������ ������ ������������� � ������ ������
    static bool IsProcessed(const KeyEvent *event);
    /// ��� ������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� �������� ������� ������� ������.
    static Key::E pressButton;
    /// ��� ���������� ������ � ��� ������������ � ��� ���������� � �������� ��� �� ��������� ������� ���������� ������.
    static Key::E releaseButton;
    /// ��� ������� ����� �������� ���� ��� ����� Menu::Update().
    static pFuncVV funcAterUpdate;
    /// ����� ���������� ������� ������. ����� ��� ����, ����� ������������ ��������� ���������
    static uint timeLastPressedButton;
    /// ��� ������� ����� ���������� ������ ����
    static pFuncVV funcUpdate;

public:

    class Title
    {
    public:
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
    };

    class Item
    {
    public:
        static const int HEIGHT = 23;

        class Value
        {
        public:
            static const int HEIGHT = 13;
            static const int WIDTH = 320 / 5 - 4; // -V112
        };
    };

    /// ����� ����� ����� ����� ���������� ������� ������ �������� ����.
    struct AutoHide
    {
        enum E
        {
            None = 0,   ///< �������.
            _5 = 5,   ///< ����� 5 ������.
            _10 = 10,  ///< ����� 10 ������.
            _15 = 15,  ///< ����� 15 ������.
            _30 = 30,  ///< ����� 30 ������.
            _60 = 60   ///< ����� 60 ������.
        } value;
    };
};


/** @}
 */
