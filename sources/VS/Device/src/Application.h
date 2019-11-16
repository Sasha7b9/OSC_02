#pragma once
#pragma warning(push, 0)
#include <wx/wx.h>
#pragma warning(pop)



class Application : public wxApp
{
public:
    virtual bool OnInit() wxOVERRIDE;
    virtual int OnExit() wxOVERRIDE;
};



class Frame : public wxFrame
{
public:

    static const int WIDTH = 640;
    static const int HEIGHT = 480;

    Frame(const wxString& title);

    void OnSize(wxCommandEvent &);
    void OnQuit(wxCommandEvent &);
    void OnGeneratorTune(wxCommandEvent &);
    void OnAbout(wxCommandEvent &);
    void OnTimer(wxTimerEvent &);
    void OnTimerLong(wxTimerEvent &);

private:
    wxTimer timer;
    /// ������ ��� "�������� �������" ������
    wxTimer timerLongPress;

    void DrawFPS();
    void HandlerEvents();

public:
    void OnDown(wxCommandEvent &event);
    void OnUp(wxCommandEvent &event);
};
