#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Multimeter/Multimeter.h"


extern const PageBase pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageService::PageDebug::PageMultimeter::pointer = &pMultimeter;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_Calibrate0()
{
    //Multimeter::Calibrate(0);
}

DEF_BUTTON( bCalibrate0,
    "������ 0", "Calibrate 0",
    "",
    "",
    pMultimeter, FuncActive, OnPress_Calibrate0, Button::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnPress_Calibrate1()
{
    //Multimeter::Calibrate(1);
}

DEF_BUTTON(bCalibrate1,
    "������ 1", "Calibrate 1",
    "",
    "",
    pMultimeter, FuncActive, OnPress_Calibrate1, Button::FuncDraw
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2(pMultimeter, // -V641 //-V1027 //-V641
    "����������", "MULTIMETER",
    "",
    "",
    &bCalibrate0,
    &bCalibrate1,
    Page::Name::Debug_Multimeter, PageService::PageDebug::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
