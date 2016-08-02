#pragma once

/*****************************************************************************/
/* �t�@�C���C���N���[�h                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* �萔��`                                                                  */
/*****************************************************************************/
#define INI_FILE_NAME        (TEXT("Create2Compet.ini"))
#define STR_NULL             ("")

/* *** �Z�N�V������ *** */
#define SECT_GENERAL         ("GENERAL")
#define SECT_MQTT            ("MQTT")
#define SECT_SOCKET          ("SOCKET")

/* *** �L�[�� *** */
// ��ʍ���
#define KEY_GEN_IPADDR1      ("IPAddress1")
#define KEY_GEN_IPADDR2      ("IPAddress2")
#define KEY_GEN_PYTHON_BIN   ("PythonBin")
#define KEY_GEN_PENALTY      ("PenaltyPoint")
#define KEY_GEN_INIT_PT      ("InitPoints")
#define KEY_GEN_RAND_SEED    ("RandSeed")
#define KEY_GEN_COMP_TIME    ("CompetTime")

// MQTT����
#define KEY_MQT_PATH         ("BrokerExecPath")
#define KEY_MQT_BIN          ("BrokerExecName")
#define KEY_MQT_CONF1        ("BrokerConfigFile1")
#define KEY_MQT_CONF2        ("BrokerConfigFile2")

// �\�P�b�g����
#define KEY_SOC_PATH         ("ScriptPath")
#define KEY_SOC_NAME         ("ScriptName")
#define KEY_SOC_PORT         ("Port")

/*****************************************************************************/
/* �񋓌^��`                                                                */
/*****************************************************************************/
typedef enum _enIniProfile {
	PROF_GEN_IPADDR1 = 0,
	PROF_GEN_IPADDR2,
	PROF_GEN_PYTHON_BIN,
	PROF_GEN_PENALTY,
	PROF_GEN_INIT_PT,
	PROF_GEN_RAND_SEED,
	PROF_GEN_COMP_TIME,

	PROF_MQT_PATH,
	PROF_MQT_BIN,
	PROF_MQT_CONF1,
	PROF_MQT_CONF2,

	PROF_SOC_PATH,
	PROF_SOC_NAME,
	PROF_SOC_PORT,

	PROF_NUM
} enIniProfile;

/*****************************************************************************/
/* �N���X��`                                                                */
/*****************************************************************************/
class CIniConfig
{
private:
	CString m_strFileName;

	CString m_strProfiles[PROF_NUM];
#if 0
	/* ��ʍ��� */
	CString m_strIPAddress1;        // IP�A�h���X1
	CString m_strIPAddress2;        // IP�A�h���X2
	CString m_strBinPython;         // Python�o�C�i����
	int     m_iPenaltyPoint = 0;    // ���_�|�C���g
	CString m_strInitPoints;        // �e�R�[�i�[�̏����|�C���g

	/* MQTT�ʐM���� */
	CString m_strPathBroker;        // MQTT�u���[�J�[�̃p�X
	CString m_strBinBroker;         // �u���[�J�[�̎��s�t�@�C����
	CString m_strConfigBroker1;     // �u���[�J�[�̐ݒ�t�@�C����
	CString m_strConfigBroker2;     // �u���[�J�[�̐ݒ�t�@�C����

	/* �\�P�b�g�ʐM */
	CString m_strPathScorer;        // �X�R�A���[�̃p�X
	CString m_strNameScorer;        // �X�R�A���[�̖��O
	int     m_iPort = 1024;         // �|�[�g�ԍ�
#endif /*0 */
public:

	CIniConfig();
	~CIniConfig();
	//bool bGetBrokerExecPath(TCHAR*);
	bool CIniConfig::bGetProfile(enIniProfile, CString&);

private:
	void
		vLoadKeyValue(char*, char*, char*, CString&),
		vLoadKeyValue(char*, char*, int, CString&);

		//vLoadKeyValue(TCHAR*, TCHAR*, TCHAR*, CString),
		//vLoadKeyValue(TCHAR*, TCHAR*, TCHAR*, char*),
		//vLoadKeyValue(TCHAR*, TCHAR*, TCHAR*, CString&);
		//vFreeMemory(TCHAR*),
		//vFreeMemory(char*);

	


	

};