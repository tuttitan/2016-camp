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