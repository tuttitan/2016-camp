#pragma once

/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "opencv2/core/core.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/highgui/highgui.hpp"

/*****************************************************************************/
/* 名前空間解決                                                              */
/*****************************************************************************/
using namespace cv;


/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
#define DRAW_STAT_OK       (0)         // エラーなし
#define DRAW_STAT_ERR_NULL (-1)        // NULLポインタ

#define CANVAS_WIDTH       (800)       // 描画エリアの幅
#define CANVAS_HEIGHT      (600)       // 描画エリアの高さ

#define CORNER_NUM         (6)         // コーナーの数（正六角形）
#define INITIAL_TIME       (120)       // もち時間 [centisec]
#define LED7SEG_NUM        (10)        // 7segで取り扱う数字（0～9）
#define INIT_POSITION      ('a')       // create2初期位置
#define FIRST_CORNER       ('a')       // 最初のコーナー

#define TOP_VIEW_MAP_FILE  ("TopviewMap.bmp")

#define PENALTY_POINT      (1)


/*****************************************************************************/
/* クラス宣言                                                                */
/*****************************************************************************/
class CConMQTT;


typedef enum _en_Symbol {
	COLON = 0,
	PERIOD,
	SYMBOL_NUM
} en_Symbol;
/*****************************************************************************/
/* クラス定義                                                                */
/*****************************************************************************/
class CDrawMap
{
public:
	CDrawMap(HWND, int);
	~CDrawMap();
	int iStartDraw(void);
	bool bUpdateTimer(void);
	

	void
		vDestroyWindows(void),
		vUpdatePoints(char, unsigned int uiPoints[]),

		vResetTime(void),
		vSetPlayStatus(bool),
		vSetCurrPoints(unsigned int uiPoints[]);

private:
	int iDrawTempl(bool);
	int iDrawGrid(void);
	void vDrawCornPoints(char);
	void vDrawTotalScore(void);
	bool bLoad7SegImage(void);
	void vDrawTimeRemain(int, int, int, int, bool);

private:
	HWND m_hWnd = NULL;  // ウィンドウハンドル

	Mat* m_pimCanvas = NULL;
	char* m_szNameCanvas = "競技会実況";

	int m_iAllotedTime;
	int m_iLeftTime; // = INITIAL_TIME * 10;
	Point m_ptPoints[CORNER_NUM] = {
		Point(240, 180),  // A
		Point(400, 270),  // B
		Point(400, 440),  // C
		Point(240, 530),  // D
		Point(90, 440),   // E
		Point(90, 270)    // F
	};
	Point m_ptMachine[CORNER_NUM] = {
		Point(250, 180),  // A
		Point(390, 270),  // B
		Point(390, 430),  // C
		Point(250, 520),  // D
		Point(110, 430),  // E
		Point(110, 270)   // F
	};
	Mat m_im7SegOra[LED7SEG_NUM];     // 橙色のLED（数字）
	Mat m_imSymbOra[SYMBOL_NUM];      // 橙色のLED（記号）
	Mat m_im7SegRed[LED7SEG_NUM];     // 赤色のLED（数字）
	Mat m_imSymbRed[SYMBOL_NUM];;     // 赤色のLED（記号）

	unsigned int m_uiCurrPoint[CORNER_NUM] = { 0 };   // 現在の各ポイントの点数
	char m_cPrevPos = INIT_POSITION;  // 前回の到達位置

	int m_iTotalScore = 0;
	bool m_bPlay = false;    // 競技中フラグ
};