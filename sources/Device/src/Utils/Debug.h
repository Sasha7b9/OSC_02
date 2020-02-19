#pragma once


// ������ ������������ ��� �����������, ���� ����� � HardFault_Handler() ����������� ��� ����������
#define START_PROFILING  Debug::StartProfilingMS();
#define POINT_PROFILING  Debug::PointProfilingMS(__FILE__, __LINE__)

//#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;

namespace Debug
{
    void StartProfiling();
    uint PointProfiling();

    void StartProfilingMS();
    uint PointProfilingMS(const char *file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line[10];
    extern const char *file[10];
};
