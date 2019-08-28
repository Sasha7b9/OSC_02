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
    PageName::Measures, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)
