#pragma once


namespace Multimeter
{
    namespace Settings
    {
        /// ��� - ��������� �������
        struct AVP
        {
            enum E
            {
                Off,
                On
            } value;
            explicit AVP(E v) : value(v) { };
        };

        /// ������ ��������� ����������� ����������
        struct RangeDC
        {
            enum E
            {
                _2V,
                _20V,
                _500V
            } value;
            explicit RangeDC(E v) : value(v) { };
        };

        /// ������ ��������� ����������� ����������
        struct RangeAC
        {
            enum E
            {
                _2V,
                _20V,
                _400V
            } value;
            explicit RangeAC(E v) : value(v) {};
        };

        /// ������ ��������� ����
        struct RangeCurrent
        {
            enum E
            {
                _20mA,
                _2A
            } value;
            explicit RangeCurrent(E v) : value(v) {};
        };

        /// ������ ��������� ������������ ����������� ����
        struct RangeResistance
        {
            enum E
            {
                _2k,
                _20k,
                _200k,
                _10M
            } value;
            explicit RangeResistance(E v) : value(v) {};
        };
    }
}
