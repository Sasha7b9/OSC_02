#include "defines.h"
#include "common/Display/Font/Font_d.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FreqMeter/DisplayFreqMeter.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <cstring>


#define EMPTY_STRING    "---.---"
#define OVERFLOW_STRING ">>>"


//                         0    1    2    3    4    5    6 
static char buffer[11] = { '0', '0', '0', '0', '0', '0', '0', 0, 0, 0, 0 };


void DisplayFreqMeter::Draw()
{
    /// \todo � ���� ������ ����� ������� �� ��� �������, � � ��� �������, ��� ��� ������ ��������� ���

    if(!FreqMeter::Enabled())
    {
        return;
    }

    DFont::Set(DTypeFont::_GOST28);
    int spacing = DFont::GetSpacing();
    DFont::SetSpacing(1);

    int width = 200;
    int height = 90;
    int x = Grid::Left() + (Grid::Width() - width) / 2;
    int y = Grid::Top() + (Grid::Height() / 2 - height) / 2;


    Region(width - 2, height - 2).Fill(x + 1, y + 1, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    x += 2;
    y += 2;

    if(FreqMeter::ModeView::IsFrequency())
    {
        DrawFrequency(x, y);
    }
    else
    {
        DrawPeriod(x, y);
    }

    if(false)
    {
        DrawDebugInfo();
    }

    DFont::Pop();
    DFont::SetSpacing(spacing);
    DFont::SetMinWidth(0);
}


void DisplayFreqMeter::DrawFrequency(int x, int _y)
{
    _y += 4;

    int yF = _y;
    int yT = _y + 4 + DFont::GetHeight();

    x += 6;

    Text("F").Draw(x, yF, Color::FILL);
    Text("T").Draw(x, yT);

    ProgressBarFreqMeter::Draw(x, yT + 4 + DFont::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yF);
    Text("=").Draw(x + dX, yT);

    dX = 32;

    char strFreq[50];
    std::strcpy(strFreq, FreqSetToString(&FreqMeter::freqActual));

    Text(strFreq).DrawDigitsMonospace(x + dX, yF, DFont::GetWidth('0'));

    float freq = SU::StringToFloat(strFreq);

    if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "���") == 0)
    {
        freq *= 1e6F;
    }
    else if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "���") == 0)
    {
        freq *= 1e3F;
    }
    else if(std::strcmp(&strFreq[std::strlen(strFreq) - 3], "���") == 0)
    {
        freq *= 1e-3F;
    }

    Time time(1.0F / freq);

    if(time.Value() == std::numeric_limits<float>::infinity())
    {
        Text(EMPTY_STRING).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));
    }
    else
    {
        Text(time.ToStringAccuracy(false, strFreq, 6)).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));
    }
}


void DisplayFreqMeter::DrawPeriod(int x, int _y)
{
    _y += 4;

    int yT = _y;
    int yF = _y + 4 + DFont::GetHeight();

    x += 6;

    Text("T").Draw(x, yT, Color::FILL);
    Text("F").Draw(x, yF);

    ProgressBarFreqMeter::Draw(x, yF + 4 + DFont::GetHeight());

    int dX = 17;

    Text("=").Draw(x + dX, yT);

    Text("=").Draw(x + dX, yF);

    dX = 32;

    char strPeriod[50];
    std::strcpy(strPeriod, PeriodSetToString(&FreqMeter::periodActual));

    Text(strPeriod).DrawDigitsMonospace(x + dX, yT, DFont::GetWidth('0'));

    if((std::strcmp(strPeriod, EMPTY_STRING) == 0) || (std::strcmp(strPeriod, OVERFLOW_STRING) == 0))
    {
        return;
    }

    float per = SU::StringToFloat(strPeriod);

    if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "��") == 0)
    {
        per *= 1e-9F;
    }
    else if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 3], "���") == 0)
    {
        per *= 1e-6F;
    }
    else if(std::strcmp(&strPeriod[std::strlen(strPeriod) - 2], "��") == 0)
    {
        per *= 1e-3F;
    }

    Frequency freq(1.0F / per);

    Text(freq.ToStringAccuracy(strPeriod, 6)).DrawDigitsMonospace(x + dX, yF, DFont::GetWidth('0'));
}


