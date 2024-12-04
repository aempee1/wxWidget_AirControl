#include <string>
#include <boost/asio.hpp>
#include <vector>
#include "comport_setting.hpp"
#include "serial_utils.hpp"
#include "modbus_utils.hpp"

#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <dirent.h>
#include <regex>
#endif

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

        modbus_t* modbusContext = InitialModbus(selectedModbusPort.c_str());
    if (modbusContext) {
        wxLogMessage("Modbus initialized successfully.");
        // สามารถใช้ modbusContext ต่อไปได้ที่นี่
    } else {
        wxLogError("Failed to initialize Modbus.");
    }

    serial_port power_supply_Context = InitSerial(selectedPowerSupplyPort);


        // ปิด Dialog
    this->EndModal(wxID_OK);
    });

    SetSizer(mainSizer);
    Layout();
}

boost::asio::serial_port ComportSettingsDialog::InitSerial(const string& port ){
    io_service io;
    serial_port serial = init_serial_port(io, port);

    return serial ;
}

modbus_t* ComportSettingsDialog::InitialModbus(const char* modbus_port) {
    modbus_t* ctx = initialize_modbus(modbus_port);
    if (ctx == nullptr) {
        std::cerr << "Error initializing Modbus on port: " << modbus_port << std::endl;
        return nullptr;
    }
    std::cout << "Successfully initialized Modbus on port: " << modbus_port << std::endl;
    return ctx;
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