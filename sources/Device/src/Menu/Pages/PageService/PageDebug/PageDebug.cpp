#include "defines.h"
#include "Data/Reader.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/ROM.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageService.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


BalanceADC &BalanceADC::Ref()
{
    return set.dbg.nrst.balanceADC;
}


/// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
typedef struct
{
    int number;     ///< ��������������, ���������� �����.
    int year;       ///< ��������������, ���.
    int curDigt;    ///< ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
} StructForSN;


DEF_CHOICE_2( cStats,                                                                                                                                          //--- ������� - ���������� ---
    "����������",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������"
    ,
    "�� ����������",
    "����������",
    set.dbg.ShowStats, &PageDebug::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


static int IncreaseY(int &y, int dY)
{
    y += dY;
    return y;
}


static void DebugShowSetInfo_Draw()
{
    Painter::BeginScene(Color::BACK);
    Rectangle(319, 239).Draw(0, 0, Color::FILL);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define INC_Y IncreaseY(y, dY)
#define DRAW_TEXT(str)                  Painter::DrawText(x0, INC_Y, str)
#define DRAW_FORMAT(str, value)         String(str, value).Draw(x0, INC_Y)
#define DRAW_FORMAT2(str, val1, val2)   String(str, val1, val2).Draw(x0, INC_Y)

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "������ �������� ��������� %d", sizeof(set));
    DRAW_FORMAT("������ �������� ��������� : %d", sizeof(set)); //-V2528
    String("�������������� ���������:").Draw(x0, INC_Y); //-V2528

    int x = String("rShiftAdd :").Draw(x0, INC_Y) + 5; //-V2528

    int ddY = 0;

    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < Range::Count; ++range)
        {
            String("%d", ShiftADC::Value(static_cast<Chan::E>(ch), static_cast<Range::E>(range))).Draw(x + range * 20, y + dY * ddY);
        }
        ddY++;
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", set.dbg.nrst.correctionTime); //-V2528
    DRAW_FORMAT2("balanceADC : %d %d", BalanceADC::Value(Chan::A), BalanceADC::Value(Chan::B)); //-V2528
    DRAW_FORMAT("numAveForRand : %d", set.dbg.nrst.numAveForRand); //-V2528

    pString s[3] = {"���������", "��������� �������������", "������ �������"};
    DRAW_FORMAT("balanceADCtype : %s", (BalanceADC::Ref().value < 3 ? s[BalanceADC::Ref().value] : "!!! ������������ �������� !!!")); //-V547 //-V2528
    DRAW_FORMAT("stretchADCtype : %s", (StretchADC::Type() < 3 ? s[StretchADC::Type()] : "!!! ������������ �������� !!!")); //-V547 //-V2528

    x = String("stretchADC :").Draw(x0, INC_Y) + 5; //-V2528

    for (int ch = 0; ch < 2; ch++)
    {
        String("%d", set.dbg.nrst.stretchADC[ch]).Draw(x, y + dY * ch);
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    DRAW_FORMAT("numSmoothForRand : %d", set.dbg.nrst.numSmoothForRand); //-V2528

    Menu::Draw();
    Painter::EndScene();
}


static void OnOpenClose_Settings(bool)
{
    Display::SetDrawMode(DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_5( pSettings,                                                                                                                         //--- ������� - ��������� ---
    "���������",
    "�������� ���������� � ����������",
    0,
    0,
    0,
    0,
    0,
    PageName::Debug_Settings,
    &PageDebug::self, Item::Active, Page::NormalTitle, OnOpenClose_Settings, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::PageSettings::self = static_cast<const Page *>(&pSettings);



static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::Message::Show("�������� ��������", false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite(NAME_FILE_FIRMWARE, &structForWrite);

    uint8 *address = reinterpret_cast<uint8 *>(0x08020000); //-V566
    uint8 *endAddress = address + SIZE_FILE_FIRMWARE; //-V566

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::Message::Hide();

    Display::ShowWarning("�������� ���������");
}

DEF_BUTTON( bSaveFirmware,                                                                                                                                 //--- ������� - ����. �������� ---
    "����. ��������",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    &PageDebug::self, IsActive_SaveFirmware, OnPress_SaveFirmware
)


static void OnPress_SerialNumber_Change()
{
    Color::ChangeFlash(true);
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Char(SymbolUGO2::TAB).Draw4SymbolsInRect(x + 2, y + 2);
}

DEF_GRAPH_BUTTON( bSerialNumber_Change,                                                                                                                    //--- ������� - �/� - �������� ---
    "��������",
    "��������� �������� ������",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
)


static void OnPress_SerialNumber_Save()
{
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Char(SymbolUGO2::SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
}

DEF_GRAPH_BUTTON( bSerialNumber_Save,                                                                                                                     //--- ������� - �/� - ��������� ---
    "���������",
    "���������� �������� ����� � OTP",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
)


static void Draw_EnterSerialNumber()
{
}

static void OnOpenClose_SerialNumber(bool)
{
    Draw_EnterSerialNumber();
}

static bool HandlerKey_SerialNumber(const KeyEvent &)
{
    return false;
}

DEF_PAGE_2( pSerialNumber,                                                                                                                                            //--- ������� - �/� ---
    "�/�",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    &bSerialNumber_Change,
    &bSerialNumber_Save,
    PageName::Debug_SerialNumber,
    &PageDebug::self, Item::Active, Page::NormalTitle, OnOpenClose_SerialNumber, Page::BeforeDraw, HandlerKey_SerialNumber
)

const Page * const PageDebug::PageSerialNumber::self = static_cast<const Page *>(&pSerialNumber);


DEF_PAGE_6( pDebug,                                                                                                                                                         //--- ������� ---
    "�������",
    "",
    PageDebug::PageConsole::self,
    PageDebug::PageADC::self,
    PageDebug::PageRand::self,
    &cStats,
    &bSaveFirmware,
    PageDebug::PageTests::self,
    PageName::Debug,
    &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageDebug::self = static_cast<const Page *>(&pDebug);
