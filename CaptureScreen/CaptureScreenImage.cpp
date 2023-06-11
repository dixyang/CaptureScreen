

#include "pch.h"
#include "CaptureScreenImage.h"

std::string GetCurrentTimeStr()
{
    SYSTEMTIME st;
    GetLocalTime(&st);
    std::ostringstream ossTime;
    ossTime << std::setw(4) << std::setfill('0') << st.wYear << "-"
            << std::setw(2) << std::setfill('0') << st.wMonth << "-"
            << std::setw(2) << std::setfill('0') << st.wDay << "_"
            << std::setw(2) << std::setfill('0') << st.wHour << ":"
            << std::setw(2) << std::setfill('0') << st.wMinute << ":"
            << std::setw(2) << std::setfill('0') << st.wSecond << "."
            << std::setw(3) << std::setfill('0') << st.wMilliseconds;
    return ossTime.str();
}

//#define SHOW_IMG

CaptureScreenImage::CaptureScreenImage()
{
    m_listener = nullptr;
    Init();
}

CaptureScreenImage::CaptureScreenImage(CaptureScreenListener * listener)
{
    m_listener = listener;
    Init();
}

CaptureScreenImage::~CaptureScreenImage()
{
    delete[] m_lpbitmap;
}

void CaptureScreenImage::StartCaptureScreen()
{
    // 刚打开程序(Wait)或停止(Stop)之后,再次点击开始(Start),重新创建视频文件并开始录制保存 
    if (m_captureStatus == CaptureStatus::Stop) {
        double fps = 25;
        int codec = cv::VideoWriter::fourcc('m', 'p', '4', 'v');
        std::string time = GetCurrentTimeStr();
        std::string sla = "-";
        std::string filePath = "./CaptureScreen_" + time.substr(0, 19).replace(13, 1, sla).replace(16, 1, sla) + ".mp4";
        // filePath = "CaptureScreen_2022-06-22_01:10:20.mp4"
        // 文件名不可以有 ':'

        m_videoWriter.open(filePath, codec, fps, cv::Size(m_width, m_height), true);
        if (!m_videoWriter.isOpened())
        {
            MessageBox(NULL, "VideoWriter is not opened ", "Error", MB_OK);
            return;
        }
        m_captureStatus = CaptureStatus::Start;
    }
    // 暂停(Pause)之后,再次点击开始(Start),不重新创建视频文件,继续录制保存 
    else if (m_captureStatus == CaptureStatus::Pause) {
        m_captureStatus = CaptureStatus::Start;
    }
    //Process();
}

void CaptureScreenImage::PauseCaptureScreen()
{
    m_captureStatus = CaptureStatus::Pause;
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
#ifdef SHOW_IMG
    cv::destroyAllWindows();
#endif // SHOW_IMG
}

void CaptureScreenImage::StopCaptureScreen()
{
    m_captureStatus = CaptureStatus::Stop;
    //std::this_thread::sleep_for(std::chrono::milliseconds(100));
    if (m_videoWriter.isOpened())
    {
        m_videoWriter.release();
    }
#ifdef SHOW_IMG
    cv::destroyAllWindows();
#endif // SHOW_IMG
}

void CaptureScreenImage::Init()
{
    m_captureStatus = CaptureStatus::Stop;

    m_width = GetSystemMetrics(SM_CXSCREEN);
    m_height = GetSystemMetrics(SM_CYSCREEN);
    //int width = 1920;
    //int height = 1080;

    m_hdcScreen = GetDC(NULL);
    m_hdcMemDC = CreateCompatibleDC(m_hdcScreen);
    m_hbmScreen = CreateCompatibleBitmap(m_hdcScreen, m_width, m_height);

    m_bi.bmiHeader.biSize = sizeof(m_bi.bmiHeader);
    m_bi.bmiHeader.biWidth = m_width;
    m_bi.bmiHeader.biHeight = m_height;
    m_bi.bmiHeader.biPlanes = 1;
    m_bi.bmiHeader.biBitCount = 24;
    m_bi.bmiHeader.biCompression = BI_RGB;
    m_bi.bmiHeader.biSizeImage = 0;
    m_bi.bmiHeader.biXPelsPerMeter = 0;
    m_bi.bmiHeader.biYPelsPerMeter = 0;
    m_bi.bmiHeader.biClrUsed = 0;
    m_bi.bmiHeader.biClrImportant = 0;
    SelectObject(m_hdcMemDC, m_hbmScreen);

    m_lineBytes = ((m_width * m_bi.bmiHeader.biBitCount + 31) / 32) * 4;
    m_bmpSize = m_lineBytes * m_height;
    m_lpbitmap = new char[m_bmpSize];

    std::thread([this]() {
        cv::Mat bmpMat(m_height, m_width, CV_8UC3);
        cv::Mat imgDesktopWindow;

        while (true)
        {
            if (m_captureStatus == CaptureStatus::Stop) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                continue;
            }
            else if (m_captureStatus == CaptureStatus::Pause) {
                std::this_thread::sleep_for(std::chrono::milliseconds(100));
                //cv::destroyAllWindows();
                continue;
            }
            else if (m_captureStatus == CaptureStatus::Start) {
                std::this_thread::sleep_for(std::chrono::milliseconds(40));
                if (BitBlt(m_hdcMemDC, 0, 0, m_width, m_height, m_hdcScreen, 0, 0, SRCCOPY))
                {
                    GetDIBits(m_hdcMemDC, m_hbmScreen, 0, m_height, m_lpbitmap, &m_bi, DIB_RGB_COLORS);
                    for (int i = 0; i < m_height; i++)
                    {
                        int srcIndex = (m_height - i - 1) * m_lineBytes;
                        int destIndex = i * m_width * 3;
                        memcpy(&bmpMat.data[destIndex], &m_lpbitmap[srcIndex], m_width * 3);
                    }
#ifdef SHOW_IMG
                    cv::resize(bmpMat, imgDesktopWindow, cv::Size(960, 540));
                    cv::putText(imgDesktopWindow, GetCurrentTimeStr(), cv::Point(50, 50),
                        cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 255, 255), 1, 8, 0);
                    cv::imshow("imgDesktopWindow", imgDesktopWindow);
                    cv::waitKey(10);

#endif // SHOW_IMG
                    cv::putText(bmpMat, GetCurrentTimeStr(), cv::Point(50, 50),
                        cv::FONT_HERSHEY_COMPLEX, 1, cv::Scalar(0, 0, 255), 1, 8, 0);

                    m_videoWriter.write(bmpMat);//videoWriter << bmpMat;
                }
            }

        }
    }).detach();
}

