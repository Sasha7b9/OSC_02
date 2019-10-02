#pragma once

#define START_MULTI_MEASUREMENT() Timer::StartMultiMeasurement()
#define PAUSE_ON_TICKS(ticks)     Timer::PauseOnTicks(ticks)
#define PAUSE_ON_MS(ms)           Timer::PauseOnTime(ms)


/// ���������� �����, ��������� � ������� ���������� ������ ������� Timer_StartMultiMeasurement(). �� ����� (1 << 32)
 /// � ����� ������� 120.000.000 ����� ��� �8-53 � 90.000.000 ����� ��� �8-54.
 /// ������������ ������� �������, ������� ����� ��������� � � ������� - 35 ���.
#define TIME_TICKS Timer::TimeTicks()
#define TIME_US    Timer::TimeUS()
#define TIME_MS    Timer::TimeMS()


namespace Timer
{
    struct Type
    {
        enum E
        {
            ShowLevelTrigLev,       ///< ����� �������������, ����� ���������� ��������� ����� ������ �������������
            FlashDisplay,           ///< ������ ��� ��������� �������� ������ �����-������ �������.
            ShowMessages,           ///< ������ ��� ��������� ����� ������ �������������� � ��������������� ���������.
            MenuAutoHide,           ///< ������ ��� ������� ������� ��������� ����.
            RShiftMarkersAutoHide,  ///< ������ ��� ������� ������� ��������� �������������� ������� �����.
            StopSound,              ///< ��������� ����
            StrNaviAutoHide,        ///< ������� ������ ��������� ����
            Display,                ///< ������ �� ������ ��������� ������
            RemoveAddFunction,      ///< ������ �� �������� �������������� ������� ���������
            USB,                    ///<
            Temp,
            Number
        };
    };


    void Init();

    void DeInit();
    /// ��������� ������� timer ������� � ����� ������������
    void Set(Timer::Type::E type, pFuncVV func, uint dTms);

    void SetAndStartOnce(Timer::Type::E type, pFuncVV func, uint dTms);

    void SetAndEnable(Timer::Type::E type, pFuncVV func, uint dTms);

    void StartOnce(Timer::Type::E type);

    void Enable(Timer::Type::E type);

    void Disable(Timer::Type::E type);

    bool IsRun(Timer::Type::E type);

    void PauseOnTime(uint timeMS);

    void PauseOnTicks(uint numTicks);
    /// ��������� ������� ��� ��������� ����� �������� �������
    void StartMultiMeasurement();
    /// ������� ����������� ����� ������������ �������� � ����� for
    void PauseOnOPS(uint ops);

    bool IsBusy();
    /// ���������� ���������� �����������, ��������� � ������� ���������� ������ StartMultiMeasurement()
    uint TimeUS();
    /// ���������� ���������� �����, ��������� � ������� ���������� ������ StartMultiMeasurement();
    uint TimeTicks();
    /// ���������� ���������� �����������, ��������� � ������� ������ ���������
    uint TimeMS();
    /// ������������� ��������� ����� ������������. ����� ������ Timer_LogPoint �������� ��������� ��������� �� ��� �����
    void StartLogging();

    uint LogPointUS(char *name);

    uint LogPointMS(char *name);
    /// ��������� �������. ���������� ������ �� ����������
    void ElapsedCallback();
}
