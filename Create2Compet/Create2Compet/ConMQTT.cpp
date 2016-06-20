// MQTT通信のためのクラス

/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConMQTT.h"
#include "DrawMap.h"
#include "ConSocket.h"

#include <windows.h>

/*****************************************************************************/
/* 静的メンバ変数宣言                                                        */
/*****************************************************************************/
volatile MQTTClient_deliveryToken CConMQTT::m_deliveredtoken;
//CDrawMap* CConMQTT::s_pDrawMap = NULL;
//CConSocket* CConMQTT::s_pConSocket = NULL;


/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/
// コンストラクタ
CConMQTT::CConMQTT(HWND hWnd)
{
	// 親ウィンドウの関連付け
	m_hWnd = hWnd;
}

// デストラクタ
CConMQTT::~CConMQTT()
{

}


// 通信の開始を試みる
DWORD __stdcall CConMQTT::startConnect(LPVOID lpArg)
{
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	conn_opts.keepAliveInterval = 20;
	conn_opts.cleansession = 1;

	if (MQTTClient_connect(*reinterpret_cast<MQTTClient*>(lpArg), &conn_opts) != MQTTCLIENT_SUCCESS)
	{
		printf("Failed to connect\n");
		return -1;
		//exit(-1);
	}

	return 0;
}

// 通信の開始
bool CConMQTT::bConnect(const char* szAddress)
{
	//MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;

	MQTTClient_create(&m_client, szAddress, CLIENT_ID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);
	//conn_opts.keepAliveInterval = 20;
	//conn_opts.cleansession = 1;
	//conn_opts.connectTimeout = 0;
	//conn_opts.retryInterval = 3;

	// 非同期通信におけるコールバックの設定
	MQTTClient_setCallbacks(m_client, this, connlost, msgarrvd, delivered);

	//int iWaitTime = 10;
	//_beginthread(waitConnect, 0x200, &iWaitTime);
	HANDLE hThread;
	DWORD dwRet;
	hThread = CreateThread(NULL, 0, startConnect, &m_client, 0, NULL);

	// MQTTClient_connectOptionsのconnectTimeoutの使い方が分からなかったので
	// スレッドの終了まちで対応
	dwRet = WaitForSingleObject(hThread, 3000);
	if (WAIT_TIMEOUT == dwRet) {
		TerminateThread(hThread, -1);

		return false;
	}


	printf("Subscribing to topic %s\nfor client %s using QoS%d\n", 
		TOPIC, CLIENT_ID, QOS);
	MQTTClient_subscribe(m_client, TOPIC, QOS);

	return true;
}


// 通信の終了
bool CConMQTT::bDisconnect(void)
{
	MQTTClient_disconnect(m_client, TIMEOUT);
	MQTTClient_destroy(&m_client);
	return true;
}

// 通信切断時
void CConMQTT::connlost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

// メッセージ配送時
void CConMQTT::delivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	m_deliveredtoken = dt;
}


// メッセージ到着時
int CConMQTT::msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	// 局所変数宣言
	int i;                 // ループカウンタ
	char* payloadptr;      // ペイロードの先頭アドレス
	int iLenTopic;         // トピック長
	int iMsg;              // メッセージ [0, 1]

	// トピックとメッセージの表示
	printf("Message arrived\n");
	printf("     topic: %s\n", topicName);
	printf("   message: ");

	payloadptr = (char*)message->payload;
	for (i = 0; i<message->payloadlen; i++)
	{
		putchar(*payloadptr++);
	}
	putchar('\n');

	// トピックの最後の文字を抽出
	iLenTopic = strlen(topicName);
	printf("--- %c --- (%d)\n", topicName[iLenTopic -1], iLenTopic);


	// メッセージを比較（メッセージは[0, 1]しか到着しない想定）
	// 1: enterの場合のみ、処理する
	payloadptr = (char*)message->payload;
	iMsg = atoi(payloadptr);

	if (1 == iMsg) {
		printf("[Message Arrived] Enter Sensor: %c\n", topicName[iLenTopic - 1]);
		//reinterpret_cast<CConMQTT*>(context)->vTestMethod();
		::PostMessage(
			reinterpret_cast<CConMQTT*>(context)->m_hWnd, 
			WM_USER_PROC, PROC_RECV_MQTT, 
			(LPARAM)&topicName[iLenTopic - 1]);
	}
	else {
		printf("[Message Arrived] Leave Sensor: %c\n", topicName[iLenTopic - 1]);
	}

	// 解放処理
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);

	return 1;
}
