#pragma once
#pragma warning(disable:4996)

#include <Windows.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <thread>
#include <chrono>

extern std::string GetCurrentTimeStr();

class CaptureScreenListener
{
public:
    CaptureScreenListener() {
        index = 0;
    }
    void currentMat(cv::Mat img) {
        std::cout << "listener index:" << index++ << std::endl;
        cv::imshow("imgDesktopWindow", img);
        cv::waitKey(10);
    };
private:
    int index;
};

// ״̬��
enum CaptureStatus {
    Start,      // ����ѭ������ʼ¼�� ������¼�Ƶ�״̬ 
    Pause,      // ����ѭ������¼�� 
    Stop        // ����ѭ���������ļ� (�մ򿪳����¼��һ�κ�) 
};


class CaptureScreenImage
{
public:
    CaptureScreenImage();
    CaptureScreenImage(CaptureScreenListener* listener); // deprecate
    ~CaptureScreenImage();
    void StartCaptureScreen();
    void PauseCaptureScreen();
    void StopCaptureScreen();

private:
    void Init();

private:
    CaptureScreenListener* m_listener;
    CaptureStatus m_captureStatus;

    int m_width;
    int m_height;
    HDC m_hdcScreen;
    HDC m_hdcMemDC;
    HBITMAP m_hbmScreen;
    BITMAPINFO m_bi;
    int m_lineBytes;
    int m_bmpSize;
    char* m_lpbitmap;

    cv::VideoWriter m_videoWriter;
};

