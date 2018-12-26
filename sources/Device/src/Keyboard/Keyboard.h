#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Key
{
    enum E
    {
        None,
        Function,
        Measure,
        Memory,
        Service,
        ChannelA,
        ChannelB,
        Time,
        Start,
        Trig,
        Display,
        RangeMoreA,
        RangeLessA,
        RShiftMoreA,
        RShiftLessA,
        RangeMoreB,
        RangeLessB,
        RShiftMoreB,
        RShiftLessB,
        TBaseMore,
        TBaseLess,
        TShiftMore,
        TShiftLess,
        TrigLevMore,
        TrigLevLess,
        Left,
        Right,
        Up,
        Down,
        Enter,
        F1,
        F2,
        F3,
        F4,
        F5,
        Number
    } value;

    explicit Key(E v = None) : value(v) {};
    bool IsFunctional() const { return value >= F1 && value <= F5; };
    bool IsArrow() const { return value >= Left && value <= Down; };
    /// ���������� true, ���� ������ ��������� ���������� ������� - ��������� ���, ���������
    bool IsControlSignal() const;
    pString Name();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypePress
{
    enum E
    {
        Press,      ///< ������� ������
        Repeat,     ///< �������������� ������
        Release,    ///< ���������� ������
        Long,       ///< "�������" ������� ������
        None,
        Number
    } value;

    explicit TypePress(E v) : value(v)  { };
    pString ToString();
    bool IsLong() const { return value == Long; };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyEvent
{
    explicit KeyEvent(Key::E k = Key::None, TypePress::E t = TypePress::None) : key(k), type(t) { };
    Key::E key;
    TypePress::E type;
    /// ���������� true, ���� ������ �������� ����������
    bool IsAboveZero() const { return key == Key::Up || key == Key::Right; }
    /// ���������� 1, ���� ������ �������� ����������, � -1 � �������� ������
    int Delta() const { return IsAboveZero() ? 1 : -1; }
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:
    /// �������� ������ ����������
    /** ����� ������ ���������, �� ���������� ���� ���������� ��������� ���������� releasedButton */
    static void Disable();
    /// �������� ������� �������
    static Key::E WaitPressingButton();

    static void Enable();
};
