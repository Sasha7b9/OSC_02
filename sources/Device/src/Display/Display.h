#pragma once
#include "Display_Types.h"
#include "Display_Warnings.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    void Init();

    void Update();

    void RemoveAddDrawFunction();

    void ShowWarning(Warning::E warning);
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode::Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode::Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    void SetDrawMode(DrawMode::E mode, pFuncVV func);

    void FuncOnWaitStart(const char *text, bool eraseBackground);

    void FuncOnWaitStop();

    void SetAddDrawFunction(pFuncVV func, uint time = MAX_UINT);

    void ChangedRShiftMarkers(bool active);

    extern Key::E key;

    void SetOrientation(Orientation orientation);
    /// ������������� ����������� ������� ������
    static void SetNumSignalsInS(int maxFPS);
    /// ���������� ����� ����� ������������� ������� (����� ������� - �����������)
    //static int NumPointSmoothing();
    /// ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
    BitSet64 PointsOnDisplay();
    /// \brief ��������� ����� ������ � ��������� ����� � ����������� ������
    BitSet64 BytesOnDisplay();
    /// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
    int TimeMenuAutoHide();
    /// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    bool IsSeparate();
};
