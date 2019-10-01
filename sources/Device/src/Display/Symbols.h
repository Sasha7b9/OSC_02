#pragma once


 // Font::Type::_5
#define SYMBOL_COUPLE_AC        '\x91'
#define SYMBOL_COUPLE_DC        '\x92'
#define SYMBOL_COUPLE_GND       '\x90'

struct Symbol8
{
    enum E
    {
        GOVERNOR_SHIFT_3 = '\x0e',  ///< ������ ����������
        GOVERNOR_LEFT    = '\x80',
        GOVERNOR_RIGHT   = '\x81',
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


struct SymbolUGO2
{
    enum E
    {
        PEAK_DET_LEFT   = '\x12',  ///< ����� ����� ������ �������� ���������
        PEAK_DET_RIGHT  = '\x13',  ///< ������ ����� ������ �������� ���������
        ARROW_LEFT      = '\x20',
        DELETE          = '\x22',  ///< ����� ��-�����-�������
        INSERT          = '\x26',  ///< ����� ��-���-��������
        SAVE_TO_MEM     = '\x2c',
        ROM             = '\x40',  ///< �����������
        FLASH_DRIVE_BIG = '\x42',  ///< ������ ������
        FOLDER          = '\x46',  ///< �������
        TRIANGLE_UP     = '\x4c',  ///< ������� ������������� �����
        TRIANGLE_DOWN   = '\x4e',  ///< ������� ������������� ����
        ARROW_RIGHT     = '\x64',
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',  ///< ������ ������ ����� ������
        ETHERNET        = '\x82',  ///< ������ Ethernet ����� ������
        USB             = '\x84',  ///< ������ USB ����� ������
        MATH_FUNC_MUL   = '\x8a',  ///< ����������-�������-���������
        TRIANGLE_LEFT   = '\x8c',  ///< ������� ������������� �����
        TRIANGLE_RIGHT  = '\x8e',  ///< ������� ������������� ������
        SELECT          = '\xa0'   ///< ������ ������
    };
};
