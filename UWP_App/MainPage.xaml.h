//
// MainPage.xaml.h
// MainPage 类的声明。
//

#pragma once

#include "MainPage.g.h"

namespace UWP_App {
    /// <summary>
    /// 可用于自身或导航至 Frame 内部的空白页。
    /// </summary>
public
    ref class MainPage sealed {
    public:
        MainPage();

        void SetWindowSize();

    private:
        void Button_Click(Platform::Object ^ sender, Windows::UI::Xaml::RoutedEventArgs ^ e);
    };
} // namespace UWP_App
