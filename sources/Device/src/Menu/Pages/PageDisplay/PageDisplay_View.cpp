#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Display/Display_Types.h"
#include "Settings/Settings.h"


extern const PageBase pageViewSignal;

const PageBase *PageDisplay::PageView::pointer = &pageViewSignal;


DEF_CHOICE_2( cViewMode,                                                                                                                                      //--- ������� - ����������� ---
    "�������",
    "����� ����� ����������� �������",
    "������",
    "�����",
    MODE_DRAW_SIGNAL, pageViewSignal, 0, 0, 0
)


DEF_PAGE_1( pageViewSignal,
    "�������",
    "��������� ����������� ��������",
    &cViewMode,
    Page::Name::Display_View, PageDisplay::pointer, 0, 0, 0, 0
)
