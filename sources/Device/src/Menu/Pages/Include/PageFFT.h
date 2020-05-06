#pragma once


struct ScaleFFT
{
    enum E
    {
        Log,           // ��� �������� �������� ��������������� ����� ������ �������.
        Linear         // ��� �������� �������� �������� ����� ������ �������.
    };
};

struct SourceFFT
{
    enum E
    {
        A,
        B,
        Both
    };
};

struct WindowFFT
{
    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    };
};

struct MaxDBFFT
{
    enum E
    {
        _40,
        _60,
        _80
    };

    static float MaxDBforFFT(MaxDBFFT::E maxDB)
    {
        static const float arrayMAX_DB_FOR_FFT[] = { -40.0F, -60.0F, -80.0F };

        return arrayMAX_DB_FOR_FFT[maxDB];
    }
};


#define S_FFT_ENABLED   (set.fft.enabled)


struct SettingsFFT
{ //-V802
    bool            enabled;
    uint8           posCur[2];      // ������� ������� �������. ���������� 0...256.
    ScaleFFT::E     scale;
    SourceFFT::E    source;
    WindowFFT::E    window;
    MaxDBFFT::E     maxDB;
    uint8           cursor;         // ����������, ����� �������� ������� ��������� ����� ���������.
};


struct PageFFT
{
    static const Page *const self;

    struct Cursors
    {
        static const Page *const self;
    };
};
