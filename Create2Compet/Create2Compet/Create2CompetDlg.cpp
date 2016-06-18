﻿
// Create2CompetDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Create2Compet.h"
#include "Create2CompetDlg.h"
#include "afxdialogex.h"

#include "DrawMap.h"
#include "ConMQTT.h"
#include "ConSocket.h"
#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define NO_CONF   // 確認を行わない


CDrawMap CCreate2CompetDlg::s_DrawMap;
CConMQTT CCreate2CompetDlg::s_ConMQTT;
CConSocket CCreate2CompetDlg::s_ConSocket;
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
	DDX_Control(pDX, IDC_IPADDRESS, m_ctrlAddressMQTT);
	DDX_Control(pDX, IDC_BTN_DP, m_ctrlBtnDp);
	DDX_Control(pDX, IDC_IPADDRESS_SOCKET, m_ctrlAddressSocket);
	DDX_Control(pDX, IDC_EDIT_PT_INIT, m_ctrlEditPtInit);
	DDX_Control(pDX, IDC_EDIT_PT_SEED, m_ctrlEditPtSeed);
	DDX_Control(pDX, IDC_CHK_LOCK, m_ctrlBtnChkLock);
}

BEGIN_MESSAGE_MAP(CCreate2CompetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_COMPET, &CCreate2CompetDlg::OnBnClickedBtnCompet)
	ON_BN_CLICKED(IDC_BTN_START, &CCreate2CompetDlg::OnBnClickedBtnStart)
	ON_BN_CLICKED(IDC_BTN_STOP, &CCreate2CompetDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_DP, &CCreate2CompetDlg::OnBnClickedBtnDp)
	ON_BN_CLICKED(IDC_CHK_LOCK, &CCreate2CompetDlg::OnBnClickedChkLock)
	ON_BN_CLICKED(IDC_BUTTON2, &CCreate2CompetDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCreate2CompetDlg::OnBnClickedButton3)
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

	// コンソールを作る
	::AllocConsole();
	freopen_s(&m_fpConsole, "CON", "w", stdout);

	// オブジェクトの関連付け
	s_ConMQTT.vRelateObject(&s_DrawMap);
	s_ConMQTT.vRelateObject(&s_ConSocket);
	s_DrawMap.vRelateObject(&s_ConMQTT);
	s_ConSocket.vRelateObject(&s_DrawMap);
	s_pOwn = this;

	unsigned int uiAddrFiled[4];  // IPアドレスのビットフィールド
	wchar_t wszEdit[31];          // エディットボックスの文字

	// MQTTブローカーアドレスのビットフィールドを取得
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressMQTT.SetAddress(uiAddrFiled[0], uiAddrFiled[1], uiAddrFiled[2], uiAddrFiled[3]);

	// Socketサーバーアドレスのビットフィールドを取得
	vSplitString(SERV_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressSocket.SetAddress(uiAddrFiled[0], uiAddrFiled[1], uiAddrFiled[2], uiAddrFiled[3]);

	
	wsprintf(wszEdit, L"%S", POINT_INIT);
	m_ctrlEditPtInit.SetWindowText(wszEdit);

	wsprintf(wszEdit, L"%d", POINT_SEED);
	m_ctrlEditPtSeed.SetWindowText(wszEdit);

	// TODO: ボタンの見た目を変更

	// 編集ロックをかける
	m_ctrlBtnChkLock.SetCheck(BST_CHECKED);
	vUpdateLock(true);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

#if 0
// IPアドレスのバイトフィールドを取得する
void CCreate2CompetDlg::vSetIPAddrFiled(BYTE byAddr[], const char* cszAddr)
{
	// 局所変数宣言
	char *szAddr = NULL;
	char* pszFiled;
	int i = 0;
	char* pszDummy;

	szAddr = new char[strlen(cszAddr) + 1];

	if (NULL == szAddr) {
		return;
	}

	strcpy_s(szAddr, strlen(cszAddr) + 1, cszAddr);

	
	pszFiled = strtok_s(szAddr, ".", &pszDummy);
	byAddr[i] = atoi(pszFiled);
	++i;

	while (NULL != pszFiled) {
		pszFiled = strtok_s(NULL, "." , &pszDummy);
		if (NULL != pszFiled) {
			byAddr[i] = atoi(pszFiled);
			++i;
		}
	}

	if (NULL != szAddr) {
		delete[] szAddr;
	}

	return;
}
#endif /* 0 */

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

	// コンソールを閉じる
	fclose(m_fpConsole);
	::FreeConsole();
	DestroyWindow();
}

