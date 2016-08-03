#include "stdafx.h"


// 文字列を切り分けて数字を配列に格納する
void vSplitString(const char* cszMsg, const char* cszDelim, unsigned int uiDstVals[])
{
	// 局所変数宣言
	char* pszMsg = NULL;
	char* pszMsgRem;
	int i = 0;
	char* pszDummy;

	pszMsg = new char[strlen(cszMsg) + 1];

	if (NULL == pszMsg) {
		return;
	}

	strcpy_s(pszMsg, strlen(cszMsg) + 1, cszMsg);


	pszMsgRem = strtok_s(pszMsg, cszDelim, &pszDummy);
	uiDstVals[i] = atoi(pszMsgRem);
	++i;

	while (NULL != pszMsgRem) {
		pszMsgRem = strtok_s(NULL, cszDelim, &pszDummy);
		if (NULL != pszMsgRem) {
			uiDstVals[i] = atoi(pszMsgRem);
			++i;
		}
	}

	if (NULL != pszMsg) {
		delete[] pszMsg;
	}

	return;
}




// 外部プロセスの起動
bool bLaunchExtProc(
	//wchar_t* ptszCommand,
	CString strCommand,
	PROCESS_INFORMATION* pProcInfo)
{
	// 局所変数宣言
	BOOL bRet = FALSE;
	TCHAR tszAppName[255];   // 実行プロセス名および引数
	STARTUPINFO stStartupInfo = { 0 };

	// コマンドのコピー
	//_tcscpy_s(tszAppName, ptszCommand);
	_tcscpy_s(tszAppName, strCommand);


	// スタートアップ情報の設定
	stStartupInfo.cb = sizeof(stStartupInfo);
	stStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	stStartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;

	// プロセスの起動
	bRet = CreateProcess(NULL, tszAppName,
		NULL, NULL, FALSE,
		//NORMAL_PRIORITY_CLASS,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
		NULL, NULL, &stStartupInfo, pProcInfo);

	// スレッドハンドルは不要なので閉じる
	CloseHandle(pProcInfo->hThread);

	return (FALSE != bRet) ? true : false;
}
