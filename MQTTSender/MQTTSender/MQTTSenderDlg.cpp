
// MQTTSenderDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "MQTTSender.h"
#include "MQTTSenderDlg.h"
#include "afxdialogex.h"


#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CMQTTSenderDlg ダイアログ



CMQTTSenderDlg::CMQTTSenderDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MQTTSENDER_DIALOG, pParent)
	, m_iRadioMsgSW(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMQTTSenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS_BLK, m_ctrlAddr);
	DDX_Radio(pDX, IDC_RADIO_ENTER, m_iRadioMsgSW);
	DDX_Control(pDX, IDC_MFCBTN_CONN, m_ctrlBtnConn);
}

BEGIN_MESSAGE_MAP(CMQTTSenderDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_A, &CMQTTSenderDlg::OnBnClickedBtnA)
	ON_BN_CLICKED(IDC_BTN_B, &CMQTTSenderDlg::OnBnClickedBtnB)
	ON_BN_CLICKED(IDC_BTN_C, &CMQTTSenderDlg::OnBnClickedBtnC)
	ON_BN_CLICKED(IDC_BTN_D, &CMQTTSenderDlg::OnBnClickedBtnD)
	ON_BN_CLICKED(IDC_BTN_E, &CMQTTSenderDlg::OnBnClickedBtnE)
	ON_BN_CLICKED(IDC_BTN_F, &CMQTTSenderDlg::OnBnClickedBtnF)
	ON_BN_CLICKED(IDC_MFCBTN_CONN, &CMQTTSenderDlg::OnBnClickedMfcbtnConn)
END_MESSAGE_MAP()


// CMQTTSenderDlg メッセージ ハンドラー

BOOL CMQTTSenderDlg::OnInitDialog()
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
	// 局所変数宣言
	unsigned int uiAddrFiled[4];  // IPアドレスのビットフィールド

	// CMFCButtonのsetfacecolorを有効にする
	CMFCButton::EnableWindowsTheming(FALSE);

	// ブローカーIPアドレスの設定
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddr.SetAddress(uiAddrFiled[0], uiAddrFiled[1], uiAddrFiled[2], uiAddrFiled[3]);

	// ラジオボタンの状態を変更する（0: Enter, 1: Leave）
	m_iRadioMsgSW = 0;
	UpdateData(FALSE);

	// 接続ボタンの状態を変更
	m_iStatConn = STAT_DISC;
	m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_CONN], FALSE);
	m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_CONN]);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CMQTTSenderDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMQTTSenderDlg::OnPaint()
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
HCURSOR CMQTTSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMQTTSenderDlg::OnBnClickedBtnA()
{
	// 局所変数宣言
	const char ccID = 'a';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

							 // コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnB()
{
	// 局所変数宣言
	const char ccID = 'b';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

	// コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnC()
{
	// 局所変数宣言
	const char ccID = 'c';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

							 // コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnD()
{
	// 局所変数宣言
	const char ccID = 'd';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

							 // コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnE()
{
	// 局所変数宣言
	const char ccID = 'e';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

							 // コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnF()
{
	// 局所変数宣言
	const char ccID = 'f';   // コーナーの識別
	int        iDir = 1;     // 検出方向 [1: Enter, 0: Leave]

							 // コントロール変数の値を更新する
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// メッセージの発行
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedMfcbtnConn()
{
	// 局所変数宣言
	bool bRet = false;       // 関数呼出しの戻り値
	BYTE byAddrField[4];
	char szAddr[31];

	// 接続していない状態
	if (STAT_DISC == m_iStatConn) {

		// ブローカーIPアドレスの取得
		m_ctrlAddr.GetAddress(byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);
		sprintf_s(szAddr, "%d.%d.%d.%d", byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);

		// MQTT通信の開始
		bRet = m_ConMQTT.bConnect(szAddr);
		if (false == bRet) {
			AfxMessageBox(TEXT("MQTT通信を開始できません"), MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		
		// ボタンの変更
		m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_DISC], FALSE);
		m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_DISC]);

		// 内部状態の変更
		m_iStatConn = STAT_CONN;
	}
	// 接続している状態
	else {

		// MQTT通信の切断
		bRet = m_ConMQTT.bDisconnect();
		if (false == bRet) {
			AfxMessageBox(TEXT("MQTT通信を終了できません"), MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		// ボタンの変更
		m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_CONN], FALSE);
		m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_CONN]);

		// 内部状態の変更
		m_iStatConn = STAT_DISC;
	}

	UpdateData(FALSE);

	return;
}
