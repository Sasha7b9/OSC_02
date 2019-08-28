#pragma once
#include "Display/Display.h"
#include "Display/Colors.h"
#include "MenuItemsDefs.h"


class Page;


struct DataItem
{
    uint8               type;           ///< ��� �����
    const char         *title;          ///< ��������� �����
    const char         *hint;           ///< ��������� ��� ������ ������
    const Page * const *keeper;         ///< ����� ��������, ������� �����������. ��� Page_Main = 0
    pFuncBV             funcOfActive;   ///< ������� �� ������ �������
    const void         *ad;             ///< ��������� �� ��������� � �������, �������������� ��� ������� ��������� Item
};


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

    const DataItem *data;
    
    Item(const DataItem * const data = nullptr);
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
    bool IsActive() const { if (data->funcOfActive) { return data->funcOfActive(); }; return true; };

    bool IsCurrentItem() const;
    /// ���������� ����� ��������
    const Page *Keeper() const { if (data->keeper == nullptr) { return nullptr; } return *data->keeper; }
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

    bool Is(Type::E t) const { return data->type == t; };

    virtual void Draw(int /*x*/, int /*y*/, bool /*opened*/) const {};
    /// ���������� ��� "��������" ����������
    virtual void KeyRelease() const;
    /// ���������� ��� �������������� ������������ ������ (������� � ��������� ����� 0.5 ���)
    virtual void KeyAutoRelease() const;
    /// ��������� ������� ������
    virtual bool ProcessKey(KeyEvent) { return false; };
    /// ���������� ������ � �������� ��������� �������� Choice ��� Page::Name
    virtual int HeightOpened() const;

    static Item empty;

    static DataItem emptyData;
};


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Page ///
struct DataPage
{
    uint8               name;               ///< ��� �� ������������ Page::Name
    const Item * const *items;              ///< ����� ��������� �� ������ ���� �������� (� ������� ��������)
                                            ///< ��� �������� ����� ������  ����� �������� 6 ���������� �� SButton : 0 - K_Enter, 1...5 - K_1...K_5
    pFuncVB             funcOnEnterExit;    ///< ����� ���������� ��� ������� �� �������� �������� � ��� ������ �� ���� �������� �� ����������
    pFuncVV             funcOnDraw;         ///< ����� ���������� ����� ��������� ������
    pFuncBKE            funcOnKey;          ///< � �������� ����� ������ ���������� ��� ������� �������

    void FuncOnDraw() { if (funcOnDraw) { funcOnDraw(); } };
    void FuncOnEnterExit(bool enter) { if (funcOnEnterExit) { return funcOnEnterExit(enter); } };
    bool FuncOnKey(KeyEvent event) { if (funcOnKey) { return funcOnKey(event); }; return false; };
};

