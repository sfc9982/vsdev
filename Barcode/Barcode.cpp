#include <opencv2/core/utils/logger.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/opencv.hpp>

#include <iostream>
#include <random>

using namespace cv;
using namespace std;

constexpr int    side_len   = 45 * 4;
constexpr size_t RED_QR_CNT = 1000;

std::vector<std::string> message = {"Hi!",
                                    "I've got some QR Code for you.",
                                    "If you find something strange, they are not flag!",
                                    "real flag looks like flag{***}"};

std::string RandomANString(int length = 0) {
    static constexpr char                  alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZbcdefghijklmnopqrstuvwxyz1234567890";
    static std::random_device              rd;
    static std::mt19937                    gen(rd());
    static std::uniform_int_distribution<> dist(0, _countof(alphabet) - 2);
    static std::uniform_int_distribution<> dist_len(5, 15);
    std::string                            result;

    // Get random string length
    if (length == 0)
        length = dist_len(gen);

    for (int i = 0; i < length; i++)
        result.push_back(alphabet[dist(gen)]);

    return result;
}

int main(int argc, const char **argv) {
    setLogLevel(utils::logging::LOG_LEVEL_ERROR);
    QRCodeEncoder::Params params;
    params.version = 6; //41x41
    params.mode    = QRCodeEncoder::EncodeMode::MODE_AUTO;

    for (int i = 0; i < RED_QR_CNT; ++i) {
        if (i != RED_QR_CNT / 3 * 2) {
            message.push_back(RandomANString());
        } else {
            message.emplace_back("flag{A3E6DE86-0F24-4C1A-8A61-CF59B725E557}");
        }
    }

    int  n = message.size(), cnt = 0;
    auto qr = QRCodeEncoder::create(params);
    Mat  res(side_len, n * side_len, CV_8U);
    for (const auto &each : message) {
        Mat opt;
        qr->encode(each, opt);
        resize(opt, opt, Size(side_len, side_len), 0, 0, INTER_NEAREST_EXACT);
        opt.copyTo(res(Rect(cnt++ * side_len, 0, side_len, side_len)));
    }

    imwrite("flag.png", res);

    // Display the QR code
    imshow("QR Code", res);
    waitKey(0);

    return 0;
}