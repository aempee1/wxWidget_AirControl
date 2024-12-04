#include "manual_calibrate.hpp"
ManualCalibrationDialog::ManualCalibrationDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Manual Calibration", wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      setpoint(0)  // กำหนดค่าเริ่มต้นให้ setpoint เป็น 0
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(4, 3, 10, 10);

    // Set Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Set Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    setFlowInput = new wxTextCtrl(this, wxID_ANY);
    gridSizer->Add(setFlowInput, 1, wxEXPAND);

    wxButton *setButton = new wxButton(this, wxID_ANY, "Set");
    gridSizer->Add(setButton, 0, wxALIGN_CENTER);

    // Bind event handler ให้กับปุ่ม set
    setButton->Bind(wxEVT_BUTTON, &ManualCalibrationDialog::OnSetButtonClick, this);

    // Ref. Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Ref. Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    refFlowInput = new wxTextCtrl(this, wxID_ANY);
    refFlowInput->SetEditable(false); // Make the field read-only
    gridSizer->Add(refFlowInput, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "m3/h"), 0, wxALIGN_CENTER_VERTICAL);

    // Act. Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Act. Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    actFlowInput = new wxTextCtrl(this, wxID_ANY);
    actFlowInput->SetEditable(false); // Make the field read-only
    gridSizer->Add(actFlowInput, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "m3/h"), 0, wxALIGN_CENTER_VERTICAL);

    // Error Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Error:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    errorInput = new wxTextCtrl(this, wxID_ANY);
    errorInput->SetEditable(false);
    gridSizer->Add(errorInput, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "%"), 0, wxALIGN_CENTER_VERTICAL);

    // Center Grid
    wxBoxSizer *gridCenterSizer = new wxBoxSizer(wxHORIZONTAL);
    gridCenterSizer->Add(gridSizer, 0, wxALIGN_CENTER);

    mainSizer->Add(gridCenterSizer, 1, wxALL | wxALIGN_CENTER, 10);

    // Done Button
    wxButton *doneButton = new wxButton(this, wxID_OK, "Done");
    mainSizer->Add(doneButton, 0, wxALL | wxALIGN_CENTER, 10);

    SetSizer(mainSizer);
    Layout();
    Center();
}

void ManualCalibrationDialog::OnSetButtonClick(wxCommandEvent &event) {
    wxString inputStr = setFlowInput->GetValue(); // อ่านค่าจากช่องข้อความ setFlowInput
    long value;
    if (inputStr.ToLong(&value)) {
        setpoint = static_cast<int>(value); // แปลงค่าเป็น int และเก็บใน setpoint

        // แสดงค่า setpoint ในช่องข้อความที่ต้องการ
        refFlowInput->SetValue(wxString::Format("%d", setpoint));   // แสดงค่าใน refFlowInput
        actFlowInput->SetValue(wxString::Format("%d", setpoint));   // แสดงค่าใน actFlowInput
        errorInput->SetValue(wxString::Format("%d", setpoint));    // แสดงค่าใน errorInput

    
    } else {
        wxMessageBox("Invalid input. Please enter an integer value.", "Error", wxOK | wxICON_ERROR, this);
    }
}
