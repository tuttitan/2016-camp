
// MQTTSenderDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "MQTTSender.h"
#include "MQTTSenderDlg.h"
#include "afxdialogex.h"


#include "utility.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
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


// CMQTTSenderDlg �_�C�A���O



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


// CMQTTSenderDlg ���b�Z�[�W �n���h���[

BOOL CMQTTSenderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
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

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	// �Ǐ��ϐ��錾
	unsigned int uiAddrFiled[4];  // IP�A�h���X�̃r�b�g�t�B�[���h

	// CMFCButton��setfacecolor��L���ɂ���
	CMFCButton::EnableWindowsTheming(FALSE);

	// �u���[�J�[IP�A�h���X�̐ݒ�
	vSplitString(BRK_ADDRESS, ".", uiAddrFiled);
	m_ctrlAddr.SetAddress(uiAddrFiled[0], uiAddrFiled[1], uiAddrFiled[2], uiAddrFiled[3]);

	// ���W�I�{�^���̏�Ԃ�ύX����i0: Enter, 1: Leave�j
	m_iRadioMsgSW = 0;
	UpdateData(FALSE);

	// �ڑ��{�^���̏�Ԃ�ύX
	m_iStatConn = STAT_DISC;
	m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_CONN], FALSE);
	m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_CONN]);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
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

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CMQTTSenderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CMQTTSenderDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMQTTSenderDlg::OnBnClickedBtnA()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'a';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

							 // �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnB()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'b';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

	// �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnC()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'c';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

							 // �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnD()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'd';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

							 // �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnE()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'e';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

							 // �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedBtnF()
{
	// �Ǐ��ϐ��錾
	const char ccID = 'f';   // �R�[�i�[�̎���
	int        iDir = 1;     // ���o���� [1: Enter, 0: Leave]

							 // �R���g���[���ϐ��̒l���X�V����
	UpdateData(TRUE);
	iDir = (0 == m_iRadioMsgSW) ? 1 : 0;

	// ���b�Z�[�W�̔��s
	m_ConMQTT.bPublishMessage(ccID, iDir);

	return;
}


void CMQTTSenderDlg::OnBnClickedMfcbtnConn()
{
	// �Ǐ��ϐ��錾
	bool bRet = false;       // �֐��ďo���̖߂�l
	BYTE byAddrField[4];
	char szAddr[31];

	// �ڑ����Ă��Ȃ����
	if (STAT_DISC == m_iStatConn) {

		// �u���[�J�[IP�A�h���X�̎擾
		m_ctrlAddr.GetAddress(byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);
		sprintf_s(szAddr, "%d.%d.%d.%d", byAddrField[0], byAddrField[1], byAddrField[2], byAddrField[3]);

		// MQTT�ʐM�̊J�n
		bRet = m_ConMQTT.bConnect(szAddr);
		if (false == bRet) {
			AfxMessageBox(TEXT("MQTT�ʐM���J�n�ł��܂���"), MB_OK | MB_ICONEXCLAMATION);
			return;
		}
		
		// �{�^���̕ύX
		m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_DISC], FALSE);
		m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_DISC]);

		// ������Ԃ̕ύX
		m_iStatConn = STAT_CONN;
	}
	// �ڑ����Ă�����
	else {

		// MQTT�ʐM�̐ؒf
		bRet = m_ConMQTT.bDisconnect();
		if (false == bRet) {
			AfxMessageBox(TEXT("MQTT�ʐM���I���ł��܂���"), MB_OK | MB_ICONEXCLAMATION);
			return;
		}

		// �{�^���̕ύX
		m_ctrlBtnConn.SetFaceColor(m_crBtnColour[STAT_CONN], FALSE);
		m_ctrlBtnConn.SetWindowTextW(m_wszBtnConn[STAT_CONN]);

		// ������Ԃ̕ύX
		m_iStatConn = STAT_DISC;
	}

	UpdateData(FALSE);

	return;
}
