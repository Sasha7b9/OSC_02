#pragma once


class Recorder
{
public:
    static bool IsEnabledA();

    static bool IsEnabledB();

    static bool IsEnabledSensor();

    static void Init();

    static void Update();

    static void DeInit();

    static void OnPressStart();

    static void Start();

    static void Stop();

    static bool IsRunning();

    static void ReadPoint();

    /// ��� �������� ������
    struct TypeMemory
    {
        enum E
        {
            RAM,    ///< ���
            ROM,    ///< ����� ��
            EXT     ///< ����� ��
        };
        static TypeMemory::E &Ref();
    };

    /// ������������ ��� ������ ���
    struct Axis
    {
        enum E
        {
            X,
            Y
        };
    };

    /// ������ �� �������
    struct ScaleX
    {
        enum E
        {
            _100ms, // TBase = 0.1c
            _200ms, // TBase = 0.2c
            _500ms, // TBase = 0.5c
            _1s,    // TBase = 1c
            _2s,    // TBase = 2c
            _5s,    // TBase = 5c
            _10s,   // TBase = 10c
            Count
        };
        ScaleX() {}
        static ScaleX::E &Ref();
        operator ScaleX::E() { return Ref(); }
        static void Load();
        static void Change(int delta);
        static pString ToString();
        /// ���������� �������� ����������� ������ � ������/���
        uint BytesToSec() const;
        /// ���������� ����� ���������� ����� �����
        static uint TimeForPointMS();
    };
};
