#include "defines.h"
#include "Settings/Settings.h"


DEF_GOVERNOR( gTimeDisableDisplay,
    "�������",
    "����� ���������� �������",
    S_SERV_TIME_DISABLE_DISPLAY, 0, 240, &PagePowerSaving::self, Item::Active, Governor::Changed
)


DEF_GOVERNOR( gTimeDisablePower,
    "�������",
    "����� ���������� �������",
    S_SERV_TIME_DISABLE_POWER, 0, 240, &PagePowerSaving::self, Item::Active, Governor::Changed
)


DEF_PAGE_2( pPowerSaving,
    "��.����",
    "��������� ���������� ����������������",
    &gTimeDisableDisplay,
    &gTimeDisablePower,
    PageName::Service_PowerSaving,
    &PageService::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PagePowerSaving::self = static_cast<const Page *>(&pPowerSaving);
