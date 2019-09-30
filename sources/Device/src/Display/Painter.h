#pragma once
#include "Colors.h"
#include "Display/Font/Font.h"



namespace Painter
{
    void Init();

    void BeginScene(Color color);

    void EndScene();

    void DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y);
};
