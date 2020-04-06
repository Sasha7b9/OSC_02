#include "defines.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include "Hardware/Beeper.h"
#include "Display/Painter.h"



DEF_PAGE_5( pFunction,                                                                                                                                                      //--- ������� ---
    "�������",
    "����� �������",
    PageFreqMeter::self,
    PageFFT::self,
    PageMultimeter::self,
    PageTester::self,
    PageRecorder::self,
    PageName::Function, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFunction::self = static_cast<const Page *>(&pFunction);
