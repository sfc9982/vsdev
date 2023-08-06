//
// MainPage.xaml.cpp
// MainPage 类的实现。
//

#include "pch.h"

#include "MainPage.xaml.h"

using namespace Base64_UWP;

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
using namespace Windows::Security::Cryptography;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::ViewManagement;

// https://go.microsoft.com/fwlink/?LinkId=402352&clcid=0x804 上介绍了“空白页”项模板

MainPage::MainPage() {
    InitializeComponent();
    SetWindowSize();
}

void MainPage::SetWindowSize() {
    ApplicationView::PreferredLaunchViewSize      = Size(445, 367);
    ApplicationView::PreferredLaunchWindowingMode = ApplicationViewWindowingMode::PreferredLaunchViewSize;
}

void Base64_UWP::MainPage::Base64_Encode(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e) {
    Platform::String ^ msg;
    MainPage::textBox->TextDocument->GetText(Windows::UI::Text::TextGetOptions::NoHidden, &msg);
    auto buffMsg   = CryptographicBuffer::ConvertStringToBinary(msg, BinaryStringEncoding::Utf8);
    auto base64Msg = CryptographicBuffer::EncodeToBase64String(buffMsg);
    MainPage::textBox->TextDocument->SetText(Windows::UI::Text::TextSetOptions::None, base64Msg);
}


void Base64_UWP::MainPage::Base64_Decode(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e) {
    Platform::String ^ msg;
    MainPage::textBox->TextDocument->GetText(Windows::UI::Text::TextGetOptions::NoHidden, &msg);
    try {
        auto buffMsg = CryptographicBuffer::DecodeFromBase64String(msg);
        auto oriMsg  = CryptographicBuffer::ConvertBinaryToString(BinaryStringEncoding::Utf8, buffMsg);
        MainPage::textBox->TextDocument->SetText(Windows::UI::Text::TextSetOptions::None, oriMsg);
    } catch (COMException ^ ex) {
    }
}
