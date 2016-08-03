#include "stdafx.h"


// �������؂蕪���Đ�����z��Ɋi�[����
void vSplitString(const char* cszMsg, const char* cszDelim, unsigned int uiDstVals[])
{
	// �Ǐ��ϐ��錾
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




// �O���v���Z�X�̋N��
bool bLaunchExtProc(
	//wchar_t* ptszCommand,
	CString strCommand,
	PROCESS_INFORMATION* pProcInfo)
{
	// �Ǐ��ϐ��錾
	BOOL bRet = FALSE;
	TCHAR tszAppName[255];   // ���s�v���Z�X������ш���
	STARTUPINFO stStartupInfo = { 0 };

	// �R�}���h�̃R�s�[
	//_tcscpy_s(tszAppName, ptszCommand);
	_tcscpy_s(tszAppName, strCommand);


	// �X�^�[�g�A�b�v���̐ݒ�
	stStartupInfo.cb = sizeof(stStartupInfo);
	stStartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	stStartupInfo.wShowWindow = SW_SHOWMINNOACTIVE;

	// �v���Z�X�̋N��
	bRet = CreateProcess(NULL, tszAppName,
		NULL, NULL, FALSE,
		//NORMAL_PRIORITY_CLASS,
		NORMAL_PRIORITY_CLASS | CREATE_NEW_CONSOLE | CREATE_NEW_PROCESS_GROUP,
		NULL, NULL, &stStartupInfo, pProcInfo);

	// �X���b�h�n���h���͕s�v�Ȃ̂ŕ���
	CloseHandle(pProcInfo->hThread);

	return (FALSE != bRet) ? true : false;
}
