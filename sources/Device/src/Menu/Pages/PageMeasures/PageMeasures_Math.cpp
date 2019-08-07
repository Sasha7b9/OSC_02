#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Settings/SettingsMath.h"


extern const PageBase pageMath;


const PageBase *PageMeasures::PageMath::pointer = &pageMath;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cFunction,
    "�������",
    "����� �������������� �������",
    "�����",
    "������������",
    MATH_FUNC, pageMath, 0, Choice::FuncChange, Choice::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_1( pageMath, //-V641 //-V1027
    "����������",
    "�������������� �������",
    &cFunction,
    Page::Name::Measures_Math, PageMeasures::pointer, 0, Page::FuncPress, Page::FuncDraw, Page::FuncRegSet
)
