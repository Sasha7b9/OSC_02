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
    /// ������� ���������� ��� �������, ������� � ���������� ������
    static void ButtonEvent(KeyEvent event);
    /// ������������� ����������. ����� ��������� ������ ������ �� ������� keys, ���������������� ����
    static void LockKeyboard(const Key::E * const keys);
    /// �������������� ����������
    static void UnlockKeyboard();
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
    /// ���������� ��������� �� ������� ������� �������� ����
    static Page *MainPage();
    /// ���� true - ���� ��������� � �������������� ������.
    static bool IsMinimize();

    static const Control *ItemUnderButton(Key::E button);

    static void SetItemUnderButton(Key::E button, const Control *control);

    static Page::Name::E GetNameOpenedPage();

    static void SaveScreenToDrive();

    static void SaveRow(int row);

    static Page *OpenedPage();
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
    /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    static int Y();

    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    static const Control *itemUnderKey;
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    static Control *itemHint;

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

    static void SetMainPage(Page *page);
};
