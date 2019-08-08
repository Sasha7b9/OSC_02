#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Object.h"


#define PAGE_IS_MAIN                   (name == Page::Name::Main)
#define MENU_TEMP_ENABLE_STRING_NAVI() Menu::TemporaryEnableStrNavi()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Menu
{
    /// �������������
    void Init();

    void ChangeStateFlashDrive();
    /// ������� ������ ���������� � ������� �����.
    void Update();
    /// ������� ���������� ��� �������, ������� � ���������� ������
    void ButtonEvent(KeyEvent event);
    /// ������������� ����������. ����� ��������� ������ ������ �� ������� keys, ���������������� ����
    void LockKeyboard(Key *keys);
    /// �������������� ����������
    void UnlockKeyboard();
    /// ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    void SetAutoHide(bool active);
    /// ��������/������ ����.
    void Show(bool show);
    /// ���������� true, ���� ���� ������������ �� ������
    bool IsShown();
    /// ������� �������� �������� ����������� ������ ��������� ����, ���� ������ �����������.
    void TemporaryEnableStrNavi();
    /// ���������� ����� ��������� �������� ����
    Control *OpenedItem();
    /// ���������� ��������� �� ������� ������� �������� ����
    PageBase *MainPage();
    /// ���� true - ���� ��������� � �������������� ������.
    bool IsMinimize();

    Control *ItemUnderButton(Key::E button);

    void SetItemUnderButton(Key::E button, Control *control);

    Page::Name::E GetNameOpenedPage();

    void SaveScreenToDrive();

    void SaveRow(int row);

    PageBase *OpenedPage();
    /// ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    Control *CurrentItem();
    /// ������� �������� ������� ����
    void CloseOpenedItem();

    void SetItemForHint(void *item);

    void SaveSettings();
    /// ����� �������� ��� ���������� �������������� ������
    void ReleaseFunctionalButton(Key::E key);
    /// ����� �������� ��� "�������" ������� �������������� ������
    void LongFunctionalButton(Key::E key);
    ///\brief  ����� �������� ����� �������� ����, ���������������� �������������� ������� [1..5], ���� ��� ��������� � ������ ���������, � 0, 
    /// ���� �� ���� ������ �� ������.
    extern Control *itemUnderKey;
    /// ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    extern const char *stringForHint;
    /// Item, ��������� ��� �������� ����� �������� � ������ ����������� ������ ���������.
    extern Control *itemHint;

    /// ���������� ����
    void Draw();
    /// � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    int Y();

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

    void SetMainPage(PageBase *page);
};
