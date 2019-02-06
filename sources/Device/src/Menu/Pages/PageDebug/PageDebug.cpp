#include "defines.h"
#include "Menu/Pages/Include/PageDebug.h"
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
#include "Menu/Pages/Include/Definition.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdio>


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase mainPage;
extern const PageBase pageDebug;
extern const PageBase ppADC;
extern const PageBase pppADC_Balance;
extern const PageBase pppADC_Stretch;
extern const PageBase pppADC_Shift;
extern const PageBase ppSettings;
extern const PageBase ppSerialNumber;

const PageBase *PageDebug::pointer = &pageDebug;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

static void Draw_ADC_Balance_Mode(int, int)
{
    int8 shift[2][3] =
    {
        {0, SET_BALANCE_ADC_A, (int8)NRST_BALANCE_ADC_A},
        {0, SET_BALANCE_ADC_B, (int8)NRST_BALANCE_ADC_B}
    };

    shiftADCA = shift[Chan::A][NRST_BALANCE_ADC_TYPE];
    shiftADCB = shift[Chan::B][NRST_BALANCE_ADC_TYPE];
}

static void OnChanged_ADC_Balance_Mode(bool)
{
    Draw_ADC_Balance_Mode(0, 0);
}

DEF_CHOICE_3( cADC_Balance_Mode,                                                                                                                     //--- ������� - ��� - ������ - ����� ---
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    NRST_BALANCE_ADC_TYPE, pppADC_Balance, FuncActive, OnChanged_ADC_Balance_Mode, Draw_ADC_Balance_Mode
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_Balance_ShiftAB()
{
    return NRST_BALANCE_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Balance_ShiftA()
{
    NRST_BALANCE_ADC_A = shiftADCA;
}

DEF_GOVERNOR( gADC_Balance_ShiftA,                                                                                                              //--- ������� - ��� - ������ - �������� 1 ---
    "�������� 1", "Offset 1",
    "",
    "",
    shiftADCA, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftA, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Balance_ShiftB()
{
    NRST_BALANCE_ADC_B = shiftADCB;
}

DEF_GOVERNOR( gADC_Balance_ShiftB,                                                                                                              //--- ������� - ��� - ������ - �������� 2 ---
    "�������� 2", "Offset 2",
    "",
    "",
    shiftADCB, -125, 125, pppADC_Balance, IsActive_ADC_Balance_ShiftAB, OnChanged_ADC_Balance_ShiftB, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pppADC_Balance, // -V641                                                                                                                         //--- ������� - ��� - ������ ---
    "������", "BALANCE",
    "",
    "",
    &cADC_Balance_Mode,      // ������� - ��� - ������ - �����
    &gADC_Balance_ShiftA,    // ������� - ��� - ������ - �������� 1
    &gADC_Balance_ShiftB,    // ������� - ��� - ������ - �������� 2
    Page::Name::Debug_ADC_Balance, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 stretchA;
static int16 stretchB;

void PageDebug::OnChanged_ADC_Stretch_Mode(bool)
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

DEF_CHOICE_3( cADC_Stretch_Mode,                                                                                                                   //--- ������� - ��� - �������� - ����� ---
    "�����", "Mode",
    "",
    "",
    DISABLE_RU, DISABLE_EN,
    "��������", "Real",
    "������",   "Manual",
    NRST_STRETCH_ADC_TYPE, pppADC_Stretch, FuncActive, PageDebug::OnChanged_ADC_Stretch_Mode, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_ADC_StretchAB()
{
    return NRST_STRETCH_ADC_TYPE_IS_HAND;
}

static void OnChanged_ADC_Stretch_A()
{
    NRST_STRETCH_ADC_A(NRST_STRETCH_ADC_TYPE) = stretchA;
}

DEF_GOVERNOR( gADC_Stretch_A,                                                                                                                //--- ������� - ��� - �������� - �������� 1� ---
    "�������� 1�", "Stretch 1ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the first channel.\n1 = 0.0001",
    stretchA, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Stretch_B()
{
    NRST_STRETCH_ADC_B(NRST_STRETCH_ADC_TYPE) = stretchB;
}

DEF_GOVERNOR( gADC_Stretch_B,                                                                                                                //--- ������� - ��� - �������� - �������� 2� ---
    "�������� 2�", "Stretch 2ch",
    "����� ������ �������� ������� ������.\n1 ������� = 0.0001",
    "Sets the manual stretching of the second channel.\n1 = 0.0001",
    stretchB, -10000, 10000, pppADC_Stretch, IsActive_ADC_StretchAB, OnChanged_ADC_Stretch_B, FuncBeforeDraw
)

/*
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 1� ---
    "20��/1� 1�", "20mV/1V 1k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak50mV,                                                                  //--- ������� - ��� - �������� - 50�� 1� ---
    "50�� 1�", "50mV 1k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 1� ---
    "100��/5� 1�", "100mV/5V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_100mV_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Ak2V,                                                                      //--- ������� - ��� - �������� - 2� 1� ---
    "2� 1�", "2V 1ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_A, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk20mV,                                                               //--- ������� - ��� - �������� - 20��/1� 2� ---
    "20��/1� 2�", "20mV/1V 2k",
    "",
    "",
    NRST_ADD_STRETCH_20mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk50mV,                                                         //--- ������� - ��� - �������� - ��� ���� 50�� 2� ---
    "50�� 2�", "50mV 2k",
    "",
    "",
    NRST_ADD_STRETCH_50mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk100mV,                                                             //--- ������� - ��� - �������� - 100��/5� 2� ---
    "100��/5� 2�", "100mV/5V 2k",
    "",
    "",
    NRST_ADD_STRETCH_100mV_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR(       gADC_Stretch_Bk2V,                                                                      //--- ������� - ��� - �������� - 2� 2� ---
    "2� 2�", "2V 2ch",
    "",
    "",
    NRST_ADD_STRETCH_2V_B, -10000, 10000, pppADC_Stretch, FuncActive, FuncChanged, FuncBeforeDraw
)
*/

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//static const ChoiceBase emptyChoice = {Control::Type::Choice, 0, false, Page::Name::NoPage, 0, FuncActive, {}, 0, 0, 0, 0};

DEF_PAGE_3( pppADC_Stretch, // -V641                                                                                                                       //--- ������� - ��� - �������� ---
    "��������", "STRETCH",
    "������������� ����� � �������� �������� (��� ������� ������)",
    "Sets mode and the value of stretching (manual mode)",
    &cADC_Stretch_Mode,      // ������� - ��� - �������� - �����
    &gADC_Stretch_A,         // ������� - ��� - �������� - �������� 1�
    &gADC_Stretch_B,
    Page::Name::Debug_ADC_Stretch, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

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

DEF_BUTTON( bADC_Shift_Reset,                                                                                                                       //-- ������� - ��� - ��� ���� - ����� ---
    "�����", "Reset",
    "",
    "",
    pppADC_Shift, FuncActive, OnPress_ADC_Shift_Reset, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_A()
{
    RShift::Set(Chan::A, SET_RSHIFT_A);
}

DEF_GOVERNOR( gADC_Shift_A2mV, //-V641                                                                                                    //--- ������� - ��� - ��� ���� - �� 1� 2�� ���� ---
    "�� 1� 2�� ����", "Shift 1ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_ADC_Shift_B()
{
    RShift::Set(Chan::B, SET_RSHIFT_B);
}

DEF_GOVERNOR( gADC_Shift_B2mV,                                                                                                            //--- ������� - ��� - ��� ���� - �� 2� 2�� ���� ---
    "�� 2� 2�� ����", "Shift 2ch 2mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_2mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_A5mV,                                                                                                            //--- ������� - ��� - ��� ���� - �� 1� 5�� ���� ---
    "�� 1� 5�� ����", "Shift 1ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_B5mV,                                                                                                            //--- ������� - ��� - ��� ���� - �� 2� 5�� ���� ---
    "�� 2� 5�� ����", "Shift 2ch 5mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_5mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_A10mV,                                                                                                          //--- ������� - ��� - ��� ���� - �� 1� 10�� ���� ---
    "�� 1� 10�� ����", "Shift 1ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_A(Range::_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_A, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_GOVERNOR( gADC_Shift_B10mV,                                                                                                          //--- ������� - ��� - ��� ���� - �� 2� 10�� ���� ---
    "�� 2� 10�� ����", "Shift 2ch 10mV DC",
    "",
    "",
    RSHIFT_ADD_STABLE_B(Range::_10mV), -100, 100, pppADC_Shift, FuncActive, OnChanged_ADC_Shift_B, FuncBeforeDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_7( pppADC_Shift, // -V641  // -V1027                                                                                                              //--- ������� - ��� - ��� ���� ---
    "��� ����", "ADD RSHFIT",
    "",
    "",
    &bADC_Shift_Reset,   // ������� - ��� - ��� ���� - �����
    &gADC_Shift_A2mV,    // ������� - ��� - ��� ���� - �� 1� 2�� ����
    &gADC_Shift_B2mV,    // ������� - ��� - ��� ���� - �� 2� 2�� ����
    &gADC_Shift_A5mV,    // ������� - ��� - ��� ���� - �� 1� 5�� ����
    &gADC_Shift_B5mV,    // ������� - ��� - ��� ���� - �� 2� 5�� ����
    &gADC_Shift_A10mV,   // ������� - ��� - ��� ���� - �� 1� 10�� ����
    &gADC_Shift_B10mV,   // ������� - ��� - ��� ���� - �� 2� 10�� ����
    Page::Name::Debug_ADC_Shift, &ppADC, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( ppADC, // -V641                                                                                                                                           //--- ������� - ��� ---
    "���", "ADC",
    "",
    "",
    &pppADC_Balance, // ������� - ��� - ������
    &pppADC_Stretch, // ������� - ��� - ��������
    &pppADC_Shift,   // ������� - ��� - ��� ����
    Page::Name::Debug_ADC, &pageDebug, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cStats,                                                                                                                                          //--- ������� - ���������� ---
    "����������", "Statistics",
    "����������/�� ���������� �����/����, ������ � �������, ���������� �������� � ���������� ����������� � ������/���������� ����������� � ������ "
    "��������",
    "To show/not to show a time/shot, frames per second, quantity of signals with the last settings in memory/quantity of the signals kept in memory",
    "�� ����������", "Hide",
    "����������",    "Show",
    SHOW_STAT, pageDebug, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageDebug::OnChanged_DisplayOrientation(bool)
{
    Display::SetOrientation(DISPLAY_ORIENTATION);
}

DEF_CHOICE_2( cDisplayOrientation,                                                                                                                             //--- ������� - ���������� ---
    "����������", "DisplayOrientation",
    "������������� ���������� �������",
    "Sets display orientation",
    "������",   "Direct",
    "��������", "Back",
    DISPLAY_ORIENTATION, pageDebug, FuncActive, PageDebug::OnChanged_DisplayOrientation, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 pred;

static void OnChanged_Pred()
{
    FPGA::pred = (uint16)(~pred);
}

DEF_GOVERNOR( mgPred,                                                                                                                                          //--- ������� - ���������� ---
    "����������", "",
    "", "",
    pred, 0, 15000, pageDebug, FuncActive, OnChanged_Pred, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int16 post;

static void OnChanged_Post()
{
    FPGA::post = (uint16)~post;
}

DEF_GOVERNOR( mgPost,                                                                                                                                         //--- ������� - ����������� ---
    "�����������", "",
    "", "",
    post, 0, 15000, pageDebug, FuncActive, OnChanged_Post, FuncBeforeDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Settings_Exit()
{
    Display::SetDrawMode(Display::DrawMode::Auto, 0);
}

DEF_SMALL_BUTTON_EXIT( bSettings_Exit,                                                                                                                  //--- ������� - ��������� - ����� ---
    ppSettings, FuncActive, OnPress_Settings_Exit, DrawSB_Exit
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DebugShowSetInfo_Draw()
{
    Painter::BeginScene(Color::BACK);
    Rectangle(319, 239).Draw(0, 0, Color::FILL);

    int x0 = 30;
    int y0 = 25;
    int dY = 10;
    int y = y0 - dY;

#define Y_AND_INCREASE (y += dY, y)
#define DRAW_TEXT(str)                  Painter::DrawText(x0, Y_AND_INCREASE, str)
#define DRAW_FORMAT(str, value)         String(str, value).Draw(x0, Y_AND_INCREASE)
#define DRAW_FORMAT2(str, val1, val2)   String(str, val1, val2).Draw(x0, Y_AND_INCREASE)

    //Painter_DrawFormatText(x0, Y_AND_INCREASE, "������ �������� ��������� %d", sizeof(set));
    DRAW_FORMAT("������ �������� ��������� : %d", sizeof(set)); //-V2528
    String("�������������� ���������:").Draw(x0, Y_AND_INCREASE); //-V2528

    int x = String("rShiftAdd :").Draw(x0, Y_AND_INCREASE) + 5; //-V2528

    int ddY = 0;

    for (int type = 0; type < 2; type++)
    {
        for (int ch = 0; ch < 2; ch++)
        {
            for (int range = 0; range < Osci::Settings::Range::Size; ++range)
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

    x = String("stretchADC :").Draw(x0, Y_AND_INCREASE) + 5; //-V2528

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

static void OnPress_Settings(bool)
{
    Display::SetDrawMode(Display::DrawMode::Auto, DebugShowSetInfo_Draw);
}

DEF_PAGE_SB( ppSettings, // -V641 // -V1027                                                                                                                     //--- ������� - ��������� ---
    "���������", "SETTINGS",
    "�������� ���������� � ����������",
    "Show settings information",
    &bSettings_Exit,            // ������� - ��������� - �����
    0,
    0,
    0,
    0,
    0,
    Page::Name::SB_Debug_Settings, &pageDebug, FuncActive, OnPress_Settings, FuncDrawPage, FuncRegSetPage
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsActive_SaveFirmware()
{
    return FDrive::IsConnected();
}

static void OnPress_SaveFirmware()
{
    Display::FuncOnWaitStart(DICT(DSaveFirmware), false);

    StructForWrite structForWrite;

    FDrive::OpenNewFileForWrite("S8-54.bin", &structForWrite);

    uint8 *address = (uint8 *)0x08020000;   // -V566
    uint8 *endAddress = address + 128 * 1024 * 3;

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
    "����. ��������", "Save firmware",
    "���������� �������� - �������� 5, 6, 7 ����� ������� 3 � 128 ��, ��� �������� ���������",
    "Saving firmware - sectors 5, 6, 7 with a total size of 3 x 128 kB, where the program is stored",
    pageDebug, IsActive_SaveFirmware, OnPress_SaveFirmware, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_SerialNumber_Exit()
{
    OnPressSB_Exit();
}

DEF_SMALL_BUTTON_EXIT( bSerialNumber_Exit,                                                                                                                    //--- ������� - �/� - ����� ---
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Exit, DrawSB_Exit
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

DEF_SMALL_BUTTON( bSerialNumber_Change,                                                                                                                    //--- ������� - �/� - �������� ---
    "��������", "Insert",
    "��������� �������� ������",
    "Inserts the chosen symbol",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Change, Draw_SerialNumber_Change
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

DEF_SMALL_BUTTON( bSerialNumber_Save,                                                                                                                     //--- ������� - �/� - ��������� ---
    "���������", "Save",
    "���������� �������� ����� � OTP",
    "Records the serial number in OTP",
    ppSerialNumber, FuncActive, OnPress_SerialNumber_Save, Draw_SerialNumber_Save
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void Draw_EnterSerialNumber()
{
}

static void OnPress_SerialNumber(bool)
{
    Draw_EnterSerialNumber();
}

static bool HandlerKey_SerialNumber(KeyEvent /*event*/)
{
    return true;
}

DEF_PAGE_SB( ppSerialNumber, // -V641 // -V1027                                                                                                                       //--- ������� - �/� ---
    "�/�", "S/N",
    "������ ��������� ������ � OTP-������. ��������!!! ��P-������ - ������ � ����������� �������.",
    "Serial number recording in OTP-memory. ATTENTION!!! OTP memory is a one-time programming memory.",
    &bSerialNumber_Exit,            // ������� - �/� - �����
    &bSerialNumber_Change,          // ������� - �/� - �������
    0,
    0,
    0,
    &bSerialNumber_Save,            // ������� - �/� - ���������
    Page::Name::SB_Debug_SerialNumber, &pageDebug, FuncActive, OnPress_SerialNumber, FuncDrawPage, HandlerKey_SerialNumber
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_EraseData()
{
    Display::FuncOnWaitStart(DICT(DDeleteFromMemory), false);
    ::Memory::DeleteAllData();
    Display::FuncOnWaitStop();
}

DEF_BUTTON( bEraseData,                                                                                                                                    //--- ������� - ������� ������ ---
    "������� �����", "Erase data",
    "������� ����������� ������ �� ����",
    "Erase all saved datas from EEPROM",
    pageDebug, FuncActive, OnPress_EraseData, Button::FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageDebug, // -V641                                                                                                                                             //--- ������� ---
    "�������", "DEBUG",
    "",
    "",
    PageDebug::PageConsole::pointer,    ///< ������� - �������
    &ppADC,                             ///< ������� - ���
    PageDebug::PageRand::pointer,       ///< ������� - ����-���
    &cStats,			                ///< ������� - ����������
    &cDisplayOrientation,               ///< ������� - ����������
//    &mgPred,			          ///< ������� - ����������
//    &mgPost,			          ///< ������� - �����������
//    &ppSettings,		          ///< ������� - ���������
//    &bSaveFirmware,             ///< ������� - ����. ��������
//    &ppSerialNumber,            ///< ������� - �/�
//    &bEraseData,                ///< ������� - ������� ������
    Page::Name::Debug, Menu::pageMain, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
