#pragma once
/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#define   MSG_INIT    ("init")          // 初期化指示メッセージ
#define   MSG_UPDATE  ("update")        // 更新指示メッセージ
#define   MSG_QUIT    ("quit")          // 終了指示メッセージ

#define   SERV_ADDRESS ("127.0.0.1")    // 点数表問い合わせサーバーIPアドレス
//#define   SERV_ADDRESS ("192.168.137.11")    // 点数表問い合わせサーバーIPアドレス
#define   POINT_SEED   (544)           // 乱数シード
//#define   POINT_INIT   ("1,1,2,2,3,3")  // 点数の初期値


// TODO: INIファイルで指定できるようにする
// TODO: デフォルト値を定数で定義しておく
#define   POINT_INIT   ("2,1,3,3,2,1")  // 点数の初期値

#define   SOCKET_PORT  (4000)           // ポート番号
#define   SCRIPT_NANE  ("tcp_server.py")
#define CMD_PYTHON     ("python")


/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/


/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
class CConSocket
{
public:
	CConSocket(HWND);     // コンストラクタ
	~CConSocket();        // デストラクタ
	bool 
		bConnect(const char*, unsigned int),    // 通信の開始
		bDisconnect(void),                      // 通信の終了
		bSetSendMessage(const char* szMsg);     // 送信メッセージの設定

private:
	static void vStartConnThread(void*);    // メッセージループスレッド処理
	void vProcMessage(void);                // メッセージ処理ループ
	
private:
	SOCKET m_Socket = -1;             // ソケット
	HWND m_hWnd;                      // ウィンドウハンドル
	char m_szMsgSend[127];            // 送信メッセージ
	char m_szMsgRecv[127];            // 受信メッセージ
	volatile bool m_bCont = false;    // 継続フラグ
	volatile bool m_bSent = true;     // 送信完了フラグ
	CRITICAL_SECTION m_csSend;        // 送信メッセージのクリティカルセッション
};