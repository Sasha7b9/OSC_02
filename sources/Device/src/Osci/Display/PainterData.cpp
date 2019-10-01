#include "defines.h"
#include "PainterData.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Symbols.h"
#include "Osci/Osci.h"
#include "Osci/Display/MemoryWindow.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Osci/Display/Accumulator.h"
#include "Hardware/Timer.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Utils/StringUtils.h"
#include <cstdlib>
#include <cstring>


using namespace Display::Primitives;
using namespace FPGA;



/// ���������� ���������� ������ - ��������������� ������� ����������
static void DrawCurrent();
/// ���������� ������ �� ���
static void DrawRAM();
/// ���������� ������ �� ����
static void DrawROM();

static void DrawChannel(Chan::E ch);

static void DrawTPos(int leftX, int rightX);

static void DrawTShift(int leftX, int rightX, int numPoints);

static void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

static void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);

static void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x);

static void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

static void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

static void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x);
/// ���������� ������
static void DrawSpectrum();



void Osci::Display::PainterData::DrawData()
{
    if (Measurements::DataIsSetting())
    {
        static const pFuncVV func[ModeWork::Number] =
        {
            DrawCurrent,
            DrawRAM,
            DrawROM
        };

        func[set.mem.modeWork]();
    }

    DrawTPos(0, 0);

    DrawTShift(0, 0, 0);

    DrawSpectrum();

    Accumulator::NextFrame();
}


static void DrawCurrent()
{
    if(set.disp.lastAffectedChannel == Chan::A)
    {
        DrawChannel(Chan::B);
        DrawChannel(Chan::A);
    }
    else
    {
        DrawChannel(Chan::A);
        DrawChannel(Chan::B);
    }
    Osci::Display::MemoryWindow::Draw();
}


static void DrawRAM()
{
    DrawChannel(Chan::A);
    DrawChannel(Chan::B);
    Osci::Display::MemoryWindow::Draw();
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSpectrumChannel(const float *spectrum, Color color)
{
    color.SetAsCurrent();
    int gridLeft = Grid::Left();
    int gridBottom = Grid::MathBottom();
    int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++)
    {
        int height = (int)(gridHeight * spectrum[i]);

        VLine(height).Draw(gridLeft + i, gridBottom - height);
    }
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Color::FILL.SetAsCurrent();

    Text(Osci::Measurements::Freq2String(freq0, false, buffer)).Draw(x, y);

    y += dY;

    Text(Osci::Measurements::Freq2String(freq1, false, buffer)).Draw(x, y);

    if (Chan(ch).IsA())
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }

    Color::CHAN[ch].SetAsCurrent();

    Text((set.fft.scale == ScaleFFT::Log) ? SU::Db2String(density0, 4, buffer) : Osci::Measurements::Float2String(density0, false, buffer)).Draw(x, y);

    y += dY;

    Text((set.fft.scale == ScaleFFT::Log) ? SU::Db2String(density1, 4, buffer) : Osci::Measurements::Float2String(density1, false, buffer)).Draw(x, y);
}

//----------------------------------------------------------------------------------------------------------------------------------------------------
static void DRAW_SPECTRUM(const uint8 *dataIn, int numPoints, Chan::E ch)
{
    if (!set.ch[ch].enabled)
    {
        return;
    }

#undef SIZE_BUFFER
#define SIZE_BUFFER (1024 * 4)

    if (numPoints > SIZE_BUFFER)
    {
        numPoints = SIZE_BUFFER;
    }

    float dataR[SIZE_BUFFER];

    float freq0 = 0.0F;
    float freq1 = 0.0F;
    float density0 = 0.0F;
    float density1 = 0.0F;
    int y0 = 0;
    int y1 = 0;
    int s = 2;

    float *spectrum = (float *)std::malloc(numPoints * sizeof(float));

    if (spectrum)
    {
        FPGA::Math::PointsRel2Voltage(dataIn, numPoints, RANGE_DS(ch), (int16)RSHIFT_DS(ch), dataR);

        FPGA::Math::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);

        DrawSpectrumChannel(spectrum, Color::CHAN[ch]);

        Color color = Color::FILL;

        WriteParametersFFT(ch, freq0, density0, freq1, density1);

        Rectangle(s * 2, s * 2).Draw(set.fft.posCur[0] + Grid::Left() - s, y0 - s, color);
        Rectangle(s * 2, s * 2).Draw(set.fft.posCur[1] + Grid::Left() - s, y1 - s);
        VLine(y0 + s - Grid::MathBottom()).Draw(Grid::Left() + set.fft.posCur[0], Grid::MathBottom());
        VLine(y1 + s - Grid::MathBottom()).Draw(Grid::Left() + set.fft.posCur[1], Grid::MathBottom());

        std::free(spectrum);
    }
}


