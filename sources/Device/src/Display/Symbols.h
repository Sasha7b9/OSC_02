#pragma once


 // Font::Type::_5
#define SYMBOL_COUPLE_AC        '\x91'
#define SYMBOL_COUPLE_DC        '\x92'
#define SYMBOL_COUPLE_GND       '\x90'


// Font::Type::_8
#define SYMBOL_GOVERNOR_SHIFT_3 '\x0e'  ///< ������ ����������
#define SYMBOL_TSHIFT_NORM_2    '\x84'  ///< 2-� ����� ������� �������� �� ������� �����������
#define SYMBOL_TSHIFT_NORM_1    '\x85'  ///< 1-� ����� ������� �������� �� ������� �����������
#define SYMBOL_TSHIFT_LEFT_2    '\x86'  ///< 2-� ����� ������� �������� �� ������� ����� �� ������
#define SYMBOL_TSHIFT_RIGHT_2   '\x87'  ///< 2-� ����� ������� �������� �� ������� ������ �� ������
#define SYMBOL_TPOS_1           '\x8a'  ///< 2-� ����� ������� TPos
#define SYMBOL_TPOS_2           '\x8b'  ///< 1-� ����� ������� TPos
#define SYMBOL_GOVERNOR_SHIFT_2 '\x8c'  ///< ������ ����������
#define SYMBOL_TSHIFT_RIGHT_1   '\x93'  ///< 1-� ����� ������� �������� �� ������� ������ �� ������
#define SYMBOL_RSHIFT_NORMAL    '\x93'  ///< ������ �������� �� ���������� ����������
#define SYMBOL_TSHIFT_LEFT_1    '\x94'  ///< 1-� ����� ������� �������� �� ������� ����� �� ������
#define SYMBOL_TRIG_LEV_NORMAL  '\x94'  ///< ������ ������� ������������� ����������
#define SYMBOL_TRIG_LEV_LOWER   '\x95'  ///< ������ ������� ������������� ���� �����
#define SYMBOL_RSHIFT_LOWER     '\x95'  ///< ������ �������� �� ���������� ���� �����
#define SYMBOL_TRIG_LEV_ABOVE   '\x96'  ///< ������ ������� ������������� ���� �����
#define SYMBOL_RSHIFT_ABOVE     '\x96'  ///< ������ �������� �� ���������� ���� �����
#define SYMBOL_GOVERNOR_SHIFT_0 '\xaa'  ///< ������ ����������
#define SYMBOL_GOVERNOR_SHIFT_1 '\xac'  ///< ������ ����������
#define SYMBOL_PLAY             '\xae'  ///< ������ ������ ������ ������ ������
#define SYMBOL_TPOS_1           '\x8a'  ///< 2-� ����� ������� TPos
#define SYMBOL_GOVERNOR_LEFT    '\x80'
#define SYMBOL_GOVERNOR_RIGHT   '\x81'


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
