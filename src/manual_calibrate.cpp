#include "manual_calibrate.hpp"
#include <fstream>
#include <string>
#include <tuple>
#include <stdexcept>

using namespace std;
using namespace boost::asio;

boost::asio::serial_port ManualCalibrationDialog::init_serial_port(boost::asio::io_service& io, const std::string& port_name)
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

modbus_t* ManualCalibrationDialog::InitialModbus(const char* modbus_port) {
    modbus_t* ctx = initialize_modbus(modbus_port);
    if (ctx == nullptr) {
        std::cerr << "Error initializing Modbus on port: " << modbus_port << std::endl;
        return nullptr;
    }
    std::cout << "Successfully initialized Modbus on port: " << modbus_port << std::endl;
    return ctx;
}

std::tuple<std::string, std::string, std::string> ReadPortsFromFile(const std::string& fileName) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        throw std::runtime_error("Unable to open the port configuration file."); // โยนข้อผิดพลาดหากเปิดไฟล์ไม่ได้
    }

    std::string bluetoothPort, modbusPort, serialPort;
    std::getline(file, bluetoothPort);
    std::getline(file, modbusPort);
    std::getline(file, serialPort);

    if (bluetoothPort.empty() || modbusPort.empty() || serialPort.empty()) {
        throw std::runtime_error("The port configuration file must contain at least 3 lines."); // โยนข้อผิดพลาดหากข้อมูลไม่ครบ
    }

    return std::make_tuple(bluetoothPort, modbusPort, serialPort); // คืนค่าพอร์ตทั้งหมดในรูปแบบ tuple
}

bool ManualCalibrationDialog::CheckAndLoadPorts(const std::string& fileName, std::vector<std::string>& ports) {
    std::ifstream file(fileName);
    if (!file.is_open()) {
        wxMessageBox("Unable to open the file selected_ports.txt", "Error", wxOK | wxICON_ERROR, this);
        return false;
    }

    std::string port;
    while (std::getline(file, port)) {
        if (!port.empty()) {
            ports.push_back(port);
        }
    }

    if (ports.empty()) {
        wxMessageBox("You don't have selected ports", "Warning", wxOK | wxICON_WARNING, this);
        return false;
    }

    return true;
}

ManualCalibrationDialog::ManualCalibrationDialog(wxWindow *parent)
    : wxDialog(parent, wxID_ANY, "Manual Calibration", wxDefaultPosition, wxSize(400, 300), wxDEFAULT_DIALOG_STYLE | wxRESIZE_BORDER),
      setpoint(0), timer(new wxTimer(this)) , modbusCtx(nullptr) // กำหนดค่าเริ่มต้นให้ timer
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

    std::srand(std::time(nullptr));

    // Load ports from file
    std::vector<std::string> selectedPorts;
    if (!CheckAndLoadPorts("selected_ports.txt", selectedPorts)) {
        return; // หากไม่มีพอร์ตที่เลือกให้หยุดการทำงานของ dialog
    }

    if (selectedPorts.size() < 2) {
        wxMessageBox("Selected ports are insufficient.", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    // Initialize Modbus
    modbusCtx = InitialModbus(selectedPorts[1].c_str());
    if (!modbusCtx) {
        wxMessageBox("Failed to initialize Modbus.", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    std::cout << "Modbus initialized successfully." << std::endl;

}

ManualCalibrationDialog::~ManualCalibrationDialog() {
    if (timer->IsRunning()) {
        timer->Stop();
    }
    if (modbusCtx) {
        modbus_close(modbusCtx);
        modbus_free(modbusCtx);
    }
    delete timer;
}

void ManualCalibrationDialog::OnSetButtonClick(wxCommandEvent &event) {
    wxString inputStr = setFlowInput->GetValue();
    long value;
    if (inputStr.ToLong(&value)) {
        setpoint = static_cast<int>(value);

        // เริ่มทำงาน timer สำหรับสุ่มค่า
        if (!timer->IsRunning()) {
            timer->Start(1000); // สุ่มทุก 1 วินาที
        }
    } else {
        wxMessageBox("Invalid input. Please enter an integer value.", "Error", wxOK | wxICON_ERROR, this);
    }
}

void ManualCalibrationDialog::OnTimer(wxTimerEvent &event) {
    if (!modbusCtx) {
        wxMessageBox("Modbus context is not initialized.", "Error", wxOK | wxICON_ERROR, this);
        return;
    }

    uint16_t refFlow[4] ;
    int rc ;
    do{
        rc = modbus_read_registers(modbusCtx, /*register address=*/6, /*num registers=*/2, refFlow);
        if (rc == -1) {
        return;
    }
    }while(rc == -1);
    float refFlowValue ;
    memcpy(&refFlowValue , refFlow, sizeof(refFlowValue));

    // อัปเดตค่า refFlow
    refFlowInput->SetValue(wxString::Format("%2f", refFlowValue));

    // สุ่มค่าอื่น (ตามตัวอย่างเดิม)
    int randomActFlow = std::rand() % 100; // สุ่มเลข 0-99
    int randomError = std::rand() % 100;   // สุ่มเลข 0-99

    actFlowInput->SetValue(wxString::Format("%d", randomActFlow));
    errorInput->SetValue(wxString::Format("%d", randomError));
}

// Bind Event Table
wxBEGIN_EVENT_TABLE(ManualCalibrationDialog, wxDialog)
    EVT_TIMER(wxID_ANY, ManualCalibrationDialog::OnTimer)
wxEND_EVENT_TABLE()
