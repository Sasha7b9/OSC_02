#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/Definition.h"
#include "Display/Painter.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageFunction;

const PageBase *PageFunction::pointer = &pageFunction;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_5( pageFunction,                                                                                                                                                   //--- ������� ---
    "�������",
    "����� �������",
    PageFunction::PageFrequencyCounter::pointer,   ///< ������� - ����������
    PageFunction::PageFFT::pointer,                ///< ������� - ������
    PageFunction::PageMultimeter::pointer,         ///< ������� - ����������
    PageFunction::PageTester::pointer,             ///< ������� - ������
    PageFunction::PageRecorder::pointer,           ///< ������� - �����������
    Page::Name::Function, nullptr, 0, Page::FuncPress, Page::FuncDraw, Page::FuncRegSet
)
