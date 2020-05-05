#pragma once



class Message
{
public:
    explicit Message();
    explicit Message(int size, uint8);
    explicit Message(int size, uint8, uint8);
    explicit Message(int size, uint8, uint16, uint8);
    explicit Message(int size, uint8, uint8, uint);
    explicit Message(int size, uint8, uint16, uint8, uint8);
    explicit Message(uint8, uint8, uint8, uint8 *, uint16, uint8 *, uint16);
    explicit Message(int size, uint8, uint16, uint8, uint16, uint8);
    explicit Message(uint8, uint16, uint8, char *);
    ~Message();

    int Size() const { return allocated; }

    uint8 *Data(int pos = 0) { return data + pos; }	// -V104

    class Create
    {
    public:

        static void DrawHLine(Message &message, int y, int x0, int x1);

        static void DrawRectangle(Message &message, int x, int y, int width, int height);
    };

private:

    // �������� ���� ����
    void PutByte(uint8);
    
    // �������� �������
    void PutHalfWord(uint16);
    
    // �������� �����
    void PutWord(uint);
    
    // �������� ������
    bool Allocate(int size);
    
    // ���������� ������� ������
    void Free();
    
    // ������� �������� ������
    int allocated;
    
    // ������� ��������� (��������� PutX())
    int used;
    
    // ��������� �� ���������� ������� ������
    uint8 *data;
};
