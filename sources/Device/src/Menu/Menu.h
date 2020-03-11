#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


struct SettingsMenu
{
    bool        show;                               // ���� true, �� ����� ���������� ������� ��������
    int8        posActItem[PageName::Count];        // \brief ������� ��������� ������. bit7 == 1 - item is opened, 0x7f - ��� ��������� ������.
    int8        currentSubPage[PageName::Count];    // ����� ������� �����������.
    PageName::E mainPage;                           // ��� ������� ������� ��������
};



struct Menu
{
    struct Position
    {
        // ���������� ������� ��������� ������ �� �������� name
        static int8 &ActItem(PageName::E name);
    };

    static void Init();

    static void ChangeStateFlashDrive();

    static void Update();

    static void Draw();
    
    // ���������� ����� ��������������� �������� ���� � ������������ � �����������.
    static void SetAutoHide(bool active);
    
    static void Show();

    static void Hide();

    static bool IsShown();

    static Item *OpenedItem();
    
    // ���������� ����� �������� �������� ���� (�������, ��� �������, �������� �������, ������ �������� ���� ������ ���������
    static Item *CurrentItem();

    static Page *OpenedPage();

    static void CloseOpenedItem();

    static void SetItemForHint(const Item *item);

    // � ������ ������� Y �������� ����. ������� ������ ��� ����� ���������
    static int Y0();

    static const Item *ItemUnderFunctionalKey(Key::E key);

    // ���������� ��������� �� �������� � ������ name
    static Page *PageFromName(PageName::E name);

    // ������������� ������� ������� ��������
    static void SetMainPage(const Page *page);
   
    struct Title
    {
        static const int HEIGHT = 9;
        static const int WIDTH = 320 / 5 * 2 - 1;
    };

private:

    // ���������� ��������� �� ������� ������� ��������
    static const Page *GetMainPage();
    
    // ��������� �������� ������� �� ������ �������� page
    static Item *LastOpened(Page *page);
    
    // ��������� ������� ������� ��������������� �������� ����
    static void OnTimerAutoHide();

    static void ProcessButtonForHint(Key::E button);
    
    // �������� ���������
    static void DrawHint();
    
    // ������� ��� ��������, ������� �� ����� ���� ��������� ��� ���������
    static void CloseAllBadOpenedPages();
    
    // ������� parent, ���� �� �������� ���������� page
    static void CloseIfSubPage(Page *parent, Page *page);

    static void ClosePage(Page *page);

    static void DrawHintItem(int x, int y, int width);
    
    // ����, ��� �������� ����� �������� ���������
    static Item *itemHint;
      
    // ������ ���������, ������� ���� �������� � ������ ����������� ������ ���������.
    static const char *stringForHint;
};
