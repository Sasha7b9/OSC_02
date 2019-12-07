#pragma once


struct Recorder
{
    static bool IsEnabledA();

    static bool IsEnabledB();

    static bool IsEnabledSensor();

    static void Init();

    static void DeInit();

    static void Update();

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
};
