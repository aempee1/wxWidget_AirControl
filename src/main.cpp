#include <wx/wx.h>
#include <wx/aboutdlg.h>  // สำหรับ wxAboutDialogInfo
#include <wx/choice.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/sizer.h>
#include <wx/textctrl.h>

#include <vector>
#include <string>

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <dirent.h>
#include <regex>
#endif

// สร้างคลาสหลักสำหรับแอปพลิเคชัน
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// สร้างคลาสหลักสำหรับหน้าต่าง
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);

private:
    void SetupMainMenu();                // ฟังก์ชันสำหรับสร้างเมนู
    void OnAboutSoftware(wxCommandEvent& event); // ฟังก์ชันสำหรับจัดการคลิกเมนู About

    void OnComportSettings(wxCommandEvent& event); // ฟังก์ชันสำหรับจัดการคลิกเมนู Comport Settings
    
    void OnManualFlowsystem(wxCommandEvent& event); //
    wxDECLARE_EVENT_TABLE();             // ประกาศ Event Table
};

class ComportSettingsDialog : public wxDialog {
public:
    ComportSettingsDialog(wxWindow *parent);
private:
    std::vector<std::string> FetchAvailablePorts(); // ฟังก์ชันสำหรับดึงพอร์ตที่สามารถใช้ได้

};

class ManualCalibrationDialog : public wxDialog {
public:
    ManualCalibrationDialog(wxWindow *parent);
};


// เริ่มต้นตาราง Event
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(1001, MyFrame::OnAboutSoftware) // เชื่อมเมนู About กับฟังก์ชัน OnAboutSoftware
    EVT_MENU(1002, MyFrame::OnComportSettings)
    EVT_MENU(1003, MyFrame::OnManualFlowsystem) 
wxEND_EVENT_TABLE()

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Flow System Software v. 1.0.0", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    SetupMainMenu(); // เรียกใช้ฟังก์ชันสร้างเมนู

    this->SetMinSize(this->FromDIP(wxSize(200, 200)));
    this->SetSize(this->FromDIP(wxSize(500, 200)));
}

void MyFrame::OnComportSettings(wxCommandEvent& WXUNUSED(event)) {
    ComportSettingsDialog dialog(this);
    dialog.ShowModal();
}

ComportSettingsDialog::ComportSettingsDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Comport Settings", wxDefaultPosition, wxSize(500, 200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(3, 2, 10, 10);

    auto availablePorts = FetchAvailablePorts();

    // BLEAgent Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "BLEAgent:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *bleAgentChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        bleAgentChoice->Append(port);
    }
    if (!availablePorts.empty()) {
        bleAgentChoice->SetSelection(0);
    }
    gridSizer->Add(bleAgentChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    // Modbus Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Modbus:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *modbusChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        modbusChoice->Append(port);
    }
    if (!availablePorts.empty()) {
        modbusChoice->SetSelection(0);
    }
    gridSizer->Add(modbusChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    // Power Supply Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Power Supply:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *powerSupplyChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        powerSupplyChoice->Append(port);
    }
    if (!availablePorts.empty()) {
        powerSupplyChoice->SetSelection(0);
    }
    gridSizer->Add(powerSupplyChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    mainSizer->Add(gridSizer, 1, wxALL | wxALIGN_CENTER, 10);

    // ปุ่ม OK
    wxButton *okButton = new wxButton(this, wxID_OK, "OK");
    mainSizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

    SetSizer(mainSizer);
    Layout();
}

ManualCalibrationDialog::ManualCalibrationDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Manual Calibration", wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(4, 3, 10, 10);

    // Set Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Set Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    wxTextCtrl *setFlowInput = new wxTextCtrl(this, wxID_ANY);
    gridSizer->Add(setFlowInput, 1, wxEXPAND);
    wxButton *setButton = new wxButton(this, wxID_ANY, "Set");
    gridSizer->Add(setButton, 0, wxALIGN_CENTER);

    // Ref. Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Ref. Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    wxTextCtrl *refFlowInput = new wxTextCtrl(this, wxID_ANY);
    gridSizer->Add(refFlowInput, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "m3/h"), 0, wxALIGN_CENTER_VERTICAL);

    // Act. Flow Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Act. Flow:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    wxTextCtrl *actFlowInput = new wxTextCtrl(this, wxID_ANY);
    gridSizer->Add(actFlowInput, 1, wxEXPAND);
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "m3/h"), 0, wxALIGN_CENTER_VERTICAL);

    // Error Row
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Error:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_RIGHT);
    wxTextCtrl *errorInput = new wxTextCtrl(this, wxID_ANY);
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


