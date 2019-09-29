#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"


/// ��� �������� ������
struct RecorderTypeStorage
{
    enum E
    {
        RAM,    ///< ���
        ROM,    ///< ����� ��
        EXT     ///< ����� ��
    } value;
    explicit RecorderTypeStorage(E v) : value(v) {};
};

/// ������������ ��� ������ ���
struct RecorderAxis
{
    enum E
    {
        X,
        Y
    } value;
    explicit RecorderAxis(E v) : value(v) {};
};

/// ������ �� �������
struct RecorderScaleX
{
    enum E
    {
        _100ms, // TBase = 0.1c
        _200ms, // TBase = 0.2c
        _500ms, // TBase = 0.5c
        _1s,    // TBase = 1c
        _2s,    // TBase = 2c
        _5s,    // TBase = 5c
        _10s,   // TBase = 10c
        Size
    } value;
    explicit RecorderScaleX(E v) : value(v) {};
    static void Load();
    static void Change(int delta);
    static RecorderScaleX &Current();
    pString ToString() const;
    /// ���������� �������� ����������� ������ � ������/���
    uint BytesToSec() const;
    /// ���������� ����� ���������� ����� �����
    uint TimeForPointMS() const;
};


#define REC_AXIS_MOVE                   (set.rec.axisMove)
#define REC_AXIS_ZOOM                   (set.rec.axisZoom)
#define REC_STORAGE_RECORD              (set.rec.storageRecord)
#define REC_STORAGE_PLAY                (set.rec.storagePlay)

#define REC_SRC_A                       (set.rec.enabledChanA)
#define REC_SRC_A_IS_ENABLED            (REC_SRC_A)

#define REC_SRC_B                       (set.rec.enabledChanB)
#define REC_SRC_B_IS_ENABLED            (REC_SRC_B)

#define REC_SRC_SENSOR                  (set.rec.enabledSensor)
#define REC_SRC_SENSOR_IS_ENABLED       (REC_SRC_SENSOR)

#define REC_SCALE_X                     (RecorderScaleX::Current())

struct SettingsRecorder
{
    bool                    enabledChanA;   ///< ��������� ������ ������ 1
    bool                    enabledChanB;   ///< ��������� ������ ������ 2
    bool                    enabledSensor;  ///< ��������� ������ ��������� �������
    RecorderTypeStorage::E  storageRecord;  ///< ���� ��������� ������
    RecorderTypeStorage::E  storagePlay;    ///< ������ ����� ���������� ������
    RecorderAxis::E         axisMove;       ///< ������� ��� �����������
    RecorderAxis::E         axisZoom;       ///< ������� ��� ���������������
    RecorderScaleX          scaleX;         ///< ������������� ������� �� �������
    int8                    currentCursor;
    Range::E                unused[2];      ///< ������� �� ��������� � ������ ������������
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRecorder
{
    static const Page *const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSource
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageShow
    {
        static const Page *const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageOperations
        {
            static const Page *const self;
        };

        struct PageView
        {
            static const Page *const self;

            //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            struct PageCursors
            {
                static const Page *const self;
            };
        };
    };
};
