#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


/*
    ��������� ���� XxxDef ���������� ������ ��� ������������� ��������
*/

#define GRAPH_BUTTON_FROM_PAGE(page, numButton)     ((GraphButton *)((Page *)page)->items[numButton])

extern int8 gCurDigit;

#define MENU_ITEMS_ON_DISPLAY       5   ///< ������� ������� ���� ���������� �� ������ �� �����������.

#define IS_PAGE(item)           (item->type == Item::Type::Page)
#define NOT_PAGE(item)          (item->type != Item::Type::Page)
#define IS_PAGE_SB(item)        (item->isPageSB)
#define IS_CHOICE(item)         (item->type == Item::Type::Choice)
#define IS_CHOICE_REG(item)     (item->type == Item::Type::ChoiceReg)
#define NOT_CHOICE_REG(item)    (item->type != Item::Type::ChoiceReg)
#define IS_GOVERNOR(item)       (item->type == Item::Type::Governor)
#define NOT_GOVERNOR(item)      (item->type != Item::Type::Governor)
#define IS_GOVERNOR_COLOR(item) (item->type == Item::Type::GovernorColor)
#define IS_TIME(item)           (item->type == Item::Type::Time)
#define IS_BUTTON(item)         (item->type == Item::Type::Button)
#define IS_GRAPH_BUTTON(item)   (item->type == Item::Type::GraphButton)
    
class Item
{
public:
    COMMON_PART_MENU_ITEM;
    /// ���������� ������ � �������� ��������� �������� Choice ��� Page::Name
    int HeightOpened() const;
    /// @brief ���������� true, ���� ������� ���� item ������ (��������� �� �� ����� ������� ����. ��� �������, ��� ��������, ��� ������� 
    /// �������������� ������� ���� ����� Choice ��� Governor
    bool IsShade() const;
    /// ���������� true, ���� ������, ��������������� �������� ���� item, ��������� � ������� ���������
    bool IsPressed() const;
    /// �������/��������� �������
    void SetCurrent(bool active) const;
    /// ���������� true, ���� ������� ���� �� ������ item ������
    bool IsOpened() const;

    void Open(bool open) const;
    /// ���������� �������� ��������, ��� ��� �������� �� ������� �������
    String Title() const;
    /// ��������� ������� ������
    bool ProcessKey(KeyEvent event);
    /// ���������� ��� ������� ������
    void KeyPress() const;
    /// ���������� ��� "��������" ����������
    void KeyRelease() const;
    /// ���������� ��� �������������� ������������ ������ (������� � ��������� ����� 0.5 ���)
    void KeyAutoRelease() const;
    /// ���������� true, ���� ������� ��������� � �������� ��������� (��������� �� ������ ����������)
    bool IsActive() const { if (funcOfActive) { return funcOfActive(); } return true; };

    void Draw(int x, int y, bool opened) const;

    bool IsCurrentItem() const;
    /// ���������� ����������� ����������, ��������������� ��� �������� ���������
    char GetSymbol() const;
    /// ���������� ����� ��������
    const Page *Keeper() const { if (keeper) { return *keeper; }; return nullptr; }
    /// ���������� true, ���� � ����� ������� ����� keeper
    bool ExistKeeper(const Page *keeper) const;
    /// ����� �������� - �� �������� ������� ��������� ����
    bool HaveParent() const { return Keeper() != nullptr; };

    static Item empty;

    /// ������ ���� ������� ����
    struct Type
    {
        enum E
        {
            None,
            Choice,         ///< ����� ������ - ��������� ������� ���� �� ���������� �������� ��������.
            Button,         ///< ������.
            Page,           ///< ��������.
            Governor,       ///< ��������� - ��������� ������� ����� ����� �������� �������� �� ������� ��������� ����������. �������� �� ����� ��������� [ -(1 << 16) / 2 , (1 << 16) / 2]
            Time,           ///< ��������� ������ �����.
            GovernorColor,  ///< ��������� ������� ����.
            ChoiceReg,      ///< ������� ������, � ������� ����� �������������� �� �������, � ������
            GraphButton,    ///< ������ ��� ������ ����� ������
            ChoiceParameter,
            Number
        } value;

        explicit Type(E v) : value(v) {};
    };

    /// ������ �����
    int Height() const;
    /// ������ �����
    static const int WIDTH = 320 / 5 + 1;



