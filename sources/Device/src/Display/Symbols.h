#pragma once


struct Symbol5
{
    enum E
    {
        COUPLE_AC  = '\x91',
        COUPLE_DC  = '\x92',
        COUPLE_GND = '\x90'
    };
};

struct Symbol8
{
    enum E
    {
        GOVERNOR_SHIFT_3 = '\x0e',  ///< ������ ����������
        GOVERNOR_LEFT    = '\x80',
        GOVERNOR_RIGHT   = '\x81',
        CROSS            = '\x88',  ///< ������ ������������� ������
        TPOS_1           = '\x8a',  ///< 2-� ����� ������� TPos
        TPOS_2           = '\x8b',  ///< 1-� ����� ������� TPos
        RSHIFT_NORMAL    = '\x93',  ///< ������ �������� �� ���������� ����������
        TRIG_LEV_NORMAL  = '\x94',  ///< ������ ������� ������������� ����������
        TRIG_LEV_LOWER   = '\x95',  ///< ������ ������� ������������� ���� �����
        TRIG_LEV_ABOVE   = '\x96',  ///< ������ ������� ������������� ���� �����
        GOVERNOR_SHIFT_2 = '\x8c',  ///< ������ ����������
        GOVERNOR_SHIFT_0 = '\xaa',  ///< ������ ����������
        GOVERNOR_SHIFT_1 = '\xac',  ///< ������ ����������
        PLAY             = '\xae',  ///< ������ ������ ������ ������ ������
    };
};

struct SymbolUGO
{
    enum E
    {
        Count
    };
};

struct SymbolUGO2
{
    enum E
    {
        PEAK_DET_LEFT   = '\x12',   ///< ����� ����� ������ �������� ���������
        PEAK_DET_RIGHT  = '\x13',   ///< ������ ����� ������ �������� ���������
        ARROW_LEFT      = '\x20',
        DELETE          = '\x22',   ///< ����� ��-�����-�������
        INSERT          = '\x26',   ///< ����� ��-���-��������
        SAVE_TO_MEM     = '\x2c',
        ROM             = '\x40',   ///< �����������
        FLASH_DRIVE_BIG = '\x42',   ///< ������ ������
        FOLDER          = '\x46',   ///< �������
        LEAVE           = '\x48',   ///< ������ ������
        ENTER           = '\x4a',   ///< ������ "Enter"
        TRIANGLE_UP     = '\x4c',   ///< ������� ������������� �����
        TRIANGLE_DOWN   = '\x4e',   ///< ������� ������������� ����
        BIG_M           = '\x60',   ///< ������� ����� "�"
        SETTINGS        = '\x62',   ///< ������ ��������
        ARROW_RIGHT     = '\x64',
        CURRENT         = '\x6a',   ///< "���"
        RECORDERD       = '\x6c',   ///< "���"
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',   ///< ������ ������ ����� ������
        ETHERNET        = '\x82',   ///< ������ Ethernet ����� ������
        USB             = '\x84',   ///< ������ USB ����� ������
        MATH_FUNC_MUL   = '\x8a',   ///< ����������-�������-���������
        TRIANGLE_LEFT   = '\x8c',   ///< ������� ������������� �����
        TRIANGLE_RIGHT  = '\x8e',   ///< ������� ������������� ������
        SELECT          = '\xa0'    ///< ������ ������
    };
};
