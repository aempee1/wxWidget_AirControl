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
    ComportSettingsDialog(wxWindow *parent);
    
private:
    std::vector<std::string> FetchAvailablePorts(); // ฟังก์ชันสำหรับดึงพอร์ตที่สามารถใช้ได้

    void InitSerial(const std::string& port);

    // ตัวแปรสำหรับเก็บพอร์ตที่เลือก
    std::string selectedBleAgentPort;
    std::string selectedModbusPort;
    std::string selectedPowerSupplyPort;

};

#endif // COMPORT_SETTINGS_HPP