static void DrawSpectrum()
{
    if (!set.fft.enabled)
    {
        return;
    }

    VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Right(), Grid::ChannelBottom(), Color::BACK);
    
    
    if (set.mem.modeWork == ModeWork::Dir)
    {
        int numPoints = DS->SizeChannel();
    
        if (numPoints > 2048)       /// \todo ���� 8� � ����� �� ����� �������
        {
            numPoints = 2048;
        }
    
        if (set.fft.source == SourceFFT::A)
        {
            DRAW_SPECTRUM(OUT_A, numPoints, Chan::A);
        }
        else if (set.fft.source == SourceFFT::B)
        {
            DRAW_SPECTRUM(OUT_B, numPoints, Chan::B);
        }
        else
        {
            if (set.disp.lastAffectedChannel == Chan::A)
            {
                DRAW_SPECTRUM(OUT_B, numPoints, Chan::B);
                DRAW_SPECTRUM(OUT_A, numPoints, Chan::A);
            }
            else
            {
                DRAW_SPECTRUM(OUT_A, numPoints, Chan::A);
                DRAW_SPECTRUM(OUT_B, numPoints, Chan::B);
            }
        }
    
    }
    
    HLine(Grid::Right() - Grid::Left()).Draw(Grid::ChannelBottom(), Grid::Left(), Color::FILL);
    HLine(Grid::Right() - Grid::Left()).Draw(Grid::MathBottom(), Grid::Left());
}


static void DrawROM()
{

}


static void DrawChannel(Chan::E ch)
{
    if (!set.ch[ch].enabled)
    {
        return;
    }

    uint8 *data = OUT(ch);


    data += SHIFT_IN_MEMORY;

    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        data += SHIFT_IN_MEMORY;
    }

    /// ��� ��������� ����������� ������
    Buffer buffer;

    if (Osci::InModeP2P())
    {
        if (DATA_P2P)
        {
            DATA_P2P->FillBufferForDraw(ch, &buffer);
            data = buffer.data;
        }
        else if(DATA == nullptr)
        {
            return;
        }
        else
        {
            // ����� ������� ����� �����, �������� �� ��, ��� ���������� �����
        }
    }

    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();

    int left = Grid::Left();

    float scale = (float)Grid::Height() / (VALUE::MAX - VALUE::MIN);

    if (set.fft.enabled)
    {
        center -= (Grid::Bottom() - Grid::Top()) / 4;
        scale /= 2.0F;
    }


    if (set.disp.modeDrawSignal == ModeDrawSignal::Lines)
    {
        DrawModeLines(ch, left, center, data, scale);
    }
    else
    {
        DrawModePoints(ch, left, center, data, scale);
    }
}


static void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    int x = left;

    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        DrawModeLinesPeakDetOn(center, data, scale, x);
    }
    else
    {
        DrawModeLinesPeakDetOff(center, data, scale, x);
    }
}


static void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        int min = (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int max = (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F);

        VLine(min - max).Draw(x++, max);
    }
}


