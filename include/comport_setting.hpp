#ifndef COMPORT_SETTING_HPP
#define COMPORT_SETTING_HPP

#include <wx/wx.h>         // สำหรับ wxWidget
#include <wx/choice.h>
#include <vector>
#include <string>
#include <boost/asio.hpp>

using namespace std;
using namespace boost::asio;


class ComportSettingsDialog : public wxDialog {
public:
    ComportSettingsDialog(wxWindow *parent)
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

    // เพิ่มการจัดการปุ่ม OK
    okButton->Bind(wxEVT_BUTTON, [this, bleAgentChoice, modbusChoice, powerSupplyChoice](wxCommandEvent& event) {
        // บันทึกพอร์ตที่เลือก
        selectedBleAgentPort = bleAgentChoice->GetStringSelection().ToStdString();
        selectedModbusPort = modbusChoice->GetStringSelection().ToStdString();
        selectedPowerSupplyPort = powerSupplyChoice->GetStringSelection().ToStdString();

        // แสดงผลหรือใช้พอร์ตที่เลือกตามต้องการ
        wxLogMessage("BLEAgent Port: %s", selectedBleAgentPort);
        wxLogMessage("Modbus Port: %s", selectedModbusPort);
        wxLogMessage("Power Supply Port: %s", selectedPowerSupplyPort);

        InitSerial(selectedPowerSupplyPort);
        // ปิด Dialog
        this->EndModal(wxID_OK);
    });

    SetSizer(mainSizer);
    Layout();
}
private:
    std::vector<std::string> FetchAvailablePorts(); // ฟังก์ชันสำหรับดึงพอร์ตที่สามารถใช้ได้

    void InitSerial(const std::string& port);

    // ตัวแปรสำหรับเก็บพอร์ตที่เลือก
    std::string selectedBleAgentPort;
    std::string selectedModbusPort;
    std::string selectedPowerSupplyPort;

};

#endif // COMPORT_SETTINGS_HPP