#include <wx/wx.h>
#include "mainwindow_properties.hpp"

using namespace std;
using namespace boost::asio;

// สร้างคลาสหลักสำหรับแอปพลิเคชัน
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};


wxIMPLEMENT_APP(MyApp);

//----------------------------------------------------------------

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Flow System Software v. 1.0.0", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}

