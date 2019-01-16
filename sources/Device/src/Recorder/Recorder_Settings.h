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
        };

        /// ������������ ��� ������ ���
        struct Axis
        {
            enum E
            {
                X,
                Y
            } value;
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
                _50s,   //
                _100s   //
            } value;
        };
    };
}

