#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


class Page;
    
class Item
{
public:
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
            GovernorColor,  ///< ��������� ������� ����.
            GraphButton,    ///< ������ ��� ������ ����� ������
            Number
        } value;

        explicit Type(E v) : value(v) {};
    };
    
    uint8               type;           ///< ��� �����
    int8                num;            ///< ����� ��������� ��� Choice ��� ����� ��������� ��� Page
    uint8               name;           ///< ��� �� ������������ Page::Name
    const Page *const  *keeper;         ///< ����� ��������, ������� �����������. ��� Page_Main = 0
    pFuncBV             funcOfActive;   ///< ������� �� ������ �������
    const char * const *titleHint;      ///< �������� ��������. ����� ��������� ��� ������ ������
    Item(uint8 type = Item::Type::None, const char * const *titleHint = nullptr, const Page *const *keeper = nullptr, int8 num = 0, pFuncBV funcActive = nullptr, uint8 name = 0);
    /// ���������� ������� ����, ������������ �� ������
    static const int NUM_ON_DISPLAY = 5;
    /// ���������� true, ���� ������, ��������������� �������� ���� item, ��������� � ������� ���������
    bool IsPressed() const;
    /// �������/��������� �������
    void SetCurrent(bool active) const;
    /// ���������� true, ���� ������� ���� �� ������ item ������
    bool IsOpened() const;

    void Open(bool open) const;
    /// ���������� �������� ��������, ��� ��� �������� �� ������� �������
    String Title() const;
    /// ���������� ��� ������� ������
    void KeyPress() const;
    /// ���������� true, ���� ������� ��������� � �������� ��������� (��������� �� ������ ����������)
    bool IsActive() const { return funcOfActive(); };

    bool IsCurrentItem() const;
    /// ���������� ����� ��������
    const Page *Keeper() const { if (keeper) { return *keeper; }; return nullptr; }
    /// ���������� true, ���� � ����� ������� ����� keeper
    bool ExistKeeper(const Page *keeper) const;
    /// ����� �������� - �� �������� ������� ��������� ����
    bool HaveParent() const { return Keeper() != nullptr; };
    /// ������� ����� �� ����������� - ��������� ���� ����������.
    int PositionOnScreenX() const;
    /// ���������� ���� ������� � ������ ��������
    int PositionInKeeperList() const;
    /// ������ �����
    int Height() const;
    /// ������ �����. pos - ������� ����� �� ������.
    int Width(int pos = -1) const;
    /// ���������� ���� ���� ��������� �����
    Color ColorTitleBackground() const;
    /// ���������� ����, ������� ����� �������� �� ��������� �����
    Color ColorTitleDraw() const;
    /// ���� ������� �����
    Color ColorFrame() const;

    struct Value
    {
        static const int HEIGHT = 13;
    };

    static Item empty;

    bool Is(Type::E t) const { return type == t; };

    virtual void Draw(int /*x*/, int /*y*/, bool /*opened*/) const {};
    /// ���������� ��� "��������" ����������
    virtual void KeyRelease() const;
    /// ���������� ��� �������������� ������������ ������ (������� � ��������� ����� 0.5 ���)
    virtual void KeyAutoRelease() const;
    /// ��������� ������� ������
    virtual bool ProcessKey(KeyEvent) { return false; };
    /// ���������� ������ � �������� ��������� �������� Choice ��� Page::Name
    virtual int HeightOpened() const;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///

/// ��������� �������� ����
class Page : public Item
{
public:
    const Item * const *items;      ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                    ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB     funcOnEnterExit;    ///< ����� ���������� ��� ������� �� �������� �������� � ��� ������ �� ���� �������� �� ����������
    pFuncVV     funcOnDraw;         ///< ����� ���������� ����� ��������� ������
    pFuncBKE    funcKey;            ///< � �������� ����� ������ ���������� ��� ������� �������
    Page(uint8 name, const char * const * titleHint, const Page * const *keeper, const Item * const *items, int8 num, pFuncBV funcActive, pFuncVB funcEnterExit, pFuncVV funcDraw, pFuncBKE funcKey);
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

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    void DrawPagesUGO(int right, int bottom) const;

    void DrawNestingPage(int left, int bottom) const;
    /// true, ���� �������� ��������� ������������ �������� parent
    bool IsSubPage(const Page *parent);

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;
    /// ��������� ������� ������
    virtual bool ProcessKey(KeyEvent event);
    /// ���������� � �������� �����������
    virtual void Draw(int x, int y, bool opened) const;

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
    Button(const char * const * titleHint, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw);
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// SButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< ��������� �� ������� ��������� ����������� �������� ������
    pString     helpUGO;        ///< ������� � ������� �����������.
};


