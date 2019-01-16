#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RECORDER_VIEW_ALLWAYS   (set.rec_viewAlways)
#define RECORDER_SOURCE_A       (set.rec_sourceChanA)
#define RECORDER_SOURCE_B       (set.rec_sourceChanB)
#define RECORDER_SOURCE_SENSOR  (set.rec_sourceSensor)
#define RECORDER_STORAGE_RECORD (set.rec_storageRecord)
#define RECORDER_STORAGE_PLAY   (set.rec_storagePlay)
#define RECORDER_AXIS_MOVE      (set.rec_axisMove)
#define RECORDER_AXIS_ZOOM      (set.rec_axisZoom)


#define RECORDER_SCALE_X        (Recorder::Settings::ScaleX::Current())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Recorder
{
    namespace Settings
    {
        /// ���������� �� ������ ���������� ���������
        struct ViewAlways
        {
            enum E
            {
                InMode,
                InAlways
            } value;
            explicit ViewAlways(E v) : value(v) {};
        };

        /// C ����� ���������� ��������
        struct Storage
        {
            enum E
            {
                RAM,
                Flash,
                PC
            } value;
            explicit Storage(E v) : value(v) {};
        };

        /// ������������ ��� ������ ���
        struct Axis
        {
            enum E
            {
                X,
                Y
            } value;
            explicit Axis(E v) : value(v) {};
        };

        /// ������ �� �������
        struct ScaleX
        {
            enum E
            {
                _5ms,   // TBase = 100ms
                _10ms,  // TBase = 200ms
                _25ms,  // TBase = 500ms
                _50ms,  // TBase = 1s
                _100ms, // TBase = 2s
                _250ms, // TBase = 5s,
                _500ms, // TBase = 10s,
                _1s,    // TBase = 20s,
                _2s,    //
                _5s,    //
                _10s,   //
                _20s,   //
                _30s,   //
                _60s,   //
                Size
            } value;
            explicit ScaleX(E v) : value(v) {};
            static void Load();
            static void Change(int delta);
            static ScaleX &Current();
            pString ToString() const;
        };
    };
}

