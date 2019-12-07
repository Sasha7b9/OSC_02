#pragma once


struct Recorder
{
    static bool IsEnabledA();

    static bool IsEnabledB();

    static void Init();

    static void DeInit();

    static void Update();

    static void OnPressStart();

    static void Start();

    static void Stop();

    static bool IsRunning();

    static void ReadPoint();
};