    struct Value
    {
        static const int HEIGHT = 13;
        static const int WIDTH = 320 / 5 - 4; // -V112
    };
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
class Page : public Item
{
public:
    const Item * const *items;      ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                    ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< ����� ���������� ��� ������� �� �������� �������� � ��� ������ �� ���� �������� �� ����������
    pFuncVV     funcOnDraw;         ///< ����� ���������� ����� ��������� ������
    pFuncBKE    funcKey;            ///< � �������� ����� ������ ���������� ��� ������� �������
    /// ���������� true, ���� ������� ������� �������� ������
    bool CurrentItemIsOpened() const;
    /// D��������� ����� ���������� � �������� �� ������ page
    int NumSubPages() const;        
    /// ���������� ���������� ��������� � �������� �� ������ page
    int NumItems() const;
    /// ���������� ������� ������ ��������.
    void SetAsCurrent() const;
    /// ���������� ����� ������� ����������� (���, ������� ������������ � ������ ������)
    int8 CurrentSubPage() const;
    /// ���������� ������� ������ �����������
    void SetCurrentSubPage(int8 pos) const;
    ///< ������������� ������� ��������� ������ ����
    void SetPosActItem(int8 pos) const;
    ///< ���������� ��������� �� �������� ������� ��������
    Item *GetItem(int numItem) const;
    /// \todo ���������� ������� ������� �������� �������� �� ������ page �� ������. ���� ������� ����������� 0, ��� ����� 0, ���� ������� 
    /// ����������� 1, ��� ����� 5 � �.�.
    int PosItemOnLeft() const;
    /// ���������� ������� �������� �������� ������� page
    int8 PosCurrentItem() const;
    /// �������� ����� ������� ����������� �� �������� delta
    void ChangeSubPage(int delta);
    /// ���������� � �������� �����������
    void Draw(int x, int y, bool opened) const;
    /// ��������� ������� ������
    bool ProcessKey(KeyEvent event);

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    static int ItemOpenedPosX(const Item *item);

    void DrawPagesUGO(int right, int bottom) const;

    void DrawNestingPage(int left, int bottom) const;
    /// true, ���� �������� ��������� ������������ �������� parent
    bool IsSubPage(const Page *parent);

    void ShortPress() const;
    /// ���������� ����� ��������, ���������������� �������������� �������
    const Item *ItemForFuncKey(Key::E key) const;

    struct Name
    {
        enum E
        {
            Main,                                   ///< ������� �������� ����

            Function,                               ///< �������
            Function_FrequencyCounter,              ///< ������� - ����������
            Function_FFT,                           ///< ������� - ������
            Function_Multimeter,                    ///< ������� - ����������
            Function_Tester,                        ///< ������� - ������
            Function_Recorder,                      ///< ������� - �����������
            Function_Recorder_Source,               ///< ������� - ����������� - ��������
            Function_Recorder_Record,               ///< ������� - ����������� - ������
            Function_Recorder_Show,                 ///< ������� - ����������� - ��������
            Function_Recorder_Show_Operations,      ///< ������� - ����������� - �������� - ��������
            Function_Recorder_Show_View,            ///< ������� - ����������� - �������� - ��������
            Function_Recorder_Show_View_Cursors,    ///< ������� - ����������� - �������� - �������� - �������

            Measures,                   ///< ���������
            Measures_Auto,              ///< ��������� - �������
            Measures_Auto_Tune,         ///< ��������� - ������� - ���������
            Measures_Cursors,           ///< ��������� - �������
            Measures_Cursors_Set,       ///< ��������� - ������� - ����������

            Display,                    ///< ������
            Display_Accumulation,       ///< ������� - ����������
            Display_Average,            ///< ������� - ����������
            Display_Grid,               ///< ������� - �����
            Display_Settings,           ///< ������� - ���������
            Display_Settings_Colors,    ///< ������� - ��������� - �����

            ChannelA,                   ///< ����� 1
            ChannelB,                   ///< ����� 2
            Trig,                       ///< �����
            Trig_Search,                ///< ����� - �����
            Trig_HoldOff,               ///< ����� - ���������
            Time,                       ///< ���������
            Memory,                     ///< ������
            SB_Memory_Last,             ///< ������ - ���������
            SB_Memory_Internal,         ///< ������ - ����� ��
            Memory_Drive,               ///< ������ - ����� ��
            SB_Memory_Drive_Manager,    ///< ������ - ����� �� - �������
            SB_Memory_Drive_Mask,       ///< ������ - ����� �� - �����
            SB_Memory_SetName,          ///< \brief ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� 
                                        ///< ����� �� - ��� �����
            Service,                    ///< ������
            Service_Calibrator,         ///< ������ - ����������
            Service_Ethernet,           ///< ������ - ETHERNET
            Service_RTC,                ///< ������ - �����
            Service_Battery,            ///< ������ - �������
            SB_Service_FFT_Cursors,     ///< ������ - ������ - �������
            SB_Service_Function,        ///< ������ - �������
            SB_Service_Information,     ///< ������ - ����������
            SB_Help,                    ///< ������
            Debug,                      ///< �������
            Debug_Console,              ///< ������� - �������
            Debug_Console_Registers,    ///< ������� - ������� - ��������
            Debug_ADC,                  ///< ������� - ���
            Debug_ADC_Balance,          ///< ������� - ��� - ������
            Debug_ADC_Stretch,          ///< ������� - ��� - ��������
            Debug_ADC_Shift,            ///< ������� - ��� - ��� ����
            Debug_ADC_AltShift,         ///< ������� - ��� - ��� ���� ���
            Debug_Rand,                 ///< ������� - ����-���
            Debug_Channels,             ///< ������� - ������
            Debug_Multimeter,           ///< ������� - ����������
            SB_Debug_Settings,          ///< ������� - ���������
            SB_Debug_SerialNumber,      ///< ������� - �/�
            Measures_Math,              ///< ��������� - ����������
            Function_Multimeter_Cal,    ///< ������� - ���������� - ����������
            Display_View,               ///< ������� - �����������
            Service_Info,               ///< ������ - ����������
            Service_RTC_Set,            ///< ������ - ����� - ����������
            Service_RTC_Correct,        ///< ������ - ����� - ���������
            Number,
            NoPage
        } value;
    };

