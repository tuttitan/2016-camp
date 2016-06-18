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
#define   POINT_SEED   (2244)           // 乱数シード
#define   POINT_INIT   ("1,1,2,2,3,3")  // 点数の初期値

#define   SOCKET_PORT  (4000)           // ポート番号


/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
class CDrawMap;

/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
class CConSocket
{
public:
	CConSocket();
	~CConSocket();
	bool 
		bConnect(const char*, unsigned int),    // 通信の開始
		bDisconnect(void),         // 通信の終了
		bSetSendMessage(const char* szMsg);

	void 
		vRelateObject(CDrawMap* pObj),
		vSetPosCorner(char);

private:
	static void vStartConnThread(void*);

	void vProcMessage(void);

	volatile bool m_bSent;  // 送信完了フラグ



private:
	//CSocket m_Socket;       // ソケット
	SOCKET m_Socket;
	char m_szMsgSend[127];    // 送信メッセージ
	char m_szMsgRecv[127];    // 受信メッセージ
	bool m_bCont = true;    // 継続フラグ
	//FILE* m_fpConsole = NULL;         // コンソール（ログ用）
	CRITICAL_SECTION m_csSend;
	CDrawMap* m_pDrawMap;

	char m_cCurrPos;
};