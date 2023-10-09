#include <wx/wx.h>

#include <Windows.h>

class MyFrame : public wxFrame {
public:
    MyFrame()
        : wxFrame(nullptr,
                  wxID_ANY,
                  "",
                  wxDefaultPosition,
                  wxSize(160, 112),
                  wxPOPUP_WINDOW | wxNO_BORDER | wxFRAME_NO_TASKBAR | wxFRAME_TOOL_WINDOW | wxSTAY_ON_TOP) {
        Hide();

        m_sizer = new wxBoxSizer(wxVERTICAL);

        auto *panel1      = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        auto  panel_sizer = new wxBoxSizer(wxVERTICAL);

        panel1->SetSizer(panel_sizer);

        m_indicator = new wxStaticText(panel1,
                                       wxID_ANY,
                                       "AA",
                                       wxDefaultPosition,
                                       wxDefaultSize,
                                       wxALIGN_CENTER);
        m_indicator->SetFont(m_indicator->GetFont().Scale(3.f));
        panel_sizer->Add(m_indicator, wxSizerFlags().CenterHorizontal().Border(wxALL, 10));
        m_sizer->Add(panel1, wxSizerFlags(1).Expand());

        auto *panel2       = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxTAB_TRAVERSAL);
        auto *panel_sizer2 = new wxBoxSizer(wxVERTICAL);

        panel2->SetSizer(panel_sizer2);

        m_hintText = new wxStaticText(panel2,
                                      wxID_ANY,
                                      "Caps Lock Off",
                                      wxDefaultPosition,
                                      wxDefaultSize,
                                      wxALIGN_CENTER);

        m_hintText->SetFont(m_hintText->GetFont().Scale(1.2f));
        panel_sizer2->Add(m_hintText, wxSizerFlags().CenterHorizontal());
        m_sizer->Add(panel2, wxSizerFlags().Border(wxRIGHT | wxLEFT, 10).Expand());

        wxWindowBase::SetBackgroundColour(wxColour(244, 244, 244));
        SetSizerAndFit(m_sizer);

        const wxSize screenSize = wxGetDisplaySize();
        const int    y          = floor(screenSize.GetHeight() * 7 / 8.0);
        CenterOnScreen(wxHORIZONTAL);
        Move(wxPoint(GetPosition().x, y - GetSize().GetHeight() / 2));

        m_timer.Start(100);

        m_timer.Bind(wxEVT_TIMER, &MyFrame::OnTimer, this);

        m_nap.Bind(wxEVT_TIMER, &MyFrame::TakeNap, this);
    }

    void OnTimer(wxTimerEvent &event) {
        if ((GetKeyState(VK_CAPITAL) & 1) != m_capslockOn) {
            if (GetKeyState(VK_CAPITAL) & 1) {
                m_capslockOn = true;
                m_hintText->SetLabel("Caps Lock On");
                m_indicator->SetLabel("AA");
            } else {
                m_capslockOn = false;
                m_hintText->SetLabel("Caps Lock Off");
                m_indicator->SetLabel("aa");
            }
            SetSizerAndFit(m_sizer);
            Show();
            m_nap.Start(1000, wxTIMER_ONE_SHOT);

        } else if ((GetKeyState(VK_NUMLOCK) & 1) != m_numlockOn) {
            if (GetKeyState(VK_NUMLOCK) & 1) {
                m_numlockOn = true;
                m_hintText->SetLabel("Num Lock On");
                m_indicator->SetLabel("Num");
            } else {
                m_numlockOn = false;
                m_hintText->SetLabel("Num Lock Off");
                m_indicator->SetLabel("Fn");
            }
            SetSizerAndFit(m_sizer);
            Show();
            m_nap.Start(1000, wxTIMER_ONE_SHOT);
        }
    }

    void TakeNap(wxTimerEvent &event) {
        Hide();
    }


private:
    wxStaticText *m_hintText  = nullptr;
    wxStaticText *m_indicator = nullptr;
    wxBoxSizer   *m_sizer     = nullptr;
    wxTimer       m_timer;
    wxTimer       m_nap;
    bool          m_capslockOn = false;
    bool          m_numlockOn  = true;
};


class MyApp : public wxApp {
public:
    bool OnInit() override {
        SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_SYSTEM_AWARE);

        auto *frame = new MyFrame();
        frame->Hide();
        return true;
    }
};

wxIMPLEMENT_APP(MyApp);