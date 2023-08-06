//
// MainPage.xaml.h
// MainPage 类的声明。
//

#pragma once

#include "MainPage.g.h"

namespace Base64_UWP {
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
public
    ref class MainPage sealed {
    public:
        MainPage();
        void SetWindowSize();

    private:
        void Base64_Encode(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e);
        void Base64_Decode(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e);
    };
} // namespace Base64_UWP
