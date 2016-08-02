// inifile�������N���X

/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "IniConfig.h"

#include "ConSocket.h"    // POINT_INIT, SOCKET_PORT
#include "DrawMap.h"      // PENALTY_POINT

/*****************************************************************************/
/* �����o�֐���`                                                            */
/*****************************************************************************/

CIniConfig::CIniConfig()
{
	// �Ǐ��ϐ��錾
	wchar_t wszFilePath[MAX_PATH];

	// INI�t�@�C���̃t���p�X���̌���
	GetCurrentDirectory(sizeof(wszFilePath) - 1, wszFilePath);
	wcscat_s(wszFilePath, TEXT("\\"));
	wcscat_s(wszFilePath, INI_FILE_NAME);
	
	m_strFileName = wszFilePath;


	// INI�t�@�C������L�[�l��ǂݍ���
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_IPADDR1, STR_NULL, m_strProfiles[PROF_GEN_IPADDR1]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_IPADDR2, STR_NULL, m_strProfiles[PROF_GEN_IPADDR2]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_PYTHON_BIN, CMD_PYTHON, m_strProfiles[PROF_GEN_PYTHON_BIN]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_PENALTY, PENALTY_POINT, m_strProfiles[PROF_GEN_PENALTY]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_INIT_PT, POINT_INIT, m_strProfiles[PROF_GEN_INIT_PT]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_RAND_SEED, POINT_SEED, m_strProfiles[PROF_GEN_RAND_SEED]);
	vLoadKeyValue(SECT_GENERAL, KEY_GEN_COMP_TIME, INITIAL_TIME, m_strProfiles[PROF_GEN_COMP_TIME]);

	vLoadKeyValue(SECT_MQTT, KEY_MQT_PATH, STR_NULL, m_strProfiles[PROF_MQT_PATH]);
	vLoadKeyValue(SECT_MQTT, KEY_MQT_BIN, STR_NULL, m_strProfiles[PROF_MQT_BIN]);
	vLoadKeyValue(SECT_MQTT, KEY_MQT_CONF1, STR_NULL, m_strProfiles[PROF_MQT_CONF1]);
	vLoadKeyValue(SECT_MQTT, KEY_MQT_CONF2, STR_NULL, m_strProfiles[PROF_MQT_CONF2]);

	vLoadKeyValue(SECT_SOCKET, KEY_SOC_PATH, STR_NULL, m_strProfiles[PROF_SOC_PATH]);
	vLoadKeyValue(SECT_SOCKET, KEY_SOC_NAME, SCRIPT_NANE, m_strProfiles[PROF_SOC_NAME]);
	vLoadKeyValue(SECT_SOCKET, KEY_SOC_PORT, SOCKET_PORT, m_strProfiles[PROF_SOC_PORT]);


}

CIniConfig::~CIniConfig()
{
	// �������̉��
	//vFreeMemory(m_strIPAddress1);
}


// �L�[�l�̓Ǎ��݁i������j
void CIniConfig::vLoadKeyValue(
	char*  szSect,
	char*  szKey,
	char*  szDefault,
	CString& strValue)
{
	// �Ǐ��ϐ��錾
	TCHAR tszReadValue[255];    // �ǂݏo�����l
	TCHAR tszSect[255];
	TCHAR tszKey[255];
	TCHAR tszDefault[255];

	// char -> TCHAR�ϊ�
	_stprintf_s(tszSect, 255 - 1, TEXT("%S"), szSect);
	_stprintf_s(tszKey, 255 - 1, TEXT("%S"), szKey);
	_stprintf_s(tszDefault, 255 - 1, TEXT("%S"), szDefault);

	GetPrivateProfileString(
		tszSect,
		tszKey,
		tszDefault,
		tszReadValue,
		sizeof(tszReadValue),
		m_strFileName
	);

	strValue = tszReadValue;

	return;
}

// �L�[�l�̓Ǎ��݁i������j
void CIniConfig::vLoadKeyValue(
	char*  szSect,
	char*  szKey,
	int    iDefault,
	CString&   strValue)
{
	// �Ǐ��ϐ��錾
	TCHAR tszReadValue[255];    // �ǂݏo�����l
	TCHAR tszSect[255];
	TCHAR tszKey[255];
	TCHAR tszDefault[255];

	// char -> TCHAR�ϊ�
	_stprintf_s(tszSect, 255 - 1, TEXT("%S"), szSect);
	_stprintf_s(tszKey, 255 - 1, TEXT("%S"), szKey);
	_stprintf_s(tszDefault, 255 - 1, TEXT("%d"), iDefault);

	GetPrivateProfileString(
		tszSect,
		tszKey,
		tszDefault,
		tszReadValue,
		sizeof(tszReadValue),
		m_strFileName
	);

	strValue = tszReadValue;

	return;
}



// �v���t�B�[���l�̎擾
bool CIniConfig::bGetProfile(
	enIniProfile enProf, 
	CString& strValue)
{
	if ((0 > enProf) && (PROF_NUM <= enProf)) {
		return false;
	}
	strValue = m_strProfiles[enProf];
	if (strValue == "") {
		return false;
	}

	return true;

}