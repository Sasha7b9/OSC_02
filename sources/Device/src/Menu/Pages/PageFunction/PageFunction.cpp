#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const Page pageFunction;

const Page * const PageFunction::self = (const Page *)&pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                                                                   //--- ������� ---
    "�������",
    "����� �������",
    PageFrequencyCounter::self,   ///< ������� - ����������
    PageFFT::self,                ///< ������� - ������
    PageMultimeter::self,         ///< ������� - ����������
    PageTester::self,             ///< ������� - ������
    PageRecorder::self,           ///< ������� - �����������
    PageName::Function, nullptr, E_BtV, E_VB, E_VV, E_BfKE
)
