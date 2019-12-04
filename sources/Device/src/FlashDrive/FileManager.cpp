#include "defines.h"
#include "common/Display/Font/Font.h"
#include "Data/DataSettings.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Beeper.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include <cstring>


#define FM_CURSOR_IN_DIRS       (bf.cursorsInDirs)

static struct BitFieldFileManager
{
    uint  cursorsInDirs : 1;
    uint  notUsed       : 31;
} bf = {1, 0};


#define RECS_ON_PAGE    23
#define WIDTH_COL       135

static char currentDir[255] = "\\";
static int numFirstDir = 0;         // ����� ������� ����������� �������� � ����� ������. ����� ����� ���� �������� RECS_ON_PAGE ���������
static int numCurDir = 0;           // ����� ������������� ��������
static int numFirstFile = 0;        // ����� ������� ����������� ����� � ������ ������. ����� ����� ���� �������� RECS_ON_PAGE ������.
static int numCurFile = 0;          // ����� ������������� �����
static int numDirs = 0;
static int numFiles = 0;



void FileManager::Init()
{
    std::strcpy(currentDir, "\\");
    numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
}


static void DrawLongString(int x, int y, const char *string, bool hightlight)
{
    int length = Font::GetLengthText(string);

    Color color = Color::FILL;
    if (hightlight)
    {
        Region(WIDTH_COL + 9, 8).Fill(x - 1, y, color);
        color = Color::BACK;
    }

    if (length <= WIDTH_COL)
    {
        String(string).Draw(x, y, color);
    }
    else
    {
        color.SetAsCurrent();

		Text(string).DrawWithLimitation(x, y, x, y, WIDTH_COL, 10);

        String("...").Draw(x + WIDTH_COL + 3, y);
    }
}


static void DrawHat(int x, int y, const char *string, int num1, int num2)
{
    Region(WIDTH_COL + 9, RECS_ON_PAGE * 9 + 11).Fill(x - 1, y, Color::BACK);
    String(string, num1, num2).Draw(x + 60, y, Color::FILL);

	HLine(138).Draw(x + 2, y + 10);
}


static void DrawDirs(int x, int y)
{
    FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
    DrawHat(x, y, "������� : %d/%d", numCurDir + ((numDirs == 0) ? 0 : 1), numDirs);
    char nameDir[255];
    StructForReadDir sfrd;
    y += 12;
    if (FDrive::GetNameDir(currentDir, numFirstDir, nameDir, &sfrd))
    {
        int  drawingDirs = 0;
        DrawLongString(x, y, nameDir, FM_CURSOR_IN_DIRS && ( numFirstDir == numCurDir));
        while (drawingDirs < (RECS_ON_PAGE - 1) && FDrive::GetNextNameDir(nameDir, &sfrd))
        {
            drawingDirs++;
            DrawLongString(x, y + drawingDirs * 9, nameDir, FM_CURSOR_IN_DIRS && ( numFirstDir + drawingDirs == numCurDir));
        }
    }
}


static void DrawFiles(int x, int y)
{
    DrawHat(x, y, "���� : %d/%d", numCurFile + ((numFiles == 0) ? 0 : 1), numFiles);
    char nameFile[255];
    StructForReadDir sfrd;
    y += 12;
    if (FDrive::GetNameFile(currentDir, numFirstFile, nameFile, &sfrd))
    {
        int drawingFiles = 0;
        DrawLongString(x, y, nameFile, !FM_CURSOR_IN_DIRS && (numFirstFile == numCurFile));
        while (drawingFiles < (RECS_ON_PAGE - 1) && FDrive::GetNextNameFile(nameFile, &sfrd))
        {
            drawingFiles++;
            DrawLongString(x, y + drawingFiles * 9, nameFile, !FM_CURSOR_IN_DIRS && (numFirstFile + drawingFiles == numCurFile));
        }
    }
}


static void DrawNameCurrentDir(int left, int top) //-V2506
{
    Color::FILL.SetAsCurrent();
    int length = Font::GetLengthText(currentDir);
    if (length < 277)
    {
        String(currentDir).Draw(left + 1, top + 1);
    }
    else
    {
        char *pointer = currentDir + 2;
        while (length > 277)
        {
            while (*pointer != '\\' && pointer < currentDir + 255)
            {
                pointer++;
            }
            if (pointer >= currentDir + 255)
            {
                return;
            }
            length = Font::GetLengthText(++pointer);
        }
        String(pointer).Draw(left + 1, top + 1);
    }
}


void FileManager::Draw() //-V2506
{
    if (!FM_NEED_REDRAW)
    {
        return;
    }

    //ModeFSMC mode = FSMC::GetMode();
    //FSMC_SetMode(ModeFSMC_Display);

    int left = 1;
    int top = 1;
    int width = 297;
    int left2col = width / 2;

    if (FM_NEED_REDRAW == FM_REDRAW_FULL)
    {
        Painter::BeginScene(Color::BACK);
        Menu::Draw();
        Rectangle(width, 239).Draw(0, 0, Color::FILL);
        Region(Grid::Width() - 2, Grid::Height() - 2).Fill(left, top, Color::BACK);
        FDrive::GetNumDirsAndFiles(currentDir, &numDirs, &numFiles);
        DrawNameCurrentDir(left, top + 2);

        VLine(223 - top).Draw(left2col, top + 16, Color::FILL);
		HLine(width).Draw(0, top + 15);
    }

    if (FM_NEED_REDRAW != FM_REDRAW_FILES)
    {
        DrawDirs(left + 2, top + 18);
    }

    if (FM_NEED_REDRAW != FM_REDRAW_FOLDERS)
    {
        DrawFiles(left2col + 3, top + 18);
    }

    Painter::EndScene();

    FM_NEED_REDRAW = 0;

    //FSMC::SetMode(mode);
}

