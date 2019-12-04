#include "defines.h"
#include "Display.h"
#include "Display/Painter.h"
#include "Hardware/Controls.h"
#include "Hardware/CPU.h"
#include "Hardware/LTDC.h"
#include "Hardware/HAL/HAL.h"
#include <stdlib.h>



LTDC_HandleTypeDef hltdc;

uint8  front[BUFFER_WIDTH * BUFFER_HEIGHT];
uint8  back[BUFFER_WIDTH * BUFFER_HEIGHT];
uint8 *Display::frontBuffer = front;
uint8 *Display::backBuffer = back;



void Display::Init()
{
    HAL_DAC2::Init();
    HAL_DAC2::SetValue(50);
    LTDC_::Init(reinterpret_cast<uint>(frontBuffer), reinterpret_cast<uint>(backBuffer));
    Painter::LoadPalette();
}


void Display::ToggleBuffers(void)
{
    uint destination = reinterpret_cast<uint>(frontBuffer);
    uint source = reinterpret_cast<uint>(backBuffer);

    DMA2D_HandleTypeDef hDMA2D;

    hDMA2D.Init.Mode = DMA2D_M2M;
    hDMA2D.Init.ColorMode = DMA2D_INPUT_L8;
    hDMA2D.Init.OutputOffset = 0;

    hDMA2D.XferCpltCallback = 0;

    hDMA2D.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
    hDMA2D.LayerCfg[1].InputAlpha = 0xFF;
    hDMA2D.LayerCfg[1].InputColorMode = DMA2D_INPUT_L8;
    hDMA2D.LayerCfg[1].InputOffset = 0;

    hDMA2D.Instance = DMA2D;

    if (HAL_DMA2D_Init(&hDMA2D) == HAL_OK)
    {
        if (HAL_DMA2D_ConfigLayer(&hDMA2D, 1) == HAL_OK)
        {
            if (HAL_DMA2D_Start(&hDMA2D, source, destination, 320, 240) == HAL_OK)
            {
                HAL_DMA2D_PollForTransfer(&hDMA2D, 100);
            }
        }
    }
}


uint8 *Display::GetBuffer()
{
    return backBuffer;
}


uint8 *Display::GetBufferEnd()
{
    return backBuffer + BUFFER_WIDTH * BUFFER_HEIGHT;
}
