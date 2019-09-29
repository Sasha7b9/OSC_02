#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Recorder::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // ���� �������
    {
        Display::ModeDrawSignal::Lines,                             // modeDrawSignal
        Display::Background::Black,                                 // background
        Display::ENumMinMax::_1,                                    // ENumMinMax
        0,                                                          // not used
        Display::ENumAverage::_1,                                   // ENumAverage
        Display::ENumAccum::_1,                                     // ENumAccum
        Display::ModeAccumulation::NoReset,                         // modeAccumulation
        Display::ENumSmoothing(Display::ENumSmoothing::Disable),    // ENumSmoothing
        Display::ENumSignalsInSec(Display::ENumSignalsInSec::_25),  // ENumSignalsInSec
        Display::TypeGrid::_1,                                      // typeGrid
        50,                                                         // brightnessGrid
        Display::LinkingRShift::Position,                           // linkingRShift
        100,                                                        // brightness
        Menu::AutoHide::None,                                       // menuAutoHide
        370,                                                        // shiftInMemory
        Chan::A,                                                    // lastAffectedChannel
    },

    // ���� ����� 1, ����� 2
    {
        {RShift::ZERO, RShift::ZERO},                               // shift[Chan::Size]
        {Range::_500mV, Range::_500mV},                             // range[Chan::Size]
        {ModeCouple::DC, ModeCouple::DC},                           // couple[Chan::Size]
        {true, true},                                               // enabled[Chan::Size]
        {0, 0},                                                     // balanceShiftADC[2]
        {Bandwidth(Bandwidth::Full), Bandwidth(Bandwidth::Full)},   // bandwidth[2]
        {false, false},                                             // inverse[2]
        {Divider::_1, Divider::_1},                                 // divider[2]
        {CalibrationMode::Disable, CalibrationMode::Disable},       // calibrationMode[2]
    },

    // ���� �������������
    {
        Trig::Source::A,                        // source
        Trig::Input::Full,                      // input
        Trig::Polarity::Rising,                 // polarity
        {Trig::Level::ZERO, Trig::Level::ZERO}, // lev[Chan::Size]
        Trig::StartMode::Auto,                  // startMode
        Trig::ModeFind::Hand,                   // modeFind
        10,                                     // holdOff
        false,                                  // holdOffEnable
    },

    // ���� ���¨����
    {
        0,                      // time_shift
        TBase::_5ms,            // time_base
        PeakDetMode::Disabled,  // time_peakDet
        TPos::Center,           // time_TPos
        SampleType::Real,       // time_sampleType
        FunctionTime::Time,     // time_timeDivXPos
        LinkingTShift::Time,    // time_linkingTShift
        SampleType::Real,       // time_sampleTypeOld
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
        { 80, 200 }},                                                                                   // ����� 2
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
        {},                                             // fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    },

    // ��������� ����������
    {
        FuncModeDraw::Disable,      // modeDraw
        false,                      // enableFFT
        ScaleFFT::Log,              // scaleFFT
        SourceFFT::Both,            // sourceFFT
        WindowFFT::Hamming,         // windowFFT
        0,                          // currentCursor
        { 50, 50 },                 // posCur[2]
        FFTmaxDB::_60,              // FFTmaxDB
        MathFunction::Sum,          // function
        ModeRegSet::Range,          // modeRegSet
        Range::_1V,                 // range
        Divider::_1,                // divider
        RShift::ZERO,               // rShift
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
        Measure::Type::Freq,                // marked
    },

    // ���� ������

    0,                                  // notUsed
    Color::Scheme::WhiteLetters,        // serv_colorScheme
    false,                              // serv_recorder
    CalibratorMode::Freq,               // serv_calibratorMode
    0,                                  // serv_soundVolume
    false,                              // serv_showInfoVoltage  
    
    // ��������� �����������
    
    FrequencyCounter::Enabled::Off,         // freq_enabled;      
    FrequencyCounter::ModeView::Frequency,  // freq_modeView      
    FrequencyCounter::TimeCounting::_1s,    // freq_timeCounting; 
    FrequencyCounter::FreqClc::_1MHz,       // freq_freqClc;      
    FrequencyCounter::NumberPeriods::_10,   // freq_numberPeriods;
    
    // ���� �������

    true,                                   // dbg_showConsole
    30,                                     // dbg_numStrings;         
    8,                                      // dbg_sizeFont;           
    false,                                  // dbg_modePauseConsole;   
    false,                                  // dbg_showAll;            
    false,                                  // dbg_showFlag;           
    { false, false },                       // dbg_showRShift[2];      
    false,                                  // dbg_showTrigLev;        
    { false, false },                       // dbg_showRange[2];       
    { false, false },                       // dbg_showChanParam[2];   
    false,                                  // dbg_showTrigParam;      
    false,                                  // dbg_showTShift;         
    false,                                  // dbg_showTBase;          
    10,                                     // dbg_numMeasuresForGates;
    false,                                  // dbg_showRandInfo;       
    false,                                  // dbg_showRandStat;       
    0,                                      // dbg_timeCompensation;   
    0,                                      // dbg_pretriggered;       
    { Bandwidth::Full, Bandwidth::Full },   // dbg_bandwidth[2];       
    false,                                  // dbg_ShowStats;          
    Display::Orientation::Normal,           // dbg_Orientation;        
    
    // ��������� ������-����������
    
    Tester::Settings::Control::Voltage,     // test_control; 
    Tester::Settings::Polarity::Positive,   // test_polarity;
    Tester::Settings::StepU::_100mV,        // test_stepU;   
    Tester::Settings::StepI::_4mA,          // test_stepI;   
    Display::ENumAverage::_1,               // test_smooth;  
    Tester::Settings::ViewMode::Lines,      // test_viewMode 
    
    // ��������� ����
    
    false,                          // menu_show;
    {                               // menu_posActItem[Page_NumPages];    
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
        0x7f, 0x7f, 0x7f, 0x7f
    },                             
    {},                             // menu_currentSubPage[Page_NumPages];
    
    // �������������� ��������� ����������
    
    {},                             // nrst_balanceADC[Chan::Size];               
    BalanceADC::Disable,            // nrst_balanceADCtype;                         
    StretchADC::Disable,            // nrst_stretchADCtype;                         
    {},                             // nrst_StretchADC[Chan::Size][3];            
    {},                             // nrst_rShiftAddStable[Chan::Size][3];       
    0,                              // nrst_numAveForRand;                          
    0,                              // nrst_numSmoothForRand;                       
    {},                             // nrst_rShiftAdd[Chan::Size][Range::Size][2];
    0,                              // nrst_correctionTime;                         
    
    // ��������� �����������

    Multimeter::Settings::AVP::Off,                 // multi_avp
    Multimeter::Measure::VoltageDC,                 // multi_meas
    Multimeter::Settings::RangeDC::_2V,             // multi_rangeCV
    Multimeter::Settings::RangeAC::_2V,             // multi_rangeVV
    Multimeter::Settings::RangeCurrent::_2A,        // multi_rangeCurrentAC
    Multimeter::Settings::RangeCurrent::_2A,        // multi_rangeCurrentDC
    Multimeter::Settings::RangeResistance::_10M,    // multi_rangeResist
    
    // ��������� ������������

    true,                               // rec_sourceChanA
    true,                               // rec_sourceChanB
    false,                              // rec_sourceSensor
    TypeStorage::RAM,                   // rec_storageRecord
    TypeStorage::RAM,                   // rec_storagePlay
    Axis::X,                            // rec_axisMove
    Axis::X,                            // rec_axisZoom
    ScaleX(ScaleX::_100ms),             // rec_scaleX
    { Range::_1V, Range::_1V },         // rec_range

    0,  // dbg_gate_max
    0,  // dbg_gate_min

    {
        {0}, {0}
    }
};
