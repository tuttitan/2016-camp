#pragma once
/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* �萔��`                                                                  */
/*****************************************************************************/
#define   MSG_INIT    ("init")          // �������w�����b�Z�[�W
#define   MSG_UPDATE  ("update")        // �X�V�w�����b�Z�[�W
#define   MSG_QUIT    ("quit")          // �I���w�����b�Z�[�W

#define   SERV_ADDRESS ("127.0.0.1")    // �_���\�₢���킹�T�[�o�[IP�A�h���X
//#define   SERV_ADDRESS ("192.168.137.11")    // �_���\�₢���킹�T�[�o�[IP�A�h���X
#define   POINT_SEED   (544)           // �����V�[�h
//#define   POINT_INIT   ("1,1,2,2,3,3")  // �_���̏����l


// TODO: INI�t�@�C���Ŏw��ł���悤�ɂ���
// TODO: �f�t�H���g�l��萔�Œ�`���Ă���
#define   POINT_INIT   ("2,1,3,3,2,1")  // �_���̏����l

#define   SOCKET_PORT  (4000)           // �|�[�g�ԍ�
#define   SCRIPT_NANE  ("tcp_server.py")
#define CMD_PYTHON     ("python")


/*****************************************************************************/
/* �N���X�錾                                                                */
/*****************************************************************************/


/*****************************************************************************/
/* �N���X��`                                                                */
/*****************************************************************************/
class CConSocket
{
public:
	CConSocket(HWND);     // �R���X�g���N�^
	~CConSocket();        // �f�X�g���N�^
	bool 
		bConnect(const char*, unsigned int),    // �ʐM�̊J�n
		bDisconnect(void),                      // �ʐM�̏I��
		bSetSendMessage(const char* szMsg);     // ���M���b�Z�[�W�̐ݒ�

private:
	static void vStartConnThread(void*);    // ���b�Z�[�W���[�v�X���b�h����
	void vProcMessage(void);                // ���b�Z�[�W�������[�v
	
private:
	SOCKET m_Socket = -1;             // �\�P�b�g
	HWND m_hWnd;                      // �E�B���h�E�n���h��
	char m_szMsgSend[127];            // ���M���b�Z�[�W
	char m_szMsgRecv[127];            // ��M���b�Z�[�W
	volatile bool m_bCont = false;    // �p���t���O
	volatile bool m_bSent = true;     // ���M�����t���O
	CRITICAL_SECTION m_csSend;        // ���M���b�Z�[�W�̃N���e�B�J���Z�b�V����
};