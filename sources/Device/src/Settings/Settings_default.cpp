#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"
#endif


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Recorder::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // ���� �������

    Display::ModeDrawSignal::Lines,         // disp_modeDrawSignal;      
    Display::ThicknessSignal::_1,           // disp_thickness;           
    Display::Background::Black,             // disp_background;          
    Display::ShowStrNavi::Temp,             // disp_showStringNavigation;
    Display::ENumMinMax::_1,                // disp_ENumMinMax;          
    Display::ModeAveraging::Accurately,     // disp_modeAveraging;       
    Display::ENumAverage::_1,               // disp_ENumAverage;         
    Display::ENumAccum::_1,                 // disp_ENumAccum;           
    Display::ModeAccumulation::NoReset,     // disp_modeAccumulation;    
    Display::ENumSmoothing::Disable,        // disp_ENumSmoothing;       
    Display::ENumSignalsInSec::_25,         // disp_ENumSignalsInSec;    
    Grid::Type::_1,                         // disp_typeGrid;            
    50,                                     // disp_brightnessGrid;      
    Display::LinkingRShift::Position,       // disp_linkingRShift;       
    100,                                    // disp_brightness;          
    5,                                      // disp_timeShowLevels;      
    5,                                      // disp_timeMessages;        
    Display::AltMarkers::AutoHide,          // disp_altMarkers;          
    Menu::AutoHide::None,                   // disp_menuAutoHide;
    370,                                    // disp_shiftInMemory;
    Chan::A,                                // disp_lastAffectedChannel;

    // ���� ����� 1, ����� 2

    {RShift::ZERO, RShift::ZERO},                           // chan_shift[Chan::Size];  
    {Range::_500mV, Range::_500mV},                         // chan_range[Chan::Size];  
    {ModeCouple::DC, ModeCouple::DC},                       // chan_couple[Chan::Size]; 
    {true, true},                                           // chan_enabled[Chan::Size];
    {0, 0},                                                 // chan_balanceShiftADC[2];   
    {Bandwidth::Full, Bandwidth::Full},                     // chan_bandwidth[2];         
    {false, false},                                         // chan_inverse[2];           
    {Divider::_1, Divider::_1},                             // chan_divider[2];           
    {CalibrationMode::Disable, CalibrationMode::Disable},   // chan_calibrationMode[2];   

    // ���� �������������

    Trig::Source::A,            // trig_source;           
    Trig::Input::Full,          // trig_input;            
    Trig::Polarity::Rising,     // trig_polarity;         
    {Trig::ZERO, Trig::ZERO},   // trig_lev[Chan::Size];
    Trig::StartMode::Auto,      // trig_startMode;        
    Trig::ModeFind::Hand,       // trig_modeFind;         
    10,                         // trig_holdOff;          
    false,                      // trig_holdOffEnable     

    // ���� ���¨����

    TShift(0),              // time_shift;         
    TBase::_500us,          // time_base;         
    PeakDetMode::Disabled,  // time_peakDet;      
    TPos::Center,           // time_TPos;         
    SampleType::Real,       // time_sampleType;   
    FunctionTime::Time,     // time_timeDivXPos;  
    LinkingTShift::Time,    // time_linkingTShift;
    SampleType::Real,       // time_sampleTypeOld;

    // ���� �������

    false,                                                  // curs_showCursors;             
    {Cursors::LookMode::None, Cursors::LookMode::None},     // curs_lookMode[2];             
    true,                                                   // curs_showFreq;                
    Cursors::Active::None,                                  // curs_active;                  
    Chan::A,                                                // curs_source;                  
    {Cursors::Control::Disable, Cursors::Control::Disable}, // curs_cntrlU[Chan::Size];    
    {Cursors::Control::Disable, Cursors::Control::Disable}, // curs_cntrlT[Chan::Size];    
    Cursors::Movement::Pixels,                              // curs_movement;                
    {80,  80},                                              // curs_deltaU100percents[2];    
    {120, 120},                                             // curs_deltaT100percents[2];    
    {                                                       // curs_posCurU[Chan::Size][2];
    { 60, 140 },                                            // ����� 1
    { 60, 140 }},                                           // ����� 2
    {                                                       // curs_posCurT[Chan::Size][2];
    { 80, 200 },                                            // ����� 1
    { 80, 200 }},                                           // ����� 2

    // ���� ������

    ENumPointsFPGA::_1k,    // mem_enumPoints;                            
    ModeBtnMemory::Menu,    // mem_modeBtnMemory;                         
    ModeWork::Dir,          // mem_modeWork;                              
    false,                  // mem_flashAutoConnect;                      
    0,                      // mem_indexCurSymbolNameMask;                
    ModeSaveSignal::BMP,    // mem_modeSaveSignal;                        
    {},                     // mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];    
    ModeShowIntMem::Saved,  // mem_modeShowIntMem;                        
    FileNamingMode::Mask,   // mem_fileNamingMode;                        
    {},                     // mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME];
    
    // ���� ���������
    
    false,                                                  // meas_show;  
    Processing::Measures::Settings::OnDisplay::_1_5,        // meas_number;
    Processing::Measures::Settings::Source::A_B,            // meas_source;
    Processing::Measures::Settings::ModeViewSignals::AsIs,  // meas_modeViewSignals; 
    {                                                       // meas_measures[15];
        Processing::Measure::Type::VoltageMax,     Processing::Measure::Type::VoltageMin,  Processing::Measure::Type::VoltagePic,      Processing::Measure::Type::VoltageMaxSteady,
        Processing::Measure::Type::VoltageMinSteady,
        Processing::Measure::Type::VoltageAverage, Processing::Measure::Type::VoltageAmpl, Processing::Measure::Type::VoltageRMS,      Processing::Measure::Type::VoltageVybrosPlus,
        Processing::Measure::Type::VoltageVybrosMinus,
        Processing::Measure::Type::Period,         Processing::Measure::Type::Freq,        Processing::Measure::Type::TimeNarastaniya, Processing::Measure::Type::TimeSpada,
        Processing::Measure::Type::DurationPlus
    },
    Processing::Measure::Type::Freq,    // meas_marked;

    // ���� ������

    Language::RU,                       // serv_lang
    Color::Scheme::WhiteLetters,        // serv_colorScheme
    false,                              // serv_recorder
    CalibratorMode::Freq,               // serv_calibratorMode
    true,                               // serv_soundEnable
    100,                                // serv_soundVolume
    false,                              // serv_showInfoVoltage
    
    // ��������� ����������
    
    FuncModeDraw::Disable,      // math_modeDraw;     
    false,                      // math_enableFFT;    
    ScaleFFT::Log,              // math_scaleFFT;     
    SourceFFT::Both,            // math_sourceFFT;    
    WindowFFT::Hamming,         // math_windowFFT;    
    0,                          // math_currentCursor;
    { 50, 50 },                 // math_posCur[2];    
    FFTmaxDB::_60,              // math_FFTmaxDB;     
    Function::Sum,              // math_function;     
    ModeRegSet::Range,          // math_modeRegSet;   
    Range::_1V,                 // math_range;        
    Divider::_1,                // math_divider;      
    RShift::ZERO,               // math_rShift;       
    
    // ��������� �����������
    
    FrequencyCounter::Enabled::Off,         // freq_enabled;      
    FrequencyCounter::ModeView::Frequency,  // freq_modeView      
    FrequencyCounter::TimeCounting::_1s,    // freq_timeCounting; 
    FrequencyCounter::FreqClc::_1MHz,       // freq_freqClc;      
    FrequencyCounter::NumberPeriods::_10,   // freq_numberPeriods;
    
    // ���� �������
    
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
    Display::Orientation::Direct,           // dbg_Orientation;        
    
    // ��������� ������-����������
    
    Tester::Control::Voltage,       // test_control; 
    Tester::Polarity::Positive,     // test_polarity;
    Tester::StepU::_100mV,          // test_stepU;   
    Tester::StepI::_4mA,            // test_stepI;   
    Tester::Smoothing::_1,          // test_smooth;  
    Tester::ViewMode::Lines,        // test_viewMode 
    
    // ��������� ����
    
    0,                              // menu_currentPage;                  
    false,                          // menu_show;                         
    {                               // menu_posActItem[Page_NumPages];    
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
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

    Multimeter::AVP::Off,               // multi_avp
    Multimeter::Measure::VoltageDC,     // multi_meas
    Multimeter::RangeDC::_2V,           // multi_rangeCV
    Multimeter::RangeAC::_2V,           // multi_rangeVV
    Multimeter::RangeResistance::_10M,  // multi_rangeResist
    
    // ��������� ������������

    ViewAlways::InMode,                 // rec_viewAlways
    false,                              // rec_sourceChanA
    false,                              // rec_sourceChanB
    false,                              // rec_sourceSensor
    TypeStorage::Flash,                 // rec_storageRecord
    TypeStorage::Flash,                 // rec_storagePlay
    Axis::X,                            // rec_axisMove
    Axis::X,                            // rec_axisZoom
    ScaleX(ScaleX::_50ms),              // rec_scaleX
    { Range::_1V, Range::_1V },         // rec_range

    0,  // dbg_gate_max
    0   // dbg_gate_min
};
