#pragma once
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Hardware
 *  @{
 *  @defgroup Sound
 *  @brief ������ �� �������� ���������
 *  @{
 */


struct TypeWave
{
    enum E
    {
        Sine,
        Meandr,
        Triangle
    } value;
};


namespace Beeper
{
    void Init();

    /// ���� ������� �� ������.
    void ButtonPress();
    /// ������� ������� ���� ����������� ������ ������ ���� ����� ���� ������������ ���� ������� ������.
    void ButtonRelease();

    void GovernorChangedValue();
    
    void RegulatorShiftRotate();

    void RegulatorSwitchRotate();

    void WarnBeepBad();

    void WarnBeepGood();
    /// ��� ������� ���� �������� ����� �������/��������� ����. ���� ����������� � ����.
    void WaitForCompletion();

    void Beep(TypePress::E type);

    void Test();
};


/** @}  @}
 */
