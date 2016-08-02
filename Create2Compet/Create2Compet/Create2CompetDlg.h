
// Create2CompetDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "ConSettingDlg.h"
#include "IniConfig.h"

/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#define CALLBACK_INTERVAL    (100)   // コールバック呼出し間隔
#define CALLBACK_RESOLUTION  (10)    // コールバック呼出し分解能

//#define EXEC_STAT_NUM        (2)


/*****************************************************************************/
/* 列挙型定義                                                                */
/*****************************************************************************/
// 通信状態
typedef enum _EN_EXEC_STAT {
	EXEC_STAT_START = 0,     // 開始状態
	EXEC_STAT_STOP,          // 停止状態
	EXEC_STAT_NUM            // 状態数
} EN_EXEC_STAT;

/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
class CDrawMap;
class CConMQTT;
class CConSocket;
class CConSettingDlg;
class CIniConfig;

/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
// CCreate2CompetDlg ダイアログ
class CCreate2CompetDlg : public CDialogEx
{
// コンストラクション
public:
	CCreate2CompetDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE2COMPET_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV サポート


// 実装
protected:
	HICON m_hIcon;

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void CCreate2CompetDlg::OnCancel();
public:
	afx_msg void OnBnClickedBtnCompet();
	afx_msg LRESULT CCreate2CompetDlg::OnUserMessage(WPARAM wParam, LPARAM lParam);

public:
	CDrawMap*           m_pDrawMap;         // マップ描画オブジェクト
	CConMQTT*           m_pConMqtt;         // MQTT通信オブジェクト
	CConSocket*         m_pConSock;         // ソケット通信オブジェクト
	CConSettingDlg      m_ConSettingDlg;
	CIniConfig          m_IniConfig;

private:
	//bool m_bCompetMode = false;                  // 競技会モードか

	static bool               s_bFirst;          // 初回のコールバックか
	static DWORD              s_dwBaseTime;      // コールバックの基準時間 [ms]
	static unsigned int       s_uiCntCallback;   // コールバックカウント

private:
	static void CALLBACK voStartTimer(
		HWND hWnd,      // handle of CWnd that called SetTimer
		UINT nMsg,      // WM_TIMER
		UINT_PTR nIDEvent,   // timer identification
		DWORD dwTime    // system time
		);

	void vStartTimerWrapper(UINT_PTR nIDEvent);
	void vStopTimerWrapper(UINT_PTR nIDEvent);
	void vUpdateLock(bool bCheck);
	//void vLaunchExtProc(const TCHAR*, PROCESS_INFORMATION*);
	//void vLaunchExtProc(wchar_t*, PROCESS_INFORMATION*);
	void vAddServList(const TCHAR*);
	bool bCallExtProcesses(void);


public:
	~CCreate2CompetDlg();   // デストラクタ
	afx_msg void OnBnClickedBtnStart();   // 競技の開始終了
	//afx_msg void OnBnClickedBtnStop();    // 競技終了
	void CCreate2CompetDlg::vStopCompet(void);
private:
	CMFCButton m_ctrlBtnStart;


	FILE* m_fpConsole = NULL;         // コンソール（ログ用）
	CMFCButton m_ctrlBtnDp;
	
	CEdit m_ctrlEditPtInit;
	CEdit m_ctrlEditPtSeed;
	CButton m_ctrlBtnChkLock;

public:
	afx_msg void OnBnClickedChkLock();
	afx_msg void OnBnClickedBtnDp();
	
public:
	afx_msg void OnBnClickedButton2();    // 暫定コントロール
	afx_msg void OnBnClickedButton3();    // 暫定コントロール
	afx_msg void OnBnClickedButton4();    // 暫定コントロール
	afx_msg void OnBnClickedButton5();    // 暫定コントロール
private:
	CComboBox m_ctrlCmbServ;
	
private:
	PROCESS_INFORMATION m_ProcInfoBroker;
	PROCESS_INFORMATION m_ProcInfoScorer;

private:
	static BOOL CALLBACK CCreate2CompetDlg::bEnumWindowsProc(HWND hWnd, LPARAM lParam);

	int m_iCmbServ;

	EN_EXEC_STAT m_enExecStat = EXEC_STAT_STOP;   // 実行状態

	const TCHAR* m_tszBtnExec[EXEC_STAT_NUM] = {
		TEXT("スタート"),    // 開始状態
		TEXT("ストップ")     // 停止状態
	};

	const COLORREF m_crBtnExec[EXEC_STAT_NUM] = {
		RGB(0, 255, 127),    // 開始状態
		RGB(255, 127, 127)   // 停止状態
	};

private:
	int m_iRadioMode;    // モード切替（ラジオボタン）
	int m_iIndexLastSL;  // サーバーリストの最後のインデックス
public:
	afx_msg void OnBnClickedRadioMode();
private:
	CButton m_ctrlBtnCompet;
	//bool m_bDoneMode = false;   // モード選択済み
	CButton m_ctrlRadioMode;
	CButton m_ctrlRadioMode2;
	CEdit m_ctrlEditLog;
	CString m_LogMsg = TEXT("");
public:
	void vAppendLogMsg(CString, bool = true);

	afx_msg void OnCbnSelchangeCmbServ();

private:
	static void vStopCompetThread(void*);
};
