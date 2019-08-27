#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"


extern const Page pageMeasures;

const Page * const PageMeasures::self = (const Page *)&pageMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pageMeasures, // -V1027                                                                                                                                       //--- ��������� ---
    "���������",
    "������ � ���������� ��������� - ��������� � ��������������",
    PageMeasuresCursors::self,     ///< ��������� - �������
    PageMeasuresAuto::self,        ///< ��������� - �������
    PageMeasuresMath::self,        ///< ��������� - ����������
    Page::Name::Measures, nullptr, 0, 0, 0, 0
)
