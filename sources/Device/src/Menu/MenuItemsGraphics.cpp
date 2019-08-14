#include "defines.h"
#include "log.h"
#include "MenuItems.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Menu/Menu.h"
#include "Utils/Values.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>

#ifdef WIN32
#include <cstdio>
#endif


using namespace Display::Primitives;
using namespace Hardware;

using Utils::Stack;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Item *item, int x, int y, bool opened);
/// ���������� �������� ���������� � ������ ����������� �����������
/// setPosFromEnd - ������������ (��������) ������, ������� � �����. ���� selPosFromEnd == -1, ������������ �� �����
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool fillNull);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////q
void GovernorColor::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawOpened(int x, int y)
{
    static const int delta = 43;
    x -= delta;
    ct->Init();
    Rectangle(Height() + delta + 2, Height() + 2).Draw(x - 1, y - 1, Color::BACK);
    Rectangle(Width() + delta, Height()).Draw(x, y, Color::MenuTitleText());
    Region(Value::WIDTH + 2 + delta, Item::Value::HEIGHT + 3).Fill(x + 1, y + 1, Color::MenuItem());

    HLine(Width() + delta).Draw(x, y + Height() / 2 + 2, Color::MenuTitleText());

    Text(Title().CString()).DrawInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), Width() + delta, Height() / 2 + 2, Color::WHITE);

    DrawValue(x + 1, y + 19, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawClosed(int x, int y)
{
    ct->Init();
    DrawCommonHiPart(this, x, y, false);
    Region(Item::Value::WIDTH + 1, Item::Value::HEIGHT - 3).Fill(x + 1, y + 13, ct->color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawValue(int x, int y, int delta)
{
    int8 field = ct->currentField;
    const pString texts[4] = {"��", "��", "��", "��"};

    uint color = COLOR(ct->color.value);
    uint red = R_FROM_COLOR(color);
    uint green = G_FROM_COLOR(color);
    uint blue = B_FROM_COLOR(color);
    ct->Init();
    int16 vals[4] = {(int16)(ct->brightness * 100.0F), (int16)blue, (int16)green, (int16)red};

    Region(Width() + delta - 2, Height() / 2 - 3).Fill(x, y, Color::BLACK);
    x += 92;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(29, 10).Fill(x - 1, y + 1, colorBack);
        String(texts[i]).Draw(x, y + 2, colorDraw);
        Integer(vals[i]).ToString(false, 1).Draw(x + 14, y + 2);
        x -= 30;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::Draw(int x, int y, bool opened)
{
    if (funcBeforeDraw)
    {
        funcBeforeDraw();
    }
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawOpened(int x, int y)
{
    DrawCommonHiPart(this, x, y, true);
    DrawLowPart(x, y + 13);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawClosed(int x, int y)
{
    DrawLowPart(x, y + 14);
    DrawCommonHiPart(this, x, y, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawValue(int x, int y)
{
    int startX = x + 40;
    int signGovernor = (GetValue() < 0) ? -1 : 1;
    
    int value = GetValue();
    
    
    if (signGovernor == -1)
    {
        value = -value;
    }
    
    Font::SetCurrent(Font::Type::_5);
    bool sign = (minValue < 0);
    
    //if(maxValue < 65536)
    {
        Integer(maxValue).ToString(sign, 1).Draw(x + 55, y - 5, Color::WHITE);
        Integer(minValue).ToString(sign, 1).Draw(x + 55, y + 2);
    }
    
    Font::SetCurrent(Font::Type::_8);

    DrawValueWithSelectedPosition(x + 2, y, value, NumDigits(), gCurDigit, true);

    if (sign)
    {
        Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 30, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawLowPart(int x, int y)
{
    Color colorTextDown = Color::BLACK;

    Region(Value::WIDTH + 1, Value::HEIGHT - 3).Fill(x + 1, y - 1, Color::MenuItemField());

    if (Menu::OpenedItem() != this)
    {
        if (IsCurrentItem())
        {
            Char(GetSymbol()).Draw4SymbolsInRect(x + Width() - 12, y - 1, Color::BACK);
        }
    
        x = Char(SYMBOL_GOVERNOR_LEFT).Draw(x + 4, y, colorTextDown);
        
        int delta = (int)Step();
        if (delta == 0)
        {
            x = Integer(GetValue()).ToString(false, 1).Draw(x + 1, y);
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y - 2;
            int limWidth = Value::WIDTH;
            int limHeight = Value::HEIGHT - 1;
            Color::BLACK.SetAsCurrent();
            if (delta > 0)
            {
				x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
				Text(Integer(NextValue()).ToString(false, 1)).DrawWithLimitation(drawX, y + 10 - delta, limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
				x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
				Text(Integer(PrevValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - 10 - delta, limX, limY, limWidth, limHeight);
            }
        }
        
        Char(SYMBOL_GOVERNOR_RIGHT).Draw(x + 1, y, colorTextDown);
    }
    else
    {
        DrawValue(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawOpened(int x, int y)
{
    int height = HeightOpened();
    
    Rectangle(Width(), height).Draw(x, y, Color::FILL);
    DrawCommonHiPart(this, x, y, true);

    Region(Width() - 2, height - MOI_HEIGHT_TITLE + 4).Fill(x + 1, y + MOI_HEIGHT_TITLE - 5, Color::BACK);
    int8 index = *cell;
    for (int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Region(Width() - 2, MOSI_HEIGHT - 1).Fill(x + 1, yItem + 2, ColorMenuField(this));
        }
        NameSubItem(i).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : ColorMenuField(this));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawClosed(int x, int y)
{
    Region(Item::Value::WIDTH + 2, Item::Value::HEIGHT - 3).Fill(x + 1, y + Item::Value::HEIGHT, ColorMenuField(this));

    int deltaY = (int)Step();
    Color colorText = Color::BLACK;
    colorText.SetAsCurrent();
    if (deltaY == 0)
    {
        NameCurrentSubItem().Draw(x + 4, y + Item::Value::HEIGHT + 1);
    }
    else
    {
        Color::BACK.SetAsCurrent();
		Text(NameCurrentSubItem()).DrawWithLimitation(x + 4, y + Item::Value::HEIGHT - deltaY + 1, x, y + 11, Item::Value::WIDTH, Item::Value::HEIGHT - 1);

        HLine(Item::Value::WIDTH + 1).Draw(x + 1, y + (deltaY > 0 ? 24 : 19) - deltaY);

		Text(deltaY > 0 ? NameNextSubItem() : NamePrevSubItem()).DrawWithLimitation(x + 4, y + (deltaY > 0 ? (Item::Value::HEIGHT + 13) : 9) - deltaY, x, y + 11,
            Item::Value::WIDTH, Item::Value::HEIGHT - 1);
    }
  
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
    
    DrawCommonHiPart(this, x, y, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y) const
{
    Color color = Color::WHITE;
    
    Region(Width() - 2, Height() - 2).Fill(x + 1, y + 2, Color::MenuItem());
    Region(Width() - 6, Height() - 6).Fill(x + 3, y + 4, Color::MenuItem());

    int delta = IsPressed() ? 2 : 1;

    Text(Title().CString()).DrawInCenterRect(x + delta, y + delta, Width(), Height(), color);

    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphButton::Draw(int x, int y) const
{
    x += 2;
    y += 3;
    
    if (IsActive())
    {
        if (IsPressed())
        {
            Region(GraphButton::Width() - 5, GraphButton::Height() - 4).Fill(x, y, Color::FILL);
            Color::BLACK.SetAsCurrent();
        }
        else
        {
            Color::FILL.SetAsCurrent();
        }
        funcForDraw(x + 20, y);
    }
    else
    {
        Color::FILL.SetAsCurrent();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        if (CurrentItemIsOpened())
        {
            Item *item = GetItem(PosCurrentItem());

            x = ItemOpenedPosX(item);
            y = Menu::Y0() - item->HeightOpened() + Item::Height() + 1;

            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(x, y, true);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(x, y, true);
            }
            else if (IS_GOVERNOR_COLOR(item))
            {
                ((GovernorColor *)item)->Draw(x, y, true);
            }
            else if (IS_TIME(item))
            {
                ((TimeItem *)item)->Draw(x, y, true);
            }
            else
            {
                // ��������� �������� �� ������������
            }
        }
        else
        {
            DrawTitle(x, y - Menu::Title::HEIGHT);

            DrawItems(x, y);
        }

        if (funcOnDraw)
        {
            funcOnDraw();
        }
    }
    else
    {
        //Region(Width() - 3, Height() - 2).Fill(x + 1, y + 2, Color::MenuItem(false));
        if (IsPressed())
        {
            Region(Width() - 5, Height() - 4).Fill(x + 2, y + 3, Color::FILL);
        }
        Text(Title().CString()).DrawInCenterRect(x, y + 1, Width(), Height(), IsPressed() ? Color::BACK : Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawTitle(int x, int yTop) const
{
    int eX = x;

    if (isPageSB)
    {
        GRAPH_BUTTON_FROM_PAGE(this, 0)->Draw(x, yTop + 3);
        return;
    }

    bool shade = CurrentItemIsOpened();

    Region(Menu::Title::WIDTH + 2, Menu::Title::HEIGHT + 2).Fill(x - 1, yTop, Color::BACK);

    Rectangle(Menu::Title::WIDTH + 1, Menu::Title::HEIGHT + 1).Draw(x, yTop, Color::FILL);

    if (shade)
    {
        Region(Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MENU_TITLE_DARK);
        Region(Menu::Title::WIDTH - 7, Menu::Title::HEIGHT - 7).Fill(x + 4, yTop + 4, Color::MENU_TITLE_DARK);
    }
    else
    {
        Region(Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MenuTitleText());
    }

    bool condDrawRSet = NumSubPages() > 1 &&
        NOT_CHOICE_REG(Menu::CurrentItem()) &&
        NOT_GOVERNOR(Menu::CurrentItem()) &&
        IS_PAGE(Menu::OpenedItem());

    int delta = condDrawRSet ? -10 : 0;
    Color colorText = Color::FILL;

    x = Text(Title().CString()).DrawInCenterRect(x, yTop, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, colorText);

    Color::GRAY_75.SetAsCurrent();
    DrawPagesUGO(eX + Menu::Title::WIDTH - 3, yTop + Menu::Title::HEIGHT);
    DrawNestingPage(eX + 5, yTop + Menu::Title::HEIGHT - 6);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawItems(int x, int y) const
{
    for (int i = 0; i < 5; i++)
    {
        Rectangle(Width(i) - 1, Height()).Draw(x, y + 1, Color::FILL);
        Region(Width(i) - 3, Height() - 2).Fill(x + 1, y + 2, Color::BACK);
        
        Item *item = GetItem(PosItemOnLeft() + i);
        
        if (item)
        {
            if (item->IsActive())
            {
                item->Draw(x, y, false);
            }
        }

        x += Width(i) - 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Draw(int x, int y, bool opened) const
{
    if (IS_CHOICE(this) || IS_CHOICE_REG(this))
    {
        ((Choice *)this)->Draw(x, y, opened);
    }
    else if (IS_BUTTON(this))
    {
        ((Button *)this)->Draw(x, y);
    }
    else if (IS_PAGE(this))
    {
        ((Page *)this)->Draw(x, y, opened);
    }
    else if (IS_GOVERNOR(this))
    {
        ((Governor *)this)->Draw(x, y, opened);
    }
    else if (IS_TIME(this))
    {
        ((TimeItem *)this)->Draw(x, y, opened);
    }
    else if (IS_GOVERNOR_COLOR(this))
    {
        ((GovernorColor *)this)->Draw(x, y, opened);
    }
    else if (IS_GRAPH_BUTTON(this))
    {
        ((GraphButton *)this)->Draw(x, y);
    }
    else
    {
        // ��������� ���� ��������� �� ������������
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::Draw(int x, int y, bool opened)
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::DrawClosed(int x, int y)
{
    DrawCommonHiPart(this, x, y, false);

    Region(Item::Value::WIDTH + 2, Item::Value::HEIGHT + 3).Fill(x + 1, y + 17, Color::MenuItemField());

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = Clock::GetTime();
    Integer((int)time.hours).ToString(false, 2).Draw(x + startX, y, Color::BLACK);
    String(':').Draw(x + startX + deltaField, y);
    Integer((int)time.minutes).ToString(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    String(':').Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Integer((int)time.seconds).ToString(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);

    startX = 44;
    Integer((int)time.day).ToString(false, 2).Draw(x + startX, y);
    String(':').Draw(x + startX + deltaField, y);
    Integer((int)time.month).ToString(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    String(':').Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Integer((int)time.year).ToString(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::DrawOpened(int x, int y)
{
    int width = Item::Value::WIDTH + 3;
    int height = 61;
    Rectangle(width + 2, height + 3).Draw(x - 1, y - 1, Color::BACK);
    DrawCommonHiPart(this, x - 1, y - 1, false);

    Rectangle(width + 1, height + 1).Draw(x - 1, y, Color::MenuTitleText());

    HLine(Width()).Draw(x, y + MOI_HEIGHT_TITLE - 1);

    Region(Width() - 1, height - MOI_HEIGHT_TITLE).Fill(x, y + MOI_HEIGHT_TITLE, Color::BLACK);

    int y0 = 21;
    int y1 = 31;

    typedef struct
    {
        int x;
        int y;
        int width;
    } StructPaint;

    int y2 = 41;
    int y3 = 51;
    int dX = 13;
    int wS = 10;
    int x0 = 41;
    StructPaint strPaint[8] =
    {
        {3,             y3, 60},    // �� ���������
        {x0,            y0, wS},    // ����
        {x0 + dX,       y0, wS},    // �����
        {x0 + 2 * dX,   y0, wS},    // ���
        {x0,            y1, wS},    // ����
        {x0 + dX,       y1, wS},    // ���
        {x0 + 2 * dX,   y1, wS},    // ���
        {3,             y2, 46}     // ���������
    };

    char strI[8][20];
    std::strcpy(strI[iEXIT],  "�� ���������");
    std::strcpy(strI[iDAY],   Integer(*day).ToString(false, 2).CString());
    std::strcpy(strI[iMONTH], Integer(*month).ToString(false, 2).CString());
    std::strcpy(strI[iYEAR],  Integer(*year).ToString(false, 2).CString());
    std::strcpy(strI[iHOURS], Integer(*hours).ToString(false, 2).CString());
    std::strcpy(strI[iMIN],   Integer(*minutes).ToString(false, 2).CString());
    std::strcpy(strI[iSEC],   Integer(*seconds).ToString(false, 2).CString());
    std::strcpy(strI[iSET],   "���������");

    String("� � � - ").Draw(x + 3, y + y0, Color::WHITE);
    String("� � � - ").Draw(x + 3, y + y1);

    for (int i = 0; i < 8; i++)
    {
        if (*curField == i)
        {
            Region(strPaint[i].width, 8).Fill(x + strPaint[i].x - 1, y + strPaint[i].y, Color::FLASH_10);
        }
        String(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, *curField == i ? Color::FLASH_01 : Color::WHITE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Item *item, int x, int y, bool opened)
{
    int width = Item::Value::WIDTH;

    Color color = Color::WHITE;

    Region(width + (opened ? 2 : 1), Item::Value::HEIGHT - (opened ? 2 : 3)).Fill(x + 1, y + (opened ? 1 : 2), Color::MenuItem());

    item->Title().Draw(x + (opened ? 4 : 6), y + (opened ? 2 : 3), color);

    if (opened)
    {
        HLine(width + 2).Draw(x + 1, y + Item::Value::HEIGHT, Color::FILL);
    }

    if(item->IsCurrentItem())
    {
        char symbol = 0;
   
        if (IS_CHOICE_REG(item) || (item->IsOpened() && IS_CHOICE(item)))
        {
            symbol = item->GetSymbol();
        }
        else if (IS_TIME(item))
        {
            TimeItem* time = (TimeItem*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                symbol = time->GetSymbol();
            }
        }
        else
        {
            // ��� ��������� ��������� �� �����
        }

        Char(symbol).Draw4SymbolsInRect(x + item->Width() - 13, y + (item->IsOpened() ? 0 : 13), Color::BLACK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool)
{
    Stack<uint8> stack(numDigits);
    
    for(uint i = 0; i < numDigits; i++)
    {
        stack.Push((uint8)(value % 10));
        value /= 10;
    }
    
    int height = 8;
    
    Color fill = Color::BACK;
    Color back = Color::FILL;
    
    
    for (uint i = 0; i < numDigits; i++)
    {
        if (selPosFromEnd == ((int)numDigits - (int)i - 1))
        {
            Region(5, height).Fill(x - 1, y, back);
        }
        
        uint8 val = stack.Pop();
        
        Char((char)(val + 48)).Draw(x, y, fill);
        
        x += 6;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::ItemOpenedPosX(const Item *item)
{
    const Page *page = item->Keeper();

    if (page)
    {
        return (page->PosCurrentItem() % MENU_ITEMS_ON_DISPLAY) * (320 / 5 + 1);
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawPagesUGO(int right, int bottom) const
{
    int size = 4;
    int delta = 2;

    int allPages = (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
    int currentPage = CurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for (int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if (p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawNestingPage(int left, int bottom) const
{
    Page *parent = (Page *)Keeper();

    if (parent == nullptr)
    {
        return;
    }

    int nesting = 0;

    while (parent != nullptr)
    {
        const Page *page = (Page *)parent;
        parent = (Page *)page->Keeper();
        nesting++;                                  // -V127
    }

    int size = 4;
    int delta = 2;

    for (int i = 0; i <= nesting; i++)
    {
        int x = left + i * (size + delta);
        Rectangle(size, size).Draw(x, bottom);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphButton::DrawHints(int x, int y, int width) const
{
    if (numHints == 0)
    {
        return;
    }
    Region(width, 239 - y).Fill(x, y, Color::BACK);
    Rectangle(width, 239 - y).Draw(x, y, Color::FILL);
    const StructHelpDrawButton *structHelp = &hintUGO[0];
    x += 3;
    y += 3;
    for (int i = 0; i < numHints; i++)
    {
        Rectangle(GraphButton::Width(), GraphButton::Height()).Draw(x, y);
        structHelp->funcDrawUGO(x, y);

        int yNew = Text(structHelp->helpUGO).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);

        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}
