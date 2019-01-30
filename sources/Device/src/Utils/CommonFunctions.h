#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace CF
{
    /// ������� � ������� num ������ �������� �� buffer � ����������������� ����
    void LogBufferU8_HEX(const uint8 *buffer, int num);
    /// ������� � ������� num ������ �������� �� buffer � ���������� ����
    void LogBufferU8_DEC(const uint8 *buffer, int num);
    /// ���������� ����� ���� � ������ string
    int NumWords(char *string);
    /// ���������� ��������� �� n ����� � ������. ���� char == 0 - ����� ���, ���� ret value == 0xffffffff - �������� ����� ������� ���
    char *GetWord(char *string, int n, char *out, int size);
};


// � ������ ����� ������� ������ Push �������� ���� ���������� ����� ���������� ������ Clear() ��������
class BufferMax5
{
public:
    BufferMax5();
    void Clear();
    void Push(uint16 value);
    uint16 GetValue(int index);
private:
    uint16 buffer[5];
    int numElements;
};
