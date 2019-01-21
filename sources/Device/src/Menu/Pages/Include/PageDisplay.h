#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct PageDisplay
{
    static void Init();

    static void OnChanged_RefreshFPS(bool);

    static const PageBase *pointer;

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageAccumulation
    {
        static const PageBase *pointer;

        static void OnPress_Accumulation_Clear();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageAverage
    {
        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageGrid
    {
        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSettings
    {
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        struct PageColors
        {
            static const PageBase *pointer;

            static void Init();
            /// ��� �����
            static ColorType colorTypeGrid;
            /// ��� ������� ������
            static ColorType colorTypeA;
            /// ��� ������� ������
            static ColorType colorTypeB;
        };
    };
};
