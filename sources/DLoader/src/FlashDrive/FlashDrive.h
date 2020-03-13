#pragma once
#include "main.h"


class FDrive
{
    friend class CPU;

public:
    static void Init();

    // ������� ������� ����������
    static void AttemptUpdate();

    static bool FileExist(const char *fileName);

    // ��������� ���� ��� ������. ���������� ������ �����
    static int OpenFileForRead(const char *fileName);

    // ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
    static int ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();

    static void *GetHandleHCD();

    static void *GetHandleUSBH();

    class LL_
    {
    public:
        static void InitHCD(void *phost);

        static void SetToggle(uint8 pipe, uint8 toggle);

        static uint8 GetToggle(uint8 pipe);
    };
};
