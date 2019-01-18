#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
    TypePage_Content,       ///< �� ���� �������� ����������� ������ �� ������ ��������
    TypePage_Description    ///< �� ���� �������� ��� ������, ������ ��������
} TypePageHelp;


#define MAX_PAGES 3


struct PageHelpContent
{
    TypePageHelp       type;
    uint8              notUsed[3];
    void              *parent;              ///< ����� ������������ ��������
    /// \todo ���������� �� ����� ���������
    pFuncBV            funcNotUsed;         ///< ��������� ��� ������������ � ����� Page - ����� ��� ���������.
    const char * const titleHint[4];        ///< \brief �������� �������� �� ������� � ���������� �����, � ����� ���������� �� ������� � ���������� 
                                            /// (��� ������ TypePage_Description)
    void              *pages[MAX_PAGES];    ///< ������ �������� ������ ��������� ������� � ������ TypePage_Content
};

extern const PageHelpContent helpMenu;

static const PageHelpContent helpMenuCommon =
{
    TypePage_Description, {},
    (void *)&helpMenu, 0,
    {
        "����� �������� ��������� ����",
        "General description of the principles of the menu",
        "������ �� ������ ���������� ����� ��� ���� ������� - ��������, ������������� ����� 0.5 ��� � �������, ������������� ����� 0.5 ���. "
        "���� ������������ ����� ����������� ��������� ���������. "
        "������� �������� ���� ����������� �������� ���� ������� �������� ������ ����. "
        "�� ���� ��������� ������� �������� ������� ���������������� ������ � ������� ������� ����� �� ������ �� ������ ����������:\n"
        "\"�������\" - �������\n"
        "\"����� 1\" - ����� 1\n"
        "\"����� 2\" - ����� 2\n"
        "\"�����.\" - �����\n"
        "\"����E����\" - ����\n"
        "\"�������\" - �������\n"
        "\"������\" - ������\n"
        "\"���������\" - �����\n"
        "\"������\" - ������\n"
        "\"������\" - ������\n"
        "� ������ �������� ������� �������� ������� ��������� ��������������� ���������"
        ,
        ""
    },
    {}
};

static const PageHelpContent helpMenuControls =
{
    TypePage_Description, {},
    (void *)&helpMenu, 0,
    {
        "�������� ������� �����������",
        "Description of the controls",
        "",
        ""
    },
    {}
};

extern const PageHelpContent helpMain;

static const PageHelpContent helpSCPI =
{
    TypePage_Description, {},
    (void *)&helpMain, 0,
    {
        "������ � SCPI",
        "Working with SCPI",
        "",
        ""
    },
    {}
};

const PageHelpContent helpMenu =
{
    TypePage_Content, {},
    (void *)&helpMain, 0,
    {
        "������ � ����",
        "Working with menus"
    },
    {
        (void *)&helpMenuCommon,
        (void *)&helpMenuControls
    }
};

const PageHelpContent helpMain =
{
    TypePage_Content, {},
    0, 0,
    {
        "������",
        "HELP"
    },
    {
        (void *)&helpMenu,
        (void *)&helpSCPI
    }
};
