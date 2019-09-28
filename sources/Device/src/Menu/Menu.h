#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Menu
{
    static void Init();

    static void ChangeStateFlashDrive();

    static void ProcessingAllKeyboardEvents();

    static void Draw();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    
    static void Show();

    static void Hide();

    static bool IsShown();

    static Item *OpenedItem();

    static PageName::E GetNameOpenedPage();
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Item *CurrentItem();

    static void CloseOpenedItem();

    static void SetItemForHint(const Item *item);

    static void SaveSettings();
    /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    static int Y0();

    static const Item *ItemUnderFunctionalKey(Key::E key);
    /// ������� ������� ��������
    static const Page *mainPage;

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 1;
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