void FileManager::Press_LevelDown() //-V2506
{
    FM_NEED_REDRAW = FM_REDRAW_FULL;
    if (!FM_CURSOR_IN_DIRS)
    {
        return;
    }
    char nameDir[100];
    StructForReadDir sfrd;
    if (FDrive::GetNameDir(currentDir, numCurDir, nameDir, &sfrd))
    {
        if (std::strlen(currentDir) + std::strlen(nameDir) < 250)
        {
            FDrive::CloseCurrentDir(&sfrd);
            std::strcat(currentDir, "\\");
            std::strcat(currentDir, nameDir);
            numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
        }

    }
    FDrive::CloseCurrentDir(&sfrd);
}


void FileManager::Press_LevelUp() //-V2506
{
    FM_NEED_REDRAW = FM_REDRAW_FULL;
    if (std::strlen(currentDir) == 1)
    {
        return;
    }
    char *pointer = currentDir + std::strlen(currentDir);
    while (*pointer != '\\')
    {
        pointer--;
    }
    *pointer = '\0';
    numFirstDir = numFirstFile = numCurDir = numCurFile = 0;
    FM_CURSOR_IN_DIRS = 1;
}


static void IncCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir++;
        if (numCurDir > numDirs - 1)
        {
            numCurDir = 0;
            numFirstDir = 0;
        }
        if (numCurDir - numFirstDir > RECS_ON_PAGE - 1)
        {
            numFirstDir++;
        }
    }
}


static void DecCurrentDir()
{
    if (numDirs > 1)
    {
        numCurDir--;

        if (numCurDir < 0)
        {
            numCurDir = numDirs - 1;
            numFirstDir = numDirs - RECS_ON_PAGE;
            LIMITATION(numFirstDir, 0, numCurDir);
        }

        if (numCurDir < numFirstDir)
        {
            numFirstDir = numCurDir;
        }
    }
}


static void IncCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile++;
        if (numCurFile > numFiles - 1)
        {
            numCurFile = 0;
            numFirstFile = 0;
        }
        if (numCurFile - numFirstFile > RECS_ON_PAGE - 1)
        {
            numFirstFile++;
        }
    }
}


static void DecCurrentFile()
{
    if (numFiles > 1)
    {
        numCurFile--;

        if (numCurFile < 0)
        {
            numCurFile = numFiles - 1;
            numFirstFile = numFiles - RECS_ON_PAGE;;
            LIMITATION(numFirstFile, 0, numCurFile);
        }

        if (numCurFile < numFirstFile)
        {
            numFirstFile = numCurFile;
        }
    }
}


bool FileManager::HandlerKey(const KeyEvent &event)
{
    int delta = (event.IsUp() || event.IsRight()) ? 1 : -1;

    Beeper::RegulatorSwitchRotate();
    if (FM_CURSOR_IN_DIRS)
    {
        delta > 0 ? DecCurrentDir() : IncCurrentDir();
        FM_NEED_REDRAW = FM_REDRAW_FOLDERS;
    }
    else
    {
        delta > 0 ? DecCurrentFile() : IncCurrentFile();
        FM_NEED_REDRAW = FM_REDRAW_FILES;
    }

    return true;
}


bool FileManager::GetNameForNewFile(char name[255]) //-V2506
{
    static int number = 0;

    std::strcpy(name, currentDir);
    std::strcat(name, "\\");

    int size = static_cast<int>(std::strlen(set.mem.fileName));
    if (size == 0)
    {
        return false;
    }

    if (set.mem.fileNamingMode == FileNamingMode::Manually)
    {
        LIMITATION(size, 1, 95);
        std::strcat(name, set.mem.fileName);
        std::strcat(name, ".");
        std::strcat(name, (set.mem.modeSaveSignal == ModeSaveSignal::BMP) ? "bmp" : "txt");
        return true;
    }
    else
    {
        PackedTime time = HAL_RTC::GetPackedTime();
                           //  1          2           3         4           5             6
        uint values[] = {0U, time.year, time.month, time.day, time.hours, time.minutes, time.seconds};

        char *ch = set.mem.fileNameMask;
        char *wr = name;

        while (*wr != '\0')
        {
            wr++;
        }

        while (*ch)
        {
            if (*ch >= 0x30)
            {
                *wr = *ch;
                wr++;
            }
            else
            {
                if (*ch == 0x07)
                {
                    number++;
                    char *strNumber = Integer(number).ToString(false, *(ch + 1)).c_str();
                    std::strcpy(wr, strNumber);
                    wr += std::strlen(strNumber);
                    ch++;
                }
                else
                {
                    if (*ch >= 0x01 && *ch <= 0x06)
                    {
                        char *strValue = Integer(static_cast<int>(values[*ch])).ToString(false, 2).c_str();
                        std::strcpy(wr, strValue);
                        wr += std::strlen(strValue);
                    }
                }
            }
            ch++;
        }

        *wr = '.';
        *(wr + 1) = '\0';

        std::strcat(name, (set.mem.modeSaveSignal == ModeSaveSignal::BMP) ? "bmp" : "txt");

        return true;
    }
}


void FileManager::Press_Tab()
{
    FM_NEED_REDRAW = FM_REDRAW_FOLDERS;

    if (FM_CURSOR_IN_DIRS)
    {
        if (numFiles != 0)
        {
            FM_CURSOR_IN_DIRS = 0;
        }
    }
    else
    {
        if (numDirs != 0)
        {
            FM_CURSOR_IN_DIRS = 1;
        }
    }
}
