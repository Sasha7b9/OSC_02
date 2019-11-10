#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


/// ���������� ������� ��������� ������ �� �������� Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu.posActItem[name])


struct SettingsMenu
{
    bool    show;                               ///< ���� true, �� ����� ���������� ������� ��������
    int8    posActItem[PageName::Count];        ///< \brief ������� ��������� ������. bit7 == 1 - item is opened, 0x7f - ��� ��������� ������.
    int8    currentSubPage[PageName::Count];    ///< ����� ������� �����������.
};



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
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Item *CurrentItem();

    static Page *OpenedPage();

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
};
