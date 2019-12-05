#include "defines.h"
#include "Settings/Settings.h"



const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // ��������� ����
    {
        false,                                  // menu_show;
        {                                       // menu_posActItem[Page_NumPages];    
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f
        },
        {}                                      // menu_currentSubPage[Page_NumPages];
    },

    // ���� �������
    {
        DisplayMapping::Lines,                      // modeDrawSignal
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
            0,                              // shift
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
            0,                              // shift
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
        Chan::A,                            // source
        TrigInput::Full,                    // input
        TrigPolarity::Rising,               // polarity
        { 0, 0},                            // lev[Chan::Count]
        TrigStartMode::Auto,                // startMode
        TrigModeFind::Hand,                 // modeFind
        10,                                 // holdOff
        false                               // holdOffEnable
    },

    // ���� ���¨����
    {
        0,                                  // shift
        TBase::_5ms,                        // base
        PeakDetMode::Disabled,              // peakDet
        TPos::Center,                       // TPos
        SampleType::Real,                   // sampleType
        FunctionTime::Time,                 // timeDivXPos
        LinkingTShift::Time,                // linkingTShift
        SampleType::Real                    // sampleTypeOld
    },

    // ���� ������
    {
        ENumPointsFPGA::_1k,        // enumPoints
        ModeBtnMemory::Menu,        // modeBtnMemory
        TypeSignalROM::Recorded,    // typeSignalROM
        ModeWork::Dir,              // modeWork
        false,                      // flashAutoConnect
        0,                          // indexCurSymbolNameMask
        ModeSaveSignal::BMP,        // modeSaveSignal
        {},                         // fileName[MAX_SYMBOLS_IN_FILE_NAME]
        ModeShowIntMem::Saved,      // modeShowIntMem
        FileNamingMode::Mask,       // fileNamingMode
        {}                          // fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    },

    // ��������� ���������
    {
        false,                                              // showCursors
        {CursorsLookMode::None, CursorsLookMode::None},     // lookMode[2]
        true,                                               // showFreq
        CursorsActive::None,                                // active
        Chan::A,                                            // source
        {CursorsControl::Disable, CursorsControl::Disable}, // cntrlU[Chan::Count]
        {CursorsControl::Disable, CursorsControl::Disable}, // cntrlT[Chan::Count]
        CursorsMovement::Pixels,                            // movement
        {80,  80},                                          // deltaU100percents[2]
        {120, 120},                                         // deltaT100percents[2]
        {                                                   // posCurU[Chan::Count][2]
        { 60, 140 },                                        // ����� 1
        { 60, 140 }},                                       // ����� 2
        {                                                   // posCurT[Chan::Count][2]
        { 80, 200 },                                        // ����� 1
        { 80, 200 }}                                        // ����� 2
    },

    // ���� ���������
    {
        false,                              // show
        MeasuresOnDisplay::_1_5,            // number
        MeasuresSource::A_B,                // source
        MeasuresModeViewSignals::AsIs,      // modeViewSignals
        {                                   // measures[15]
            TypeMeasure::VoltageMax,          TypeMeasure::VoltageMin,  TypeMeasure::VoltagePic,      TypeMeasure::VoltageMaxSteady,
            TypeMeasure::VoltageMinSteady,
            TypeMeasure::VoltageAverage,      TypeMeasure::VoltageAmpl, TypeMeasure::VoltageRMS,      TypeMeasure::VoltageVybrosPlus,
            TypeMeasure::VoltageVybrosMinus,
            TypeMeasure::Period,              TypeMeasure::Freq,        TypeMeasure::TimeNarastaniya, TypeMeasure::TimeSpada,
            TypeMeasure::DurationPlus
        },
        TypeMeasure::Freq                 // marked
    },

    // ��������� ����������
    {
        FuncModeDraw::Disable,      // modeDraw
        MathFunction::Sum,          // function
        ModeRegSet::Range,          // modeRegSet
        Divider::_1,                // divider
        0,
        Range::_500mV
    },

    // FFT
    {
        false,                      // enabled
        { 50, 50 },                 // posCur[2]
        ScaleFFT::Log,              // scale
        SourceFFT::Both,            // source
        WindowFFT::Hamming,         // window
        MaxDBFFT::_60,              // maxDB
        0                           // cursor
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
        TesterControl::Voltage,         // control
        TesterPolarity::Positive,       // polarity
        TesterStepU::_100mV,            // stepU
        TesterStepI::_4mA,              // stepI
        ENumAverage::_1,                // smooth
        TesterViewMode::Lines           // viewMode
    },

    // ��������� �����������
    {
        MultimeterAVP::Off,                     // avp
        MultimeterMeasure::VoltageDC,           // meas
        MultimeterRangeDC::_2V,                 // rangeCV
        MultimeterRangeAC::_2V,                 // rangeVV
        MultimeterRangeCurrent::_2A,            // rangeCurrentAC
        MultimeterRangeCurrent::_2A,            // rangeCurrentDC
        MultimeterRangeResistance::_10M         // rangeResist
    },

    // ��������� ������������
    {
        true,                                   // sourceChanA
        true,                                   // sourceChanB
        false,                                  // sourceSensor
        RecorderTypeMemory::RAM,                // typeMemory
        RecorderAxis::X,                        // axisMove
        RecorderAxis::X,                        // axisZoom
        RecorderScaleX(RecorderScaleX::_100ms), // scaleX
        0,                                      // currentCursor
        { Range::_1V, Range::_1V }              // range
    },

    // ���� ������
    {
        ColorScheme::WhiteLetters,              // colorScheme
        CalibratorMode::Freq,                   // calibratorMode
        0,                                      // soundVolume
        false                                   // showInfoVoltage
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
        false,                                   // runTest
        // �������������� ��������� ����������
        {
            0,                    // numAveForRand
            0,                    // numSmoothForRand
            0,                    // correctionTime
            0,                    // gate_max
            0,                    // gate_min

            BalanceADC::Disable,  // balanceADCtype
            {},                   // rShiftAdd[Chan::Count][Range::Count][2]
            StretchADC::Disable,  // stretchADCtype
            {},                   // StretchADC[Chan::Count][3]

            {},                   // rShiftAddStable[Chan::Count][3]
            {}                    // balanceADC[Chan::Count]
        }
    }
};
