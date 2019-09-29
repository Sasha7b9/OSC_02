#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // ���� �������
    {
        ModeDrawSignal::Lines,                      // modeDrawSignal
        Background::Black,                          // background
        ENumMinMax::_1,                             // ENumMinMax
        0,                                          // not used
        ENumAverage::_1,                            // ENumAverage
        ENumAccum::_1,                              // ENumAccum
        ModeAccumulation::NoReset,                  // modeAccumulation
        ENumSmoothing(ENumSmoothing::Disable),      // ENumSmoothing
        ENumSignalsInSec(ENumSignalsInSec::_25),    // ENumSignalsInSec
        TypeGrid::_1,                               // typeGrid
        50,                                         // brightnessGrid
        LinkingRShift::Position,                    // linkingRShift
        100,                                        // brightness
        MenuAutoHide::None,                         // menuAutoHide
        370,                                        // shiftInMemory
        Chan::A                                     // lastAffectedChannel
    },

    // ���� ����� 1, ����� 2
    {
        {
            RShift::ZERO,                   // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            0,                              // balanceShiftADC
            Bandwidth(Bandwidth::Full),     // bandwidth
            false,                          // inverse
            Divider::_1,                    // divider
            CalibrationMode::Disable,       // calibrationMode
        },
        {
            RShift::ZERO,                   // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            0,                              // balanceShiftADC
            Bandwidth(Bandwidth::Full),     // bandwidth
            false,                          // inverse
            Divider::_1,                    // divider
            CalibrationMode::Disable,       // calibrationMode
        }
    },

    // ���� �������������
    {
        TrigSource::A,                            // source
        TrigInput::Full,                          // input
        TrigPolarity::Rising,                     // polarity
        {Trig::Level::ZERO, Trig::Level::ZERO},     // lev[Chan::Size]
        TrigStartMode::Auto,                      // startMode
        TrigModeFind::Hand,                       // modeFind
        10,                                         // holdOff
        false                                       // holdOffEnable
    },

    // ���� ���¨����
    {
        0,                                          // shift
        TBase::_5ms,                                // base
        PeakDetMode::Disabled,                      // peakDet
        TPos::Center,                               // TPos
        SampleType::Real,                           // sampleType
        FunctionTime::Time,                         // timeDivXPos
        LinkingTShift::Time,                        // linkingTShift
        SampleType::Real                            // sampleTypeOld
    },

    // ���� �������
    {
        false,                                                                                          // showCursors
        {Osci::Measurements::Cursors::LookMode::None, Osci::Measurements::Cursors::LookMode::None},     // lookMode[2]
        true,                                                                                           // showFreq
        Osci::Measurements::Cursors::Active::None,                                                      // active
        Chan::A,                                                                                        // source
        {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // cntrlU[Chan::Size]
        {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // cntrlT[Chan::Size]
        Osci::Measurements::Cursors::Movement::Pixels,                                                  // movement
        {80,  80},                                                                                      // deltaU100percents[2]
        {120, 120},                                                                                     // deltaT100percents[2]
        {                                                                                               // posCurU[Chan::Size][2]
        { 60, 140 },                                                                                    // ����� 1
        { 60, 140 }},                                                                                   // ����� 2
        {                                                                                               // posCurT[Chan::Size][2]
        { 80, 200 },                                                                                    // ����� 1
        { 80, 200 }}                                                                                    // ����� 2
    },

    // ���� ������
    {
        Osci::Settings::Memory::ENumPointsFPGA::_1k,    // enumPoints
        ModeBtnMemory::Menu,                            // modeBtnMemory
        ModeWork::Dir,                                  // modeWork
        false,                                          // flashAutoConnect
        0,                                              // indexCurSymbolNameMask
        ModeSaveSignal::BMP,                            // modeSaveSignal
        {},                                             // fileName[MAX_SYMBOLS_IN_FILE_NAME]
        ModeShowIntMem::Saved,                          // modeShowIntMem
        FileNamingMode::Mask,                           // fileNamingMode
        {}                                              // fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    },

    // ��������� ����������
    {
        FuncModeDraw::Disable,      // modeDraw
        false,                      // enableFFT
        ScaleFFT::Log,              // scaleFFT
        SourceFFT::Both,            // sourceFFT
        WindowFFT::Hamming,         // windowFFT
        0,                          // currentCursor
        FFTmaxDB::_60,              // FFTmaxDB
        MathFunction::Sum,          // function
        ModeRegSet::Range,          // modeRegSet
        Divider::_1,                // divider
        RShift::ZERO,
        Range::_500mV
    },

    {
        { 50, 50 }                  // posCur[2]
    },
    
    // ���� ���������
    {
        false,                              // show
        MeasuresOnDisplay::_1_5,            // number
        MeasuresSource::A_B,                // source
        MeasuresModeViewSignals::AsIs,      // modeViewSignals
        {                                   // measures[15]
            Measure::Type::VoltageMax,          Measure::Type::VoltageMin,  Measure::Type::VoltagePic,      Measure::Type::VoltageMaxSteady,
            Measure::Type::VoltageMinSteady,
            Measure::Type::VoltageAverage,      Measure::Type::VoltageAmpl, Measure::Type::VoltageRMS,      Measure::Type::VoltageVybrosPlus,
            Measure::Type::VoltageVybrosMinus,
            Measure::Type::Period,              Measure::Type::Freq,        Measure::Type::TimeNarastaniya, Measure::Type::TimeSpada,
            Measure::Type::DurationPlus
        },
        Measure::Type::Freq                 // marked
    },

    // ���� ������
    {
        Color::Scheme::WhiteLetters,        // colorScheme
        CalibratorMode::Freq,               // calibratorMode
        0,                                  // soundVolume
        false                               // showInfoVoltage
    },
    
    // ��������� �����������
    {
        FreqMeterEnabled::Off,         // enabled
        FreqMeterModeView::Frequency,  // modeView
        FreqMeterTimeCounting::_1s,    // timeCounting
        FreqMeterFreqClc::_1MHz,       // freqClc
        FreqMeterNumberPeriods::_10    // numberPeriods
    },
    
    // ��������� ������-����������
    {
        TesterControl::Voltage,     // control
        TesterPolarity::Positive,   // polarity
        TesterStepU::_100mV,        // stepU
        TesterStepI::_4mA,          // stepI
        ENumAverage::_1,            // smooth
        TesterViewMode::Lines       // viewMode
    },
    
    // ��������� ����
    {
        false,                          // menu_show;
        {                               // menu_posActItem[Page_NumPages];    
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f
        },
        {}                              // menu_currentSubPage[Page_NumPages];
    },
    
    // �������������� ��������� ����������
    {
        {},                             // nrst_balanceADC[Chan::Size]
        BalanceADC::Disable,            // nrst_balanceADCtype
        StretchADC::Disable,            // nrst_stretchADCtype
        {},                             // nrst_StretchADC[Chan::Size][3]
        {},                             // nrst_rShiftAddStable[Chan::Size][3]
        0,                              // nrst_numAveForRand
        0,                              // nrst_numSmoothForRand
        {},                             // nrst_rShiftAdd[Chan::Size][Range::Size][2]
        0                               // nrst_correctionTime
    },
    
    // ��������� �����������
    {
        MultimeterAVP::Off,                 // avp
        Multimeter::Measure::VoltageDC,     // meas
        MultimeterRangeDC::_2V,             // rangeCV
        MultimeterRangeAC::_2V,             // rangeVV
        MultimeterRangeCurrent::_2A,        // rangeCurrentAC
        MultimeterRangeCurrent::_2A,        // rangeCurrentDC
        MultimeterRangeResistance::_10M     // rangeResist
    },
    
    // ��������� ������������
    {
        true,                                   // sourceChanA
        true,                                   // sourceChanB
        false,                                  // sourceSensor
        RecorderTypeStorage::RAM,               // storageRecord
        RecorderTypeStorage::RAM,               // storagePlay
        RecorderAxis::X,                        // axisMove
        RecorderAxis::X,                        // axisZoom
        RecorderScaleX(RecorderScaleX::_100ms), // scaleX
        0,                                      // currentCursor
        { Range::_1V, Range::_1V }              // range
    },

    // ���� �������
    {
        true,                                   // showConsole
        30,                                     // numStrings
        8,                                      // sizeFont
        false,                                  // modePauseConsole
        false,                                  // showAll
        false,                                  // showFlag
        { false, false },                       // showRShift[2]
        false,                                  // showTrigLev
        { false, false },                       // showRange[2]
        { false, false },                       // showChanParam[2]
        false,                                  // showTrigParam
        false,                                  // showTShift
        false,                                  // showTBase
        10,                                     // numMeasuresForGates
        false,                                  // showRandInfo
        false,                                  // showRandStat
        0,                                      // timeCompensation
        0,                                      // pretriggered
        false,                                  // ShowStats
        0,                                      // gate_max
        0,                                      // gate_min
        {
            {0}, {0}
        }
    }
};
