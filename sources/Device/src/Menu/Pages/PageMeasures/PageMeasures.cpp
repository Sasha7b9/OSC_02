#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Menu.h"


extern const PageBase pageMeasures;

const Page * const PageMeasures::self = (const Page * const)&pageMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ��������� ---
    "���������",
    "������ � ���������� ��������� - ��������� � ��������������",
    PageMeasures::PageCursors::self,     ///< ��������� - �������
    PageMeasures::PageAuto::self,        ///< ��������� - �������
    PageMeasures::PageMath::self,        ///< ��������� - ����������
    Page::Name::Measures, nullptr, 0, 0, 0, 0
)
