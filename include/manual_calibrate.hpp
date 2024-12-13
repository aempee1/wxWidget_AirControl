#ifndef MANUAL_CALIBRATE_HPP
#define MANUAL_CALIBRATE_HPP

#include <wx/wx.h>
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>
#include "modbus_utils.hpp"
#include "serial_utils.hpp"

using namespace std;
using namespace boost::asio;

tuple<string, string, string> ReadPortsFromFile(const string& fileName) ;
class ManualCalibrationDialog : public wxDialog {
public:
    ManualCalibrationDialog(wxWindow *parent);
    ~ManualCalibrationDialog();
    void OnSetButtonClick(wxCommandEvent &event); // ฟังก์ชัน event handler

    double calculatePID(double setpointValue, double currentValue);
    wxDECLARE_EVENT_TABLE();

private:

    wxTimer *timer; // Timer สำหรับสุ่มค่า

    modbus_t* modbusCtx; // เพิ่มตัวแปรนี้เป็นสมาชิกของคลาส

    serial_port serialCtx;
    void OnTimer(wxTimerEvent &event); // Handler สำหรับ Timer
    serial_port InitialSerial(io_service& io, const string& port_name);
    modbus_t* InitialModbus(const char* modbus_port);

    bool CheckAndLoadPorts(const string& fileName, vector<string>& ports);

protected:
    int setpoint;

    wxTextCtrl *setFlowInput;
    wxTextCtrl *refFlowInput;
    wxTextCtrl *actFlowInput;
    wxTextCtrl *errorInput;

};

#endif // MANUAL_CALIBRATE_HPP
