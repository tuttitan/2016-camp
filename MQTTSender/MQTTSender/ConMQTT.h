#pragma once
/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "MQTTClient.h"


/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
//#define BRK_ADDRESS ("192.168.137.1")
#define BRK_ADDRESS ("127.0.0.1")             // ブローカーIPアドレス
#define CLIENT_ID   ("LEDCamp4 Corner Publisher GUI")       // クライアントID
#define TOPIC       ("/course/corner")        // 発行トピック
#define QOS         (1)                       // 通信品質 [0-2]
#define TIMEOUT     10000L                    // タイムアウト時間 [ms]


class CConMQTT
{
public:
	CConMQTT();                      // コンストラクタ
	~CConMQTT();                     // デストラクタ
	bool
		bConnect(char*),                // 接続処理
		bDisconnect(void),              // 切断処理
		bPublishMessage(char, int);     // メッセージ発行
private:
	static DWORD __stdcall vTryConnect(LPVOID);   // 接続の試行

	// コールバック関数
	static void vConnLost(void*, char*);                              // 途中で接続が切れたとき
	static void vDelivered(void*, MQTTClient_deliveryToken);          // メッセージの配送が完了したとき
	static int iMsgArrived(void*, char*, int, MQTTClient_message*);   // メッセージが到着したとき

private:
	bool m_bConected = false;                                     // 通信状態
	MQTTClient m_Client;                                          // クライアント
	static volatile MQTTClient_deliveryToken s_DeliveredToken;    // 配達トークンの状態
};