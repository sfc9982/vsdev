#include "opencv2/objdetect.hpp"
#include "opencv2/wechat_qrcode.hpp"
#include "test_precomp.hpp"
#include <iostream>
#include <string>
#include <vector>

#include <opencv2/>

using namespace std;
using namespace cv;

int main(int argc, char **argv) {
    if (argc < 2)
        return 1;
    string image_path(argv[1]);
    Mat    src = imread(image_path, IMREAD_GRAYSCALE);
    if (!src.empty()) {
        wechat_qrcode::WeChatQRCode detector;
        vector<string>              outs = detector.detectAndDecode(src);
        // print outs
        for (size_t i = 0; i < outs.size(); i++) {
            cout << outs[i] << endl;
        }
    }
    return 0;
}