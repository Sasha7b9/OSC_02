#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Utils
{
    template<typename T>
    /// ���������� - ��������� ��������� �������� � ���� ��������
    class Averager
    {
    public:
        Averager(uint size);
        ~Averager();
        void Push(T elem);
        T Value() const;
    private:
        T *buffer;
        uint size;
        uint numELements;
    };

    /// �������������� ����������
    template <typename T>
    class AroundAverager
    {
    public:
        AroundAverager(uint parts);
        /// ����� ��������� �������� � ������������ ���������� ����������
        void Push(T elem);
        T Value();
    private:
        T value;
        /// �� ������� ������ ������� ��������
        const uint parts;
        /// ������� �������� ����������
        uint pushed;
    };
}
