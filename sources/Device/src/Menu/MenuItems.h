#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


/*
    ��������� ���� XxxDef ���������� ������ ��� ������������� ��������
*/

#define GRAPH_BUTTON_FROM_PAGE(page, numButton)     ((GraphButton *)((Page *)page)->items[numButton])

#define MENU_ITEMS_ON_DISPLAY       5   ///< ������� ������� ���� ���������� �� ������ �� �����������.

#define IS_PAGE(item)           (item->type == Item::Type::Page)
#define NOT_PAGE(item)          (item->type != Item::Type::Page)
#define IS_PAGE_SB(item)        (item->isPageSB)
#define IS_CHOICE(item)         (item->type == Item::Type::Choice)
#define IS_GOVERNOR(item)       (item->type == Item::Type::Governor)
#define NOT_GOVERNOR(item)      (item->type != Item::Type::Governor)
#define IS_GOVERNOR_COLOR(item) (item->type == Item::Type::GovernorColor)
#define IS_BUTTON(item)         (item->type == Item::Type::Button)
#define IS_GRAPH_BUTTON(item)   (item->type == Item::Type::GraphButton)

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
    
    uint8               type;            ///< ��� �����
    int8                num;             ///< ����� ��������� ��� Choice ��� ����� ��������� ��� Page
    uint8               name;            ///< ��� �� ������������ Page::Name
    const Page *const  *keeper;          ///< ����� ��������, ������� �����������. ��� Page_Main = 0
    pFuncBV             funcOfActive;    ///< ������� �� ������ �������
    const char * const *titleHint;       ///< �������� ��������. ����� ��������� ��� ������ ������
    Item(uint8 _type = Item::Type::None, const char * const *_titleHint = nullptr, const Page *const *_keeper = nullptr, int8 _num = 0, pFuncBV funcActive = nullptr) :
        type(_type), num(_num), keeper(_keeper), funcOfActive(funcActive), titleHint(_titleHint)
    {
    };
    /// ���������� ������ � �������� ��������� �������� Choice ��� Page::Name
    int HeightOpened() const;
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
    virtual void KeyRelease() const;
    /// ���������� ��� �������������� ������������ ������ (������� � ��������� ����� 0.5 ���)
    void KeyAutoRelease() const;
    /// ���������� true, ���� ������� ��������� � �������� ��������� (��������� �� ������ ����������)
    bool IsActive() const { if (funcOfActive) { return funcOfActive(); } return true; };

    virtual void Draw(int /*x*/, int /*y*/, bool /*opened*/) const {};

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
    Page(const char * const * titleHint, const Page * const *keeper, const Item * const *_items, int8 num, pFuncBV funcActive, pFuncVB funcEnterExit, pFuncVV funcDraw, pFuncBKE _funcKey) :
        Item(Item::Type::Page, titleHint, keeper, num, funcActive), items(_items), funcOnEnterExit(funcEnterExit), funcOnDraw(funcDraw), funcKey(_funcKey) {};
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
    virtual void Draw(int x, int y, bool opened) const;
    /// ��������� ������� ������
    bool ProcessKey(KeyEvent event);

    void DrawTitle(int x, int y) const;

    void DrawItems(int x, int y) const;

    void DrawPagesUGO(int right, int bottom) const;

    void DrawNestingPage(int left, int bottom) const;
    /// true, ���� �������� ��������� ������������ �������� parent
    bool IsSubPage(const Page *parent);

    virtual void KeyRelease() const;

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
    Button(const char * const * titleHint, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw) :
        Item(Item::Type::Button, titleHint, keeper, 0, funcActive),
        funcOnPress(funcPress), funcForDraw(funcDraw)
    {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    void KeyAutoRelease() const;
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
    GraphButton(const char * const * titleHint, const StructHelpDrawButton *_hintUGO, int num, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcPress, pFuncVII funcDraw) :
        Item(Item::Type::GraphButton, titleHint, keeper, 0, funcActive),
        funcOnPress(funcPress), funcForDraw(funcDraw), hintUGO(_hintUGO), numHints(num) {};

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void KeyRelease() const;
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
    Governor(const char * const * titleHint, int16 *_cell, int16 min, int16 max, const Page * const *keeper, pFuncBV funcActive, pFuncVV funcChanged, pFuncVV funcDraw) :
        Item(Item::Type::Governor, titleHint, keeper, 0, funcActive),
        cell(_cell), minValue(min), maxValue(max), funcOfChanged(funcChanged), funcBeforeDraw(funcDraw) {};
    /// ��������� ������� ������
    bool ProcessKey(KeyEvent event);
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

    virtual void Draw(int x, int y, bool opened) const;

    void DrawOpened(int x, int y) const;

    void DrawClosed(int x, int y) const;

    void DrawValue(int x, int y) const;

    char GetSymbol() const;

    int16 GetValue() const;

    void SetValue(int16 v) const;

    virtual void KeyRelease() const;

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
    Choice(const char * const * titleHint, pString *_names, int8 num, int8 *_cell, const Page * const *keeper, pFuncBV funcActive, pFuncVB funcChanged, pFuncVII funcDraw) :
        Item(Item::Type::Choice, titleHint, keeper, num, funcActive),
        cell(_cell), names(_names), funcOnChanged(funcChanged), funcForDraw(funcDraw) {};
    /// ��������� ������� ��������� �������� �� delta
    void  StartChange(int delta) const;
    /// ������������ ��������� ���� ��������.
    float Step() const;
    /// �������� �������� choice � ����������� �� �������� � ����� delta.
    void  ChangeIndex(int delta) const;
    /// ���������� ���������� ��������� ������ � �������� �� ������ choice
    int   NumSubItems() const { return num; };

    bool ProcessKey(KeyEvent event);

    virtual void  Draw(int x, int y, bool opened) const;

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
    /// �������� ������� funcOnChanged, ���� ������� �������
    void Change(bool active) const { if (funcOnChanged) { funcOnChanged(active); } }

    virtual void KeyRelease() const;
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
