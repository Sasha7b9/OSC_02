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
    /// ����� ���������� ��� ���������
    void Update();
    /// ������������ �������� true ��������, ��� ������� ��������� � ��������� ���������
    bool InProcess();
    /// ������������� �������, ������� ��������� ����� ��������� ����� ����������
    void SetFuncAfterUpadteOnce(pFuncVV func);

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
    /// ������������� �������������� �������, ������� ����� �������������� ������ ��� ����� ���������� ������
    /// timeRemove - �����, �� ��������� �������� �������������� ������� ��������� ����� �������. ���� ��� �� ���������, ������� ��������� �������������� �� �����
    void SetAddDrawFunction(pFuncVV func, uint timeRemove = 0);
    /// ������� ������������� ������� ���������, ������������� ������� ������� SetAddDrawFunction()
    void RemoveAddDrawFunction();

    void ChangedRShiftMarkers(bool active);

    void SetOrientation(Orientation::E orientation);
    /// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
    int TimeMenuAutoHide();
    /// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    bool IsSeparate();
};
