// 非同期のMQTT通信クラス
/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConMQTT.h"


/*****************************************************************************/
/* ライブラリリンク                                                          */
/*****************************************************************************/
#pragma comment(lib, "paho-mqtt3c.lib")


/*****************************************************************************/
/* 静的メンバ変数宣言                                                        */
/*****************************************************************************/
volatile MQTTClient_deliveryToken CConMQTT::s_DeliveredToken;


/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/

// コンストラクタ
CConMQTT::CConMQTT()
{
	return;
}

// デストラクタ
CConMQTT::~CConMQTT()
{
	// 終了時に通信が確立されていたら終了する
	if (true == m_bConected) {
		bDisconnect();
	}

	return;
}

// 通信の開始を試みる
DWORD __stdcall CConMQTT::vTryConnect(LPVOID lpArg)
{
	// 局所変数宣言
	MQTTClient_connectOptions conn_opts = MQTTClient_connectOptions_initializer;    // 接続オプション

	conn_opts.keepAliveInterval = 20;   // キープアライブ [sec]
	conn_opts.cleansession = 1;         // ？

	if (MQTTClient_connect(*reinterpret_cast<MQTTClient*>(lpArg), &conn_opts) != MQTTCLIENT_SUCCESS)
	{
		return -1;
	}

	return 0;
}

bool CConMQTT::bConnect(char* szAddr)
{
	// 局所変数宣言
	HANDLE hThread;     // スレッドハンドル
	DWORD  dwRet;       // 関数呼出しの戻り値

	// クライアントの作成
	MQTTClient_create(&m_Client, szAddr, CLIENT_ID,
		MQTTCLIENT_PERSISTENCE_NONE, NULL);


	// 非同期通信におけるコールバックの設定
	MQTTClient_setCallbacks(m_Client, NULL, vConnLost, iMsgArrived, vDelivered);

	hThread = CreateThread(NULL, 0, vTryConnect, &m_Client, 0, NULL);

	/* *** MQTTClient_connectOptionsのconnectTimeoutの使い方が *** */
	/* *** 分からなかったのでスレッドの終了まちで対応          *** */
	/* *** 3000 [ms] でタイムアウト                            *** */
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
	// 局所変数宣言
	int iRet;             // 関数呼出しの戻り値
	char szTopic[31];     // トピック名
	char szMsg[31];       // メッセージペイロード
	MQTTClient_message stMsg = MQTTClient_message_initializer;     // メッセージ

	// トピックの決定
	sprintf_s(szTopic, "%s/%c", TOPIC, cCorner);

	// メッセージの決定
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
	// 局所変数宣言
	int iRet;

	// 通信を切断する
	iRet = MQTTClient_disconnect(m_Client, TIMEOUT);
	if (MQTTCLIENT_SUCCESS != iRet) {
		return false;
	}

	MQTTClient_destroy(&m_Client);

	m_bConected = false;

	return true;
}

// 通信切断時
void CConMQTT::vConnLost(void *context, char *cause)
{
	printf("\nConnection lost\n");
	printf("     cause: %s\n", cause);
}

// メッセージ配送時
void CConMQTT::vDelivered(void *context, MQTTClient_deliveryToken dt)
{
	printf("Message with token value %d delivery confirmed\n", dt);
	s_DeliveredToken = dt;
}


// メッセージ到着時
int CConMQTT::iMsgArrived(void *context, char *topicName, int topicLen, MQTTClient_message *message)
{
	// 局所変数宣言
	int i;                 // ループカウンタ
	char* payloadptr;      // ペイロードの先頭アドレス


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


	// 解放処理
	MQTTClient_freeMessage(&message);
	MQTTClient_free(topicName);

	return true;
}
