
// TestFFmpegDlg.h: 头文件
//

#pragma once

#include "FFmpegTask.h"

// CTestFFmpegDlg 对话框
class CTestFFmpegDlg : public CDialogEx, public ITaskCallback
{
// 构造
public:
	CTestFFmpegDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTFFMPEG_DIALOG };
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
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnBnClickedButtonBrowse2();

	virtual void OnTaskCompleted();

private:
	CString mSourceFile;
	CString mSourceFile2;
	FFmpegTask mTaskRunner;

public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonTranscodeWave();
	afx_msg void OnBnClickedButtonAudioChannelMerge();
};
