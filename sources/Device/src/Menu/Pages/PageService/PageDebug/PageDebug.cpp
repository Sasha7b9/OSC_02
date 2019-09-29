#include "defines.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Hardware/Memory.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageService.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


using namespace Display::Primitives;
using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// � ���� ��������� ����� ��������� ������ ��������� ������ ��� �������� �������� ppSerialNumer
typedef struct
{
    int number;     ///< ��������������, ���������� �����.
    int year;       ///< ��������������, ���.
    int curDigt;    ///< ��������������, ������� (0) ��� ����� (1) ��������� ����� ���������.
} StructForSN;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 shiftADCA;
static int16 shiftADCB;

static void Draw_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, (int8)NRST_BALANCE_ADC_A},
        {0, SET_BALANCE_ADC_B, (int8)NRST_BALANCE_ADC_B}
    };

    shiftADCA = shift[Chan::A][NRST_BALANCE_ADC_TYPE];
    shiftADCB = shift[Chan::B][NRST_BALANCE_ADC_TYPE];
}

static void OnChanged_Balance_Mode(bool)
{
    Draw_Balance_Mode(0, 0);
}

DEF_CHOICE_3( cBalance_Mode,                                                                                                                         //--- ������� - ��� - ������ - ����� ---
    "�����",
    "",
    DISABLE_RU,
    "��������",
    "������",
    NRST_BALANCE_ADC_TYPE,
    &PageDebug::PageADC::PageBalance::self, Item::Active, OnChanged_Balance_Mode, Draw_Balance_Mode
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ShiftAB()
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ShiftA()
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR( gShiftA,                                                                                                                          //--- ������� - ��� - ������ - �������� 1 ---
    "�������� 1",
    "",
    shiftADCA, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftA
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ShiftB()
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR( gShiftB,                                                                                                                          //--- ������� - ��� - ������ - �������� 2 ---
    "�������� 2",
    "",
    shiftADCB, -125, 125,
    &PageDebug::PageADC::PageBalance::self, IsActive_ShiftAB, OnChanged_ShiftB
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pBalance, // -V641 // -V1027                                                                                                                     //--- ������� - ��� - ������ ---
    "������",
    "",
    &cBalance_Mode,     ///< ������� - ��� - ������ - �����
    &gShiftA,           ///< ������� - ��� - ������ - �������� 1
    &gShiftB,           ///< ������� - ��� - ������ - �������� 2
    PageName::Debug_ADC_Balance,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageBalance::self = (const Page *)&pBalance;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 stretchA;
static int16 stretchB;

void PageDebug::PageADC::PageStretch::OnChanged_Mode(bool)
{
    if (NRST_STRETCH_ADC_TYPE_IS_DISABLE)
    {
        stretchA = NRST_STRETCH_ADC_A(StretchADC::Disable) = 0;
        stretchB = NRST_STRETCH_ADC_B(StretchADC::Disable) = 0;
    }
    else
    {
        stretchA = NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE);
        stretchB = NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE);
    }
}

DEF_CHOICE_3( cStretch_Mode,                                                                                                                   //--- ������� - ��� - �������� - ����� ---
    "�����",
    "",
    DISABLE_RU,
    "��������",
    "������",
    NRST_STRETCH_ADC_TYPE,
    &PageDebug::PageADC::PageStretch::self, Item::Active, PageDebug::PageADC::PageStretch::OnChanged_Mode, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_StretchAB()
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_Stretch_A()
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR( gStretch_A,                                                                                                                //--- ������� - ��� - �������� - �������� 1� ---
    "�������� 1�",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    stretchA, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_A
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Stretch_B()
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR( gStretch_B,                                                                                                                //--- ������� - ��� - �������� - �������� 2� ---
    "�������� 2�",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    stretchB, -10000, 10000,
    &PageDebug::PageADC::PageStretch::self, IsActive_StretchAB, OnChanged_Stretch_B
)

/*
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 1� ---
    "20��/1� 1�", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ������� - ��� - �������� - 50�� 1� ---
    "50�� 1�", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 1� ---
    "100��/5� 1�", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ������� - ��� - �������� - 2� 1� ---
    "2� 1�", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 2� ---
    "20��/1� 2�", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ������� - ��� - �������� - ��� ���� 50�� 2� ---
    "50�� 2�", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 2� ---
    "100��/5� 2�", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
_DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ������� - ��� - �������� - 2� 2� ---
    "2� 2�", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pStretch, 0, 0, FuncBeforeDraw
)
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static const ChoiceBase emptyChoice = {Item::Type::Choice, 0, false, Page::Name::NoPage, 0, 0, {}, 0, 0, 0, 0};

DEF_PAGE_3( pStretch, // -V641 // -V1027                                                                                                                   //--- ������� - ��� - �������� ---
    "��������",
    "������������� ����� � �������� �������� (��� ������� ������)",
    &cStretch_Mode,      ///< ������� - ��� - �������� - �����
    &gStretch_A,         ///< ������� - ��� - �������� - �������� 1�
    &gStretch_B,
    PageName::Debug_ADC_Stretch,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageStretch::self = (const Page *)&pStretch;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_ADC_Shift_Reset()
{
    for (int ch = 0; ch < 2; ch++)
    {
        for (int range = 0; range < 3; range++)
        {
            RSHIFT_ADD_STABLE(ch, range) = 0;
        }
    }
    RShift::Set(Chan::A, SET_RSHIFT_A);
    RShift::Set(Chan::B, SET_RSHIFT_B);
}

DEF_BUTTON( bShift_Reset,                                                                                                                       //-- ������� - ��� - ��� ���� - ����� ---
    "�����",
    "",
    &PageDebug::PageADC::PageShift::self, Item::Active, OnPress_ADC_Shift_Reset
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Shift_A()
{
    RShift::Set(Chan::A, SET_RSHIFT_A);
}

DEF_GOVERNOR( gShift_A2mV, //-V641                                                                                                        //--- ������� - ��� - ��� ���� - �� 1� 2�� ���� ---
    "�� 1� 2�� ����",
    "",
    RSHIFT_ADD_STABLE_A(Range::_2mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Shift_B()
{
    RShift::Set(Chan::B, SET_RSHIFT_B);
}

DEF_GOVERNOR( gShift_B2mV,                                                                                                                //--- ������� - ��� - ��� ���� - �� 2� 2�� ���� ---
    "�� 2� 2�� ����",
    "",
    RSHIFT_ADD_STABLE_B(Range::_2mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gShift_A5mV,                                                                                                                //--- ������� - ��� - ��� ���� - �� 1� 5�� ���� ---
    "�� 1� 5�� ����",
    "",
    RSHIFT_ADD_STABLE_A(Range::_5mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gShift_B5mV,                                                                                                                //--- ������� - ��� - ��� ���� - �� 2� 5�� ���� ---
    "�� 2� 5�� ����",
    "",
    RSHIFT_ADD_STABLE_B(Range::_5mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gShift_A10mV,                                                                                                              //--- ������� - ��� - ��� ���� - �� 1� 10�� ���� ---
    "�� 1� 10�� ����",
    "",
    RSHIFT_ADD_STABLE_A(Range::_10mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_A
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gShift_B10mV,                                                                                                              //--- ������� - ��� - ��� ���� - �� 2� 10�� ���� ---
    "�� 2� 10�� ����",
    "",
    RSHIFT_ADD_STABLE_B(Range::_10mV), -100, 100,
    &PageDebug::PageADC::PageShift::self, 0, OnChanged_Shift_B
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_7( pShift, // -V641  // -V1027                                                                                                                    //--- ������� - ��� - ��� ���� ---
    "��� ����",
    "",
    &bShift_Reset,   // ������� - ��� - ��� ���� - �����
    &gShift_A2mV,    // ������� - ��� - ��� ���� - �� 1� 2�� ����
    &gShift_B2mV,    // ������� - ��� - ��� ���� - �� 2� 2�� ����
    &gShift_A5mV,    // ������� - ��� - ��� ���� - �� 1� 5�� ����
    &gShift_B5mV,    // ������� - ��� - ��� ���� - �� 2� 5�� ����
    &gShift_A10mV,   // ������� - ��� - ��� ���� - �� 1� 10�� ����
    &gShift_B10mV,   // ������� - ��� - ��� ���� - �� 2� 10�� ����
    PageName::Debug_ADC_Shift,
    &PageDebug::PageADC::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::PageShift::self = (const Page *)&pShift;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pADC, //-V641 //-V1027
    "���",
    "",
    &pBalance,                              // ������� - ��� - ������
    &pStretch,                              // ������� - ��� - ��������
    PageDebug::PageADC::PageShift::self,    // ������� - ��� - ��� ����
    PageName::Debug_ADC,
    &PageDebug::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageADC::self = (const Page *)&pADC;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStats,                                                                                                                                          //--- ������� - ���������� ---
    "����������",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ ��������"
    ,
    "�� ����������",
    "����������",
    set.dbg.ShowStats, &PageDebug::self, Item::Active, Choice::Changed, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int IncreaseY(int &y, int dY)
{
    y += dY;
    return y;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < Range::Size; ++range)
            {
                String("%d", NRST_RSHIFT_ADD(ch, range, type)).Draw(x + range * 20, y + dY * ddY);
            }
            ddY++;
        }
    }

    y += dY * 3;

    DRAW_FORMAT("correctionTime : %d", NRST_CORRECTION_TIME); //-V2528
    DRAW_FORMAT2("balanceADC : %d %d", NRST_BALANCE_ADC_A, NRST_BALANCE_ADC_B); //-V2528
    DRAW_FORMAT("numAveForRand : %d", NRST_NUM_AVE_FOR_RAND); //-V2528

    pString s[3] = {"���������", "��������� �������������", "������ �������"};
    DRAW_FORMAT("balanceADCtype : %s", (NRST_BALANCE_ADC_TYPE < 3 ? s[NRST_BALANCE_ADC_TYPE] : "!!! ������������ �������� !!!")); //-V547 //-V2528
    DRAW_FORMAT("stretchADCtype : %s", (NRST_STRETCH_ADC_TYPE < 3 ? s[NRST_STRETCH_ADC_TYPE] : "!!! ������������ �������� !!!")); //-V547 //-V2528

    x = String("stretchADC :").Draw(x0, INC_Y) + 5; //-V2528

    for (int ch = 0; ch < 2; ch++)
    {
        for (int num = 0; num < 3; num++)
        {
            String("%d", NRST_STRETCH_ADC(ch, num)).Draw(x + num * 20, y + dY * ch);
        }
    }

    y += dY;

#define DRAW_STRETCH(name) DRAW_FORMAT2(#name " : %d %d", set.nrst_##name[0], set.nrst_##name[1])

    /*
    DRAW_STRETCH(AddStretch20mV);
    DRAW_STRETCH(AddStretch50mV);
    DRAW_STRETCH(AddStretch100mV);
    DRAW_STRETCH(AddStretch2V);
    */

    DRAW_FORMAT("numSmoothForRand : %d", NRST_NUM_SMOOTH_FOR_RAND); //-V2528

    Menu::Draw();
    Painter::EndScene();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnOpenClose_Settings(bool)
{
    Display::SetDrawMode(Display::DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_5( pSettings, // -V641 // -V1027                                                                                                                       //--- ������� - ��������� ---
    "���������",
    "�������� ���������� � ����������",
    0,
    0,
    0,
    0,
    0,
    PageName::Debug_Settings,
    &PageDebug::self, Item::Active, OnOpenClose_Settings, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::PageSettings::self = (const Page *)&pSettings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart("�������� ��������", false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite(NAME_FILE_FIRMWARE, &structForWrite);

    uint8 *address = (uint8 *)0x08020000;
    uint8 *endAddress = address + SIZE_FILE_FIRMWARE; //-V566

    int sizeBlock = 512;

    while (address < endAddress)
    {
        FDrive::WriteToFile(address, sizeBlock, &structForWrite);
        address += sizeBlock;
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStop();

    Display::ShowWarning(Warning::FirmwareSaved);
}

DEF_BUTTON( bSaveFirmware,                                                                                                                                 //--- ������� - ����. �������� ---
    "����. ��������",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    &PageDebug::self, IsActive_SaveFirmware, OnPress_SaveFirmware
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Change()
{
    Color::ResetFlash();
}

static void Draw_SerialNumber_Change(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);

    Char(SYMBOL_TAB).Draw4SymbolsInRect(x + 2, y + 2);

    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSerialNumber_Change,                                                                                                                    //--- ������� - �/� - �������� ---
    "��������",
    "��������� �������� ������",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Save()
{
}

static void Draw_SerialNumber_Save(int x, int y)
{
    Font::SetCurrent(Font::Type::_UGO2);
    Char(SYMBOL_SAVE_TO_MEM).Draw4SymbolsInRect(x + 2, y + 1);
    Font::SetCurrent(Font::Type::_8);
}

DEF_GRAPH_BUTTON( bSerialNumber_Save,                                                                                                                     //--- ������� - �/� - ��������� ---
    "���������",
    "���������� �������� ����� � OTP",
    &PageDebug::PageSerialNumber::self, Item::Active, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_EnterSerialNumber()
{
}

static void OnOpenClose_SerialNumber(bool)
{
    Draw_EnterSerialNumber();
}

static bool OnArrows_SerialNumber(const KeyEvent &)
{
    return true;
}

DEF_PAGE_2( pSerialNumber, // -V641 // -V1027                                                                                                                         //--- ������� - �/� ---
    "�/�",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    &bSerialNumber_Change,          // ������� - �/� - �������
    &bSerialNumber_Save,            // ������� - �/� - ���������
    PageName::Debug_SerialNumber,
    &PageDebug::self, Item::Active, OnOpenClose_SerialNumber, Page::BeforeDraw, OnArrows_SerialNumber
)

const Page * const PageDebug::PageSerialNumber::self = (const Page *)&pSerialNumber;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void OnPress_EraseData()
//{
//    Display::FuncOnWaitStart("������ ���������� ������", false);
//    ::Memory::DeleteAllData();
//    Display::FuncOnWaitStop();
//}

//_DEF_BUTTON( bEraseData,                                                                                                                                    //--- ������� - ������� ������ ---
//    "������� �����",
//    "������� ���������� ������ �� ����",
//    &PageDebug::self, 0, OnPress_EraseData, 0
//)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pDebug, // -V641 // -V1027                                                                                                                                   //--- ������� ---
    "�������",
    "",
    PageDebug::PageConsole::self,   ///< ������� - �������
    &pADC,                                         ///< ������� - ���
    PageDebug::PageRand::self,      ///< ������� - ����-���
    &cStats,			                            ///< ������� - ����������
    &bSaveFirmware,                                 ///< ������� - ����. ��������
//    &cDisplayOrientation,                           ///< ������� - ����������
//    &mgPred,			                            ///< ������� - ����������
//    &mgPost,			                            ///< ������� - �����������
//    &pSettings,		                            ///< ������� - ���������
//    &pSerialNumber,                              ///< ������� - �/�
//    &bEraseData,                                  ///< ������� - ������� ������
    PageName::Debug,
    &PageService::self, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageDebug::self = (const Page *)&pDebug;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float GetStretchADC(Chan::E ch)
{
    static const int16 *addStretch[Range::Size][2] =
    {
        {0, 0},  // 2mV
        {0, 0},  // 5mV
        {0, 0},  // 10mV
        {0, 0},
        {0, 0},
        {0, 0},
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 20mV
        {&NRST_ADD_STRETCH_50mV_A, &NRST_ADD_STRETCH_50mV_B},   // 50mV
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}, // 100mV
        */
        {0, 0}, // 200mV
        {0, 0}, // 500mV
        {0, 0},
        {0, 0},
        {0, 0}
        /*
        {&NRST_ADD_STRETCH_20mV_A, &NRST_ADD_STRETCH_20mV_B},   // 1V
        {&NRST_ADD_STRETCH_2V_A, &NRST_ADD_STRETCH_2V_B},       // 2V
        {&NRST_ADD_STRETCH_100mV_A, &NRST_ADD_STRETCH_100mV_B}  // 5V
        */
    };

    const int16 *address = addStretch[set.ch.range[ch]][ch];

    int16 stretch = NRST_STRETCH_ADC(ch, NRST_STRETCH_ADC_TYPE);

    if (address)
    {
        stretch += (*address);
    }

    return stretch * 1e-4F + 1.0F;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------
void SetStretchADC(Chan::E ch, float kStretch)
{
    NRST_STRETCH_ADC(ch, NRST_STRETCH_ADC_TYPE) = (int16)((kStretch - 1.0F) * 1e4F);
}

