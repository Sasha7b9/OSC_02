#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_SOURCE             (set.trig_source)
#define TRIG_SOURCE_IS_A        (TRIG_SOURCE == FPGA::SET::Trig::Source::A)
#define TRIG_SOURCE_IS_B        (TRIG_SOURCE == FPGA::SET::Trig::Source::B)
#define TRIG_SOURCE_IS_EXT      (TRIG_SOURCE == FPGA::SET::Trig::Source::Ext)

#define SET_TRIGLEV(ch)         (set.trig_lev[(uint8)ch])
#define SET_TRIGLEV_SOURCE      SET_TRIGLEV(TRIG_SOURCE)
#define SET_TRIGLEV_A           (SET_TRIGLEV(Chan::A))
#define SET_TRIGLEV_B           (SET_TRIGLEV(Chan::B))

#define TRIG_MODE_FIND          (set.trig_modeFind)
#define TRIG_MODE_FIND_HAND     (TRIG_MODE_FIND == FPGA::SET::Trig::ModeFind::Hand)
#define TRIG_MODE_FIND_AUTO     (TRIG_MODE_FIND == FPGA::SET::Trig::ModeFind::Auto)

#define START_MODE              (set.trig_startMode)
#define START_MODE_IS_AUTO      (START_MODE == FPGA::SET::Trig::StartMode::Auto)
#define START_MODE_IS_WAIT      (START_MODE == FPGA::SET::Trig::StartMode::Wait)
#define START_MODE_IS_SINGLE    (START_MODE == FPGA::SET::Trig::StartMode::Single)

#define TRIG_POLARITY           (set.trig_polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig_input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig_TimeDelay)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    namespace SET
    {
        void Load();

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);

        void SetModeCouple(Chan::E ch, ModeCouple::E couple);
        /// ��������/��������� ����������.
        void LoadCalibratorMode();
        /// ���������� �������� ��������� �������������
        void LoadHoldfOff();
       
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

            static String ToString(uint16 rShiftRel, FPGA::SET::Range::E range, Divider::E divider);

        private:

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
                Number
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
        class Trig
        {
        public:
            static const int MIN = 20;
            static const int MAX = 980;
            static const int ZERO = 500;
            /// ���������� true � ������ ������� ��������������
            static bool SyncPulse();
            /// ������� ������� ������������� ������ �����
            static void DrawOnGrid();
            /// ����� ������ ���� ������� ������� ������������� ����� �������������� ������ ������� � ������� timMS �����������
            static void NeedForDraw(uint timeMS);

            struct Level
            {
                static void Load();

                static void Change(int delta);
            };

            /// �������� �������������
            struct Source
            {
                enum E
                {
                    A,    /// ����� 1
                    B,    /// ����� 2
                    Ext   /// �������
                } value;

                static void Load();
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
            };

            struct ModeFind
            {
                enum E
                {
                    Hand,      ///< ������� ������������� ��������������� ������� ��� ������������� - ����������� ��������� ������.
                    Auto       ///< ���������� ������ ������������� ������������ ������������� ����� ������� ������ ���������� �������.
                } value;
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

                static void Load();
            };

            /// ������������� � true �������� ��������, ��� ����� �������� ������ ��������������
            static bool pulse;
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

            void Set(int tShift);

            operator int() const { return value; };

            String ToString(FPGA::SET::TBase::E tBase) const;

        private:

            int value;
        };
    };
}
