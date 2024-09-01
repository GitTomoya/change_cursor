
#include <iostream>
#include <opencv2/opencv.hpp>
#include <windows.h>
#include "ColorCursor.h"

using namespace cv;

auto ConvertCVMatToBMP(cv::Mat frame) -> HBITMAP
{
    auto convertOpenCVBitDepthToBits = [](const int32_t value)
        {
            auto regular = 0u;

            switch (value)
            {
            case CV_8U:
            case CV_8S:
                regular = 8u;
                break;

            case CV_16U:
            case CV_16S:
                regular = 16u;
                break;

            case CV_32S:
            case CV_32F:
                regular = 32u;
                break;

            case CV_64F:
                regular = 64u;
                break;

            default:
                regular = 0u;
                break;
            }

            return regular;
        };

    auto imageSize = frame.size();

    if (imageSize.width && imageSize.height)
    {
        auto headerInfo = BITMAPINFOHEADER{};
        ZeroMemory(&headerInfo, sizeof(headerInfo));

        headerInfo.biSize = sizeof(headerInfo);
        headerInfo.biWidth = imageSize.width;
        headerInfo.biHeight = -(imageSize.height);
        headerInfo.biPlanes = 1;

        const auto bits = convertOpenCVBitDepthToBits(frame.depth());
        headerInfo.biBitCount = frame.channels() * bits;

        auto bitmapInfo = BITMAPINFO{};
        ZeroMemory(&bitmapInfo, sizeof(bitmapInfo));

        bitmapInfo.bmiHeader = headerInfo;
        bitmapInfo.bmiColors->rgbBlue = 0;
        bitmapInfo.bmiColors->rgbGreen = 0;
        bitmapInfo.bmiColors->rgbRed = 0;
        bitmapInfo.bmiColors->rgbReserved = 0;

        auto dc = GetDC(nullptr);
        auto bmp = CreateDIBitmap(dc,
            &headerInfo,
            CBM_INIT,
            frame.data,
            &bitmapInfo,
            DIB_RGB_COLORS);

        return bmp;
    }
    else
    {
        return nullptr;
    }
}

int main()
{
     //pngファイルを読み込み、Matへ変換
     cv::Mat hMat = cv::imread("test.png");

     //MatをHBITMAPへ変換
     auto bmp = ConvertCVMatToBMP(hMat);

     //HBITMAPをHCURSORへ変換
     HCURSOR hCursor = CColorCursor::CreateCursorFromBitmap(
            bmp, RGB(0, 0, 0), 0, 0);

    //標準のマウスカーソルを設定
    SetSystemCursor(hCursor,32512);
}