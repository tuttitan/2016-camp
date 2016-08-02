#pragma once


// CConSetting ダイアログ

class CConSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CConSettingDlg)

public:
	CConSettingDlg(CWnd* pParent = NULL);   // 標準コンストラクター
	//CConSettingDlg();
	virtual ~CConSettingDlg();

// ダイアログ データ
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE2COMPET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	// 生成された、メッセージ割り当て関数
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

	void OnOK();

private:
	CIPAddressCtrl m_ctrlAddressMqtt;
	CIPAddressCtrl m_ctrlAddressSocket;

	void vUpdateLocal(void);

public:
	void vGetAddressMqtt(CString&);
	void vGetAddressSocket(CString&);
	//void CConSettingDlg::vGetMqttAddr(BYTE[4]);
	//void CConSettingDlg::vGetSocketAddr(BYTE[4]);
private:
	int m_iCmbServ;
	unsigned int m_uiAddrMqtt[4]   = { 0 };
	unsigned int m_uiAddrSocket[4] = { 0 };

	CString m_strAddrMqtt;
	CString m_strAddrSocket;
};

