#include <string>
#include <boost/asio.hpp>
#include <vector>
#include "comport_setting.hpp"
#include "serial_utils.hpp"
#include "modbus_utils.hpp"
#include <fstream>


#ifdef _WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <dirent.h>
#include <regex>
#endif

boost::asio::serial_port ComportSettingsDialog::init_serial_port(boost::asio::io_service& io, const std::string& port_name)
{
    boost::asio::serial_port serial(io, port_name);
    serial.set_option(boost::asio::serial_port_base::baud_rate(9600));
    serial.set_option(boost::asio::serial_port_base::character_size(8));
    serial.set_option(boost::asio::serial_port_base::parity(boost::asio::serial_port_base::parity::none));
    serial.set_option(boost::asio::serial_port_base::stop_bits(boost::asio::serial_port_base::stop_bits::one));
    serial.set_option(boost::asio::serial_port_base::flow_control(boost::asio::serial_port_base::flow_control::none));

    if (!serial.is_open())
    {
        cerr << "Failed to open serial port!" << endl;
        throw runtime_error("Failed to open serial port");
    }

    cout << "Serial port connected." << endl;
    return serial;
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

void ComportSettingsDialog::SaveSelectedPorts() {
    std::ofstream outFile("selected_ports.txt");
    outFile << selectedBleAgentPort << std::endl;
    outFile << selectedModbusPort << std::endl;
    outFile << selectedPowerSupplyPort << std::endl;
    outFile.close();
}

void ComportSettingsDialog::LoadSelectedPorts() {
    std::ifstream inFile("selected_ports.txt");
    if (inFile) {
        std::getline(inFile, selectedBleAgentPort);
        std::getline(inFile, selectedModbusPort);
        std::getline(inFile, selectedPowerSupplyPort);
        inFile.close();
    }
}

ComportSettingsDialog::ComportSettingsDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Comport Settings", wxDefaultPosition, wxSize(500, 200), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER)
{
    LoadSelectedPorts();  // โหลดพอร์ตที่เลือกจากไฟล์เมื่อเปิด Dialog

    wxBoxSizer *mainSizer = new wxBoxSizer(wxVERTICAL);
    wxFlexGridSizer *gridSizer = new wxFlexGridSizer(3, 2, 10, 10);

    auto availablePorts = FetchAvailablePorts();

    // BLEAgent Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "BLEAgent:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *bleAgentChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        bleAgentChoice->Append(port);
    }
    if (!availablePorts.empty() && !selectedBleAgentPort.empty()) {
        bleAgentChoice->SetStringSelection(selectedBleAgentPort);
    } else {
        bleAgentChoice->SetSelection(0);
    }
    gridSizer->Add(bleAgentChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    // Modbus Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Modbus:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *modbusChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        modbusChoice->Append(port);
    }
    if (!availablePorts.empty() && !selectedModbusPort.empty()) {
        modbusChoice->SetStringSelection(selectedModbusPort);
    } else {
        modbusChoice->SetSelection(0);
    }
    gridSizer->Add(modbusChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    // Power Supply Label และ Choice
    gridSizer->Add(new wxStaticText(this, wxID_ANY, "Power Supply:"), 0, wxALIGN_CENTER_VERTICAL | wxALIGN_CENTER_HORIZONTAL);
    wxChoice *powerSupplyChoice = new wxChoice(this, wxID_ANY);
    for (const auto& port : availablePorts) {
        powerSupplyChoice->Append(port);
    }
    if (!availablePorts.empty() && !selectedPowerSupplyPort.empty()) {
        powerSupplyChoice->SetStringSelection(selectedPowerSupplyPort);
    } else {
        powerSupplyChoice->SetSelection(0);
    }
    gridSizer->Add(powerSupplyChoice, 1, wxALIGN_CENTER_HORIZONTAL);

    mainSizer->Add(gridSizer, 1, wxALL | wxALIGN_CENTER, 10);

    // ปุ่ม OK
    wxButton *okButton = new wxButton(this, wxID_OK, "OK");
    mainSizer->Add(okButton, 0, wxALL | wxALIGN_CENTER, 10);

    // การจัดการปุ่ม OK
    okButton->Bind(wxEVT_BUTTON, [this, bleAgentChoice, modbusChoice, powerSupplyChoice](wxCommandEvent& event) {
        // บันทึกพอร์ตที่เลือก
        selectedBleAgentPort = bleAgentChoice->GetStringSelection().ToStdString();
        selectedModbusPort = modbusChoice->GetStringSelection().ToStdString();
        selectedPowerSupplyPort = powerSupplyChoice->GetStringSelection().ToStdString();

        // บันทึกค่าพอร์ตที่เลือก
        SaveSelectedPorts();

        // การเชื่อมต่อ Serial และ Modbus
        boost::asio::io_service io;
        boost::asio::serial_port serial = init_serial_port(io, selectedPowerSupplyPort);
        modbus_t* modbusContext = InitialModbus(selectedModbusPort.c_str());

        // แสดงผลหรือใช้พอร์ตที่เลือกตามต้องการ
        wxLogMessage("BLEAgent Port: %s", selectedBleAgentPort);
        wxLogMessage("Modbus Port: %s", selectedModbusPort);
        wxLogMessage("Power Supply Port: %s", selectedPowerSupplyPort);

        if (modbusContext) {
            wxLogMessage("Modbus and Serial initialized successfully.");
        } else {
            wxLogError("Failed to initialize Modbus.");
        }

        // ปิด Dialog
        this->EndModal(wxID_OK);
    });

    SetSizer(mainSizer);
    Layout();
}

