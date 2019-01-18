#pragma once
#include "SettingsMath.h"
#include "SettingsMemory.h"
#include "SettingsDebug.h"
#include "SettingsNRST.h"
#include "SettingsService.h"
#include "SettingsTime.h"
#include "Tester/Tester.h"
#include "Osci/Cursors.h"
#include "Osci/FrequencyCounter.h"
#include "Menu/Menu.h"
#include "Display/Grid_Types.h"
#include "Display/Colors.h"
#include "Multimeter/Multimeter.h"
#include "Utils/Measure.h"
#include "Recorder/Recorder_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_SIZE                    (set.size)
#define SET_CRC32                   (set.crc32)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_RANDOMIZE_MODE           (SET_TBASE < TBase::_100ns)
#define CURRENT_PAGE                (set.menu_currentPage)

#define TRIG_MODE                   (set.trig_mode)
#define TRIG_HOLDOFF                (set.trig_holdOff)
#define TRIG_HOLDOFF_ENABLED        (set.trig_holdOffEnabled)

/// ���������� ������� ��������� ������ �� �������� Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu_posActItem[name])

#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();
    static void Reset();
    /// ������������� ��������� � ������ ��� ���������� � ����������������� ������
    void TransformForSave(uint8 data[1024]);
    /// ������������� ����������� �� ����������������� ������ ��������� � ���������� ���
    static void TransformFromLoad(uint8 data[1024]);

    void SetData(uint8 data[1024]);
    /// ��������� �������� ������������� ����. ���������� ����� ������ ��������� ������ (���� ���� ���������� ��������� ��������)
    void SaveEnum16(uint8 name, uint8 value);

    uint                size;                               ///< ������ ������ ��������� � ������
    uint                crc32;                              ///< \brief ����������� ����� ������ ��������� � ����������� � ��� �����������

    // ���� �������
                                                                ///< ����������� �� ������� �����, ���������� �� ���� �����
    Display::ModeDrawSignal::E      disp_modeDrawSignal;
    Display::ThicknessSignal::E     disp_thickness;             ///< ������� �����, ������� �������� ������
    Display::Background::E          disp_background;            ///< ���� ����.
    Display::ShowStrNavi::E         disp_showStringNavigation;  ///< ����� ������ ������ ��������� ����.
    Display::ENumMinMax::E          disp_ENumMinMax;            ///< ������������ ���������� ��������� ��� ����������� ��������� � ���������.
    Display::ModeAveraging::E       disp_modeAveraging;         ///< ��� ���������� �� ����������.
    Display::ENumAverage::E         disp_ENumAverage;           ///< ����� ���������� �������.
    Display::ENumAccum::E           disp_ENumAccum;             ///< ����� ���������� ������� �� ������.
    Display::ModeAccumulation::E    disp_modeAccumulation;      ///< ����� ���������� ��������.
    Display::ENumSmoothing::E       disp_ENumSmoothing;         ///< ������������ ���������� ����� ��� ����������� �������.
    Display::ENumSignalsInSec::E    disp_ENumSignalsInSec;      ///< ������������ ����� ���������� ������� � �������.
    Grid::Type::E                   disp_typeGrid;              ///< ��� �����
    int16                           disp_brightnessGrid;        ///< ������� ����� �� 0 �� 100.
    Display::LinkingRShift::E       disp_linkingRShift;         ///< ��� �������� � �������� �� ���������.
    int16                           disp_brightness;            ///< ������� �������.
    int16                           disp_timeShowLevels;        ///< �����, � ������� �������� ����� ���������� ������ ��������.
    int16                           disp_timeMessages;          ///< ����� � ��������, �� ������� ��������� ������� �� �����.
    Display::AltMarkers::E          disp_altMarkers;            ///< ����� ����������� �������������� ������� �������� ��������.
    Menu::AutoHide::E               disp_menuAutoHide;          ///< ����� ������� ������� ����� ���������� ������� ������� ������� ����.
    int16                           disp_shiftInMemory;         ///< \brief ���������� �������� ������ ���� ����� ������������ �������� ����� ������ � 
                                                                ///< ������. �.�. ��� �������� ��������� ����� � ��� ���� ������ ���������� ����� �� ������.
    Chan::E                         disp_lastAffectedChannel;   ///< \brief ��������� ����������� �����. ������������ ��� ����������� ������ ��������
                                                                ///< �� ����� � ���������� ���� ������ �������

    // ���� ����� 1, ����� 2

    uint16                         chan_shift[Chan::Size];    ///< ����� ������ �� ���������
    Osci::Settings::Range::E       chan_range[Chan::Size];    ///< ������� ������ �� ���������
    FPGA::Settings::ModeCouple::E  chan_couple[Chan::Size];   ///< ����� �� �����
    bool                           chan_enabled[Chan::Size];  ///< �������/�������� �����
    int8                           chan_balanceShiftADC[2];     ///< ���������� �������� ��� ������������ ���.
    FPGA::Settings::Bandwidth::E   chan_bandwidth[2];           ///< ����������� ������.
    bool                           chan_inverse[2];
    Divider::E                     chan_divider[2];             ///< ���������.
    CalibrationMode::E             chan_calibrationMode[2];     ///< ����� ����������.

    // ���� �������������

    Osci::Settings::Trig::Source::E    trig_source;
    Osci::Settings::Trig::Input::E     trig_input;
    Osci::Settings::Trig::Polarity::E  trig_polarity;
    uint16                             trig_lev[Chan::Size];
    Osci::Settings::Trig::StartMode::E trig_startMode;           ///< ����� �������.
    Osci::Settings::Trig::ModeFind::E  trig_modeFind;            ///< ����� ������������� - ������� ��� �������������.
    int                                trig_holdOff;             ///< ��������� �������������
    bool                               trig_holdOffEnabled;      ///< ��������� ��������� �������������

    // ���� ���¨����

    Osci::Settings::TShift           time_shift;
    Osci::Settings::TBase::E         time_base;
    Osci::Settings::PeakDetMode::E   time_peakDet;
    Osci::Settings::TPos::E          time_TPos;
    Osci::Settings::SampleType::E    time_sampleType;
    Osci::Settings::FunctionTime::E  time_timeDivXPos;
    Osci::Settings::LinkingTShift::E time_linkingTShift;                 ///< ��� �������� �������� �� �����������
    Osci::Settings::SampleType::E    time_sampleTypeOld;

    // ���� �������

    bool                    curs_showCursors;                   ///< ���������� �� �������.
    Cursors::LookMode::E    curs_lookMode[2];                   ///< ������ �������� �� ��������� ��� ���� ��� ��������.
    bool                    curs_showFreq;                      ///< ������������� � true ��������, ��� ����� ���������� �� ������ 1/dT ����� ���������.
    Cursors::Active::E      curs_active;                        ///< ����� ������� ����� �������.
    Chan::E                 curs_source;                        ///< �������� - � ������ ������ ��������� �������.
    Cursors::Control::E     curs_cntrlU[Chan::Size];            ///< �������� ������� ����������.
    Cursors::Control::E     curs_cntrlT[Chan::Size];            ///< �������� ������� �������.
    Cursors::Movement::E    curs_movement;                      ///< ��� ������������ �������� - �� ������ ��� ���������.
    float                   curs_deltaU100percents[2];          ///< ���������� ����� ��������� ���������� ��� 100%, ��� ����� �������.
    float                   curs_deltaT100percents[2];          ///< ���������� ����� ��������� ������� ��� 100%, ��� ����� �������.
    float                   curs_posCurU[Chan::Size][2];        ///< ������� ������� �������� ���������� ����� �������.
    float                   curs_posCurT[Chan::Size][2];        ///< ������� ������� �������� ������� ����� �������.

    // ���� ������

