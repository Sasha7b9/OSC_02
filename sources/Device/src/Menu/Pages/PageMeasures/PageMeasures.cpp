#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"


extern const PageBase pageMeasures;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMeasures::pointer = &pageMeasures;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ��������� ---
    "���������",
    "������ � ���������� ��������� - ��������� � ��������������",
    PageMeasures::PageCursors::pointer,     ///< ��������� - �������
    PageMeasures::PageAuto::pointer,        ///< ��������� - �������
    PageMeasures::PageMath::pointer,        ///< ��������� - ����������
    Page::Name::Measures, nullptr, FuncActive, FuncPressPage, Page::FuncDraw, FuncRegSetPage
)
