
// Create2CompetDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"

/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#define CALLBACK_INTERVAL    (100)   // コールバック呼出し間隔
#define CALLBACK_RESOLUTION  (10)    // コールバック呼出し分解能

/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
class CDrawMap;
class CConMQTT;
class CConSocket;

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

private:
	bool m_bCompetMode = false;                  // 競技会モードか

	static CDrawMap           s_DrawMap;         // マップ描画オブジェクト
	static CConMQTT           s_ConMQTT;         // MQTT通信オブジェクト
	static CConSocket         s_ConSocket;       // ソケット通信オブジェクト
	static LARGE_INTEGER      s_llPrevTime;      // 前回のコールバック呼出し時間 [ms]
	static CCreate2CompetDlg* s_pOwn;            // 自身のインスタンス
	static bool               s_bFirst;          // 初回のコールバックか
	static DWORD              s_dwBaseTime;      // コールバックの基準時間 [ms]
	static unsigned int       s_uiCntCallback;   // コールバックカウント

public:
	afx_msg void OnBnClickedBtnStart();

private:
	static void CALLBACK voStartTimer(
		HWND hWnd,      // handle of CWnd that called SetTimer
		UINT nMsg,      // WM_TIMER
		UINT_PTR nIDEvent,   // timer identification
		DWORD dwTime    // system time
		);

	void vSetIPAddrFiled(BYTE[], const char*);

	void vStartTimerWrapper(UINT_PTR nIDEvent);
	void vStopTimerWrapper(UINT_PTR nIDEvent);
	void vUpdateLock(bool bCheck);

public:
	afx_msg void OnBnClickedBtnStop();
private:
	CButton m_ctrlBtnStart;
	CButton m_ctrlBtnStop;
	CIPAddressCtrl m_ctrlAddressMQTT;
	FILE* m_fpConsole = NULL;         // コンソール（ログ用）
public:
	afx_msg void OnBnClickedBtnDp();
private:
	CButton m_ctrlBtnDp;
	CIPAddressCtrl m_ctrlAddressSocket;
	CEdit m_ctrlEditPtInit;
	CEdit m_ctrlEditPtSeed;
public:
	afx_msg void OnBnClickedChkLock();
private:
	CButton m_ctrlBtnChkLock;
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
};
