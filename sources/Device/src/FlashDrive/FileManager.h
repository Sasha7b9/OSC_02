#pragma once
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class FileManager
{
public:
    /// ���������� ���� ��� ��� ����������� ����� ������
    static void Init();
    /// ���������� �������� ��������
    static void Draw();
    
    static void Press_LevelUp();
    
    static void Press_LevelDown();
    
    static bool OnArrows(KeyEvent event);
    
    static bool GetNameForNewFile(char name[255]);

    static void Press_Tab();
};
