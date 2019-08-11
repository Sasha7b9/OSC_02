#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Menu
{
    /// �������������
    static void Init();

    static void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    static void Update();
    /// ���������� ����
    static void Draw();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    /// ��������/������ ����.
    static void Show(bool show);
    /// ���������� true, ���� ���� ������������ �� ������
    static bool IsShown();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    static void TemporaryEnableStrNavi();
    /// ���������� ����� ��������� �������� ����
    static Item *OpenedItem();

    static Page::Name::E GetNameOpenedPage();

    static void SaveScreenToDrive();

    static void SaveRow(int row);

    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Item *CurrentItem();
    /// ������� �������� ������� ����
    static void CloseOpenedItem();

    static void SetItemForHint(const Item *item);

    static void SaveSettings();
    /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    static int Y0();
    /// ���������� ����� ��������, ���������������� �������������� �������
    static const Item *ItemForFuncKey(Key::E key);
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    static Item *itemHint;
    /// ������� ������� ��������
    static const Page *mainPage;

    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 2;
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
