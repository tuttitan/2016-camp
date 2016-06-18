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
#define   POINT_SEED   (2244)           // �����V�[�h
#define   POINT_INIT   ("1,1,2,2,3,3")  // �_���̏����l

#define   SOCKET_PORT  (4000)           // �|�[�g�ԍ�


/*****************************************************************************/
/* �N���X�錾                                                                */
/*****************************************************************************/
class CDrawMap;

/*****************************************************************************/
/* �N���X��`                                                                */
/*****************************************************************************/
class CConSocket
{
public:
	CConSocket();
	~CConSocket();
	bool 
		bConnect(const char*, unsigned int),    // �ʐM�̊J�n
		bDisconnect(void),         // �ʐM�̏I��
		bSetSendMessage(const char* szMsg);

	void 
		vRelateObject(CDrawMap* pObj),
		vSetPosCorner(char);

private:
	static void vStartConnThread(void*);

	void vProcMessage(void);

	volatile bool m_bSent;  // ���M�����t���O



private:
	//CSocket m_Socket;       // �\�P�b�g
	SOCKET m_Socket;
	char m_szMsgSend[127];    // ���M���b�Z�[�W
	char m_szMsgRecv[127];    // ��M���b�Z�[�W
	bool m_bCont = true;    // �p���t���O
	//FILE* m_fpConsole = NULL;         // �R���\�[���i���O�p�j
	CRITICAL_SECTION m_csSend;
	CDrawMap* m_pDrawMap;

	char m_cCurrPos;
};