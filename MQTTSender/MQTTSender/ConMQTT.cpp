// �񓯊���MQTT�ʐM�N���X
/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConMQTT.h"


/*****************************************************************************/
/* ���C�u���������N                                                          */
/*****************************************************************************/
#pragma comment(lib, "paho-mqtt3c.lib")


/*****************************************************************************/
/* �ÓI�����o�ϐ��錾                                                        */
/*****************************************************************************/
volatile MQTTClient_deliveryToken CConMQTT::s_DeliveredToken;


/*****************************************************************************/
/* �����o�֐���`                                                            */
/*****************************************************************************/

// �R���X�g���N�^
CConMQTT::CConMQTT()
{
	return;
}

// �f�X�g���N�^
CConMQTT::~CConMQTT()
{
	// �I�����ɒʐM���m������Ă�����I������
	if (true == m_bConected) {
		bDisconnect();
	}

	return;
}

// �ʐM�̊J�n�����݂�
DWORD __stdcall CConMQTT::vTryConnect(LPVOID lpArg)
{
	// �Ǐ��ϐ��錾
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;    // �ڑ��I�v�V����

	conn_opts.keepAliveInterval = 20;   // �L�[�v�A���C�u [sec]
	conn_opts.cleansession = 1;         // �H

	if (MQTTClient_connect(*reinterpret_cast<MQTTClient*>(lpArg), &conn_opts) != MQTTCLIENT_SUCCESS)
	{
		return -1;
	}

	return 0;
}

bool CConMQTT::bConnect(char* szAddr)
{
	// �Ǐ��ϐ��錾
	HANDLE hThread;     // �X���b�h�n���h��
	DWORD  dwRet;       // �֐��ďo���̖߂�l

	// �N���C�A���g�̍쐬
	MQTTClient_create(&m_Client, szAddr, CLIENT_ID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);


	// �񓯊��ʐM�ɂ�����R�[���o�b�N�̐ݒ�
	MQTTClient_setCallbacks(m_Client, NULL, vConnLost, iMsgArrived, vDelivered);

	hThread = CreateThread(NULL, 0, vTryConnect, &m_Client, 0, NULL);

	/* *** MQTTClient_connectOptions��connectTimeout�̎g������ *** */
	/* *** ������Ȃ������̂ŃX���b�h�̏I���܂��őΉ�          *** */
	/* *** 3000 [ms] �Ń^�C���A�E�g                            *** */
	dwRet = WaitForSingleObject(hThread, 3000);
	if (WAIT_TIMEOUT == dwRet) {
		TerminateThread(hThread, -1);

		return false;
	}

	m_bConected = true;
	
	return true;
}

bool CConMQTT::bPublishMessage(char cCorner, int iDir)
{
	// �Ǐ��ϐ��錾
	int iRet;             // �֐��ďo���̖߂�l
	char szTopic[31];     // �g�s�b�N��
	char szMsg[31];       // ���b�Z�[�W�y�C���[�h
	MQTTClient_message stMsg = MQTTClient_message_initializer;     // ���b�Z�[�W

	// �g�s�b�N�̌���
	sprintf_s(szTopic, "%s/%c", TOPIC, cCorner);

	// ���b�Z�[�W�̌���
	_itoa_s(iDir, szMsg, 10);
	stMsg.payload = szMsg;
	stMsg.payloadlen = strlen(szMsg);
	stMsg.qos = QOS;
	stMsg.retained = 0;

	iRet = MQTTClient_publishMessage(m_Client, szTopic, &stMsg, (MQTTClient_deliveryToken*)&s_DeliveredToken);
	if (MQTTCLIENT_SUCCESS != iRet) {
		return false;
	}

	return true;
}


bool CConMQTT::bDisconnect(void)
{
	// �Ǐ��ϐ��錾
	int iRet;

	// �ʐM��ؒf����
	iRet = MQTTClient_disconnect(m_Client, TIMEOUT);
	if (MQTTCLIENT_SUCCESS != iRet) {
		return false;
	}

	MQTTClient_destroy(&m_Client);

	m_bConected = false;

	return true;
}

// �ʐM�ؒf��
void CConMQTT::vConnLost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

// ���b�Z�[�W�z����
void CConMQTT::vDelivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	s_DeliveredToken = dt;
}


// ���b�Z�[�W������
int CConMQTT::iMsgArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	// �Ǐ��ϐ��錾
	int i;                 // ���[�v�J�E���^
	char* payloadptr;      // �y�C���[�h�̐擪�A�h���X


	// �g�s�b�N�ƃ��b�Z�[�W�̕\��
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");

	payloadptr = (char*)message->payload;
	for (i = 0; i<message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');


	// �������
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);

	return true;
}
