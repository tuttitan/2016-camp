
// Create2CompetDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Create2Compet.h"
#include "Create2CompetDlg.h"
#include "afxdialogex.h"

#include "DrawMap.h"
#include "ConMQTT.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NO_CONF   // 確認を行わない


CDrawMap CCreate2CompetDlg::s_DrawMap;
CConMQTT CCreate2CompetDlg::s_ConMQTT;
CCreate2CompetDlg*  CCreate2CompetDlg::s_pOwn = NULL;
LARGE_INTEGER CCreate2CompetDlg::s_llPrevTime = { 0 };
bool CCreate2CompetDlg::s_bFirst = true;                    // 初回のコールバックか
DWORD CCreate2CompetDlg::s_dwBaseTime = 0;                  // コールバックの基準時間 [ms]
unsigned int CCreate2CompetDlg::s_uiCntCallback = 1;        // コールバックカウント

// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

// 実装
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


// CCreate2CompetDlg ダイアログ



CCreate2CompetDlg::CCreate2CompetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREATE2COMPET_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreate2CompetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_START, m_ctrlBtnStart);
	DDX_Control(pDX, IDC_BTN_STOP, m_ctrlBtnStop);
	DDX_Control(pDX, IDC_IPADDRESS, m_ctrlAddress);
}

BEGIN_MESSAGE_MAP(CCreate2CompetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_COMPET, &CCreate2CompetDlg::OnBnClickedBtnCompet)
	ON_BN_CLICKED(IDC_BTN_START, &CCreate2CompetDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CCreate2CompetDlg::OnBnClickedBtnStop)
END_MESSAGE_MAP()


// CCreate2CompetDlg メッセージ ハンドラー

BOOL CCreate2CompetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "バージョン情報..." メニューをシステム メニューに追加します。

	// IDM_ABOUTBOX は、システム コマンドの範囲内になければなりません。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// このダイアログのアイコンを設定します。アプリケーションのメイン ウィンドウがダイアログでない場合、
	//  Framework は、この設定を自動的に行います。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンの設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンの設定

	// TODO: 初期化をここに追加します。

	// オブジェクトの関連付け
	s_ConMQTT.vRelateObject(&s_DrawMap);
	s_DrawMap.vRelateObject(&s_ConMQTT);
	s_pOwn = this;

	BYTE byAddrFiled[4];
	vSetIPAddrFiled(byAddrFiled);
	m_ctrlAddress.SetAddress(192, 168, 137, 1);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}


// IPアドレスのバイトフィールドを取得する
void CCreate2CompetDlg::vSetIPAddrFiled(BYTE byAddr[])
{
	// 局所変数宣言
	char szBrokerIP[] = BRK_ADDRESS;
	char* pszFiled;
	int i = 0;
	char* pszDummy;

	
	pszFiled = strtok_s(szBrokerIP, ".", &pszDummy);
	byAddr[i] = atoi(pszFiled);
	++i;

	while (NULL != pszFiled) {
		pszFiled = strtok_s(NULL, "." , &pszDummy);
		if (NULL != pszFiled) {
			byAddr[i] = atoi(pszFiled);
			++i;
		}
	}
}

void CCreate2CompetDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// ダイアログに最小化ボタンを追加する場合、アイコンを描画するための
//  下のコードが必要です。ドキュメント/ビュー モデルを使う MFC アプリケーションの場合、
//  これは、Framework によって自動的に設定されます。

void CCreate2CompetDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 描画のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// クライアントの四角形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンの描画
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ユーザーが最小化したウィンドウをドラッグしているときに表示するカーソルを取得するために、
//  システムがこの関数を呼び出します。
HCURSOR CCreate2CompetDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

// 終了するとき
void CCreate2CompetDlg::OnCancel()
{
#ifdef NO_CONF
#else /* NO_CONF */
	// 局所変数宣言
	int iRet;

	iRet = AfxMessageBox(TEXT("本当に終了しますか？"), MB_YESNO | MB_ICONQUESTION);
	if (IDYES != iRet) {
		return;
	}
#endif /* NO_CONF */
	DestroyWindow();
}

void CCreate2CompetDlg::OnBnClickedBtnCompet()
{
	// 局所変数宣言
	m_bCompetMode = true;
	BYTE byAddrField[4];      // ブローカーのIPアドレス
	char szAddr[31];          // ブローカーのIPアドレス
	bool bRet;

	

	OutputDebugString(TEXT("競技会モードに入ります\n"));

	m_ctrlAddress.GetAddress(byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);
	sprintf_s(szAddr, "%d.%d.%d.%d", byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);

	bRet = s_ConMQTT.bConnect(szAddr);
	if (false == bRet) {
		AfxMessageBox(TEXT("MQTT通信を確立できません"), MB_OK | MB_ICONEXCLAMATION);
		return;
	}
	s_DrawMap.iStartDraw();
	m_ctrlBtnStart.EnableWindow(TRUE);

	// TODO: ここにコントロール通知ハンドラー コードを追加します。
#ifdef NO_CONF
	//this->OnCancel();
#endif /* NO_CONF */
}


// 競技開始
void CCreate2CompetDlg::OnBnClickedBtnStart()
{
	// 局所変数宣言
	bool bContinue = true;

	m_ctrlBtnStart.EnableWindow(FALSE);
	m_ctrlBtnStop.EnableWindow(TRUE);
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	//m_fpCallbackTimer = &CCreate2CompetDlg::voStartTimer;
	//SetTimer(101, (CALLBACK_INTERVAL / CALLBACK_RESOLUTION), voStartTimer);

	s_bFirst = true;
	s_DrawMap.vResetTime();
	vStartTimerWrapper(101);
	
}

// タイマーコールバック開始のラッパー
void CCreate2CompetDlg::vStartTimerWrapper(UINT_PTR nIDEvent)
{
	SetTimer(nIDEvent, (CALLBACK_INTERVAL / CALLBACK_RESOLUTION), voStartTimer);
}

// タイマーコールバック停止のラッパー
void CCreate2CompetDlg::vStopTimerWrapper(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
}

// 
// static method
void CALLBACK CCreate2CompetDlg::voStartTimer(
	HWND hWnd,      // handle of CWnd that called SetTimer
	UINT nMsg,      // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime    // system time
	)
{
	// 局所変数宣言
	bool bContinue = true;       // 継続フラグ
	DWORD dwDiff;                // 前回の呼出しとの差分時間 [ms]

	static DWORD s_dwPrevTime;
	

	if (true == s_bFirst) {
		s_dwBaseTime = dwTime;
		s_bFirst = false;
		s_uiCntCallback = 1;
	}


	dwDiff = dwTime - s_dwBaseTime;


	// 100[ms] 間隔の処理
	if (s_uiCntCallback == (dwDiff / CALLBACK_INTERVAL)) {
		++s_uiCntCallback;

		bContinue = s_DrawMap.bUpdateTimer();

		// 継続できない場合は中止する
		if (false == bContinue) {
			s_pOwn->OnBnClickedBtnStop();
		}
	}


	return;
}



void CCreate2CompetDlg::OnBnClickedBtnStop()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	vStopTimerWrapper(101);
	AfxMessageBox(TEXT("競技終了です"), MB_OK | MB_ICONINFORMATION);

	m_ctrlBtnStart.EnableWindow(TRUE);
	m_ctrlBtnStop.EnableWindow(FALSE);
}
