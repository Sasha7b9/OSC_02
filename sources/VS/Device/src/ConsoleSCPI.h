#pragma once
#pragma warning(push, 0)
#include <wx/frame.h>
#pragma warning(pop)


class ConsoleSCPI : public wxFrame
{
public:
    static void Open(wxFrame *parent);

private:
    ConsoleSCPI(wxFrame *parent);

    void OnSize(wxSizeEvent &);
};
