#include "defines.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/MenuItems.h"
#include "Settings/SettingsService.h"
#include "Settings/Settings.h"


extern const PageBase pageSound;

const PageBase * const PageService::PageSound::pointer = &pageSound;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cEnable,                                                                                                                                   //--- ������ - ���� - ���� ---
    "����",
    "���������/���������� �����",
    DISABLE_RU,
    ENABLE_RU,
    SOUND_ENABLED, pageSound, 0, 0, 0
)

DEF_GOVERNOR( gVolume,                                                                                                                              //--- ������ - ���� - ��������� ---
    "���������",
    "��������� ��������� �����",
    SOUND_VOLUME, 0, 100, pageSound, 0, 0, 0
)

DEF_PAGE_2( pageSound, // -V641 // -V1027                                                                                                                               //--- ������ - ���� ---
    "����",
    "� ���� ���� ����� ��������� ��������� �����",
    &cEnable,             ///< ������ - ���� - ����
    &gVolume,             ///< ������ - ���� - ���������
    Page::Name::Service_Sound, PageService::pointer, 0, 0, 0, 0
)