void CCreate2CompetDlg::OnBnClickedBtnCompet()
{
	// 局所変数宣言
	BYTE    byAddrField[4];      // IPアドレス（MQTT, Socket）
	char    szAddr[31];          // IPアドレス（MQTT, Socket）
	bool    bRet;                // 関数呼出し戻り値
	wchar_t wszPoints[31];       // 点数表初期値
	wchar_t wszSeed[31];         // 点数表Seed値
	char    szMsg[31];           // 送信メッセージ

	
	m_bCompetMode = true;
	OutputDebugString(TEXT("競技会モードに入ります\n"));

	// Socket通信の開始
	m_ctrlAddressSocket.GetAddress(byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);
	sprintf_s(szAddr, "%d.%d.%d.%d", byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);

	bRet = s_ConSocket.bConnect(szAddr, SOCKET_PORT);

	if (false == bRet) {
		AfxMessageBox(TEXT("Socket通信を確立できません"), MB_OK | MB_ICONEXCLAMATION);
		//return;
	}

	// 点数表サーバーへの初期化指示
	s_ConSocket.bSetSendMessage(MSG_INIT);
	Sleep(100);

	m_ctrlEditPtInit.GetWindowText(wszPoints, sizeof(wszPoints));
	m_ctrlEditPtSeed.GetWindowText(wszSeed, sizeof(wszSeed));

	sprintf_s(szMsg, "%ws:%ws", wszPoints, wszSeed);
	s_ConSocket.bSetSendMessage(szMsg);


	// MQTT通信の開始
	m_ctrlAddressMQTT.GetAddress(byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);
	sprintf_s(szAddr, "%d.%d.%d.%d", byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);

	bRet = s_ConMQTT.bConnect(szAddr);
	if (false == bRet) {
		AfxMessageBox(TEXT("MQTT通信を確立できません"), MB_OK | MB_ICONEXCLAMATION);
		//return;
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

	// ボタンのコントロールを変更する
	m_ctrlBtnStart.EnableWindow(FALSE);
	m_ctrlBtnStop.EnableWindow(TRUE);

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
	if (s_uiCntCallback <= (dwDiff / CALLBACK_INTERVAL)) {
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


void CCreate2CompetDlg::OnBnClickedBtnDp()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}


void CCreate2CompetDlg::OnBnClickedChkLock()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
	// 局所変数宣言
	int iStatBtn;
	iStatBtn = m_ctrlBtnChkLock.GetCheck();
	if (BST_CHECKED == iStatBtn) {
		vUpdateLock(true);
	}
	else {
		vUpdateLock(false);
	}
	
}

void CCreate2CompetDlg::vUpdateLock(bool bCheck)
{
	if (true == bCheck) {
		m_ctrlAddressMQTT.EnableWindow(FALSE);
		m_ctrlAddressSocket.EnableWindow(FALSE);
		m_ctrlEditPtInit.EnableWindow(FALSE);
		m_ctrlEditPtSeed.EnableWindow(FALSE);

		//AfxMessageBox(TEXT("true"), MB_OK | MB_ICONINFORMATION);
	}
	else {
		m_ctrlAddressMQTT.EnableWindow(TRUE);
		m_ctrlAddressSocket.EnableWindow(TRUE);
		m_ctrlEditPtInit.EnableWindow(TRUE);
		m_ctrlEditPtSeed.EnableWindow(TRUE);

		//AfxMessageBox(TEXT("false"), MB_OK | MB_ICONINFORMATION);
	}


	

	return;
}


// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton2()
{
	s_ConSocket.bSetSendMessage(MSG_UPDATE);
}

// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton3()
{
	s_ConSocket.bSetSendMessage(MSG_QUIT);
}
