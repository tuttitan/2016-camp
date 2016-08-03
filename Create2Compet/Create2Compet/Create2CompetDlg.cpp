
// Create2CompetDlg.cpp : 実装ファイル
//
/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "Create2Compet.h"
#include "Create2CompetDlg.h"

#include "afxdialogex.h"

#include "DrawMap.h"
#include "ConMQTT.h"
#include "ConSocket.h"
#include "utility.h"

//#include "Scorer.h"


/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#define CMD_PYTHON       (TEXT("python"))
#define CMD_PRPT         (TEXT("cmd.exe /c"))
#define LOCAL_BROKER       (TEXT("\\Broker\\mosquitto.exe -v"))
#define LOCAL_SCORER       (TEXT("\\Script\\tcp_server.py"))

//#define NO_CONF   // 確認を行わない

/*****************************************************************************/
/* 静的メンバ変数宣言                                                        */
/*****************************************************************************/
//LARGE_INTEGER CCreate2CompetDlg::s_llPrevTime = { 0 };
bool CCreate2CompetDlg::s_bFirst = true;                    // 初回のコールバックか
DWORD CCreate2CompetDlg::s_dwBaseTime = 0;                  // コールバックの基準時間 [ms]
unsigned int CCreate2CompetDlg::s_uiCntCallback = 1;        // コールバックカウント


/*****************************************************************************/
/* 周辺クラス定義                                                            */
/*****************************************************************************/
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


/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/
// CCreate2CompetDlg ダイアログ
CCreate2CompetDlg::CCreate2CompetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CREATE2COMPET_DIALOG, pParent)
	, m_iCmbServ(0)
	, m_iRadioMode(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CCreate2CompetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_START, m_ctrlBtnStart);

	//DDX_Control(pDX, IDC_IPADDRESS_MQTT, m_ctrlAddressMQTT);
	DDX_Control(pDX, IDC_BTN_DP, m_ctrlBtnDp);
	//DDX_Control(pDX, IDC_IPADDRESS_SOCKET, m_ctrlAddressSocket);
	DDX_Control(pDX, IDC_EDIT_PT_INIT, m_ctrlEditPtInit);
	DDX_Control(pDX, IDC_EDIT_PT_SEED, m_ctrlEditPtSeed);
	DDX_Control(pDX, IDC_CHK_LOCK, m_ctrlBtnChkLock);
	DDX_Control(pDX, IDC_CMB_SERV, m_ctrlCmbServ);
	DDX_CBIndex(pDX, IDC_CMB_SERV, m_iCmbServ);
	DDX_Radio(pDX, IDC_RADIO_MODE, m_iRadioMode);
	DDX_Control(pDX, IDC_BTN_COMPET, m_ctrlBtnCompet);
	DDX_Control(pDX, IDC_RADIO_MODE, m_ctrlRadioMode);
	DDX_Control(pDX, IDC_RADIO_MODE2, m_ctrlRadioMode2);
	DDX_Control(pDX, IDC_EDIT_LOG, m_ctrlEditLog);
}

// メッセージとの関連付け
BEGIN_MESSAGE_MAP(CCreate2CompetDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_COMPET, &CCreate2CompetDlg::OnBnClickedBtnCompet)
	ON_BN_CLICKED(IDC_BTN_START, &CCreate2CompetDlg::OnBnClickedBtnStart)
	//ON_BN_CLICKED(IDC_BTN_STOP, &CCreate2CompetDlg::OnBnClickedBtnStop)
	ON_BN_CLICKED(IDC_BTN_DP, &CCreate2CompetDlg::OnBnClickedBtnDp)
	ON_BN_CLICKED(IDC_CHK_LOCK, &CCreate2CompetDlg::OnBnClickedChkLock)
	ON_BN_CLICKED(IDC_BUTTON2, &CCreate2CompetDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CCreate2CompetDlg::OnBnClickedButton3)
	ON_MESSAGE(WM_USER_PROC, &CCreate2CompetDlg::OnUserMessage)
	ON_BN_CLICKED(IDC_BUTTON4, &CCreate2CompetDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CCreate2CompetDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_RADIO_MODE, &CCreate2CompetDlg::OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_MODE2, &CCreate2CompetDlg::OnBnClickedRadioMode)
	ON_CBN_SELCHANGE(IDC_CMB_SERV, &CCreate2CompetDlg::OnCbnSelchangeCmbServ)
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
	// 局所変数宣言
	//unsigned int uiAddrFiled[4];  // IPアドレスのビットフィールド
	//wchar_t wszEdit[31];          // エディットボックスの文字
	CString strProfile;           // プロファイル値
	bool    bRet;   // 関数呼出しの戻り値
	int     iAllotedTime = INITIAL_TIME;
	

	// コンソールを作る
	::AllocConsole();
	freopen_s(&m_fpConsole, "CON", "w", stdout);

	vAppendLogMsg(TEXT("アプリケーションを起動しました。"), false);

