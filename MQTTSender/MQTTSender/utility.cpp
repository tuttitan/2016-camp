#include "stdafx.h"


// •¶š—ñ‚ğØ‚è•ª‚¯‚Ä”š‚ğ”z—ñ‚ÉŠi”[‚·‚é
void vSplitString(const char* cszMsg, const char* cszDelim, unsigned int uiDstVals[])
{
	// ‹ÇŠ•Ï”éŒ¾
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