void MyFrame::SetupMainMenu() {
    wxMenuBar *menuBar = new wxMenuBar();

    // เมนู Setting
    wxMenu *settingMenu = new wxMenu();
    settingMenu->Append(1002, "Comport Settings");
    settingMenu->Append(wxID_ANY, "File Settings");

    // เมนู Production
    wxMenu *productionMenu = new wxMenu();
    productionMenu->Append(1003, "Manual Flow System");
    productionMenu->Append(wxID_ANY, "Auto Flow System");

    // เมนู R&D Test
    wxMenu *R_D_testMenu = new wxMenu();

    // เมนู Help
    wxMenu *helpMenu = new wxMenu();

    // เมนู About
    wxMenu *aboutMenu = new wxMenu();
    aboutMenu->Append(1001, "About Software");

    // เพิ่มเมนูลงใน MenuBar
    menuBar->Append(settingMenu, "&Setting");
    menuBar->Append(productionMenu, "&Production");
    menuBar->Append(R_D_testMenu, "&RnD Test");
    menuBar->Append(helpMenu, "&Help");
    menuBar->Append(aboutMenu, "&About");

    // ตั้งค่า MenuBar ให้กับหน้าต่าง
    SetMenuBar(menuBar);
}

void MyFrame::OnManualFlowsystem(wxCommandEvent& WXUNUSED(event)) {
    ManualCalibrationDialog dialog(this);
    dialog.ShowModal();
}

// ฟังก์ชันจัดการคลิกเมนู About
void MyFrame::OnAboutSoftware(wxCommandEvent& WXUNUSED(event)) {
    wxDialog *aboutDialog = new wxDialog(this, wxID_ANY, "About", wxDefaultPosition, wxSize(300, 200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER);

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);

    // สร้าง GridSizer สำหรับข้อมูล 2 คอลัมน์และ 5 แถว
    wxGridSizer *gridSizer = new wxGridSizer(5, 2, 5, 0); // 5 แถว, 2 คอลัมน์, ระยะห่างระหว่างแถวและคอลัมน์

    // เพิ่มข้อมูลใน GridSizer
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Software Name :"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Flow Control"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Version :"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "1.0.0"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Release Date :"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "01 Jan 2025"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Author :"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "M"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "Email :"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gridSizer->Add(new wxStaticText(aboutDialog, wxID_ANY, "example@abc.com"), 0, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    // เพิ่ม GridSizer ลงใน MainSizer
    mainSizer->Add(gridSizer, 1, wxALL | wxEXPAND, 10);

    // เพิ่มปุ่ม OK
    wxButton *okButton = new wxButton(aboutDialog, wxID_OK, "OK");
    mainSizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

    aboutDialog->SetSizer(mainSizer);
    aboutDialog->Layout();
    aboutDialog->ShowModal();

    aboutDialog->Destroy();
}


std::vector<std::string> ComportSettingsDialog::FetchAvailablePorts() {
    std::vector<std::string> ports;

#ifdef _WIN32
    char portName[10];
    for (int i = 1; i <= 256; ++i) {
        snprintf(portName, sizeof(portName), "COM%d", i);
        HANDLE hPort = CreateFileA(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
        if (hPort != INVALID_HANDLE_VALUE) {
            ports.push_back(portName);
            CloseHandle(hPort);
        }
    }
#endif

#ifdef __APPLE__
    DIR *dir = opendir("/dev");
    if (dir) {
        struct dirent *entry;
        std::regex serialRegex("^tty\\..*"); // พอร์ต serial บน macOS มักจะขึ้นต้นด้วย "cu."
        while ((entry = readdir(dir)) != NULL) {
            if (std::regex_match(entry->d_name, serialRegex)) {
                ports.push_back("/dev/" + std::string(entry->d_name));
            }
        }
        closedir(dir);
    }
#endif

    return ports;
}