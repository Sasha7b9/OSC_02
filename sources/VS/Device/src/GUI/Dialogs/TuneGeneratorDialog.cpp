#include "defines.h"
#include "GUI/Dialogs/TuneGeneratorDialog.h"
#include "Utils/StringUtils.h"


enum
{
    ID_FREQUENCY,
    ID_AMPLITUDE,
    ID_OFFSET
};


static float storeFrequency = 1e3F;
static float storeAmplitude = 0.0F;
static float storeOffset = 0.0F;

static wxTextCtrl *tcFrequency = nullptr;
static wxTextCtrl *tcAmplitude = nullptr;
static wxTextCtrl *tcOffset = nullptr;


float TuneGeneratorDialog::frequency = storeFrequency;
float TuneGeneratorDialog::amplitude = storeAmplitude;
float TuneGeneratorDialog::offset = storeOffset;


static wxPanel *CreatePanelParameters(wxDialog *dlg, const wxString &name)
{
    wxPanel *panel = new wxPanel(dlg);

    new wxStaticBox(panel, wxID_ANY, name, wxDefaultPosition, { 160, 105 });

    int y = 20, x = 10, dY = 26;
    int dX = 80;

    tcFrequency = new wxTextCtrl(panel, ID_FREQUENCY, SU::DoubleToString(TuneGeneratorDialog::frequency), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_FREQUENCY, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("�������"), { x + dX, y + 3 });

    y += dY;
    tcAmplitude = new wxTextCtrl(panel, ID_AMPLITUDE, SU::DoubleToString(TuneGeneratorDialog::amplitude), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_AMPLITUDE, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("���������"), { x + dX, y + 3 });

    y += dY;
    tcOffset = new wxTextCtrl(panel, ID_OFFSET, SU::DoubleToString(TuneGeneratorDialog::offset), wxPoint(x, y), wxSize(75, 20));
    dlg->Connect(ID_OFFSET, wxEVT_COMMAND_TEXT_UPDATED, wxCommandEventHandler(TuneGeneratorDialog::OnChangeParameter));
    new wxStaticText(panel, wxID_ANY, wxT("��������"), { x + dX, y + 3 });

    return panel;
}


TuneGeneratorDialog::TuneGeneratorDialog() : Dialog(wxT("��������� ����������"))
{
    wxBoxSizer *hBox = new wxBoxSizer(wxHORIZONTAL);

    hBox->Add(CreatePanelParameters(this, wxT("����� 1")));
    hBox->Add(CreatePanelParameters(this, wxT("����� 2")));

    SetBoxSizer(hBox, { 320, 105 });

    storeFrequency = frequency;
    storeOffset = offset;
    storeAmplitude = amplitude;
}


/// ��������� ��������� ��������� parameter �� tc, ���� ��� ��������������
static void SetIfValid(float *parameter, const wxTextCtrl *tc)
{
    double value = 0.0;
    if (SU::StringToDouble(&value, tc->GetValue()))
    {
        *parameter = static_cast<float>(value);
    }
}


void TuneGeneratorDialog::OnChangeParameter(wxCommandEvent &)
{
    SetIfValid(&frequency, tcFrequency);

    SetIfValid(&amplitude, tcAmplitude);

    SetIfValid(&offset, tcOffset);
}


void TuneGeneratorDialog::ApplyParameters()
{

}


void TuneGeneratorDialog::CancelParameters()
{
    frequency = storeFrequency;
    amplitude = storeAmplitude;
    offset = storeOffset;
}
