// ConSetting.cpp : 実装ファイル
//

#include "stdafx.h"
#include "Create2Compet.h"
#include "ConSettingDlg.h"
#include "afxdialogex.h"

#include "ConMQTT.h"
#include "ConSocket.h"
#include "utility.h"

// CConSetting ダイアログ

IMPLEMENT_DYNAMIC(CConSettingDlg, CDialog)

CConSettingDlg::CConSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONSETTING_DIALOG, pParent)
	, m_iCmbServ(0)
{
	// 局所変数宣言
	unsigned int uiAddrFiled[4];    // IPアドレスのビットフィールド
	int          i;                 // ループカウンタ

	/* *** 初期アドレスの設定 *** */
	// MQTTブローカーアドレスのビットフィールドを取得
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	for (i = 0; 4 > i; i++) {
		m_uiAddrMqtt[i] = uiAddrFiled[i];
	}
	//m_strAddrMqtt.Format(TEXT("%d.%d.%d.%d"), m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);

	// Socketサーバーアドレスのビットフィールドを取得
	vSplitString(SERV_ADDRESS, ".", uiAddrFiled);
	for (i = 0; 4 > i; i++) {
		m_uiAddrSocket[i] = uiAddrFiled[i];
	}

	vUpdateLocal();

	return;
}

CConSettingDlg::~CConSettingDlg()
{
}

// ローカル変数の更新
void CConSettingDlg::vUpdateLocal(void)
{
	m_strAddrMqtt.Format(TEXT("%d.%d.%d.%d"), m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);
	m_strAddrSocket.Format(TEXT("%d.%d.%d.%d"), m_uiAddrSocket[0], m_uiAddrSocket[1], m_uiAddrSocket[2], m_uiAddrSocket[3]);

	return;
}

void CConSettingDlg::OnOK()
{
	//AfxMessageBox(TEXT("OK"), MB_OK | MB_ICONEXCLAMATION);

	// 局所変数宣言
	BYTE byAddrFiled[4];    // IPアドレスのビットフィールド
	int  i;                 // ループカウンタ

	// MQTTブローカーアドレスの保存
	m_ctrlAddressMqtt.GetAddress(byAddrFiled[0], byAddrFiled[1], byAddrFiled[2], byAddrFiled[3]);
	for (i = 0; 4 > i; i++) {
		m_uiAddrMqtt[i] = (unsigned int)byAddrFiled[i];
	}

	// スコアラーアドレスの保存
	m_ctrlAddressSocket.GetAddress(byAddrFiled[0], byAddrFiled[1], byAddrFiled[2], byAddrFiled[3]);
	for (i = 0; 4 > i; i++) {
		m_uiAddrSocket[i] = (unsigned int)byAddrFiled[i];
	}

	vUpdateLocal();
	CDialog::OnOK();

	return;
}

void CConSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_IPADDRESS_MQTT, m_ctrlAddressMqtt);
	DDX_Control(pDX, IDC_IPADDRESS_SOCKET, m_ctrlAddressSocket);
//	DDX_CBIndex(pDX, IDC_CMB_SERV, m_iCmbServ);
}


BEGIN_MESSAGE_MAP(CConSettingDlg, CDialog)
END_MESSAGE_MAP()



// CConSettingDlg メッセージ ハンドラー
BOOL CConSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 局所変数宣言
	unsigned int uiAddrFiled[4];  // IPアドレスのビットフィールド

	// MQTTブローカーアドレスの設定
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressMqtt.SetAddress(m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);

	// Socketサーバーアドレスの設定
	//vSplitString(SERV_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressSocket.SetAddress(m_uiAddrSocket[0], m_uiAddrSocket[1], m_uiAddrSocket[2], m_uiAddrSocket[3]);

	return TRUE;  // フォーカスをコントロールに設定した場合を除き、TRUE を返します。

}

void CConSettingDlg::vGetAddressMqtt(CString& strAddress)
{
	strAddress = m_strAddrMqtt;
	return;
}

void CConSettingDlg::vGetAddressSocket(CString& strAddress)
{
	strAddress = m_strAddrSocket;
	return;
}

#if 0
void CConSettingDlg::vGetMqttAddr(BYTE byFiled[4])
{
	m_ctrlAddressMqtt.GetAddress(byFiled[0], byFiled[1], byFiled[2], byFiled[3]);

	return;
}

void CConSettingDlg::vGetMqttAddr(CString& strAdress)
{
	m_ctrlAddressMqtt.GetAddress(byFiled[0], byFiled[1], byFiled[2], byFiled[3]);

	return;
}


void CConSettingDlg::vGetSocketAddr(BYTE byFiled[4])
{
	m_ctrlAddressSocket.GetAddress(byFiled[0], byFiled[1], byFiled[2], byFiled[3]);

	return;
}
#endif /* 0 */