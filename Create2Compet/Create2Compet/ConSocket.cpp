// ソケット通信のためのクラス
// Client側



/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConSocket.h"
#include "DrawMap.h"
#include "utility.h"

/*****************************************************************************/
/* ライブラリリンク                                                          */
/*****************************************************************************/
#pragma comment(lib,"Ws2_32.lib")




/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/
// コンストラクタ
CConSocket::CConSocket()
{
	// 局所変数宣言
	WSADATA wsaData;

	// クリティカルセクションの初期化
	InitializeCriticalSection(&m_csSend);

	// メッセージの初期化
	m_szMsgSend[0] = '\0';
	m_szMsgRecv[0] = '\0';

	// Winsockの初期化
	WSAStartup(MAKEWORD(2, 0), &wsaData);

	// ソケットの作成
	m_Socket = socket(PF_INET, SOCK_STREAM, 0);


	return;
}

// デストラクタ
CConSocket::~CConSocket()
{


	bDisconnect();

	// winsockの終了処理
	WSACleanup();

	// コンソールを閉じる
	//printf("[Socket]: CConMQTT Destructor\n");
	//fclose(m_fpConsole);
	//::FreeConsole();

	// クリティカルセクションの解放
	DeleteCriticalSection(&m_csSend);

	return;
}

// 通信の開始
bool CConSocket::bConnect(const char* szAddress, unsigned int uiPort)
{
	// 局所変数宣言
	//BOOL bRet = FALSE;
	SOCKET uRet;
	struct sockaddr_in addr;   // アドレス

	if (-1 == m_Socket) {
		return false;
	}

	// 接続先の設定
	memset(&addr, 0, sizeof(addr));
	addr.sin_family = PF_INET;
	addr.sin_port = htons(uiPort);
	addr.sin_addr.s_addr = inet_addr(szAddress);



	// 接続要求を行なう
	uRet = connect(m_Socket, (const struct sockaddr*)&addr, sizeof(addr));
	//bRet = m_Socket.Connect((LPCTSTR)szAddress, uiPort);
	//bRet = m_Socket.Connect(TEXT("127.0.0.1"), uiPort);
	if (0 != uRet) {
		return false;
	}

	// メッセージループを開始する
	_beginthread(vStartConnThread, 0, this);

	return true;
}

void CConSocket::vStartConnThread(void* pObj)
{
	reinterpret_cast<CConSocket*>(pObj)->vProcMessage();
	printf("[Socket] ConnThread is Terminated\n");

	return;
}

// 通信の終了
bool CConSocket::bDisconnect(void)
{
	if (false != m_bCont) {


		m_bCont = false;

		// TODO: スレッドの終了待ちを行なう
		closesocket(m_Socket);

		//m_Socket.Close();
	}

	return true;
}
bool CConSocket::bSetSendMessage(const char* szMsg)
{
	// 局所変数宣言
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
		/* 送信完了まで待つ */
	}
	//Sleep(500);

	return true;
}



// メッセージループ
void CConSocket::vProcMessage(void)
{
	int iLenRecv = 0;    // 受信バッファ長
	unsigned int uiPoints[CORNER_NUM];
	//static char c = 'b';

	// 無限ループ
	while (true == m_bCont) {

		//iLenRecv = 0;
		// 送信メッセージがある場合
		EnterCriticalSection(&m_csSend);

		
		if ('\0' != m_szMsgSend[0]) {
			send(m_Socket, m_szMsgSend, strlen(m_szMsgSend), 0);
			//m_Socket.Send(m_szMsgSend, strlen(m_szMsgSend));
			

			// 更新を行なう場合、受信を待つ
			if (strcmp(m_szMsgSend, MSG_UPDATE) == 0) {
				iLenRecv = recv(m_Socket, m_szMsgRecv, sizeof(m_szMsgRecv), 0);
				//iLenRecv = m_Socket.Receive(m_szMsgRecv, sizeof(m_szMsgRecv));
				if (0 < iLenRecv) {
					vSplitString(m_szMsgRecv, " ,[]", uiPoints);
					//sscanf_s(m_szMsgRecv, "[%u, %u, %u, %u, %u, %u]",
					//	&uiPoints[0], &uiPoints[1], &uiPoints[2], &uiPoints[3], &uiPoints[4], &uiPoints[5]);
					m_pDrawMap->vUpdatePoints(m_cCurrPos, uiPoints);
#if 0
					// 暫定
					c++;
					if (c > 'f') {
						c = 'a';
					}
#endif /* 0 */
					iLenRecv = 0;
				}
			}

		}
		// 送信メッセージのフラッシュ
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