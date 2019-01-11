#pragma once
#include "Display_Types.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    void Init();

    void Update();

    //static void SetKey(Key::E key);

    void RemoveAddDrawFunction();

    void ShowWarning(Warning::E warning);
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode::Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode::Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    void SetDrawMode(DrawMode::E mode, pFuncVV func);

    void FuncOnWaitStart(const char *text, bool eraseBackground);

    void FuncOnWaitStop();

    void SetAddDrawFunction(pFuncVV func, uint time = MAX_UINT);

    void ChangedRShiftMarkers(bool active);

    //static void Clear();

    extern Key::E key;

    void SetOrientation(Orientation orientation);
    /// ���������� ����� ����������
    //static ModeAveraging::E GetModeAveraging();
    /// ������������� ����������� ������� ������
    static void SetNumSignalsInS(int maxFPS);
    /// ���������� ����� ����� ������������� ������� (����� ������� - �����������)
    //static int NumPointSmoothing();
    /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
    BitSet64 PointsOnDisplay();
    /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
    BitSet64 BytesOnDisplay();
    /// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
    int TimeMenuAutoHide();
    /// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    bool IsSeparate();
    /// brightness = 1..100
    static void SetBrightness(int16 brightness);

    //static int NumAverage();
    /// ��� �������� ������ �� ������ � ������ �������� ��������� ��� ����� � ��� ���� ������, ��� ����, ������ ��� ������ ����� ������������ ����� �������
    //static int ShiftInMemoryInPoints();
};
