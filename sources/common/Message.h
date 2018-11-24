#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Message
{
public:
    explicit Message(uint size, uint8);
    explicit Message(uint size, uint8, uint8);
    explicit Message(uint size, uint8, uint16, uint8);
    ~Message();

    uint Size() const { return allocated; }

    uint8 *Data() { return data; }

private:
    /// �������� ���� ����
    void PutByte(uint8);
    /// �������� �������
    void PutHalfWord(uint16);
    /// �������� ������
    bool Allocate(uint size);
    /// ���������� ������� ������
    void Free();
    /// ������� �������� ������
    uint allocated;
    /// ������� ��������� (��������� PutX())
    uint used;
    /// ��������� �� ���������� ������� ������
    uint8 *data;
};
