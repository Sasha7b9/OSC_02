#include "defines.h"
#include "BufferButtons.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageService.h"


static const int SIZE = 100;

static KeyEvent buffer[SIZE];               // ����� �������� ��� �������
static KeyEvent prevPushEvent;              // ���������� �������� � ����� ������� ����������
static int end;                             // ������� ������� ���������� ����� � ������. ����� end == 0, ����� ������
static int start;                           // ������� ������� ��������� ������� � ������.
static uint timeLastControl = 0xFFFFFFFFU;  // ���������� �����


void BufferButtons::Push(KeyEvent event)
{
    Display::Breaker::PowerOn();

    timeLastControl = TIME_MS;

    if ((event.key == prevPushEvent.key) &&             // ���� �������� ������, ������� ������ �������� "�������" �������,
        prevPushEvent.IsLong() &&
        event.IsRelease())
    {
        return;                                         // �� ��������� ��� ������� - ������ ��� ����������
    }

    PageMultimeter::DecodePassword(event);

    PageService::DecodePassword(event);

    // \todo �� ����� ������ ������ ��� ���������� ��������� ������ ��������. ������� �������� ��� �������� ������
    Beeper::Beep(event.type);

    if(end == SIZE)
    {
//        LOG_ERROR("����� ����������");
        return;
    }

    buffer[end++] = event;

    prevPushEvent = event;
}


KeyEvent BufferButtons::Extract()
{
    KeyEvent result = buffer[start];

    start++;
    if(start == end)
    {
        start = 0;
        end = 0;
    }

    return result;
}


bool BufferButtons::IsEmpty()
{
    return end == 0;
}


uint BufferButtons::TimeAfterControlMS()
{
    return TIME_MS - timeLastControl;
}
