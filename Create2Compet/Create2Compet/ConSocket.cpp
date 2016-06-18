// �\�P�b�g�ʐM�̂��߂̃N���X
// Client��



/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConSocket.h"
#include "DrawMap.h"
#include "utility.h"

/*****************************************************************************/
/* ���C�u���������N                                                          */
/*****************************************************************************/
#pragma comment(lib,"Ws2_32.lib")




/*****************************************************************************/
/* �����o�֐���`                                                            */
/*****************************************************************************/
// �R���X�g���N�^
CConSocket::CConSocket()
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


	return;
}

// �f�X�g���N�^
CConSocket::~CConSocket()
{


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
	//BOOL bRet = FALSE;
	SOCKET uRet;
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
	//bRet = m_Socket.Connect((LPCTSTR)szAddress, uiPort);
	//bRet = m_Socket.Connect(TEXT("127.0.0.1"), uiPort);
	if (0 != uRet) {
		return false;
	}

	// ���b�Z�[�W���[�v���J�n����
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


		m_bCont = false;

		// TODO: �X���b�h�̏I���҂����s�Ȃ�
		closesocket(m_Socket);

		//m_Socket.Close();
	}

	return true;
}
bool CConSocket::bSetSendMessage(const char* szMsg)
{
	// �Ǐ��ϐ��錾
	int iLenArg = strlen(szMsg);
	const int ciLenSend = sizeof(m_szMsgSend);

	if (ciLenSend < iLenArg) {
		return false;
	}
	m_bSent = false;

	EnterCriticalSection(&m_csSend);
	strcpy_s(m_szMsgSend, szMsg);
	LeaveCriticalSection(&m_csSend);

	while (true != m_bSent) {
		/* ���M�����܂ő҂� */
	}
	//Sleep(500);

	return true;
}



// ���b�Z�[�W���[�v
void CConSocket::vProcMessage(void)
{
	int iLenRecv = 0;    // ��M�o�b�t�@��
	unsigned int uiPoints[CORNER_NUM];
	//static char c = 'b';

	// �������[�v
	while (true == m_bCont) {

		//iLenRecv = 0;
		// ���M���b�Z�[�W������ꍇ
		EnterCriticalSection(&m_csSend);

		
		if ('\0' != m_szMsgSend[0]) {
			send(m_Socket, m_szMsgSend, strlen(m_szMsgSend), 0);
			//m_Socket.Send(m_szMsgSend, strlen(m_szMsgSend));
			

			// �X�V���s�Ȃ��ꍇ�A��M��҂�
			if (strcmp(m_szMsgSend, MSG_UPDATE) == 0) {
				iLenRecv = recv(m_Socket, m_szMsgRecv, sizeof(m_szMsgRecv), 0);
				//iLenRecv = m_Socket.Receive(m_szMsgRecv, sizeof(m_szMsgRecv));
				if (0 < iLenRecv) {
					vSplitString(m_szMsgRecv, " ,[]", uiPoints);
					//sscanf_s(m_szMsgRecv, "[%u, %u, %u, %u, %u, %u]",
					//	&uiPoints[0], &uiPoints[1], &uiPoints[2], &uiPoints[3], &uiPoints[4], &uiPoints[5]);
					m_pDrawMap->vUpdatePoints(m_cCurrPos, uiPoints);
#if 0
					// �b��
					c++;
					if (c > 'f') {
						c = 'a';
					}
#endif /* 0 */
					iLenRecv = 0;
				}
			}

		}
		// ���M���b�Z�[�W�̃t���b�V��
		m_szMsgSend[0] = '\0';
		m_bSent = true;
		LeaveCriticalSection(&m_csSend);

		Sleep(0);
		//Sleep(2000);
		

		//Sleep(0);
	}

	return;
}
void CConSocket::vSetPosCorner(char cPos)
{
	m_cCurrPos = cPos;

	return;
}


void CConSocket::vRelateObject(CDrawMap* pObj)
{
	m_pDrawMap = pObj;

	return;
}