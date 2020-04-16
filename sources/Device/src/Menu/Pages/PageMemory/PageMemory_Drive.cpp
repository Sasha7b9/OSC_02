#include "defines.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Settings/Settings.h"


DEF_CHOICE_2(cSaveAs,                                                                                                                             //--- ������ - ����� �� - ��������� ��� ---
    "��������� ���"
    ,
    "���� ������ ������� \"�����������\", ������ ����� ������� � ������� �������� � ����������� ����� � ����������� BMP\n"
    "���� ������ ������� \"�����\", ������ ����� ������� � ������� �������� � ��������� ���� � ����� � ���������� TXT"
    ,
    "�����������",
    "�����",
    ModeSaveSignal::Ref(), &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cName,                                                                                                                                   //--- ������ - ����� �� - ��� ����� ---
    "��� �����"
    ,
    "����� ����� ������������ ������ ��� ���������� �� ������� ����������:\n"
    "\"�� �����\" - ����� ��������� ������������� �� ������� �������� ����� (����. ����� ����),\n"
    "\"�������\" - ������ ��� ��� ����� ����� �������� �������"
    ,
    "�� �����",
    "�������",
    FileNamingMode::Ref(), &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cModeBtnMemory,                                                                                                                      //--- ������ - ����� �� - ��� �� ������ ---
    "��� �� ������",
    "",
    "����",
    "����������",
    set.mem.modeBtnMemory, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2(cAutoconnect,                                                                                                                      //--- ������ - ����� �� - ��������������� ---
    "���������.",
    "E��� \"���\", ��� ����������� �������� ���������� ���������� �������������� ������� �� �������� ������ - ����� ��",
    DISABLE_RU,
    ENABLE_RU,
    set.mem.flashAutoConnect, &PageDrive::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_PAGE_6(pDrive,   //-V1027                                                                                                                                     //--- ������ - ����� �� ---
    "����� ��",
    "������ � ������� ������������ �����������.",
    PageDrive::Manager::self,
    &cName,
    PageDrive::Mask::self,
    &cSaveAs,
    &cModeBtnMemory,
    &cAutoconnect,
    PageName::Memory_Drive,
    &PageMemory::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageDrive::self = static_cast<const Page *>(&pDrive);
