#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"


struct SettingsRecorder
{ //-V802
    bool                    enA;            // ��������� ������ ������ 1
    bool                    enB;            // ��������� ������ ������ 2
    bool                    enSensor;       // ��������� ������ ��������� �������
    Recorder::TypeMemory::E typeMemory;     // ����� �������� ������������/������������ ������
    Recorder::Axis::E       axisMove;       // ������� ��� �����������
    Recorder::Axis::E       axisZoom;       // ������� ��� ���������������
    Recorder::ScaleX::E     scaleX;         // ������������� ������� �� �������
    int8                    currentCursor;  // ������, ������� ������ ����� ���������
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