/// ��������� �������� ����
class Page : public Item
{
public:
    Page(const DataItem * const data) : Item(data) {};
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
    /// ���������� ��������� �� ������, ����������� ��� ����� ������
    DataPage *OwnData() const { return (DataPage *)data->ad; }

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
            Memory_Last,                ///< ������ - ���������
            Memory_Internal,            ///< ������ - ����� ��
            Memory_Drive,               ///< ������ - ����� ��
            Memory_Drive_Manager,       ///< ������ - ����� �� - �������
            Memory_Drive_Mask,          ///< ������ - ����� �� - �����
            Memory_SetName,             ///< \brief ���������� ����� ����������� ����� �� ������ ��� ������� ����� ����� ��� ��������������� ����� 
                                        ///< ����� �� - ��� �����
            Service,                    ///< ������
            Service_Calibrator,         ///< ������ - ����������
            Service_Ethernet,           ///< ������ - ETHERNET
            Service_RTC,                ///< ������ - �����
            Service_Battery,            ///< ������ - �������
            Service_FFT_Cursors,        ///< ������ - ������ - �������
            Service_Function,           ///< ������ - �������
            Service_Information,        ///< ������ - ����������
            Help,                       ///< ������
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
            Debug_Settings,             ///< ������� - ���������
            Debug_SerialNumber,         ///< ������� - �/�
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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Button ///
struct DataButton
{
    pFuncVV     funcOnPress;        ///< �������, ������� ���������� ��� ������� �� ������.
    pFuncVII    funcForDraw;        ///< ������� ����� ���������� �� ����� ��������� ������.
    void FuncOnPress() { if (funcOnPress) funcOnPress(); }
    void FuncForDraw(int x, int y) { if (funcForDraw) funcForDraw(x, y); }
};

class Button : public Item
{
public:
    Button(const DataItem * const data) : Item(data) {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    DataButton *OwnData() const { return (DataButton *)data->ad; }
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GraphButton ///
struct StructHelpDrawButton
{
    pFuncVII    funcDrawUGO;    ///< ��������� �� ������� ��������� ����������� �������� ������
    pString     helpUGO;        ///< ������� � ������� �����������.
};

struct DataGraphButton
{
    pFuncVV                     funcOnPress;    ///< ��� ������� ��������� ��� ��������� ������� ������.
    pFuncVII                    funcForDraw;    ///< ��� ������� ���������� ��� ��������� ������ � ����� � ������������ x, y.
    const StructHelpDrawButton *hintUGO;

    void FuncOnPress() { if (funcOnPress) funcOnPress(); }
    void FuncForDraw(int x, int y) { if (funcForDraw) funcForDraw(x, y); }
};

/// ��������� ������ ��� ��������������� ������ ����.
class GraphButton : public Item
{
public:
    GraphButton(const DataItem * const data) : Item(data) {};

    virtual void Draw(int x, int y, bool opened) const;
    void DrawHints(int x, int y, int width) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    DataGraphButton *OwnData() const { return (DataGraphButton *)data->ad; }
    int NumHints() const;
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Governor ///
struct DataGovernor
{
    int16  *cell;
    int16   min;            ///< ���������� ��������, ������� ����� ��������� ���������.
    int16   max;            ///< ������������ ��������.
    pFuncVV funcOnChanged;  ///< �������, ������� ����� �������� ����� ����, ��� �������� ���������� ����������.
    pFuncVV funcBeforeDraw; ///< �������, ������� ���������� ����� ����������

    void FuncOnChanged() { if (funcOnChanged) funcOnChanged(); }
    void FuncBeforeDraw() { if (funcBeforeDraw) funcBeforeDraw(); }
};

/// ��������� ���������.
class Governor : public Item
{
public:
    Governor(const DataItem * const data) : Item(data) {};
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

    DataGovernor *OwnData() const { return (DataGovernor *)data->ad; }

private:

    void DrawLowPart(int x, int y) const;
    /// ���������� ����� ��������� � ���� ��� ����� �������� governor. ������� �� ������������� ��������, ������� ����� ��������� governor.
    uint  NumDigits() const;

    static int8 currentDigit;
};


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// Choice ///
struct DataChoice
{
    int8       *cell;
    pString    *names;          ///< �������� ������.
    pFuncVB     funcOnChanged;  ///< ������� ������ ���������� ����� ��������� �������� ��������.
    pFuncVII    funcForDraw;    ///< ������� ���������� ����� ��������� ��������. 

    void FuncOnChanged(bool active) { if (funcOnChanged) funcOnChanged(active); }
    void FuncForDraw(int x, int y) { if (funcForDraw) funcForDraw(x, y); }
};

class Choice : public Item
{
public:
    Choice(const DataItem * const data) : Item(data) {};
    /// ��������� ������� ��������� �������� �� delta
    void  StartChange(int delta) const;
    /// ������������ ��������� ���� ��������.
    float Step() const;
    /// �������� �������� choice � ����������� �� �������� � ����� delta.
    void  ChangeIndex(int delta) const;
    /// ���������� ���������� ��������� ������ � �������� �� ������ choice
    int   NumChoices() const;

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

    DataChoice *OwnData() const { return (DataChoice *)data->ad; }
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// GovernorColor ///
struct DataGovernorColor
{
    ColorType  *ct;                 ///< ��������� ��� �������� �����.
};

class GovernorColor : public Item
{
public:
    GovernorColor(const DataItem * const data) : Item(data) {};
    virtual void Draw(int x, int y, bool opened) const;
    virtual void KeyRelease() const;
    virtual void KeyAutoRelease() const;
    virtual int HeightOpened() const { return 27; };
    DataGovernorColor *OwnData() const { return (DataGovernorColor *)data->ad; }
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
