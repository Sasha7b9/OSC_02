#pragma once
#include "Keyboard/Keyboard.h"



class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

#define MOSI_HEIGHT         9
#define MOI_HEIGHT_TITLE    19
#define MOI_WIDTH_D_IP      34      /* ���������� ������ ��������� ip-������ � ������ ������� ����� */
#define GRID_DELTA          20      /* ���������� ����� � ������ */

#define TIME_MESSAGES               5


#define DISPLAY_ORIENTATION         (set.dbg_Orientation)
#define DISPLAY_ORIENTATION_IS_NORMAL (DISPLAY_ORIENTATION == Display::Orientation::Normal)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)

#define SHIFT_IN_MEMORY_IN_POINTS   Display::ShiftInMemoryInPoints()


struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    } value;
};

struct Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    static void Init();
    /// ����� ���������� ��� ���������
    static void Update();
    /// ������������ �������� true ��������, ��� ������� ��������� � ��������� ���������
    static bool InProcess();
    /// ������������� �������, ������� ��������� ����� ��������� ����� ����������
    static void SetFuncAfterUpadteOnce(pFuncVV func);

	static void ShowWarning(const char* warning);
    /// @brief ���������� ������� � ����� ��������� ������.
    /// @details �������� ��� ��������.
    /// 1. DrawMode_Hand - � ���� ������ ����� ���������� ������� func(), ������������ �������������, � �������� 25 ��� � �������.
    /// ������������ � �������, ����� �� ����������� ������� ����.
    /// 2. DrawMode::Auto � func == 0 - � ���� ������ ����� ����������� ������� Update() � ������� �����.
    /// 3. DrawMode::Auto � func != 0 - � ���� ������ ����� ����������� ������� func(), ������������ �������������, �� � ����
    /// �������� �����, ������ ���������� �� Update() ������ Update().
    static void SetDrawMode(DrawMode::E mode, pFuncVV func = nullptr);

    static void FuncOnWaitStart(const char *text, bool eraseBackground);

    static void FuncOnWaitStop();
    /// ������������� �������������� �������, ������� ����� �������������� ������ ��� ����� ���������� ������
    /// timeRemove - �����, �� ��������� �������� �������������� ������� ��������� ����� �������. ���� ��� �� ���������, ������� ��������� �������������� �� �����
    static void SetAddDrawFunction(pFuncVV func, uint timeRemove = 0);
    /// ������� ������������� ������� ���������, ������������� ������� ������� SetAddDrawFunction()
    static void RemoveAddDrawFunction();
    /// ���������� �����, ����� ������� ���� ������������� ����������, ���� �� ���� ������ �������
    static int TimeMenuAutoHide();
    /// ���� ����� ������� �� ��� ����� � �������� ������ ��������� ������ - ��������, � ������ ������ �������
    static bool IsSeparate();

    static void SaveScreenToDrive();

    static void SaveRow(int row);
};