static void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x)
{
    int coordVert = -1;  // �� ���� ���������� ����� ���������� ������������ �����, ����� ������ ������ ����������� ������, ����� ������� ������ ����� �����

    for (int i = 1; i < 281; i++)
    {
        int value = (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int valuePrev = (int)(center - (data[i - 1] - VALUE::AVE) * scale + 0.5F);

        if (value == valuePrev)
        {
            Point().Draw(x++, valuePrev);
        }
        else
        {
            int val = valuePrev > value ? (value + 1) : (value - 1);
            VLine(val - valuePrev).Draw(x++, valuePrev);
        }

        if (coordVert == -1 && data[i] == VALUE::NONE)
        {
            coordVert = i;
            VLine(Grid::Height()).Draw(x - 1, Grid::Top(), Color::GRID);
        }
    }
}


static void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        DrawModePointsPeakDetOn(center, data, scale, left);
    }
    else
    {
        DrawModePointsPeakDetOff(center, data, scale, left);
    }
}


static void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        Point().Draw(x, (int)(center - (data[i] - VALUE::AVE) * scale + 0.5F));
        Point().Draw(x, (int)(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F));
        x++;
    }
}


static void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 280; i++)
    {
        float value = center - (data[i] - VALUE::AVE) * scale;
        Point().Draw(x + i, ROUND(uint8, value));
    }
}


static void DrawTPos(int leftX, int rightX)
{
    int x[] = {leftX, (rightX - leftX) / 2 + leftX, rightX};
    int x0 = x[set.time.TPos];
    Region(6, 6).Fill(x0 - 3, 10, Color::BACK);

    Char(Symbol8::TPOS_1).Draw(x0 - 3, 10, Color::FILL);
}


static void DrawTShift(int /*leftX*/, int /*rightX*/, int /*numBytes*/)
{
    //float scale = (float)(rightX - leftX + 1) / ((float)numBytes - (numBytes == 281 ? 1 : 0));
    //int xShift = (int)(1.5F + (TPos(TPOS).InBytes() - SET_TSHIFT.InPoints()) * scale) - 1;
    //if (SET_PEAKDET_EN && TPOS_IS_RIGHT)
    //{
    //    --xShift;
    //}
    //if (FPGA_POINTS_512)
    //{
    //    ++xShift;                           /// \todo �������
    //}
    //LIMIT_ABOVE(xShift, rightX - 2);
    //
    //int dX01 = 1, dX02 = 2, dX11 = 3, dY11 = 7, dY12 = 6;
    //
    //if (xShift < leftX - 2)
    //{
    //    xShift = leftX - 2;
    //    dX01 = 3; dX02 = 1; dY12 = 6;
    //}
    //else if (xShift > rightX - 1)
    //{
    //    xShift = rightX - 2;
    //    dX11 = 1;
    //}
    //else
    //{
    //    dY11 = 5; dY12 = 7;
    //}
    //
    //Region(6, 6).Fill((int)xShift - 1, 1, Color::BACK);
    //Region(4, 4).Fill((int)xShift, 2, Color::FILL);
    //
    //Line((int)xShift + dX01, 3, (int)xShift + dX11, dY11 - 2).Draw(Color::BACK);
    //Line((int)xShift + dX02, 4, (int)xShift + 2, dY12 - 2).Draw();
}


int Osci::Display::PainterData::FirstPointOnScreen()
{
    return set.disp.shiftInMemory;
}


BitSet64 Osci::Display::PainterData::PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}


BitSet64 Osci::Display::PainterData::BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + 281;

    if (set.time.peakDet == PeakDetMode::Enabled)
    {
        retValue.sword1 += 281;
    }

    return retValue;
}


void Osci::Display::PainterData::ChangeTPos()
{
    int width = Grid::Width();

    if (set.time.TPos == TPos::Left)
    {
        set.disp.shiftInMemory = 0;
    }
    else if (set.time.TPos == TPos::Center)
    {
        set.disp.shiftInMemory = (int16)(FPGA_NUM_POINTS / 2 - width / 2);
    }
    else // TPOS_IS_RIGHT
    {
        set.disp.shiftInMemory = (int16)(FPGA_NUM_POINTS - width - 2);
    }
}
