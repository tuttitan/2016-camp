// ソケット通信のためのクラス
// Client側



/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "ConSocket.h"
#include "DrawMap.h"
#include "utility.h"

#include <WinSock2.h>

/*****************************************************************************/
/* ライブラリリンク                                                          */
/*****************************************************************************/
#pragma comment(lib,"Ws2_32.lib")




/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/
// コンストラクタ
CConSocket::CConSocket(HWND hWnd)
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

	// 親ウィンドウの関連付け
	m_hWnd = hWnd;

	return;
}

// デストラクタ
CConSocket::~CConSocket()
{
	// 通信の切断
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
	SOCKET uRet;               // 関数呼出しの戻り値
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
	if (0 != uRet) {
		return false;
	}

	// メッセージループを開始する
	m_bCont = true;
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
		// 通信の終了を通告
		bSetSendMessage(MSG_QUIT);
		Sleep(200);

		// 継続フラグをOFFにする
		m_bCont = false;

		// TODO: スレッドの終了待ちを行なう
		closesocket(m_Socket);

	}

	return true;
}


bool CConSocket::bSetSendMessage(const char* szMsg)
{
	// 局所変数宣言
	int iLenArg = strlen(szMsg);
	const int ciLenSend = sizeof(m_szMsgSend);

	// メッセージ長のチェック
	if (ciLenSend < iLenArg) {
		return false;
	}
	m_bSent = false;

	EnterCriticalSection(&m_csSend);
	strcpy_s(m_szMsgSend, szMsg);
	LeaveCriticalSection(&m_csSend);

	/* 送信完了まで待つ */
	while (true != m_bSent) {
		Sleep(0);
	}


	return true;
}



// メッセージループ
void CConSocket::vProcMessage(void)
{
	// 局所変数宣言
	int iLenRecv = 0;    // 受信バッファ長


	// 無限ループ
	while (true == m_bCont) {

		// 送信メッセージがある場合
		if ('\0' != m_szMsgSend[0]) {
			EnterCriticalSection(&m_csSend);
			send(m_Socket, m_szMsgSend, strlen(m_szMsgSend), 0);

			// 更新を行なう場合、受信を待つ
			if (strcmp(m_szMsgSend, MSG_UPDATE) == 0) {
				iLenRecv = recv(m_Socket, m_szMsgRecv, sizeof(m_szMsgRecv), 0);
				
				if (0 < iLenRecv) {
					// ウィンドウに対して受信完了のメッセージを送る
					::PostMessage(
						m_hWnd,
						WM_USER_PROC, PROC_RECV_SOCK,
						(LPARAM)&m_szMsgRecv);

					iLenRecv = 0;
				}
			}
			// 送信メッセージのフラッシュ
			m_szMsgSend[0] = '\0';
			LeaveCriticalSection(&m_csSend);
		}
		m_bSent = true;

		// プロセッサの占有を防ぐため眠る
		Sleep(100);
	}

	return;
}

