
// Create2Compet.h : PROJECT_NAME アプリケーションのメイン ヘッダー ファイルです。
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH に対してこのファイルをインクルードする前に 'stdafx.h' をインクルードしてください"
#endif

#include "resource.h"		// メイン シンボル


// CCreate2CompetApp:
// このクラスの実装については、Create2Compet.cpp を参照してください。
//

class CCreate2CompetApp : public CWinApp
{
public:
	CCreate2CompetApp();

// オーバーライド
public:
	virtual BOOL InitInstance();

// 実装

	DECLARE_MESSAGE_MAP()
};

extern CCreate2CompetApp theApp;