/// ��������� ������ ��� ��������������� ������ ����.
class GraphButton : public Item
{
public:
    pFuncVV                     funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                    funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpDrawButton *hintUGO; 
    int                         numHints;
    GraphButton(const char * const * titleHint, const StructHelpDrawButton *hintUGO, int num, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw);

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///

/// ��������� ���������.
class Governor : public Item
{
public:
    int16  *cell;
    int16   minValue;       ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   maxValue;       ///< ������������ ��������.
    pFuncVV funcOfChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������
    Governor(const char * const * titleHint, int16 *cell, int16 min, int16 max, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcChanged, pFuncVV funcDraw);

    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 NextValue() const;
    /// ���������� ��������� ������� ��������, ������� ����� ������� governor.
    int16 PrevValue() const;
    /// ������������ ��������� ���� ��������.
    float Step() const;
    /// �������� �������� � ������� ������� ��� ��������� ��������.
    void ChangeValue(int16 delta);
    /// ��� �������� �������� ������������ ������ �� ��������� �������.
    void NextPosition() const;
    /// ��� �������� �������� ������������ ������ �� ���������� �������
    void PrevPosition();

    void DrawOpened(int x, int y) const;

    void DrawClosed(int x, int y) const;

    void DrawValue(int x, int y) const;

    char GetSymbol() const;

    int16 GetValue() const;

    void SetValue(int16 v) const;

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;
    /// ��������� ������� ������
    virtual bool ProcessKey(KeyEvent event);

    virtual void Draw(int x, int y, bool opened) const;

private:

    void DrawLowPart(int x, int y) const;
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    uint  NumDigits() const;

    static int8 currentDigit;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
class Choice : public Item
{
public:
    int8       *cell;
    pString    *names;          ///< �������� ������.
    pFuncVB     funcOnChanged;  ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII    funcForDraw;    ///< ������� ���������� ����� ��������� ��������. 
    Choice(const char * const * titleHint, pString *names, int8 num, int8 *cell, const Page * const *keeper, pFuncBV funcActive, pFuncVB funcChanged, pFuncVII funcDraw);
    /// ��������� ������� ��������� �������� �� delta
    void  StartChange(int delta) const;
    /// ������������ ��������� ���� ��������.
    float Step() const;
    /// �������� �������� choice � ����������� �� �������� � ����� delta.
    void  ChangeIndex(int delta) const;
    /// ���������� ���������� ��������� ������ � �������� �� ������ choice
    int   NumSubItems() const { return num; };

    void  DrawOpened(int x, int y) const;

    void  DrawClosed(int x, int y) const;
    /// ���������� ��� �������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    String NameCurrentSubItem() const;
    /// ���������� ��� ���������� �������� ������ �������� choice, ��� ��� �������� � �������� ���� ���������
    const char *NameNextSubItem() const;

    const char *NamePrevSubItem() const;
    /// ���������� ��� �������� ������ �������� choice � ������� i ��� ��� �������� � �������� ���� ���������
    String NameSubItem(int i) const;
    /// ���������� ����, ������� ����� ��������� ������� ������
    static Color ColorMenuField(const Choice *choice);

    char GetSymbol();

    virtual void KeyRelease() const;

    virtual void KeyAutoRelease() const;

    virtual void Draw(int x, int y, bool opened) const;

    virtual bool ProcessKey(KeyEvent event);

    virtual int HeightOpened() const;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
class GovernorColor : public Item
{
public:
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
    pFuncVV     funcOnChanged;      ///< ��� ������� ����� �������� ����� ��������� �������� ��������.
    GovernorColor(const char * const * titleHint, ColorType *_ct, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcChanged) :
        Item(Item::Type::GovernorColor, titleHint, keeper, 0, funcActive),
        ct(_ct), funcOnChanged(funcChanged) {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    virtual int HeightOpened() const { return 27; };
private:
    void DrawOpened(int x, int y) const;
    void DrawClosed(int x, int y) const;
    void DrawValue(int x, int y) const;
    static const int widthOpened = 129;
    static const int heightOpened = 27;
};

typedef void * pVOID;
#define MAX_NUM_ITEMS_IN_PAGE 15
typedef pVOID arrayItems[MAX_NUM_ITEMS_IN_PAGE];