#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENumPointsFPGA::E   mem_enumPoints;                             ///< ����� �����
    ModeBtnMemory::E    mem_modeBtnMemory;
    ModeWork::E         mem_modeWork;                               ///< ����� ������.
    bool                mem_flashAutoConnect;                       ///< ���� true, ��� ����������� ������ ������������� ��������� ����-��������.
    int8                mem_indexCurSymbolNameMask;                 ///< ������ �������� ������� � ������ ������� ����� ��� ������ �����.
    ModeSaveSignal::E   mem_modeSaveSignal;                         ///< � ����� ���� ��������� ������.
    char                mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< ��� ����� ��� ������ ������� �������.
    ModeShowIntMem::E   mem_modeShowIntMem;                         ///< ��� ���������� � ������ ����� �� - ��������� ��� ���������� ������.
    FileNamingMode::E   mem_fileNamingMode;                         ///< ����� ���������� ������.
    char                mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief ����� ����� ��� ��������������� ���������� ������.
                        ///< \details ������� ����������.\n
                        /// \code
                        /// %y('\x42') - ���, %m('\x43') - �����, %d('\x44') - ����, %H('\x45') - ����, %M('\x46') - ������, %S('\x47') - �������
                        /// %Nn('\x48''n') - ���������� �����, ������ �������� �� ����� n ���������, ��������, 7 � %3N ����� ������������� � 007
                        /// �������
                        /// name_%4N_%y_%m_%d_%H_%M_%S ����� ������������ ����� ���� name_0043_2014_04_25_14_45_32
                        /// ��� ���� �������� ��������, ��� ���� ������������ %4N ����� ����� ��������� ����������, ��, ������ �����, ���� �������� 
                        /// ����� ������ ����� 0001, �.�. ��� ����������� ������ ���������������.
                        /// \endcode

    // ���� ���������

    bool                        meas_show;                 ///< ���������� �� ���������.
    Measure::OnDisplay::E       meas_number;               ///< ������� ��������� ��������.
    Measure::Source::E          meas_source;               ///< ��� ����� ������� �������� ���������.
    Measure::ModeViewSignals::E meas_modeViewSignals;      ///< ������� �� ������� ��� ������ ���������.
    Measure::Type::E            meas_measures[15];         ///< ��������� ��� ��������� ���������.
    Measure::Type::E            meas_marked;               ///< ���������, �� ������� ����� �������� �������.

    // ���� ������

    Language::E         serv_lang;
    Color::Scheme::E    serv_colorScheme;
    bool                serv_recorder;                  ///< ������� �� ����� ������������.
    CalibratorMode::E   serv_calibratorMode;            ///< ����� ������ �����������.
    bool                serv_soundEnable;               ///< �������� �� �����.
    int16               serv_soundVolume;               ///< ��������� ����� [0...100].

    // ��������� ����������

    FuncModeDraw::E          math_modeDraw;             ///< ���������� ��� ����� ������� � ������ �������������� �������.
    bool                     math_enableFFT;
    ScaleFFT::E              math_scaleFFT;
    SourceFFT::E             math_sourceFFT;
    WindowFFT::E             math_windowFFT;
    uint8                    math_currentCursor;        ///< ����������, ����� �������� ������� ��������� ����� ���������.
    uint8                    math_posCur[2];            ///< ������� ������� �������. ���������� 0...256.
    FFTmaxDB::E              math_FFTmaxDB;
    Function::E              math_function;
    ModeRegSet::E            math_modeRegSet;           ///< ������� ����� ��������� - ������� �� ������� ��� �������� �� ���������.
    Osci::Settings::Range::E math_range;
    Divider::E               math_divider;
    uint16                   math_rShift;

    // ��������� �����������

    FrequencyCounter::Enabled::E        freq_enabled;
    FrequencyCounter::ModeView::E       freq_modeView;      
    FrequencyCounter::TimeCounting::E   freq_timeCounting;  ///< ����� ����� �������.
    FrequencyCounter::FreqClc::E        freq_freqClc;       ///< ������� ����������.
    FrequencyCounter::NumberPeriods::E  freq_numberPeriods; ///< ���������� ��������.

    // ���� �������

    int16                        dbg_numStrings;                 ///< ����� ����� � �������.
    int8                         dbg_sizeFont;                   ///< ������ ������ ������� - 0 - 5, 1 - 8.
    bool                         dbg_modePauseConsole;           ///< ���� true, �� ����� � ������� ��������������� �������� ������ ����/����.
    bool                         dbg_showAll;
    bool                         dbg_showFlag;
    bool                         dbg_showRShift[2];
    bool                         dbg_showTrigLev;
    bool                         dbg_showRange[2];
    bool                         dbg_showChanParam[2];
    bool                         dbg_showTrigParam;
    bool                         dbg_showTShift;
    bool                         dbg_showTBase;
    int16                        dbg_numMeasuresForGates;        ///< ����� ��������� ��� �����.
    bool                         dbg_showRandInfo;               ///< �������� ���������� �� ������������� - ������ � ��������� ��������.
    bool                         dbg_showRandStat;               ///< �������� ������ ����������.
    int16                        dbg_timeCompensation;           ///< �������������� �������� �� ������� ��� ������ �������� ������ �������������.
    int16                        dbg_pretriggered;               ///< ������������ �������� ����������� ��� ������������ �������������.
    FPGA::Settings::Bandwidth::E dbg_bandwidth[2];               ///< \brief ����� ������� ������, ������� ����� ���������� ��� ������ � "�����-������" 
                                                        ///< �������� "������".
    bool                  dbg_ShowStats;                ///< ���������� ���������� �� ������ (fps, ��������).
    Display::Orientation  dbg_Orientation;

    // ��������� ������-����������

    Tester::Control::E     test_control;
    Tester::Polarity::E    test_polarity;
    Tester::StepU::E       test_stepU;
    Tester::StepI::E       test_stepI;
    Tester::Smoothing::E   test_smooth;                    ///< ���������� �����������
    Tester::ViewMode::E    test_viewMode;

    // ��������� ����

    const Page*         menu_currentPage;                                           ///< ��������� �� �������� �������� ����
    bool                menu_show;                                                  ///< ���� true, �� ����� ���������� ������� ��������
    int8                menu_posActItem[Page::Name::Number];                        ///< \brief ������� ��������� ������. bit7 == 1 - item is opened, 0x7f - ��� 
                                                                                    ///< ��������� ������.
    int8                menu_currentSubPage[Page::Name::Number];                    ///< ����� ������� �����������.

    // �������������� ��������� ����������

    int16               nrst_balanceADC[Chan::Size];                                ///< �������� ��������������� �������� ��� ��� ������ ������������.
    BalanceADC::E       nrst_balanceADCtype;                                        ///< ��� ������������.
    StretchADC::E       nrst_stretchADCtype;                                        ///< ��� �������� ������.
    int16               nrst_StretchADC[Chan::Size][3];                             ///< \brief ����������� ����������� ��� �������, �������������� �
                        ///< ������������ �������. ����� �������� � ����� ����, ����� �������� �������� �����������, ����� ��������� �� 1000 �
                        ///< ��������� �������.
    int16               nrst_rShiftAddStable[Chan::Size][3];                        ///< ���������� �������� ��� ��� ����� �������������� ����������. ������� �������� ��� ���������
    int16               nrst_numAveForRand;                                         ///< �� �������� ���������� ��������� ������ � ������ �������������.
    int16               nrst_numSmoothForRand;                                      ///< ����� ����� ��� ����������� ������ � �������������.
    int16               nrst_rShiftAdd[Chan::Size][Osci::Settings::Range::Size][2]; ///< ���������� ��������, ������� ������� ���� ��� ���������� � ������������
    int16               nrst_correctionTime;                                        ///< ����������� ��������� �������.

    // ��������� �����������
    Multimeter::AVP::E              multi_avp;
    Multimeter::Measure::E          multi_meas;
    Multimeter::RangeDC::E          multi_rangeDC;
    Multimeter::RangeAC::E          multi_rangeAC;
    Multimeter::RangeResistance::E  multi_rangeResist;

    // ��������� ������������
    Recorder::Settings::ViewAlways::E  rec_viewAlways;       ///< ���������� �� ������ ������ ������������
    bool                               rec_sourceChanA;      ///< ��������� ������ ������ 1
    bool                               rec_sourceChanB;      ///< ��������� ������ ������ 2
    bool                               rec_sourceSensor;     ///< ��������� ������ ��������� �������
    Recorder::Settings::TypeStorage::E rec_storageRecord;    ///< ���� ��������� ������
    Recorder::Settings::TypeStorage::E rec_storagePlay;      ///< ������ ����� ���������� ������
    Recorder::Settings::Axis::E        rec_axisMove;         ///< ������� ��� �����������
    Recorder::Settings::Axis::E        rec_axisZoom;         ///< ������� ��� ���������������
    Recorder::Settings::ScaleX         rec_scaleX;           ///< ������������� ������� �� �������
    Osci::Settings::Range::E           rec_range[2];         ///< ������� �� ��������� � ������ ������������


    int16   dbg_enum_gate_max;      ///< ����������� ����� � ������������� ������
    int16   dbg_enum_gate_min;      ///< ����������� ����� � ������������� �����

    /// ��������� �� ���������
    static const Settings defaultSettings;
};

#pragma pack(pop)

extern Settings set;
