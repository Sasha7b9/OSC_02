#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include <cstdio>


extern const Page pageInfo;

const Page * const PageService::PageInformation::self = (const Page *)&pageInfo;

using namespace Display::Primitives;

static void Information_Draw()
{
    Painter::BeginScene(Color::BACK);
    int x = 100;
    int dY = 20;
    int y = 20;
    Rectangle(319, 239).Draw(0, 0, Color::FILL);
    y += dY;
    String("����������").Draw(x, y);
    y += dY;

    char buffer[100];

    std::sprintf(buffer, "%s : %s", "������", MODEL_RU);
    String(buffer).Draw(x, y);

    y += 2 * dY;

    String("����������� ����������� :").Draw(x, y);
    y += dY;

    std::sprintf(buffer, "������ %s", NUM_VER);

    String(buffer).Draw(x, y);
    y += dY;

    //Painter::DrawFormText(x, y, Color::FILL, "CRC32 : %X", Hardware::CalculateCRC32());

    dY = -10;

    Text("��� ��������� ������ ������� � ����������� ������ ������").DrawInCenterRect(0, 190 + dY, 320, 20);
    Text("����� ����������: ���./����. 8-017-262-57-50").DrawInCenterRect(0, 205 + dY, 320, 20);
    Text("������������: e-mail: mnipi-24(@)tut.by, ���. 8-017-262-57-51").DrawInCenterRect(0, 220 + dY, 320, 20);

    Menu::Draw();
    Painter::EndScene();
}


static void OnPress_Close()
{
    Menu::CloseOpenedItem();
}

DEF_BUTTON( bClose,
    "�������",
    "������� �������� ����������",
    &PageService::PageInformation::self, Item::Active, OnPress_Close
)

static void OnOpenClose_Page(bool enter)
{
    if (enter)
    {
        Display::SetAddDrawFunction(Information_Draw);
        static const Key::E keys[2] = { Key::Enter, Key::None };
        Keyboard::Lock(keys);
    }
    else
    {
        Display::RemoveAddDrawFunction();
        Keyboard::Unlock();
    }
}

DEF_PAGE_1 ( pageInfo, //-V641 //-V1027
    "����������",
    "���������� �� �������",
    &bClose,
    PageName::Service_Info, &PageService::self, Item::Active, OnOpenClose_Page, Page::BeforeDraw, Page::Handler
)
