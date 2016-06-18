#pragma once
/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/
#include "MQTTClient.h"


/*****************************************************************************/
/* �萔��`                                                                  */
/*****************************************************************************/
//#define BRK_ADDRESS ("192.168.137.1")
#define BRK_ADDRESS ("127.0.0.1")             // �u���[�J�[IP�A�h���X
#define CLIENT_ID   ("LEDCamp4 Corner Publisher GUI")       // �N���C�A���gID
#define TOPIC       ("/course/corner")        // ���s�g�s�b�N
#define QOS         (1)                       // �ʐM�i�� [0-2]
#define TIMEOUT     10000L                    // �^�C���A�E�g���� [ms]


class CConMQTT
{
public:
	CConMQTT();                      // �R���X�g���N�^
	~CConMQTT();                     // �f�X�g���N�^
	bool
		bConnect(char*),                // �ڑ�����
		bDisconnect(void),              // �ؒf����
		bPublishMessage(char, int);     // ���b�Z�[�W���s
private:
	static DWORD __stdcall vTryConnect(LPVOID);   // �ڑ��̎��s

	// �R�[���o�b�N�֐�
	static void vConnLost(void*, char*);                              // �r���Őڑ����؂ꂽ�Ƃ�
	static void vDelivered(void*, MQTTClient_deliveryToken);          // ���b�Z�[�W�̔z�������������Ƃ�
	static int iMsgArrived(void*, char*, int, MQTTClient_message*);   // ���b�Z�[�W�����������Ƃ�

private:
	bool m_bConected = false;                                     // �ʐM���
	MQTTClient m_Client;                                          // �N���C�A���g
	static volatile MQTTClient_deliveryToken s_DeliveredToken;    // �z�B�g�[�N���̏��
};