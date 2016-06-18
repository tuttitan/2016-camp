
// MQTTSenderDlg.h : ヘッダー ファイル
//

#pragma once
#include "afxcmn.h"
#include "afxbutton.h"

#include "ConMQTT.h"

// 通信状態
typedef enum _EN_CONN_STAT {
	STAT_CONN = 0,     // 接続状態
	STAT_DISC,         // 切断状態
	STAT_NUM           // 状態数
} EN_CONN_STAT;

// CMQTTSenderDlg ダイアログ
class CMQTTSenderDlg : public CDialogEx
{
// コンストラクション
public:
	CMQTTSenderDlg(CWnd* pParent = NULL);	// 標準コンストラクター

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MQTTSENDER_DIALOG };
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
private:
	CIPAddressCtrl m_ctrlAddr;
	int m_iStatConn = STAT_DISC;
public:
	afx_msg void OnBnClickedBtnA();
	afx_msg void OnBnClickedBtnB();
	afx_msg void OnBnClickedBtnC();
	afx_msg void OnBnClickedBtnD();
	afx_msg void OnBnClickedBtnE();
	afx_msg void OnBnClickedBtnF();
private:
	int m_iRadioMsgSW;
public:
	afx_msg void OnBnClickedMfcbtnConn();
private:
	CMFCButton m_ctrlBtnConn;
	const COLORREF m_crBtnColour[STAT_NUM] = {
		RGB(0, 255, 127),    // 接続時
		RGB(255, 127, 127)   // 切断時
	};
	const wchar_t* m_wszBtnConn[STAT_NUM] = {
		L"接続",             // 接続時
		L"切断"              // 切断時
	};
	CConMQTT m_ConMQTT;
};
