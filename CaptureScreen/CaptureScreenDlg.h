﻿
// CaptureScreenDlg.h: 头文件
//

#pragma once

#include "CaptureScreenImage.h"

// CCaptureScreenDlg 对话框
class CCaptureScreenDlg : public CDialogEx
{
// 构造
public:
	CCaptureScreenDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAPTURESCREEN_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnBnClickedButtonStart();
    afx_msg void OnBnClickedButtonPause();
    afx_msg void OnBnClickedButtonStop();

//    void ShowNote(std::string note);

private:
    CaptureScreenImage* m_aptureScreenImage;
    CButton m_buttonStart;
    CButton m_buttonPause;
    CButton m_buttonStop;

public:
    CStatic m_currentTime;
    CStatic m_note;
};
