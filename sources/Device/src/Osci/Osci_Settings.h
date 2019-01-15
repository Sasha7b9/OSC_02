#pragma once


namespace Osci
{
    namespace Settings
    {
        /// ��������� ��������� � ���������� �����
        void Load();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Range
        {
            static void Change(Chan::E ch, int delta);

            static void LoadBoth();

            enum E
            {
                _2mV,
                _5mV,
                _10mV,
                _20mV,
                _50mV,
                _100mV,
                _200mV,
                _500mV,
                _1V,
                _2V,
                _5V,
                _10V,
                _20V,
                Number
            } value;
            explicit Range(E v) : value(v) {};
            pString Name() const;

            pString ToString(Divider::E divider);

        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct RShift
        {
            static const int MIN = 20;
            static const int MAX = 980;
            static const int ZERO = 500;

            static void Change(Chan::E ch, int delta);

            static void Set(Chan::E ch, uint16 rShift);

            static void Load(Chan::E ch);

            static void Draw();

            static String ToString(uint16 rShiftRel, Osci::Settings::Range::E range, Divider::E divider);

        private:

            static void Draw(Chan::E ch);
        };
    }
}
