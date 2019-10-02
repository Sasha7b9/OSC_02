#pragma once


/// C��������, ��������� �� �����
struct Warning
{
    enum E
    {
        LimitChan1_Volts,               ///< ������ �������� �� ����������� ������� ������.
        LimitChan2_Volts,               ///< ������ �������� �� ���������� ������� ������.
        LimitSweep_Time,                ///< ������ �������� �� �������.
        EnabledPeakDet,                 ///< ������� ������� ��������.
        LimitChan1_RShift,              ///< ������ �������� �� ���������� ������� ������.
        LimitChan2_RShift,              ///< ������ �������� �� ���������� ������� ������.
        LimitSweep_Level,               ///< ������ ������ �������������.
        LimitSweep_TShift,              ///< ������ �������� �� �������.
        TooSmallSweepForPeakDet,        ///< ������� ������� �������� ��� �������� ���������.
        TooFastScanForRecorder,         ///< ������� ������� �������� ��� ����������� ������.
        FileIsSaved,                    ///< ���� ������� �� ������.
        SignalIsSaved,                  ///< ������ ������� � ���.
        SignalIsDeleted,                ///< �� ���� ����� ������.
        MenuDebugEnabled,               ///< �������� ���� �������.
        TimeNotSet,                     ///< ����� �� �����������.
        SignalNotFound,                 ///< ������ �� ������ (���������).
        SetTPosToLeft,                  ///< ������������.
        NeedRebootDevice,               ///< ��������� ������������.
        ImpossibleEnableMathFunction,   ///< ���������� �������� �������������� �������.
        ImpossibleEnableFFT,            ///< ���������� �������� ���������� ���.
        WrongFileSystem,                ///< ���������� ���������� ������.
        WrongModePeackDet,              ///< ���������� �������� ������� ��������.
        DisableChannelB,                ///< ��������� ��������� ����� 2.
        TooLongMemory,                  ///< ��������� � ������������� ��������� ������ 32�.
        NoPeakDet32k,                   ///< ������� �������� �� �������� ��� ����� ������ 32�.
        NoPeakDet16k,                   ///< ������� �������� �� �������� ��� ����� ������ 16�.
        Warn50Ohms,                     ///< �������������� �� ����������� �������� ������ �� 50 ����.
        WarnNeedForFlashDrive,          ///< ��������� ������������ ������.
        FirmwareSaved,                  ///< �������� ��������� �� ������.
        FullyCompletedOTP,              ///< 
        Number
    } value;
};
