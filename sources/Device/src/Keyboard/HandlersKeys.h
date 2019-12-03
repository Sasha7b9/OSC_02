#pragma once
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"


typedef void (*pFuncVChI16)(Chan::E, int16);
typedef void (*pFuncVCh)(Chan::E);
typedef void (*pFuncVI)(int);


struct Handlers
{
    static void Process(KeyEvent);

private:
    /// �������������� �������
    static KeyEvent event;
    /// �����, ��������� �������� ����� �������� ��������
    static Chan drawingChan;
    /// ����� ���������� ��������� ��������� ������ - �������� ��� ��������
    static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);
    /// ������ ����������
    static void Empty();
    /// ����� ���������� ��������� ��������. ���������� true, ���� ��������� � ���������� ��������� ������� ���������� �� ���������.
    static bool CommonHandlerPage();
    /// ����� ���������� ��������� ��������� ����������
    static void OnChangeParameterTime(pFuncVI, int);
    /// ��������� �������� ��� ��������� ���� � ����������� �� ����, ����� �������� ������ ��������
    static void ShowHidePage(const Page *page);

    static void ChangeRange(Chan::E ch, int16 delta);

    static void ChangeTShift(int delta);

    static void ChangeTBase(int delta);

    static void ChangeRShift(Chan::E ch, int16 delta);
    /// ����������� ������� ������
    static void OnArrow();              // Key::Left, Key::Up, Key::Right, Key::Down
    static void OnEnter_Release();      // Key::Enter
    static void OnEnter_Long();         // Key::Enter
    static void OnTime_Long();          // Key::Time
    static void OnTime_Release();       // Key::Time
    static void OnStart();              // Key::Start
    static void OnFunction();           // Key::Function
    static void Service_Release();      // Key::Service
    static void Measures_Release();     // Key::Measure
    static void Display_Release();      // Key::Display
    static void Memory_Release();       // Key::Memory
    static void ChannelA_Release();     // Key::ChannelA
    static void ChannelB_Release();     // Key::ChannelB
    static void ChannelA_Long();        // Key::ChannelA
    static void ChannelB_Long();        // Key::ChannelB
    static void Trig_Release();         // Key::Trig
    static void Trig_Long();            // Key::Trig
    static void TrigLevLess_Press();    // Key::TrigLevLess
    static void TrigLevMore_Press();    // Key::TrigLevMore
    static void RangeLessA();           // Key::RangeLessA
    static void RangeMoreA();           // Key::RangeMoreA
    static void RangeLessB();           // Key::RangeLessB
    static void RangeMoreB();           // Key::RangeMoreB
    static void RShiftLessA();          // Key::RShiftLessA
    static void RShiftMoreA();          // Key::RShiftMoreA
    static void RShiftLessB();          // Key::RShiftLessB
    static void RShiftMoreB();          // Key::RShiftMoreB
    static void OnTBase();              // Key::TBaseLess, Key::TBase::More
    static void OnTShift();             // Key::TShiftLess, Key::TShiftMore
    static void OnFX();                 // Key::F1, Key::F2, Key::F3, Key::F4, Key::F5
};


