#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"


extern const PageBase pageMeasures;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMeasures::pointer = &pageMeasures;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageMeasures, // -V1027                                                                                                                                       //--- ��������� ---
    "���������", "MEASURES",
    "������ � ���������� ��������� - ��������� � ��������������",
    "Access to measurement settings - cursor and automatic",
    PageMeasures::PageCursors::pointer,     ///< ��������� - �������
    PageMeasures::PageAuto::pointer,        ///< ��������� - �������
    Page::Name::Measures, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
