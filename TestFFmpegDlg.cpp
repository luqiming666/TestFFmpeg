
// TestFFmpegDlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "TestFFmpeg.h"
#include "TestFFmpegDlg.h"
#include "afxdialogex.h"
#include "UMiscUtils.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CTestFFmpegDlg 对话框



CTestFFmpegDlg::CTestFFmpegDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TESTFFMPEG_DIALOG, pParent)
	, mSourceFile(_T(""))
	, mSourceFile2(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTestFFmpegDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SRC_FILE, mSourceFile);
	DDX_Text(pDX, IDC_EDIT_SRC_FILE2, mSourceFile2);
}

BEGIN_MESSAGE_MAP(CTestFFmpegDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, &CTestFFmpegDlg::OnBnClickedButtonBrowse)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_TRANSCODE_WAVE, &CTestFFmpegDlg::OnBnClickedButtonTranscodeWave)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE2, &CTestFFmpegDlg::OnBnClickedButtonBrowse2)
	ON_BN_CLICKED(IDC_BUTTON_AUDIO_CHANNEL_MERGE, &CTestFFmpegDlg::OnBnClickedButtonAudioChannelMerge)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_AUDIO, &CTestFFmpegDlg::OnBnClickedButtonReplaceAudio)
END_MESSAGE_MAP()


// CTestFFmpegDlg 消息处理程序

BOOL CTestFFmpegDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString ffmpegFile = UMiscUtils::GetRuntimeFilePath(_T("ffmpeg.exe"));
	mTaskRunner.SetFFmpegPath(ffmpegFile);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CTestFFmpegDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CTestFFmpegDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CTestFFmpegDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CTestFFmpegDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
}

void CTestFFmpegDlg::OnTaskCompleted()
{
	AfxMessageBox(_T("FFmpeg task done!"));
}

void CTestFFmpegDlg::OnBnClickedButtonBrowse()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, 
		_T("Video Files (*.mp4;*.mpg;*.avi;*.wmv;*.mov)|*.mp4;*.mpg;*.avi;*.wmv;*.mov| \
		Audio Files (*.mp3;*.ogg;*.wav;*.wma)|*.mp3;*.ogg;*.wav;*.wma| \
		All Files (*.*)|*.*||"),
		NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		mSourceFile = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

void CTestFFmpegDlg::OnBnClickedButtonBrowse2()
{
	CFileDialog fileDlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		_T("Video Files (*.mp4;*.mpg;*.avi;*.wmv;*.mov)|*.mp4;*.mpg;*.avi;*.wmv;*.mov| \
		Audio Files (*.mp3;*.ogg;*.wav;*.wma)|*.mp3;*.ogg;*.wav;*.wma| \
		All Files (*.*)|*.*||"),
		NULL);
	if (fileDlg.DoModal() == IDOK)
	{
		mSourceFile2 = fileDlg.GetPathName();
		UpdateData(FALSE);
	}
}

// ffmpeg -i %s -vn -y %s
void CTestFFmpegDlg::OnBnClickedButtonTranscodeWave()
{
	if (mSourceFile.IsEmpty()) {
		AfxMessageBox(_T("请先指定源文件！"));
		return;
	}
	
	//CString strDestFile = UMiscUtils::GetRuntimeFilePath(_T("test.wav"));

	CString strCmd;
	strCmd.Format(_T(" -i %s -vn -y %s"), mSourceFile, _T("test.wav")); // 注意：-i之前须有一个空格

	mTaskRunner.Run(strCmd, this);
}

// ffmpeg -i %s -i %s -filter_complex "[0:a]apad=pad_len=0[a1];[1:a]apad[a2];[a1][a2]amerge=inputs=2,pan=stereo|c0=c0|c1=c2[aout]" -map [aout] -y %s
void CTestFFmpegDlg::OnBnClickedButtonAudioChannelMerge()
{
	if (mSourceFile.IsEmpty() || mSourceFile2.IsEmpty()) {
		AfxMessageBox(_T("请先指定源文件！"));
		return;
	}

	CString strCmd;
	strCmd.Format(_T(" -i %s -i %s -filter_complex \"[0:a]apad=pad_len=0[a1];[1:a]apad[a2];[a1][a2]amerge=inputs=2,pan=stereo|c0=c0|c1=c2[aout]\" -map [aout] -y %s"), 
		mSourceFile, mSourceFile2, _T("audio_merge.ogg"));

	mTaskRunner.Run(strCmd, this);		
}

// ffmpeg -i %s -i %s -c:v copy -c:a aac -map 0:v -map 1:a -y %s
// ffmpeg -i %s -i %s -c:v libx265 -c:a aac -map 0:v -map 1:a -y %s
void CTestFFmpegDlg::OnBnClickedButtonReplaceAudio()
{
	if (mSourceFile.IsEmpty() || mSourceFile2.IsEmpty()) {
		AfxMessageBox(_T("请先指定源文件！"));
		return;
	}

	CString strCmd;
	strCmd.Format(_T(" -i %s -i %s -c:v libx265 -c:a aac -map 0:v -map 1:a -y %s"),
		mSourceFile, mSourceFile2, _T("audio_replaced.mp4"));

	mTaskRunner.Run(strCmd, this);
}
