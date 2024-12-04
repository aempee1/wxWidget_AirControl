#ifndef COMPORT_SETTING_HPP
#define COMPORT_SETTING_HPP

#include <wx/wx.h>         // สำหรับ wxWidget
#include <wx/choice.h>
#include <vector>
#include <string>
#include <boost/asio.hpp>
#include "modbus_utils.hpp"
#include "serial_utils.hpp"

using namespace std;
using namespace boost::asio;


class ComportSettingsDialog : public wxDialog {
public:
    ComportSettingsDialog(wxWindow *parent);
    
private:
    std::vector<std::string> FetchAvailablePorts(); // ฟังก์ชันสำหรับดึงพอร์ตที่สามารถใช้ได้


    // boost::asio::serial_port InitSerial(const std::string& port);
    boost::asio::serial_port init_serial_port(boost::asio::io_service& io, const std::string& port_name);
    modbus_t* InitialModbus(const char* modbus_port);


    // ตัวแปรสำหรับเก็บพอร์ตที่เลือก
    std::string selectedBleAgentPort;
    std::string selectedModbusPort;
    std::string selectedPowerSupplyPort;

};

#endif // COMPORT_SETTINGS_HPP