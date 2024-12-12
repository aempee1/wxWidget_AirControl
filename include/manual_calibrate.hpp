#ifndef MANUAL_CALIBRATE_HPP
#define MANUAL_CALIBRATE_HPP

#include <wx/wx.h> 
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>
#include "modbus_utils.hpp"
#include "serial_utils.hpp"

std::tuple<std::string, std::string, std::string> ReadPortsFromFile(const std::string& fileName) ;
class ManualCalibrationDialog : public wxDialog {
public:
    ManualCalibrationDialog(wxWindow *parent);
    ~ManualCalibrationDialog();
    void OnSetButtonClick(wxCommandEvent &event); // ฟังก์ชัน event handler
    wxDECLARE_EVENT_TABLE();
private:
    wxTimer *timer; // Timer สำหรับสุ่มค่า

    modbus_t* modbusCtx; // เพิ่มตัวแปรนี้เป็นสมาชิกของคลาส
    void OnTimer(wxTimerEvent &event); // Handler สำหรับ Timer
    boost::asio::serial_port init_serial_port(boost::asio::io_service& io, const std::string& port_name);
    modbus_t* InitialModbus(const char* modbus_port);

    bool CheckAndLoadPorts(const std::string& fileName, std::vector<std::string>& ports);

protected:
    int setpoint;

    wxTextCtrl *setFlowInput;
    wxTextCtrl *refFlowInput;
    wxTextCtrl *actFlowInput;
    wxTextCtrl *errorInput;

};





#endif // MANUAL_CALIBRATE_HPP
