#pragma once

/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/


/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#define INI_FILE_NAME        (TEXT("Create2Compet.ini"))
#define STR_NULL             ("")

/* *** セクション名 *** */
#define SECT_GENERAL         ("GENERAL")
#define SECT_MQTT            ("MQTT")
#define SECT_SOCKET          ("SOCKET")

/* *** キー名 *** */
// 一般項目
#define KEY_GEN_IPADDR1      ("IPAddress1")
#define KEY_GEN_IPADDR2      ("IPAddress2")
#define KEY_GEN_PYTHON_BIN   ("PythonBin")
#define KEY_GEN_PENALTY      ("PenaltyPoint")
#define KEY_GEN_INIT_PT      ("InitPoints")
#define KEY_GEN_RAND_SEED    ("RandSeed")
#define KEY_GEN_COMP_TIME    ("CompetTime")

// MQTT項目
#define KEY_MQT_PATH         ("BrokerExecPath")
#define KEY_MQT_BIN          ("BrokerExecName")
#define KEY_MQT_CONF1        ("BrokerConfigFile1")
#define KEY_MQT_CONF2        ("BrokerConfigFile2")

// ソケット項目
#define KEY_SOC_PATH         ("ScriptPath")
#define KEY_SOC_NAME         ("ScriptName")
#define KEY_SOC_PORT         ("Port")

/*****************************************************************************/
/* 列挙型定義                                                                */
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
/* クラス定義                                                                */
/*****************************************************************************/
class CIniConfig
{
private:
	CString m_strFileName;

	CString m_strProfiles[PROF_NUM];
#if 0
	/* 一般項目 */
	CString m_strIPAddress1;        // IPアドレス1
	CString m_strIPAddress2;        // IPアドレス2
	CString m_strBinPython;         // Pythonバイナリ名
	int     m_iPenaltyPoint = 0;    // 減点ポイント
	CString m_strInitPoints;        // 各コーナーの初期ポイント

	/* MQTT通信項目 */
	CString m_strPathBroker;        // MQTTブローカーのパス
	CString m_strBinBroker;         // ブローカーの実行ファイル名
	CString m_strConfigBroker1;     // ブローカーの設定ファイル名
	CString m_strConfigBroker2;     // ブローカーの設定ファイル名

	/* ソケット通信 */
	CString m_strPathScorer;        // スコアラーのパス
	CString m_strNameScorer;        // スコアラーの名前
	int     m_iPort = 1024;         // ポート番号
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