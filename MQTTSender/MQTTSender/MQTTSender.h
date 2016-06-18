
// MQTTSender.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CMQTTSenderApp:
// このクラスの実装については、MQTTSender.cpp を参照してください。
//

class CMQTTSenderApp : public CWinApp
{
public:
	CMQTTSenderApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CMQTTSenderApp theApp;