pString DisplayFreqMeter::FreqSetToString(const BitSet32 *fr)
{
    if(fr->word < 2)
    {
        return EMPTY_STRING;
    }
    else if(fr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }

    Hex value(fr->word);

    while(value.NumDigits() > 6)
    {
        value.Set(value / 10);
    }

    for(int i = 0; i < 7; i++)
    {
        buffer[i] = value.DigitInPosition(6 - i);
    }

    uint freq = fr->word;

    uint giverFreq = freq;

    /// ��� ����� * 10
#define _10Hz   (               100) /* E_2 */
#define _100Hz  (              1000) /* E_3 */
#define _1kHz   (         10 * 1000) /* E_4 */
#define _10kHz  (        100 * 1000) /* E_5 */
#define _100kHz (       1000 * 1000) /* E_6 */
#define _1MHz   (  10 * 1000 * 1000) /* E_7 */
#define _10MHz  ( 100 * 1000 * 1000) /* E_8 */
#define _100MHz (1000 * 1000 * 1000) /* E_9 */


#undef WRITE_SUFFIX
#define WRITE_SUFFIX(suffix_E4)    \
    if(giverFreq < _1kHz) { std::strcpy(buffer + 7, suffix_E4); } else if (giverFreq < _1MHz) { std::strcpy(buffer + 7, "���"); } else { std::strcpy(buffer + 7, "���"); }

#define HIGH_FREQ                            \
    if(giverFreq < _10MHz)                   \
    {                                        \
        std::memmove(buffer, buffer + 1, 2); \
        buffer[1] = '.';                     \
    }                                        \
    else if (giverFreq < _100MHz)            \
    {                                        \
        std::memmove(buffer, buffer + 1, 3); \
        buffer[2] = '.';                     \
    }                                        \
    else                                     \
    {                                        \
        std::memmove(buffer, buffer + 1, 3); \
        buffer[3] = '.';                     \
    }


    switch(FreqMeter::TimeCounting())
    {
    case FreqMeter::TimeCounting::_100ms:

        giverFreq *= 100;

        WRITE_SUFFIX("���");

        if(giverFreq < _1MHz)                       // ������ 1 ���
        {
            if(freq >= _10Hz)                       // ������ ��� ����� 10 ��
            {
                std::memmove(buffer, buffer + 1, 5);
            }
            buffer[4] = '.';
        }
        else
        {
            HIGH_FREQ;
        }
        break;

    case FreqMeter::TimeCounting::_1s:

        giverFreq *= 10;

        WRITE_SUFFIX("��");

        if(giverFreq < _1MHz)                      // ������ 1 ���
        {
            if(giverFreq < _1kHz)                   // ������ 1 ���
            {
                std::memmove(buffer, buffer + 1, 6);
                buffer[6] = '.';
            }
            else
            {
                std::memmove(buffer, buffer + 1, 4);
                buffer[3] = '.';
            }
        }
        else
        {
            HIGH_FREQ;
        }
        break;

    case FreqMeter::TimeCounting::_10s:

        WRITE_SUFFIX("��");

        if(freq < _1MHz)                       // ������ 1 ���
        {
            if(giverFreq < _1kHz)              // ������ 1 ���
            {
                std::memmove(buffer, buffer + 1, 5);
                buffer[5] = '.';
            }
            else if(giverFreq < _100kHz)
            {
                std::memmove(buffer, buffer + 1, 3);
                buffer[2] = '.';
            }
            else
            {
                std::memmove(buffer, buffer + 1, 3);
                buffer[3] = '.';
            }
        }
        else
        {
            HIGH_FREQ;
        }
        break;
    case FreqMeter::TimeCounting::Count:
    default:
        break;
    }

    return buffer;
}


