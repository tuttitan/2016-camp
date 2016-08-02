#pragma once


// CConSetting �_�C�A���O

class CConSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CConSettingDlg)

public:
	CConSettingDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	//CConSettingDlg();
	virtual ~CConSettingDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CREATE2COMPET_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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

