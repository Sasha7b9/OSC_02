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
    static void OnArrow();     // Key::Left, Key::Up, Key::Right, Key::Down
    static void OnEnter();     // Key::Enter
    static void OnTime();      // Key::Time
    static void OnStart();     // Key::Start
    static void OnFunction();  // Key::Function
    static void OnService();   // Key::Service
    static void OnMeasures();  // Key::Measure
    static void OnDisplay();   // Key::Display
    static void OnMemory();    // Key::Memory
    static void OnChannelA();  // Key::ChannelA
    static void OnChannelB();  // Key::ChannelB
    static void OnTrig();      // Key::Trig
    static void OnTrigLev();   // Key::TrigLevLess, Key::TrigLevMore
    static void OnRangeA();    // Key::RangeLessA, Key::RangeMoreA
    static void OnRangeB();    // Key::RangeLessB, Key::RangeMoreB
    static void OnRShiftA();   // Key::RShiftLessA, Key::RShiftMoreB
    static void OnRShiftB();   // Key::RShiftLessB, Key::RShiftMoreB
    static void OnTBase();     // Key::TBaseLess, Key::TBase::More
    static void OnTShift();    // Key::TShiftLess, Key::TShiftMore
    static void OnFX();        // Key::F1, Key::F2, Key::F3, Key::F4, Key::F5
};
