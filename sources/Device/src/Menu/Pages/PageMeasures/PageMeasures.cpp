#include "defines.h"
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Menu.h"


extern const Page pMeasures;

const Page * const PageMeasures::self = (const Page *)&pMeasures;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_3( pMeasures, // -V1027                                                                                                                                          //--- ��������� ---
    "���������",
    "������ � ���������� ��������� - ��������� � ��������������",
    PageMeasuresCursors::self,     ///< ��������� - �������
    PageMeasuresAuto::self,        ///< ��������� - �������
    PageMeasuresMath::self,        ///< ��������� - ����������
    PageName::Measures, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)
