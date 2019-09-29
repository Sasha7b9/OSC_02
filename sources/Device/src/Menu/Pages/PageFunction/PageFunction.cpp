#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ������� ---
    "�������",
    "����� �������",
    PageFreqMeter::self,        ///< ������� - ����������
    PageFFT::self,              ///< ������� - ������
    PageMultimeter::self,       ///< ������� - ����������
    PageTester::self,           ///< ������� - ������
    PageRecorder::self,         ///< ������� - �����������
    PageName::Function, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageFunction::self = (const Page *)&pFunction;
