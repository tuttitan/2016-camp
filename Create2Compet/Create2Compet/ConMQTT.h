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
#define CLIENT_ID   ("LEDCamp4 Viewer")       // クライアントID
#define TOPIC       ("/course/corner/#")      // 購読トピック
#define QOS         (1)                       // 通信品質 [0-2]
#define TIMEOUT     10000L                    // タイムアウト時間 [ms]

/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
//class CDrawMap;
//class CConSocket;


/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
class CConMQTT
{
public:
	CConMQTT(HWND);                 // コンストラクタ
	~CConMQTT();                    // デストラクタ
	bool bConnect(const char*);     // 通信開始
	bool bDisconnect(void);         // 通信終了


private:
	// コールバック関数
	static void connlost(void *context, char *cause);
	static void delivered(void *context, MQTTClient_deliveryToken dt);
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

	static DWORD __stdcall startConnect(LPVOID pvArg);    // 通信開始試行スレッド

private:
	//FILE* m_fpConsole = NULL;         // コンソール（ログ用）
	MQTTClient m_client;    // クライアント
	static volatile MQTTClient_deliveryToken m_deliveredtoken;


	HWND m_hWnd = NULL;  // ウィンドウハンドル

};