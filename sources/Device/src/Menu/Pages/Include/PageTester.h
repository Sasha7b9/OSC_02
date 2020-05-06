#pragma once


struct SettingsTester
{
    Tester::Control::E  _control;
    Tester::Polarity::E _polarity;       // ���������� ������������ �����������
    Tester::StepU::E    _stepU;          // ��������� ��� ��������� ����������
    Tester::StepI::E    _stepI;          // ��������� ��� ��������� ����
    ENumAverage::E      _enumAverage;    // ���������� ����������
    Tester::ViewMode::E viewMode;       // ����� ����������� ������
};



struct PageTester
{
    static void Init();

    static const Page *const self;
};
