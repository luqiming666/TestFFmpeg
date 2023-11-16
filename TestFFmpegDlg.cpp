
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
	DDX_Control(pDX, IDC_STATIC_VIDEO_WND, mVideoWnd);
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
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CTestFFmpegDlg::OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, &CTestFFmpegDlg::OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CTestFFmpegDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_TEST_ANYTHING, &CTestFFmpegDlg::OnBnClickedButtonTestAnything)
	ON_BN_CLICKED(IDC_BUTTON_PROBE, &CTestFFmpegDlg::OnBnClickedButtonProbe)
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
	mTaskRunner.SetTaskObserver(this);
	mTaskRunner.LocateTools(UMiscUtils::GetRuntimeFilePath());
	
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

	//mTaskRunner.Stop();
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
	mTaskRunner.Run(strCmd);
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
	mTaskRunner.Run(strCmd);		
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
	mTaskRunner.Run(strCmd);
}

// ffplay -hide_banner -nostats -autoexit -i %s
// 视频窗口不显示标题栏：-noborder
// 修改视频窗口的标题：-window_title \"%s\"
// 进入全屏播放：-fs
// 控制视频窗口的宽和高：-x 400 -y 300
// 控制视频窗口的起始坐标：-left 100 -top 100
// 更多选项，参见ffplay.c 的 OptionDef
void CTestFFmpegDlg::OnBnClickedButtonPlay()
{
	if (mSourceFile.IsEmpty()) {
		AfxMessageBox(_T("请先指定源文件！"));
		return;
	}
	if (mTaskRunner.IsRunning())
	{
		AfxMessageBox(_T("请等待当前的媒体文件播放结束！"));
		return;
	}

	CString strCmd;
	strCmd.Format(_T(" -hide_banner -autoexit -i %s"), mSourceFile);
	mTaskRunner.Play(strCmd);

	//注：通过CreateProcess参数就可以控制不创建控制台窗口
	//Sleep(200);
	//HideFFplayConsoleWindow();
}

void CTestFFmpegDlg::HideFFplayConsoleWindow()
{
	HWND consoleWnd = ::FindWindow(_T("ConsoleWindowClass"), NULL);
	if (consoleWnd)
	{
		TCHAR wndTitle[MAX_PATH];
		::GetWindowText(consoleWnd, wndTitle, MAX_PATH);
		
		CString strTitle(wndTitle);
		if (strTitle.Find(_T("ffplay.exe")) != -1) {
			::ShowWindow(consoleWnd, SW_HIDE);
		}
	}
}
void pressKey(WORD key) {
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = key;
	input.ki.dwFlags = 0;
	SendInput(1, &input, sizeof(INPUT));
}

void releaseKey(WORD key) {
	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = key;
	input.ki.dwFlags = KEYEVENTF_KEYUP;
	SendInput(1, &input, sizeof(INPUT));
}

void CTestFFmpegDlg::OnBnClickedButtonPause()
{
	HWND videoWindowHandle = ::FindWindow(_T("SDL_app"), NULL);
	if (videoWindowHandle)
	{
		//::PostMessage(videoWindowHandle, WM_KEYDOWN, VK_SPACE, 0);
		//Sleep(100);
		//::PostMessage(videoWindowHandle, WM_KEYUP, VK_SPACE, 0);

		// 将 ffplay.exe 窗口设置为活动窗口
		::SetForegroundWindow(videoWindowHandle);

		// 模拟按下和释放空格键，实现暂停和继续播放
		pressKey(VK_SPACE);
		Sleep(1000);  // 暂停1秒
		releaseKey(VK_SPACE);
	}
}


void CTestFFmpegDlg::OnBnClickedButtonStop()
{
	HWND videoWindowHandle = ::FindWindow(_T("SDL_app"), NULL);
	if (videoWindowHandle)
	{
		::PostMessage(videoWindowHandle, WM_CLOSE, 0, 0);
	}
}


void CTestFFmpegDlg::OnBnClickedButtonTestAnything()
{
	HWND videoWindowHandle = ::FindWindow(_T("SDL_app"), NULL);
	if (videoWindowHandle)
	{
		// 修改窗口样式
		LONG_PTR style = ::GetWindowLongPtr(videoWindowHandle, GWL_STYLE);
		style = style & ~WS_CAPTION; // 去掉标题栏
		style = style | WS_CHILD | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_EX_TOPMOST;

		::SetWindowLongPtr(videoWindowHandle, GWL_STYLE, style);

		// 使样式改变生效
		::SetWindowPos(videoWindowHandle, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);

		//::SetParent(videoWindowHandle, mVideoWnd.GetSafeHwnd());
	}
}


void CTestFFmpegDlg::OnBnClickedButtonProbe()
{
	if (mSourceFile.IsEmpty()) {
		AfxMessageBox(_T("请先指定源文件！"));
		return;
	}

	std::string result = mTaskRunner.Probe(mSourceFile);
	mVideoWnd.SetWindowText(CString(result.c_str()));
}
