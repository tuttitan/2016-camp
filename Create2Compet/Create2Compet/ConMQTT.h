#pragma once

#include <MQTTClient.h>

#define BRK_ADDRESS ("192.168.137.1")
#define CLIENT_ID   ("LEDCamp4 Viewer")
#define TOPIC       ("/course/corner/#")
#define QOS         (1)
#define TIMEOUT     10000L

/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
class CDrawMap;


/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
class CConMQTT
{
public:
	//CConMQTT(CDrawMap*);
	CConMQTT();
	~CConMQTT();
	bool bConnect(const char*);
	bool bDisconnect(void);

	void vRelateObject(CDrawMap*);

private:
	// コールバック関数
	static void connlost(void *context, char *cause);
	static void delivered(void *context, MQTTClient_deliveryToken dt);
	static int msgarrvd(void *context, char *topicName, int topicLen, MQTTClient_message *message);

	static DWORD __stdcall startConnect(LPVOID pvArg);
private:
	FILE* m_fpConsole = NULL;
	MQTTClient m_client;
	static volatile MQTTClient_deliveryToken m_deliveredtoken;

	static CDrawMap* s_pDrawMap;
};