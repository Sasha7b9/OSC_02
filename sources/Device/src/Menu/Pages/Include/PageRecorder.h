#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"
#include "Settings/SettingsOsci.h"


struct SettingsRecorder
{ //-V802
    bool                    enA;            ///< ��������� ������ ������ 1
    bool                    enB;            ///< ��������� ������ ������ 2
    bool                    enSensor;       ///< ��������� ������ ��������� �������
    Recorder::TypeMemory::E typeMemory;     ///< ����� �������� ������������/������������ ������
    Recorder::Axis::E       axisMove;       ///< ������� ��� �����������
    Recorder::Axis::E       axisZoom;       ///< ������� ��� ���������������
    Recorder::ScaleX::E     scaleX;         ///< ������������� ������� �� �������
    int8                    currentCursor;  ///< ������, ������� ������ ����� ���������
    Range::E                unused[2];      ///< ������� �� ��������� � ������ ������������
};


struct PageRecorder
{
    static const Page *const self;

    struct PageSource
    {
        static const Page *const self;
    };

    struct PageShow
    {
        static const Page *const self;

        struct PageChoice
        {
            static const Page *const self;
        };

        struct PageCursors
        {
            static const Page *const self;
        };
    };
};