int DisplayFreqMeter::LowOrder(FreqMeter::FreqClc::E freqCLC, FreqMeter::NumberPeriods::E numPeriods)
{
    /*
        ���������� �������� | ����������� �������� | ����� �� ����� | ��������� �������� ������

        +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
        |          | 100 ���                    0        | 1 ���                      1        | 10 ���                     2        | 100 ���                    3        |
        +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
        | 1        | 10   ���        1    0.01 ��  1e-5  | 1 ���          1      1. ���  1e-6  | 100 ��         1     0.1 ���  1e-7  | 10  ��         1    0.01 ���  1e-8  |
        |          | 100  ���       10    0.10 ��  1e-5  | 10 ���        10     10. ���  1e-6  | 1   ���       10     1.0 ���  1e-7  | 100 ��        10    0.10 ���  1e-8  |
        |          | 1    ��       100    1.00 ��  1e-5  | 100 ���      100    100. ���  1e-6  | 10  ���      100    10.0 ���  1e-7  | 1   ���      100    1.00 ���  1e-8  |
        |          | 10   ��      1000   10.00 ��  1e-5  | 1   ��      1000   1.000 ��   1e-6  | 100 ���     1000   100.0 ���  1e-7  | 10  ���     1000   10.00 ���  1e-8  |
        |          | 100  ��     10000  100.00 ��  1e-5  | 10  ��     10000  10.000 ��   1e-6  | 1   ��     10000  1.0000 ��   1e-7  | 100 ���    10000  100.00 ���  1e-8  |
        |          | 1    �     100000 1.00000 �   1e-5  | 100 ��    100000 100.000 ��   1e-6  | 10  ��    100000 10.0000 ��   1e-7  | 1   ��    100000 1.00000 ��   1e-8  |
        |          | 10   �    100000- 10.0000 �   1e-4  | 1   �    100000- 1.00000 �    1e-5  | 100 ��   100000- 100.000 ��   1e-6  | 10  �c   100000- 10.0000 ��   1e-7  |
        |          | 100  �   100000-- 100.000 �   1e-3  | 10  �   100000-- 10.0000 �    1e-4  | 1   �   100000-- 1.00000 �    1e-5  | 100 ��  100000-- 100.000 ��   1e-6  |
        |        0 | 1000 �   ������������               | 100 �   ������������                | 10  �   ������������                | 1   c   ������������                |
        +----------+-------------------------------------+-------------------------------------|-------------------------------------+-------------------------------------+
        | 10       |                                     |                                     |                                     | 1   ��         1      1. ��   1e-9  |
        |          |                                     |                                     |                         /           | 10  ��        10     10. ��   1e-9  |
        |          |                                     |                                     |                     /               | 100 ��       100    100. ��   1e-9  |
        |          |                                     |                                     |                  /                  | 1   ���     1000   1.000 ���  1e-9  |
        |          |                                     |                                     |             /                       | 10  ���    10000  10.000 ���  1e-9  |
        |          |                                     |                                     |         /                           | 100 ���   100000 100.000 ���  1e-9  |
        |          |                                     |                                     |   |  /                              | 1   ��   100000- 1.00000 ��   1e-8  |
        |          |                                     |                                     |   |---                              | 10  ��  100000-- 10.0000 ��   1e-7  |
        |        1 |                                     |                                     |                                     | 100 ��  ������������                |
        +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
        | 100      |                                     |                                     |                                     | 0.1 ��         1     0.1 ��   1e-10 |
        |          |                                     |                                     |                           /         | 1   ��        10     1.0 ��   1e-10 |
        |          |                                     |                                     |                       /             | 10  ��       100    10.0 ��   1e-10 |
        |          |                                     |                                     |                    /                | 100 ��      1000   100.0 ��   1e-10 |
        |          |                                     |                                     |               /                     | 1   ���    10000  1.0000 ���  1e-10 |
        |          |                                     |                                     |           /                         | 10  ���   100000 10.0000 ���  1e-10 |
        |          |                                     |                                     |     |  /                            | 100 ���  100000- 100.000 ���  1e-9  |
        |          |                                     |                                     |     |---                            | 1   ��  100000-- 1.00000 ��   1e-8  |
        |        2 |                                     |                                     |                                     | 10  ��  ������������                |
        +----------+-------------------------------------+-------------------------------------+-------------------------------------+-------------------------------------+
    */

    //                           | 100�/1 | 1�  /1  |  10� /1   | 100�/1   |            |           |           |           |
    //                           |        | 100�/10 |  1�  /10  | 10� /10  | 100�/10    |           |           |           |
    //                           |        |         |  100�/100 | 1�  /100 | 10� /100   | 100�/100  |           |           |
    //                           |        |         |           | 100�/1�  | 1�  /1�    | 10� /1�   | 100�/1�   |           |
    //                           |        |         |           |          | 100�/10�   | 1�  /10�  | 10� /10�  | 100�/10�  |
    //                           |        |         |           |          |            | 100�/100� | 1�  /100� | 10� /100� | 100�/100� |          |
    //                           |        |         |                                                 100�/          | 1�  /10�  | 10� /10�  | 100�/10� |
    //                                                                                                                                             | 100�/100�
    static const int order[] = { -5,      -6,        -7,        -8,         -9,         -10,        -11,        -12,        -13,      -14,       -15,        -16 };

    return order[freqCLC + numPeriods];
}


