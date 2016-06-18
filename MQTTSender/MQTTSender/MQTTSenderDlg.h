
// MQTTSenderDlg.h : �w�b�_�[ �t�@�C��
//

#pragma once
#include "afxcmn.h"
#include "afxbutton.h"

#include "ConMQTT.h"

// �ʐM���
typedef enum _EN_CONN_STAT {
	STAT_CONN = 0,     // �ڑ����
	STAT_DISC,         // �ؒf���
	STAT_NUM           // ��Ԑ�
} EN_CONN_STAT;

// CMQTTSenderDlg �_�C�A���O
class CMQTTSenderDlg : public CDialogEx
{
// �R���X�g���N�V����
public:
	CMQTTSenderDlg(CWnd* pParent = NULL);	// �W���R���X�g���N�^�[

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MQTTSENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �T�|�[�g


// ����
protected:
	HICON m_hIcon;

	// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
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
		RGB(0, 255, 127),    // �ڑ���
		RGB(255, 127, 127)   // �ؒf��
	};
	const wchar_t* m_wszBtnConn[STAT_NUM] = {
		L"�ڑ�",             // �ڑ���
		L"�ؒf"              // �ؒf��
	};
	CConMQTT m_ConMQTT;
};
