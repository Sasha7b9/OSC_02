#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include <cmath>
#include <cstring>



DEF_CHOICE_2( cShow,                                                                                                                                           //--- ������� - ���������� ---
    "����������",
    "��������/��������� �������.",
    "���",
    "��",
    set.curs.showCursors, &PageMeasuresCursors::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


//_DEF_CHOICE_4( cLookModeChanA,                                                                                                                            //--- ������� - �������� ����� 1 ---
//    "�������� \x8e, \x9e"
//    ,
//    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
//    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
//    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
//    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
//    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
//    ,
//    DISABLE_RU,
//    "����������",
//    "�����",
//    "������ � �����",
//    CURS_LOOK_MODE(Chan::A), &PageMeasuresCursors::self, 0, 0, 0
//)


//_DEF_CHOICE_4( cLookModeChanB,                                                                                                                            //--- ������� - �������� ����� 2 ---
//    "�������� \x8f, \x9f"
//    ,
//    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
//    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
//    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
//    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
//    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
//    ,
//    DISABLE_RU,
//    "����������",
//    "�����",
//    "������. � �����",
//    CURS_LOOK_MODE(Chan::B), &PageMeasuresCursors::self, 0, 0, 0
//)


static bool IsActive_ShowFreq()
{
    return set.curs.showCursors;
}

DEF_CHOICE_2( cShowFreq,                                                                                                                                             //--- ������� - 1/dT ---
    "1/dT"
    ,
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ "
    "�������� ����� ���������� ����� ���������� ���������."
    ,
    DISABLE_RU,
    ENABLE_RU,
    set.curs.showFreq, &PageMeasuresCursors::self, IsActive_ShowFreq, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_3( pCursors, // -V641 // -V1027                                                                                                                                  //--- ��������� ---
    "�������",
    "��������� ���������",
    &cShow,                                         ///< ��������� - ������� - ����������
//    &cLookModeChanA,                                ///< ��������� - ������� - �������� ����� 1
//    &cLookModeChanB,                                ///< ��������� - ������� - �������� ����� 2
    &cShowFreq,                                     ///< ��������� - ������� - 1/dT
    PageMeasuresCursors::PageSet::self,    ///< ��������� - ������� - ����������
    PageName::Measures_Cursors, &PageMeasures::self, Item::Active, Page::Changed, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageMeasuresCursors::self = (const Page *)&pCursors;