#if 0
	// 持ち時間の取得
	bRet = m_IniConfig.bGetProfile(PROF_GEN_COMP_TIME, strProfile);
	if (false != bRet) {
		iAllotedTime = _ttoi(strProfile);
	}
#endif /* 0 */

	// オブジェクトの生成
	//m_pDrawMap = new CDrawMap(this->m_hWnd, iAllotedTime);
	m_pConMqtt = new CConMQTT(this->m_hWnd);
	m_pConSock = new CConSocket(this->m_hWnd);

	// CMFCButtonのsetfacecolorを有効にする
	CMFCButton::EnableWindowsTheming(FALSE);
	


	// 点数表の初期配列
	bRet = m_IniConfig.bGetProfile(PROF_GEN_INIT_PT, strProfile);
	if (false != bRet) {
		m_ctrlEditPtInit.SetWindowText(strProfile);
	}
	else {
		m_ctrlEditPtInit.SetWindowText(TEXT(""));
	}
	//wsprintf(wszEdit, L"%S", POINT_INIT);
	//m_ctrlEditPtInit.SetWindowText(wszEdit);

	// 点数表のシード
	bRet = m_IniConfig.bGetProfile(PROF_GEN_RAND_SEED, strProfile);
	if (false != bRet) {
		m_ctrlEditPtSeed.SetWindowText(strProfile);
	}
	else {
		m_ctrlEditPtSeed.SetWindowText(TEXT(""));
	}
	//wsprintf(wszEdit, L"%d", POINT_SEED);
	//m_ctrlEditPtSeed.SetWindowText(wszEdit);


	// 編集ロックをかける
	m_ctrlBtnChkLock.SetCheck(BST_CHECKED);
	vUpdateLock(true);

	// コンボボックスの文字列
	// ローカルループバックアドレスはdefaultとして残す
	vAddServList(TEXT("127.0.0.1"));

	// INIファイルからサーバ1-2を読み込む
	bRet = m_IniConfig.bGetProfile(PROF_GEN_IPADDR1, strProfile);
	if (false != bRet) {
		vAddServList(strProfile);
	}
	bRet = m_IniConfig.bGetProfile(PROF_GEN_IPADDR2, strProfile);
	if (false != bRet) {
		vAddServList(strProfile);
	}

	vAddServList(TEXT("その他"));


	// 最初のリストを選択する（ローカルループバックアドレス）
	m_iCmbServ = 0;

	
	// ラジオボタンの表示変更
	m_iRadioMode = 0;
	m_ctrlBtnCompet.SetWindowTextW(TEXT("試走モード"));

	//m_ctrlBtnDp.SetFaceColor(RGB(255, 223, 0), FALSE);

	

	UpdateData(FALSE);


	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。
}

void CCreate2CompetDlg::vAddServList(const TCHAR* tszServer)
{
	// 局所変数宣言
	int iRet = -1;        // 関数戻り値

	iRet = m_ctrlCmbServ.InsertString(-1, tszServer);
	if (0 > iRet) {
		vAppendLogMsg(TEXT("サーバーアドレスの追加に失敗しました。"));
	}
	else {
		m_iIndexLastSL = iRet;
	}

	return;
}


