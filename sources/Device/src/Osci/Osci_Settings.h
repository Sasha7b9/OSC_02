#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_RSHIFT(ch)          (Osci::Settings::RShift::Value(ch))
#define SET_RSHIFT_A            (SET_RSHIFT(Chan::A))
#define SET_RSHIFT_B            (SET_RSHIFT(Chan::B))

#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == Trig::Source::B)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_IS_HAND  (TRIG_MODE_FIND == Osci::Settings::Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_IS_AUTO  (TRIG_MODE_FIND == Osci::Settings::Trig::ModeFind::Auto)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    namespace Settings
    {
        /// ���������� �������� ��������� �������������
        void LoadHoldfOff();

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct Range
        {
            static void Change(Chan::E ch, int delta);
            /// ����������� ��� ��������� ����� ������ ��� ����� ������� ������������
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
                Size
            } value;
            explicit Range(E v) : value(v) {};
            pString Name() const;

            static void Set(Chan::E ch, E range);

            pString ToString(Divider::E divider = Divider::_1);

        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct RShift
        {
            /// ��� �������� ������������� �������� ��������
            static const int MIN = 20;
            /// ��� �������� ������������ ��������� ��������
            static const int MAX = 980;
            /// ��� �������� ������������� �������� ������
            static const int ZERO = 500;
            /// �������� �� delta
            static void Change(Chan::E ch, int delta);
            /// ���������� ��������
            static void Set(Chan::E ch, uint16 rShift);
            /// ��������� � ����������
            static void Load(Chan::E ch);
            /// ���������� ��� �� ������
            static void DrawBoth();
            /// ������������� � ������
            static String ToString(uint16 rShiftRel, Osci::Settings::Range::E range, Divider::E divider);
            /// ���������� ������ �� ��������
            static uint16 &Value(Chan::E ch);
        private:
            /// ���������� ������ ������������� �������� �� �����
            static void Draw(Chan::E ch);
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct TBase
        {
            enum E
            {
                _2ns,
                _5ns,
                _10ns,
                _20ns,
                _50ns,
                _100ns,
                _200ns,
                _500ns,
                _1us,
                _2us,
                _5us,
                _10us,
                _20us,
                _50us,
                _100us,
                _200us,
                _500us,
                _1ms,
                _2ms,
                _5ms,
                _10ms,
                _20ms,
                _50ms,
                _100ms,
                _200ms,
                _500ms,
                _1s,
                _2s,
                _5,
                _10s,
                Size
            } value;
            explicit TBase(E v) : value(v) {};

            pString Name() const;

            pString ToString() const;

            static void Load();

            static void Change(int delta);

            /// � ����� �������� ������ ���������� ����� ����������� ������
            static const E MIN_P2P = _50ms;
            /// ����������� ������� �� �������, ��� ������� ��� �������� ��������� ������ �������� ���������
            static const E MIN_PEAK_DET = _200ns;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace Trig
        {
            /// ���������� true � ������ ������� ��������������
            bool SyncPulse();
            /// ������� ������� ������������� ������ �����
            void DrawOnGrid();
            /// ����� ������ ���� ������� ������� ������������� ����� �������������� ������ ������� � ������� timMS �����������
            static void NeedForDraw();

            /// �������� �������������
            struct Source
            {
                enum E
                {
                    A,    /// ����� 1
                    B     /// ����� 2
                } value;

                static void Load();
            };

            struct Level
            {
                static const int MIN = 20;
                static const int MAX = 980;
                static const int ZERO = 500;
                /// ��������� ������� ������������� � ���������� �����
                static void Load();
                /// �������� ������� ������������� �� delta ������
                static void Change(int delta);
                /// ���������� �������� ������� �������������
                static void Set(int level);
                /// ���������� ����� ������� �������������
                static void Find();
            };

            /// ����� �������.
            struct StartMode
            {
                enum E
                {
                    Auto,     ///< ��������������.
                    Wait,     ///< ������.
                    Single    ///< �����������.
                } value;
                explicit StartMode(E v) : value(v) {};
            };

            struct ModeFind
            {
                enum E
                {
                    Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
                    Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
                } value;
                explicit ModeFind(E v) : value(v) {};
            };

            struct Input
            {
                enum E
                {
                    Full,
                    HF,
                    LF
                } value;

                /// ���������� � �������������� ��������� ������, ���������� �� �������� � ���� �������������
                static void Load();
            };

            struct Polarity
            {
                enum E
                {
                    Rising,
                    Falling
                } value;
                explicit Polarity(E v) : value(v) {};

                static void Load();
            };

            extern bool pulse;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        class TShift
        {
        public:
            static int Min();
            static int Zero();
            static int Max();

            TShift(int tshift);

            int InPoints();

            static void Set(int tShift);

            static void Load();

            static void Change(int delta);

            operator int() const { return value; };

            String ToString(Osci::Settings::TBase::E tBase) const;

        private:

            static int value;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        struct PeakDetMode
        {
            enum E
            {
                Disabled,
                Enabled,
                Average
            } value;
            explicit PeakDetMode(E v) : value(v) {};
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        namespace Memory
        {
            /// ����� ����� �������, � ������� ��� ������.
            struct ENumPointsFPGA
            {
                enum E
                {
                    _512,
                    _1k,
                    _2k,
                    _4k,
                    _8k,
                    Size
                } value;
                explicit ENumPointsFPGA(E v) : value(v) {};
                uint BytesInChannel(PeakDetMode::E mode) const;
                uint PointsInChannel() const;
            };
        }

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ��������� ����� ������������� �� �������.
        struct TPos
        {
            enum E
            {
                Left,          ///< �������� � ������ ����.
                Center,        ///< �������� � ������.
                Right          ///< �������� � ������� ����.
            } value;
            explicit TPos(E v) : value(v) {};
            /// ������ �������� ���������� ������ ������������� � ������
            int InBytes();
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ��� ������� ��� ������ �������������.
        struct SampleType
        {
            enum E
            {
                Real,    ///< �������� ����� - � ���������� ��������� ������ ������� ��������� �����, ������ �� ��������������.
                Equal    ///< ������������� - ������ �������� �� ��������� ������, ���������� �� �������������.
            } value;
            explicit SampleType(E v) : value(v) {};
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ������� ��/���.
        struct FunctionTime
        {
            enum E
            {
                Time,
                ShiftInMemory
            } value;
        };

        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// ��� �������� � �������� �� �����������
        struct LinkingTShift
        {
            enum E
            {
                Time,     ///< �������� ��������� � �������
                Position  ///< �������� ��������� � �������
            } value;
        };
    }
}
