#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasures
{
    static const Page * const self;

    static const Page *GetPageFrequencyCounter();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresAuto
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageTune
    {
    public:
        static const Page * const self;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresCursors
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSet
    {
        static const Page * const self;

        static void Draw_Channel(int x, int y);

        static void OnPress_Channel();
        /// ������� ��������� ������
        static void IncCursCntrlU(Chan::E ch);
        /// ������� ��������� ������
        static void IncCursCntrlT(Chan::E ch);

        static void OnPress_U();

        static void OnPress_T();

        static void SetCursPos100(Chan::E ch);
        /// �������� �������� ������� ������� ���������� �� delta �����
        static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);
        /// �������� �������� ������� ������� ������� �� delta �����
        static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

        static bool OnArrows(const KeyEvent &event);

        static void UpdateCursorsForLook();
        /// ���������� ������� ������� ����������
        static void SetCursorU(Chan::E ch, int numCur, float pos);
        /// ���������� ������� ������� �������
        static void SetCursorT(Chan::E ch, int numCur, float pos);
        /// ���������� true, ���� �������� ����� ��������� ����� ���������� �������/�������
        static bool IsRegSetActiveOnCursors();

    private:
        static const float MAX_POS_U;
        static const float MAX_POS_T;

        /// ���������� �������� ��������� ���������
        static void SetCursSource(Chan::E ch);
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMeasuresMath
{
    static const Page * const self;
};
