// �\�P�b�g�ʐM�̂��߂̃N���X
// Client��



/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConSocket.h"
#include "DrawMap.h"
#include "utility.h"

#include <WinSock2.h>

/*****************************************************************************/
/* ���C�u���������N                                                          */
/*****************************************************************************/
#pragma comment(lib,"Ws2_32.lib")




/*****************************************************************************/
/* �����o�֐���`                                                            */
/*****************************************************************************/
// �R���X�g���N�^
CConSocket::CConSocket(HWND hWnd)
{
	// �Ǐ��ϐ��錾
	WSADATA wsaData;


	// �N���e�B�J���Z�N�V�����̏�����
	InitializeCriticalSection(&m_csSend);

	// ���b�Z�[�W�̏�����
	m_szMsgSend[0] = '\0';
	m_szMsgRecv[0] = '\0';

	// Winsock�̏�����
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// �\�P�b�g�̍쐬
	m_Socket = socket(PF_INET, SOCK_STREAM, 0);

	// �e�E�B���h�E�̊֘A�t��
	m_hWnd = hWnd;

	return;
}

// �f�X�g���N�^
CConSocket::~CConSocket()
{
	// �ʐM�̐ؒf
	bDisconnect();

	// winsock�̏I������
	WSACleanup();

	// �R���\�[�������
	//printf("[Socket]: CConMQTT Destructor\n");
	//fclose(m_fpConsole);
	//::FreeConsole();

	// �N���e�B�J���Z�N�V�����̉��
	DeleteCriticalSection(&m_csSend);

	return;
}

// �ʐM�̊J�n
bool CConSocket::bConnect(const char* szAddress, unsigned int uiPort)
{
	// �Ǐ��ϐ��錾
	SOCKET uRet;               // �֐��ďo���̖߂�l
	struct sockaddr_in addr;   // �A�h���X

	if (-1 == m_Socket) {
		return false;
	}

	// �ڑ���̐ݒ�
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(uiPort);
	addr.sin_addr.s_addr = inet_addr(szAddress);

	// �ڑ��v�����s�Ȃ�
	uRet = connect(m_Socket, (const struct sockaddr*)&addr, sizeof(addr));
	if (0 != uRet) {
		return false;
	}

	// ���b�Z�[�W���[�v���J�n����
	m_bCont = true;
	_beginthread(vStartConnThread, 0, this);

	return true;
}

void CConSocket::vStartConnThread(void* pObj)
{
	reinterpret_cast<CConSocket*>(pObj)->vProcMessage();
	printf("[Socket] ConnThread is Terminated\n");

	return;
}

// �ʐM�̏I��
bool CConSocket::bDisconnect(void)
{
	if (false != m_bCont) {
		// �ʐM�̏I����ʍ�
		bSetSendMessage(MSG_QUIT);
		Sleep(200);

		// �p���t���O��OFF�ɂ���
		m_bCont = false;

		// TODO: �X���b�h�̏I���҂����s�Ȃ�
		closesocket(m_Socket);

	}

	return true;
}


bool CConSocket::bSetSendMessage(const char* szMsg)
{
	// �Ǐ��ϐ��錾
	int iLenArg = strlen(szMsg);
	const int ciLenSend = sizeof(m_szMsgSend);

	// ���b�Z�[�W���̃`�F�b�N
	if (ciLenSend < iLenArg) {
		return false;
	}
	m_bSent = false;

	EnterCriticalSection(&m_csSend);
	strcpy_s(m_szMsgSend, szMsg);
	LeaveCriticalSection(&m_csSend);

	/* ���M�����܂ő҂� */
	while (true != m_bSent) {
		Sleep(0);
	}


	return true;
}



// ���b�Z�[�W���[�v
void CConSocket::vProcMessage(void)
{
	// �Ǐ��ϐ��錾
	int iLenRecv = 0;    // ��M�o�b�t�@��


	// �������[�v
	while (true == m_bCont) {

		// ���M���b�Z�[�W������ꍇ
		if ('\0' != m_szMsgSend[0]) {
			EnterCriticalSection(&m_csSend);
			send(m_Socket, m_szMsgSend, strlen(m_szMsgSend), 0);

			// �X�V���s�Ȃ��ꍇ�A��M��҂�
			if (strcmp(m_szMsgSend, MSG_UPDATE) == 0) {
				iLenRecv = recv(m_Socket, m_szMsgRecv, sizeof(m_szMsgRecv), 0);
				
				if (0 < iLenRecv) {
					// �E�B���h�E�ɑ΂��Ď�M�����̃��b�Z�[�W�𑗂�
					::PostMessage(
						m_hWnd,
						WM_USER_PROC, PROC_RECV_SOCK,
						(LPARAM)&m_szMsgRecv);

					iLenRecv = 0;
				}
			}
			// ���M���b�Z�[�W�̃t���b�V��
			m_szMsgSend[0] = '\0';
			LeaveCriticalSection(&m_csSend);
		}
		m_bSent = true;

		// �v���Z�b�T�̐�L��h�����ߖ���
		Sleep(100);
	}

	return;
}

