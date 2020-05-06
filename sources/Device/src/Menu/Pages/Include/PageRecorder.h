#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"


#define S_REC_ENABLED_A         (set.rec._enA)
#define S_REC_ENABLED_B         (set.rec._enB)
#define S_REC_ENABLED_SENSOR    (set.rec._enSensor)
#define S_REC_CURRENT_CURSOR    (set.rec._currentCursor)


struct SettingsRecorder
{ //-V802
    bool                    _enA;            // ��������� ������ ������ 1
    bool                    _enB;            // ��������� ������ ������ 2
    bool                    _enSensor;       // ��������� ������ ��������� �������
    Recorder::TypeMemory::E _typeMemory;     // ����� �������� ������������/������������ ������
    Recorder::Axis::E       _axisMove;       // ������� ��� �����������
    Recorder::Axis::E       _axisZoom;       // ������� ��� ���������������
    Recorder::ScaleX::E     _scaleX;         // ������������� ������� �� �������
    int8                    _currentCursor;  // ������, ������� ������ ����� ���������
};


struct PageRecorder
{
    static const Page *const self;

    struct Source
    {
        static const Page *const self;
    };

    struct Show
    {
        static const Page *const self;

        struct Choice
        {
            static const Page *const self;
        };

        struct Cursors
        {
            static const Page *const self;
        };
    };
};
