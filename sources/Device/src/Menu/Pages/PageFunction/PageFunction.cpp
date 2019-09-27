#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ������� ---
    "�������",
    "����� �������",
    PageFrequencyCounter::self,   ///< ������� - ����������
    PageFFT::self,                ///< ������� - ������
    PageMultimeter::self,         ///< ������� - ����������
    PageTester::self,             ///< ������� - ������
    PageRecorder::self,           ///< ������� - �����������
    PageName::Function, nullptr, Item::EmptyActive, E_VB, Page::EmptyBeforeDraw, E_BfKE
)

const Page * const PageFunction::self = (const Page *)&pFunction;
