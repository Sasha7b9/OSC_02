#include "defines.h"
#include "Display/Painter.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Settings/Settings.h"
#include "Keyboard/Keyboard.h"
#include "Hardware/Beeper.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/CommonFunctions.h"
#include "Utils/Dictionary.h"
#include <cmath>
#include <cstring>


extern const PageBase pageCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageMeasures::PageCursors::pointer = &pageCursors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_2( cShow,                                                                                                                                           //--- ������� - ���������� ---
    "����������",
    "��������/��������� �������.",
    "���",
    "��",
    CURS_SHOW, pageCursors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cLookModeChanA,                                                                                                                            //--- ������� - �������� ����� 1 ---
    "�������� \x8e, \x9e"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    DISABLE_RU,
    "����������",
    "�����",
    "������ � �����",
    CURS_LOOK_MODE(Chan::A), pageCursors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_4( cLookModeChanB,                                                                                                                            //--- ������� - �������� ����� 2 ---
    "�������� \x8f, \x9f"
    ,
    "����� ����� �������� �� ������ �������� ������� � ����������:\n"
    "1. \"����\" - ������ ������� � ������ ���������� ��������������� �������.\n"
    "2. \"����������\" - ��� ������ ��������� ��������� ������� ������� ������ ���������� ������������� ����������� ��������� �������.\n"
    "3. \"�����\" - ��� ������ ��������� ��������� ������� ���������� ������ ������� ������������� ����������� ��������� �������.\n"
    "4. \"������ � �����\" - ��������� ��� ���� �� ���������� �������, � ����������� �� ����, �� ����� ������ ������������� ��������� �����������."
    ,
    DISABLE_RU,
    "����������",
    "�����",
    "������. � �����",
    CURS_LOOK_MODE(Chan::B), pageCursors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cShowFreq,                                                                                                                                             //--- ������� - 1/dT ---
    "1/dT"
    ,
    "���� ������� \"���\", � ������ ������� ���� ��������� ��������, �������� ���������� ����� ��������� ������� - ������� �������, ���� ������ "
    "�������� ����� ���������� ����� ���������� ���������."
    ,
    DISABLE_RU,
    ENABLE_RU,
    CURSORS_SHOW_FREQ, pageCursors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_3( pageCursors, // -V641 // -V1027                                                                                                                               //--- ��������� ---
    "�������",
    "��������� ���������",
    &cShow,                                         ///< ��������� - ������� - ����������
//    &cLookModeChanA,                                ///< ��������� - ������� - �������� ����� 1
//    &cLookModeChanB,                                ///< ��������� - ������� - �������� ����� 2
    &cShowFreq,                                     ///< ��������� - ������� - 1/dT
    PageMeasures::PageCursors::PageSet::pointer,    ///< ��������� - ������� - ����������
    Page::Name::Measures_Cursors, PageMeasures::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
