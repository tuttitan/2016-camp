// ConSetting.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "Create2Compet.h"
#include "ConSettingDlg.h"
#include "afxdialogex.h"

#include "ConMQTT.h"
#include "ConSocket.h"
#include "utility.h"

// CConSetting �_�C�A���O

IMPLEMENT_DYNAMIC(CConSettingDlg, CDialog)

CConSettingDlg::CConSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_CONSETTING_DIALOG, pParent)
	, m_iCmbServ(0)
{
	// �Ǐ��ϐ��錾
	unsigned int uiAddrFiled[4];    // IP�A�h���X�̃r�b�g�t�B�[���h
	int          i;                 // ���[�v�J�E���^

	/* *** �����A�h���X�̐ݒ� *** */
	// MQTT�u���[�J�[�A�h���X�̃r�b�g�t�B�[���h���擾
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	for (i = 0; 4 > i; i++) {
		m_uiAddrMqtt[i] = uiAddrFiled[i];
	}
	//m_strAddrMqtt.Format(TEXT("%d.%d.%d.%d"), m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);

	// Socket�T�[�o�[�A�h���X�̃r�b�g�t�B�[���h���擾
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

// ���[�J���ϐ��̍X�V
void CConSettingDlg::vUpdateLocal(void)
{
	m_strAddrMqtt.Format(TEXT("%d.%d.%d.%d"), m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);
	m_strAddrSocket.Format(TEXT("%d.%d.%d.%d"), m_uiAddrSocket[0], m_uiAddrSocket[1], m_uiAddrSocket[2], m_uiAddrSocket[3]);

	return;
}

void CConSettingDlg::OnOK()
{
	//AfxMessageBox(TEXT("OK"), MB_OK | MB_ICONEXCLAMATION);

	// �Ǐ��ϐ��錾
	BYTE byAddrFiled[4];    // IP�A�h���X�̃r�b�g�t�B�[���h
	int  i;                 // ���[�v�J�E���^

	// MQTT�u���[�J�[�A�h���X�̕ۑ�
	m_ctrlAddressMqtt.GetAddress(byAddrFiled[0], byAddrFiled[1], byAddrFiled[2], byAddrFiled[3]);
	for (i = 0; 4 > i; i++) {
		m_uiAddrMqtt[i] = (unsigned int)byAddrFiled[i];
	}

	// �X�R�A���[�A�h���X�̕ۑ�
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



// CConSettingDlg ���b�Z�[�W �n���h���[
BOOL CConSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �Ǐ��ϐ��錾
	unsigned int uiAddrFiled[4];  // IP�A�h���X�̃r�b�g�t�B�[���h

	// MQTT�u���[�J�[�A�h���X�̐ݒ�
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressMqtt.SetAddress(m_uiAddrMqtt[0], m_uiAddrMqtt[1], m_uiAddrMqtt[2], m_uiAddrMqtt[3]);

	// Socket�T�[�o�[�A�h���X�̐ݒ�
	//vSplitString(SERV_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddressSocket.SetAddress(m_uiAddrSocket[0], m_uiAddrSocket[1], m_uiAddrSocket[2], m_uiAddrSocket[3]);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B

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