// デストラクタ
CCreate2CompetDlg::~CCreate2CompetDlg()
{
	// オブジェクトの解放
#if 0
	if (NULL != m_pDrawMap) {
		delete m_pDrawMap;
	}
#endif /* 0 */
	if (NULL != m_pConMqtt) {
		delete m_pConMqtt;
	}
	if (NULL != m_pConSock) {
		delete m_pConSock;
	}

	// プロセス終了用コールバック関数の呼び出し
	EnumWindows(bEnumWindowsProc, (LPARAM)&m_ProcInfoScorer);
	EnumWindows(bEnumWindowsProc, (LPARAM)&m_ProcInfoBroker);

	// コンソールを閉じる
	fclose(m_fpConsole);
	::FreeConsole();

	return;
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
		int x = (rect.Width()  - cxIcon + 1) / 2;
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

// 外部プロセス群の呼び出し
bool CCreate2CompetDlg::bCallExtProcesses(void)
{
	// 局所変数宣言
	//wchar_t wszBinBroker[MAX_PATH] = TEXT("");
	//wchar_t wszBinScorer[MAX_PATH] = TEXT("");
	//wchar_t wszCmdBroker[MAX_PATH] = CMD_PRPT;
	//wchar_t wszCmdScorer[MAX_PATH] = CMD_PRPT;
	int     iRet;
	bool    bRet;
	CString strBinName;
	CString strMsg;
	DWORD   dwExitCode;

	CString strFullPathBin;    // Brokerバイナリのフルパス名
	CString strFullPathConf;   // Broker設定ファイルのフルパス名
	CString strCommand;        // コマンド名
	CString strServName;       // 接続サーバー名

	// GUIよりデータ取得
	UpdateData(TRUE);

	if (m_iCmbServ != m_iIndexLastSL) {

		// Brokerバイナリのフルパス名の取得
		bRet = bGetAbsolutePath(PROF_MQT_PATH, PROF_MQT_BIN, TEXT("Broker"), strFullPathBin);
		if (false == bRet) {
			strMsg.Format(TEXT("%s\r\n  >>%s"), TEXT("ブローカーの実行ファイルが見つかりません。"), (LPCTSTR)strFullPathBin);
			vAppendLogMsg(strMsg);
			return false;
		}

		// Broker設定ファイルのフルパス名の取得
		if ((1 == m_iCmbServ) || (2 == m_iCmbServ)) {
			bRet = bGetAbsolutePath(PROF_MQT_PATH, 
				(1 == m_iCmbServ) ? PROF_MQT_CONF1 : PROF_MQT_CONF2, TEXT("Broker"), strFullPathConf);
		}
		if (false == bRet) {
			strMsg.Format(TEXT("%s\r\n  >>%s"), 
				TEXT("ブローカーの設定ファイルが見つかりません。設定ファイルを確認して下さい。"), (LPCTSTR)strFullPathConf);
			vAppendLogMsg(strMsg);
			return false;
		}
		
		// Broker実行コマンドの決定
		if (0 == m_iCmbServ) {
			strCommand.Format(TEXT("%s %s -v"), CMD_PRPT, (LPCTSTR)strFullPathBin);
		}
		else {
			strCommand.Format(TEXT("%s %s -v -c %s"), CMD_PRPT, (LPCTSTR)strFullPathBin, (LPCTSTR)strFullPathConf);
		}


		// TODO: iniファイルで指定できるようにする
		// NOTE: とりあえずパスは決め打ち
		// ブローカープロセスの起動
#if 0
		GetCurrentDirectory(sizeof(wszBinBroker) - 1, wszBinBroker);
		wcscat_s(wszBinBroker, LOCAL_BROKER);
		wcscat_s(wszCmdBroker, TEXT(" "));
		wcscat_s(wszCmdBroker, wszBinBroker);
#endif 
		//bRet = bLaunchExtProc(wszCmdBroker, &m_ProcInfoBroker);
		bRet = bLaunchExtProc(strCommand, &m_ProcInfoBroker);
		Sleep(100);
		// プロセスの終了状態を調べる
		GetExitCodeProcess(m_ProcInfoBroker.hProcess, &dwExitCode);

		if ((true != bRet) || (STILL_ACTIVE != dwExitCode)) {
			strMsg.Format(TEXT("%s\r\n  >> %s"),
				TEXT("ブローカープロセスの起動に失敗しました。"),
				(LPCTSTR)strCommand);
			vAppendLogMsg(strMsg);
			return false;
		}

		// Scorerスクリプトのフルパス名の取得
		bRet = bGetAbsolutePath(PROF_SOC_PATH, PROF_SOC_NAME, TEXT("Script"), strFullPathBin);
		if (false == bRet) {
			strMsg.Format(TEXT("%s\r\n  >>%s"), TEXT("スコアラーの実行スクリプトが見つかりません。"), (LPCTSTR)strFullPathBin);
			vAppendLogMsg(strMsg);
			return false;
		}

		//Scorer実行コマンドの決定
		m_IniConfig.bGetProfile(PROF_GEN_PYTHON_BIN, strBinName);
		m_ctrlCmbServ.GetLBText(m_iCmbServ, strServName);
		strCommand.Format(TEXT("%s %s %s %s"), CMD_PRPT, (LPCTSTR)strBinName, (LPCTSTR)strFullPathBin, (LPCTSTR)strServName);

#if 0
		// スコアラープロセスの起動
		GetCurrentDirectory(sizeof(wszBinScorer) - 1, wszBinScorer);
		wcscat_s(wszBinScorer, LOCAL_SCORER);
		wcscat_s(wszCmdScorer, TEXT(" "));

		m_IniConfig.bGetProfile(PROF_GEN_PYTHON_BIN, strBinName);
		wcscat_s(wszCmdScorer, strBinName);

		wcscat_s(wszCmdScorer, TEXT(" "));
		wcscat_s(wszCmdScorer, wszBinScorer);
#endif /* 0 */
		bRet = bLaunchExtProc(strCommand, &m_ProcInfoScorer);
		Sleep(100);
		// プロセスの終了状態を調べる
		GetExitCodeProcess(m_ProcInfoScorer.hProcess, &dwExitCode);

		if ((true != bRet) || (STILL_ACTIVE != dwExitCode)) {
			strMsg.Format(TEXT("%s\r\n  >> %s\r\n  >> %s"),
				TEXT("スコアラープロセスの起動に失敗しました。Pythonのバイナリー名を確認して下さい"),
				(LPCTSTR)strBinName,
				(LPCTSTR)strCommand);
			vAppendLogMsg(strMsg);
			return false;
		}
	}
	else {
		iRet = AfxMessageBox(TEXT("通信のための外部プロセスを起動させましたか？"), MB_YESNO | MB_ICONQUESTION);
		if (IDYES != iRet) {
			vAppendLogMsg(TEXT("通信のための外部プロセスを起動させて下さい。"));
			return false;
		}
	}

	return true;
}

// パス名とファイル名を組み合わせて文字列を返す
// ファイルがパスに存在しない場合は、現在のフォルダ＋サブフォルダ＋ファイル名を文字列として返す
// この場合、ファイルの存在確認は行なわない
bool CCreate2CompetDlg::bGetAbsolutePath(
	enIniProfile enProfPath,
	enIniProfile enProfFile,
	CString      strSubDir,
	CString&     strFullName)
{
	// 局所変数宣言
	bool bRet1;         // 関数呼出しの戻り値
	bool bRet2;         // 関数呼出しの戻り値
	CString strPath;    // パス名
	CString strFile;    // ファイル名
	CFileFind  Finder;
	wchar_t wszTargDir[MAX_PATH];

	CString strMsg;    // ログ用メッセージ


	strFullName = TEXT("");
	bRet1 = m_IniConfig.bGetProfile(enProfPath, strPath);
	bRet2 = m_IniConfig.bGetProfile(enProfFile, strFile);

	// プロファイルを取得できた場合
	if ((false != bRet1) && (false != bRet2)) {
		strFullName.Format(TEXT("%s\\%s"), (LPCTSTR)strPath, (LPCTSTR)strFile);

		// ファイルが見つからなかった場合
		if (TRUE != Finder.FindFile(strFullName)) {
			strFullName = TEXT("");

			strMsg.Format(TEXT("%s\r\n  >> %s"),
				TEXT("指定されたパスにファイルが存在しません。"),
				TEXT(">> MQTTブローカーIPアドレス"), (LPCTSTR)strFullName);
			vAppendLogMsg(strMsg);
		}
	}

	if (strFullName.IsEmpty()) {
		GetCurrentDirectory(sizeof(wszTargDir) / sizeof(wchar_t) - 1, wszTargDir);
		strFullName.Format(TEXT("%s\\%s\\%s"),
			wszTargDir, (LPCTSTR)strSubDir, (LPCTSTR)strFile);
	}

	// ローカルのサブディレクトリにもファイルが存在しない場合
	if (TRUE != Finder.FindFile(strFullName)) {
		return false;
	}

	return true;
}

// 競技会モード
void CCreate2CompetDlg::OnBnClickedBtnCompet()
{
	// 局所変数宣言
	char    szAddr[31];     // IPアドレス（MQTT, Socket）
	bool    bRet;           // 関数呼出し戻り値
	CString strServName;    // サーバー名

	// 外部プロセス群の呼び出し
	bRet = bCallExtProcesses();
	if (true != bRet) {
		return;
	}

	// GUIよりデータ取得
	UpdateData(TRUE);

	/* *** Socket通信の開始 *** */

	// 接続先アドレスの取得
	if (m_iCmbServ == m_iIndexLastSL) {
		m_ConSettingDlg.vGetAddressSocket(strServName);
	}
	else {
		m_ctrlCmbServ.GetLBText(m_iCmbServ, strServName);
	}
	sprintf_s(szAddr, "%S", (LPCTSTR)strServName);

	// 接続
	bRet = m_pConSock->bConnect(szAddr, SOCKET_PORT);
	if (false == bRet) {
		AfxMessageBox(TEXT("SOCKET通信を確立できません"), MB_OK | MB_ICONEXCLAMATION);
		vAppendLogMsg(TEXT("SOCKET通信を確立できませんでした。スコアラーが起動していることを確認して下さい。"));

		return;
	}

	/* *** MQTT通信の開始 *** */

	// 接続先アドレスの取得
	if (m_iCmbServ == m_iIndexLastSL) {
		m_ConSettingDlg.vGetAddressMqtt(strServName);
	}
	else {
		/* Socketの設定を流用する */
	}
	sprintf_s(szAddr, "%S", (LPCTSTR)strServName);

	// 接続
	bRet = m_pConMqtt->bConnect(szAddr);
	if (false == bRet) {
		AfxMessageBox(TEXT("MQTT通信を確立できません"), MB_OK | MB_ICONEXCLAMATION);
		vAppendLogMsg(TEXT("MQTT通信を確立できませんでした。MQTTブローカーを確認して下さい。"));
		return;
	}

	if (0 == m_iRadioMode) {
		vAppendLogMsg(TEXT("試走モードでアプリケーションを開始します。"));
	}
	else {
		vAppendLogMsg(TEXT("競技モードでアプリケーションを開始します。"));
	}
	vAppendLogMsg(TEXT("モードの切替えを行なう場合は、一度アプリケーションを終了して下さい。"));

	// 各ボタンを操作禁止にする
	m_ctrlBtnCompet.EnableWindow(FALSE);
	m_ctrlRadioMode.EnableWindow(FALSE);
	m_ctrlRadioMode2.EnableWindow(FALSE);
	m_ctrlCmbServ.EnableWindow(FALSE);


	// 開始ボタンの表示変更
	m_ctrlBtnStart.SetFaceColor(m_crBtnExec[EXEC_STAT_START]);
	m_ctrlBtnStart.EnableWindow(TRUE);

	return;
}


// 競技開始
void CCreate2CompetDlg::OnBnClickedBtnStart()
{
	// 局所変数宣言
	wchar_t wszPoints[31];       // 点数表初期値
	wchar_t wszSeed[31];         // 点数表Seed値
	char    szMsg[31];           // 送信メッセージ
	char    szPoints[31];        // 点数表初期値
	unsigned int uiPoints[CORNER_NUM];   // 各コーナーの点数
	size_t  sizeConvert;
	bool bRet;
	int iAllotedTime;
	CString strProfile;


	// ボタン表示の変更
	m_ctrlBtnStart.SetWindowText(m_tszBtnExec[m_enExecStat]);
	m_ctrlBtnStart.SetFaceColor(m_crBtnExec[m_enExecStat], FALSE);

	// 実行状態が停止状態なら、開始の処理
	if (EXEC_STAT_STOP == m_enExecStat) {

		// 持ち時間の取得
		bRet = m_IniConfig.bGetProfile(PROF_GEN_COMP_TIME, strProfile);
		if (false != bRet) {
			iAllotedTime = _ttoi(strProfile);
		}
		m_pDrawMap = new CDrawMap(this->m_hWnd, iAllotedTime);


		// 点数表サーバーへの初期化指示
		m_pConSock->bSetSendMessage(MSG_INIT);
		Sleep(100);

		m_ctrlEditPtInit.GetWindowText(wszPoints, sizeof(wszPoints));
		m_ctrlEditPtSeed.GetWindowText(wszSeed, sizeof(wszSeed));

		sprintf_s(szMsg, "%ws:%ws", wszPoints, wszSeed);
		m_pConSock->bSetSendMessage(szMsg);

		// 初期ポイントの反映
		m_ctrlEditPtInit.GetWindowText(wszPoints, sizeof(wszPoints));
		wcstombs_s(&sizeConvert, szPoints, sizeof(szPoints), wszPoints, wcsnlen_s(wszPoints, sizeof(wszPoints)));

		vSplitString(szPoints, " ,", uiPoints);
		uiPoints[INIT_POSITION - FIRST_CORNER] = 0;

		m_pDrawMap->vSetCurrPoints(uiPoints);
		m_pDrawMap->iStartDraw();


		m_pDrawMap->vSetPlayStatus(true);

		s_bFirst = true;
		//m_pDrawMap->vDrawTotalScore();
		m_pDrawMap->vResetTime();
		vStartTimerWrapper(101);

		m_enExecStat = EXEC_STAT_START;
	}
	// 実行状態が開始状態なら、停止の処理
	else {
		vStopCompet();

		m_enExecStat = EXEC_STAT_STOP;
	}

	UpdateData(FALSE);

	return;
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


// static method
void CALLBACK CCreate2CompetDlg::voStartTimer(
	HWND hWnd,           // handle of CWnd that called SetTimer
	UINT nMsg,           // WM_TIMER
	UINT_PTR nIDEvent,   // timer identification
	DWORD dwTime         // system time
	)
{
	// 局所変数宣言
	DWORD dwDiff;        // 前回の呼出しとの差分時間 [ms]

	
	// 初回の呼出しの場合、基準時間を記録する
	if (true == s_bFirst) {
		s_dwBaseTime = dwTime;
		s_bFirst = false;
		s_uiCntCallback = 1;
	}

	// 前回のコールバックとの差分時間を計算
	dwDiff = dwTime - s_dwBaseTime;

	// 100[ms] 間隔の処理
	if (s_uiCntCallback <= (dwDiff / CALLBACK_INTERVAL)) {
		++s_uiCntCallback;
		::SendMessage(hWnd, WM_USER_PROC, PROC_TIC_TIMER, 0);
	}

	return;
}


// 競技終了
void CCreate2CompetDlg::vStopCompet(void)
{
	// 局所変数宣言
	int iRet;


	vStopTimerWrapper(101);
	m_pDrawMap->vSetPlayStatus(false);
	iRet = AfxMessageBox(TEXT("競技終了です"), MB_OK | MB_ICONINFORMATION);
	if (IDOK == iRet) {
		m_pDrawMap->vDestroyWindows();
	}
	vAppendLogMsg(TEXT("競技が終了しました。"));

	// TODO: メモリリーク対策
	// 解放位置暫定
	if (NULL != m_pDrawMap) {
		delete m_pDrawMap;
		m_pDrawMap = NULL;
	}

}


// 減点ボタン
void CCreate2CompetDlg::OnBnClickedBtnDp()
{
	// TODO: ここにコントロール通知ハンドラー コードを追加します。
}

// 編集のロック（ボタン押下）
void CCreate2CompetDlg::OnBnClickedChkLock()
{
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

// 編集のロック
void CCreate2CompetDlg::vUpdateLock(bool bCheck)
{
	if (true == bCheck) {
		// 編集をOFF
		m_ctrlEditPtInit.EnableWindow(FALSE);
		m_ctrlEditPtSeed.EnableWindow(FALSE);
	}
	else {
		// 編集をON
		m_ctrlEditPtInit.EnableWindow(TRUE);
		m_ctrlEditPtSeed.EnableWindow(TRUE);
	}

	return;
}


// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton2()
{
	m_ConSettingDlg.DoModal();
	
	//m_pConSock->bSetSendMessage(MSG_UPDATE);
}



// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton3()
{
	CDrawMap::vWindowTestOn();
	//CDrawMap::vWindowTestOff();
	//m_pConSock->bSetSendMessage(MSG_QUIT);
}

// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton5()
{
	//CDrawMap::vWindowTestOn();
	CDrawMap::vWindowTestOff();
	//m_pConSock->bSetSendMessage(MSG_QUIT);
}


// ユーザーメッセージ受信
LRESULT CCreate2CompetDlg::OnUserMessage(WPARAM wParam, LPARAM lParam)
{
	// 局所変数宣言
	static unsigned int uiPoints[CORNER_NUM];
	static char cPos = 'a';

	switch (wParam) {

		case PROC_RECV_MQTT:
			// createが別のコーナーに移動した場合、点数表を更新する
			if (cPos != *(char*)lParam) {
				m_pConSock->bSetSendMessage(MSG_UPDATE);
				cPos = *(char*)lParam;
			}
			break;
		case PROC_RECV_SOCK:
			// 受信メッセージから各点数を取り出す
			vSplitString((char*)lParam, " ,[]", uiPoints);
			m_pDrawMap->vUpdatePoints(cPos, uiPoints);
			break;

		case PROC_TIC_TIMER:
			// 継続できない場合は中止する
			if (m_pDrawMap->bUpdateTimer() == false) {

				// 別スレッドから競技を停止する
				//_beginthread(vStopCompetThread, 0, this);

				// ボタン表示の変更
				m_ctrlBtnStart.SetWindowText(m_tszBtnExec[m_enExecStat]);
				m_ctrlBtnStart.SetFaceColor(m_crBtnExec[m_enExecStat], FALSE);
				UpdateData(FALSE);

				vStopCompet();

				m_enExecStat = EXEC_STAT_STOP;			
			}
			break;
	}

	return 0;
}


// 競技停止用スレッド
void CCreate2CompetDlg::vStopCompetThread(void* pArg)
{
	// 競技の停止
	reinterpret_cast<CCreate2CompetDlg*>(pArg)->vStopCompet();

	return;
}

// 暫定コントロール
void CCreate2CompetDlg::OnBnClickedButton4()
{
	wchar_t wszPoints[31];       // 点数表初期値
	char    szPoints[31];        // 点数表初期値
	unsigned int uiPoints[CORNER_NUM];   // 各コーナーの点数
	size_t sizeConvert;

	// 初期ポイントの反映
	m_ctrlEditPtInit.GetWindowText(wszPoints, sizeof(wszPoints));
	wcstombs_s(&sizeConvert, szPoints, sizeof(szPoints), wszPoints, wcsnlen_s(wszPoints, sizeof(wszPoints)));

	vSplitString(szPoints, " ,", uiPoints);
	uiPoints[INIT_POSITION - FIRST_CORNER] = 0;

	m_pDrawMap->vSetCurrPoints(uiPoints);
	m_pDrawMap->iStartDraw();
}



// モードの選択（ラジオボタン）
void CCreate2CompetDlg::OnBnClickedRadioMode()
{
	// 局所変数宣言
	int iPrevMode = m_iRadioMode;

	// モード値の取得
	UpdateData(TRUE);

	// 変更があった場合のみ処理する
	if (iPrevMode != m_iRadioMode) {
		if (0 == m_iRadioMode) {
			m_ctrlBtnCompet.SetWindowTextW(TEXT("試走モード"));
			vAppendLogMsg(TEXT("試走モードを選択しました。"));
		}
		else {
			m_ctrlBtnCompet.SetWindowTextW(TEXT("競技モード"));
			vAppendLogMsg(TEXT("競技モードを選択しました。"));
		}
	}

	return;
}


// ログメッセージの追記
void CCreate2CompetDlg::vAppendLogMsg(CString szMsg, bool bNewLine)
{
	// 局所変数宣言
	CTime cTime = CTime::GetCurrentTime();      // 現在時刻


	// 改行判断
	if (true == bNewLine) {
		m_LogMsg += TEXT("\r\n");
	}
	
	// ログメッセージへの追記
	m_LogMsg += cTime.Format("[%H:%M:%S] : ");
	m_LogMsg += szMsg;
	m_ctrlEditLog.SetWindowTextW(m_LogMsg);

	// キャレットを一番下へ
	m_ctrlEditLog.LineScroll(m_ctrlEditLog.GetLineCount());

	return;
}


// コンボボックスの選択が変更されたとき
void CCreate2CompetDlg::OnCbnSelchangeCmbServ()
{
	// 局所変数宣言
	INT_PTR nResponse;       // ダイアログからの戻り値
	CString strAddrMqtt;     // ブローカーアドレス
	CString strAddrSocket;   // スコアラーアドレス
	CString strMsg;          // ログ用メッセージ


	// GUIよりデータ取得
	UpdateData(TRUE);

	// コンボボックスで「その他」が選択されたとき
	if (m_iCmbServ == m_iIndexLastSL) {
		nResponse = m_ConSettingDlg.DoModal();

		if (IDOK == nResponse) {
			m_ConSettingDlg.vGetAddressMqtt(strAddrMqtt);
			m_ConSettingDlg.vGetAddressMqtt(strAddrSocket);

			strMsg.Format(TEXT("%s\r\n  %s:%s\r\n  %s:%s"),
				TEXT("以下の設定で外部プロセスを起動させて下さい。"),
				TEXT(">> MQTTブローカーIPアドレス"), (LPCTSTR)strAddrMqtt,
				TEXT(">> 点数表スコアラーIPアドレス"), (LPCTSTR)strAddrSocket);
			vAppendLogMsg(strMsg);
		}
	}
}

// ウィンドウハンドルを取得し起動したプロセスを終了させる
BOOL CALLBACK CCreate2CompetDlg::bEnumWindowsProc(HWND hWnd, LPARAM lParam)
{
	// CreateProcess()で取得したPROCESS_INFORMATION構造体のポインタを取得
	PROCESS_INFORMATION* pi = (PROCESS_INFORMATION*)lParam;

	// ウインドウを作成したプロセスIDを取得。
	DWORD lpdwProcessId = 0;
	::GetWindowThreadProcessId(hWnd, &lpdwProcessId);

	// CreateProcessで起動したアプリのプロセスIDとメインウィンドウを
	// 作成したプロセスIDが同じ場合、起動したアプリを終了させる。
	if (pi->dwProcessId == lpdwProcessId)
	{
		::PostMessage(hWnd, WM_CLOSE, 0, 0);
		return FALSE;
	}
	return TRUE;
}