    /// ���������� ��� �������� page
    Page::Name::E GetName() const;

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
class Button : public Item
{
public:
    pFuncVV     funcOnPress;        ///< �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;        ///< ������� ����� ���������� �� ����� ��������� ������.
    void Draw(int x, int y) const;
    void KeyRelease() const;
    void KeyAutoRelease() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< ��������� �� ������� ��������� ����������� �������� ������
    pString     helpUGO;        ///< ������� � ������� �����������.
};


class GraphButton : public Item
{
public:
    pFuncVV                     funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                    funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpDrawButton *hintUGO; 
    int                         numHints;
    void Draw(int x, int y) const;
    void DrawHints(int x, int y, int width) const;
    void KeyRelease() const;
    int Width() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
class Governor : public Item
{
public:
    int16  *cell;
    int16   minValue;       ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       ///< ������������ ��������.
    pFuncVV funcOfChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������
    /// ��������� ������� ������
    bool ProcessKey(KeyEvent event);
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue();
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue();
    /// ������������ ��������� ���� ��������.
    float Step();
    /// �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int16 delta);
    /// ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition() const;
    /// ��� �������� �������� ������������ ������ �� ���������� �������
    void PrevPosition();

    void Draw(int x, int y, bool opened);

    void DrawOpened(int x, int y);

    void DrawClosed(int x, int y);

    void DrawValue(int x, int y);

    char GetSymbol();

    int16 GetValue();

    void SetValue(int16 v);

    void KeyRelease() const;

private:

    void DrawLowPart(int x, int y, bool shade);
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    uint  NumDigits() const;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class Choice : public Item
{
public:
    int8       *cell;
    pString    *names;          ///< �������� ������ �� ������� � ���������� ������.
    pFuncVB     funcOnChanged;  ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII    funcForDraw;    ///< ������� ���������� ����� ��������� ��������. 
    /// ��������� ������� ��������� �������� �� delta
    void  StartChange(int delta) const;
    /// ������������ ��������� ���� ��������.
    float Step();
    /// �������� �������� choice � ����������� �� �������� � ����� delta.
    void  ChangeIndex(int delta) const;
    /// ���������� ���������� ��������� ������ � �������� �� ������ choice
    int   NumSubItems() const { return num; };

    bool ProcessKey(KeyEvent event);

    void  Draw(int x, int y, bool opened);

    void  DrawOpened(int x, int y);

    void  DrawClosed(int x, int y);
    /// ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    String NameCurrentSubItem() const;
    /// ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameNextSubItem();

    const char *NamePrevSubItem();
    /// ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    String NameSubItem(int i);
    /// ���������� ����, ������� ����� ��������� ������� ������
    static Color ColorMenuField(const Choice *choice);

    char GetSymbol();
    /// �������� ������� funcOnChanged, ���� ������� �������
    void Change(bool active) const { if (funcOnChanged) { funcOnChanged(active); } }

    void KeyRelease() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class GovernorColor : public Item
{
public:
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
    pFuncVV     funcOnChanged;      ///< ��� ������� ����� �������� ����� ��������� �������� ��������.
    void Draw(int x, int y, bool opened);
    void KeyRelease() const;
private:
    void DrawOpened(int x, int y);
    void DrawClosed(int x, int y);
    void DrawValue(int x, int y, int delta);
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Time ///

#define iEXIT   0
#define iDAY    1
#define iMONTH  2
#define iYEAR   3
#define iHOURS  4
#define iMIN    5
#define iSEC    6
#define iSET    7

/// ������������� � ���������� �����.
class TimeItem : public Item
{
public:
    int8 *curField;   ///< ������� ���� ���������. 0 - �����, 1 - ���, 2 - ���, 3 - ����, 4 - ����, 5 - �����, 6 - ���, 7 - ����������.
    int8 *hours;
    int8 *minutes;
    int8 *seconds;
    int8 *month;
    int8 *day;
    int8 *year;
    void SetOpened() const;
    void SetNewTime();
    void SelectNextPosition() const;
    void Draw(int x, int y, bool opened);
    void DrawClosed(int x, int y);
    void DrawOpened(int x, int y);
    char GetSymbol();
    void KeyRelease() const;
    void KeyAutoRelease() const;
};

typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];
