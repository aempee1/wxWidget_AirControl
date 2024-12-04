#ifndef MANUAL_CALIBRATE_HPP
#define MANUAL_CALIBRATE_HPP

#include <wx/wx.h> 
#include <wx/stattext.h>
#include <wx/textctrl.h>
#include <wx/valnum.h>

class ManualCalibrationDialog : public wxDialog {
public:
    ManualCalibrationDialog(wxWindow *parent);
private: 
    wxTextCtrl *setFlowInput;   // ตัวแปรสำหรับช่องข้อความ setFlow
    wxTextCtrl *refFlowInput;   // ช่องข้อความสำหรับ Ref. Flow
    wxTextCtrl *actFlowInput;   // ช่องข้อความสำหรับ Act. Flow
    wxTextCtrl *errorInput;     // ช่องข้อความสำหรับ Error

    int setpoint;             // ตัวแปรเก็บค่า setpoint

    void OnSetButtonClick(wxCommandEvent &event); // ฟังก์ชัน event handler
};


#endif // MANUAL_CALIBRATE_HPP
