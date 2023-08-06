//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"

#include "MainPage.xaml.h"

using namespace UWP_App;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Foundation::Collections;
using namespace Windows::UI::Xaml;
using namespace Windows::UI::Xaml::Controls;
using namespace Windows::UI::Xaml::Controls::Primitives;
using namespace Windows::UI::Xaml::Data;
using namespace Windows::UI::Xaml::Input;
using namespace Windows::UI::Xaml::Media;
using namespace Windows::UI::Xaml::Navigation;
using namespace Windows::UI::ViewManagement;


// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

MainPage::MainPage() {
    SetWindowSize();
    InitializeComponent();
}

void MainPage::SetWindowSize() {
    ApplicationView::PreferredLaunchViewSize      = Size(600, 320);
    ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;
}


void UWP_App::MainPage::Button_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e) {
    if (nameInput->Text->IsEmpty()) {
        greetingOutput->Text = "Hello!";
    } else {
        greetingOutput->Text = "Hello, " + nameInput->Text + "!";
    }
}
