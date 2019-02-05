#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ������ ������������ ��� �����������, ���� ����� � HardFault_Handler() ����������� ��� ����������
// #define DEBUG_POINT Debug::line = __LINE__; Debug::file = __FILE__;

#define POINT_PROFILING() Debug::PointProfilingMS(__FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Debug
{
    static void StartProfiling();
    static void PointProfiling(char *name);

    static void StartProfilingMS();
    static void PointProfilingMS(char *file, int line);

    static void ClearTimeCounter();
    static void StartIncreaseCounter();
    static void StopIncreaseCounter();
    static uint GetTimeCounterUS();

    static int line;
    static const char *file;
};
