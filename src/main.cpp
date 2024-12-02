#include <wx/wx.h>

// สร้างคลาสหลักสำหรับแอปพลิเคชัน
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// สร้างคลาสหลักสำหรับหน้าต่าง
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size);  // เพิ่มการประกาศ constructor

private:
    void SetupMainMenu();  // เพิ่มการประกาศฟังก์ชัน SetupMainMenu

};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit() {
    MyFrame *frame = new MyFrame("Flow Control", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}

MyFrame::MyFrame(const wxString &title, const wxPoint &pos, const wxSize &size)
    : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    SetupMainMenu();  // เรียกใช้ฟังก์ชัน SetupMainMenu

    this->SetMinSize(this->FromDIP(wxSize(200, 200)));
    this->SetSize(this->FromDIP(wxSize(800, 600)));
}

void MyFrame::SetupMainMenu() {
    wxMenuBar *menuBar = new wxMenuBar();
    

    wxMenu *settingMenu = new wxMenu();
    settingMenu->Append(wxID_ANY , "Comport Settings");
    settingMenu->Append(wxID_ANY , "File Settings");

    wxMenu *productionMenu = new wxMenu();
    productionMenu->Append(wxID_ANY,"Manual Flow System");
    productionMenu->Append(wxID_ANY , "Auto Flow System");

    wxMenu *R_D_testMenu = new wxMenu();

    wxMenu *helpMenu = new wxMenu();
    
    wxMenu *aboutMenu = new wxMenu() ;
    

    menuBar->Append(settingMenu, "&Setting");
    menuBar->Append(productionMenu, "&Production");
    menuBar->Append(R_D_testMenu,"&RnD Test");
    menuBar->Append(helpMenu, "&Help");
    menuBar->Append(aboutMenu, "&About");

    SetMenuBar(menuBar);
}


