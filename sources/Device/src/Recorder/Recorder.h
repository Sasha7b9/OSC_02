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
    struct DataStorage
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

    //static void Init();

    class Graphics
    {
    public:
        static void Update();
    };
};
