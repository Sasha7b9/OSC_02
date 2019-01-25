#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PackedTime
{
    unsigned timeMS   : 32;   /// \brief ����� � ������������� �� ������ �������. �.�. ��������� ����������� �� ����� ���������� ������ � ���������, �� 
                              /// timeMS == 0 ��������, ��� ������ ������ � ������ ���������� ������ ��� �� ������
    unsigned hours    : 5;
    unsigned minutes  : 6;
    unsigned seconds  : 6;
    unsigned year     : 7;
    unsigned month    : 4;
    unsigned notUsed0 : 4;
    unsigned day      : 5;
    unsigned notUsed1 : 27;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Hardware
{
    class Clock
    {
    public:
        static void Init();
        static PackedTime GetTime();
        static bool SetTime(int8 day, int8 month, int8 year, int8 hours, int8 minutes, int8 seconds);
    };
}
