#pragma once


struct SettingsTester
{
    Tester::Control::E  _control;
    Tester::Polarity::E _polarity;       // ���������� ������������ �����������
    Tester::StepU::E    stepU;          // ��������� ��� ��������� ����������
    Tester::StepI::E    stepI;          // ��������� ��� ��������� ����
    ENumAverage::E      enumAverage;    // ���������� ����������
    Tester::ViewMode::E viewMode;       // ����� ����������� ������
};



struct PageTester
{
    static void Init();

    static const Page *const self;
};