pString DisplayFreqMeter::PeriodSetToString(const BitSet32 *pr)
{
    if(pr->word == 0)
    {
        return EMPTY_STRING;
    }
    else if(pr->word == MAX_UINT)
    {
        return OVERFLOW_STRING;
    }
    else
    {
        // ��� ������ ����������
    }

    Stack<uint> stack(20);

    uint _period = pr->word;

    while(_period > 0)
    {
        stack.Push(_period % 10);
        _period /= 10;
    }

    int order = LowOrder(set.freq.freqClc, FreqMeter::NumberPeriods());  // � ����� - ������� �������� ��������� �������

    while(stack.Size() < 6)
    {
        stack.Push(0U);                             // �������� ����������� ����
    }
    // ������ � ����� ��� ������� �������, ������� (� �������� order - �� ��� �����)

    Stack<uint> stackResult(6);                     // ����� ����� ��������� ����� �������� �������. ��������� - �������

    while(stackResult.Size() < 6)
    {
        stackResult.Push(stack.Pop());
    }
    // ������ � stackResult ��� ������� ��������� �����. �� ������� - �������.
    // ������������� ��� �������.

    while(!stack.Empty())
    {
        stack.Pop();
        order++;
    }
    // ������ � order �������� ������� �������� ��������� ������� ���������� (�� ��������� �� �������)

    return StackToString(&stackResult, order);
}


pString DisplayFreqMeter::StackToString(Stack<uint> *stack, int order)
{
    static const struct StructOrder
    {
        int first;
        int second;
        const char *s1;
        const char *s2;
    }
    structs[] =
    {
        {3, 6, "��", "��"},     // -12
        {1, 4, "���", "��"},    // -11
        {2, 5, "���", "��"},    // -10
        {3, 6, "���", "��"},    // -9
        {1, 4, "��", "���"},    // -8
        {2, 5, "��", "���"},    // -7
        {3, 6, "��", "���"},    // -6
        {1, 4, "�", "��"},      // -5
        {2, 5, "�", "��"},      // -4
        {3, 6, "�", "��"},      // -3
        {4, 4, "�", "�"},       // -2
        {5, 5, "�", "�"},       // -1
        {6, 6, "�", "�"}        // 0
    };

    order += 12;

    const StructOrder &str = structs[order];

    if(stack->NumFirstZeros() < str.first)
    {
        WriteStackToBuffer(stack, str.first, str.s1);
    }
    else
    {
        WriteStackToBuffer(stack, str.second, str.s2);
    }

    return buffer;
}


void DisplayFreqMeter::WriteStackToBuffer(Stack<uint> *stack, int point, const char *suffix)
{
    for(int i = 6; i >= 0; i--)
    {
        if(point == i)
        {
            buffer[i] = '.';
            continue;
        }
        buffer[i] = static_cast<char>(stack->Pop()) | 0x30;
    }

    std::strcpy(&buffer[7], suffix);
}


void ProgressBarFreqMeter::Draw(int x, int y)
{
    if(FreqMeter::ModeView::IsFrequency() && FreqMeter::timeStartMeasureFreq != 0)
    {
        static const float time[FreqMeter::TimeCounting::Count] = { 100.0F, 1000.0F, 10000.0F };

        int length = 185;

        float percents = (TIME_MS - FreqMeter::timeStartMeasureFreq) / time[FreqMeter::TimeCounting()];

        int width = static_cast<int>(length * percents);

        if(width > length)
        {
            width = length;
        }

        if(FreqMeter::TimeCounting::Is100ms() && width > length / 2)
        {
            width = length;
        }

        Region(width, 3).Fill(x, y, Color::FILL);
    }
    else if(FreqMeter::ModeView::IsPeriod() && FreqMeter::timeStartMeasurePeriod != 0)
    {

    }
}
