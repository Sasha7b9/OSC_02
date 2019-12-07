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
        DisplayMapping::Lines,              // modeDrawSignal
        ENumMinMax::_1,                     // ENumMinMax
        0,                                  // not used
        ENumAverage::_1,                    // ENumAverage
        ENumAccum::_1,                      // ENumAccum
        ModeAccumulation::NoReset,          // modeAccumulation
        ENumSmoothing::Disable,             // ENumSmoothing
        ENumSignalsInSec::_25,              // ENumSignalsInSec
        TypeGrid::_1,                       // typeGrid
        LinkingRShift::Position,            // linkingRShift
        100,                                // brightness
        MenuAutoHide::None,                 // menuAutoHide
        370,                                // shiftInMemory
        Chan::A                             // lastAffectedChannel
    },

    // ���� ����� 1, ����� 2
    {
        {
            0,                              // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            0,                              // balanceShiftADC
            Bandwidth::Full,                // bandwidth
            false,                          // inverse
            Divider::_1                     // divider
        },
        {
            0,                              // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            0,                              // balanceShiftADC
            Bandwidth::Full,                // bandwidth
            false,                          // inverse
            Divider::_1                     // divider
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
        false,                                                // showCursors
        {CursorsLookMode::None, CursorsLookMode::None},       // lookMode[2]
        true,                                                 // showFreq
        CursorsActive::None,                                  // active
        Chan::A,                                              // source
        {CursorsControl::Disabled, CursorsControl::Disabled}, // cntrlU[Chan::Count]
        {CursorsControl::Disabled, CursorsControl::Disabled}, // cntrlT[Chan::Count]
        CursorsMovement::Pixels,                              // movement
        {80,  80},                                            // deltaU100percents[2]
        {120, 120},                                           // deltaT100percents[2]
        {                                                     // posCurU[Chan::Count][2]
        { 60, 140 },                                          // ����� 1
        { 60, 140 }},                                         // ����� 2
        {                                                     // posCurT[Chan::Count][2]
        { 80, 200 },                                          // ����� 1
        { 80, 200 }}                                          // ����� 2
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
        FuncModeDraw::Disabled,     // modeDraw
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
        FreqMeter::Enabled::Off,        // enabled
        FreqMeter::ModeView::Frequency, // modeView
        FreqMeter::TimeCounting::_1s,   // timeCounting
        FreqMeter::FreqClc::_1MHz,      // freqClc
        FreqMeter::NumberPeriods::_10   // numberPeriods
    },

    // ��������� ������-����������
    {
        Tester::Control::Voltage,       // control
        Tester::Polarity::Positive,     // polarity
        Tester::StepU::_100mV,          // stepU
        Tester::StepI::_4mA,            // stepI
        ENumAverage::_1,                // smooth
        Tester::ViewMode::Lines         // viewMode
    },

    // ��������� �����������
    {
        Multimeter::AVP::Off,                   // avp
        Multimeter::Measure::VoltageDC,         // meas
        Multimeter::RangeDC::_2V,               // rangeCV
        Multimeter::RangeAC::_2V,               // rangeVV
        Multimeter::RangeCurrentAC::_2A,        // rangeCurrentAC
        Multimeter::RangeCurrentDC::_2A,        // rangeCurrentDC
        Multimeter::RangeResistance::_10M       // rangeResist
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
        true,                       // showConsole
        30,                         // numStrings
        8,                          // sizeFont
        false,                      // modePauseConsole
        false,                      // showAll
        false,                      // showFlag
        { false, false },           // showRShift[2]
        false,                      // showTrigLev
        { false, false },           // showRange[2]
        { false, false },           // showChanParam[2]
        false,                      // showTrigParam
        false,                      // showTShift
        false,                      // showTBase
        10,                         // numMeasuresForGates
        false,                      // showRandInfo
        false,                      // showRandStat
        0,                          // timeCompensation
        0,                          // pretriggered
        false,                      // ShowStats
        false,                      // runTest
        // �������������� ��������� ����������
        {
            {1.0F, 1.0F},           // stretchADC[Chan::Count]
            {},                     // rShiftAddStable[Chan::Count][3]
            {},                     // shiftADC[Chan::Count][Range::Count]
            {0, 0},                 // balanceADC[Chan::Count]
            0,                      // numAveForRand
            0,                      // numSmoothForRand
            0,                      // correctionTime
            0,                      // enum_gate_max
            0,                      // enum_gate_min
            BalanceADC::Disabled,    // balanceADCtype
            ShiftADC::Disable,      // shiftADCtype
            StretchADC::Disabled     // stretchADCtype
        }
    }
};
