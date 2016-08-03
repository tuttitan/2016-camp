/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "DrawMap.h"
#include "windows.h"

#include "opencv2/imgproc/imgproc.hpp"

#include <locale.h>    // setlocale()


/*****************************************************************************/
/* ライブラリリンク                                                          */
/*****************************************************************************/
#ifdef _DEBUG
#pragma comment(lib,"Debug/opencv_core2412d.lib")
#pragma comment(lib,"Debug/opencv_highgui2412d.lib")
#pragma comment(lib,"Debug/opencv_imgproc2412d.lib")
//#pragma comment(lib,"Debug/opencv_video2412d.lib")
#else  /* _DEBUG */
#pragma comment(lib,"Release/opencv_core2412.lib")
#pragma comment(lib,"Release/opencv_highgui2412.lib")
#pragma comment(lib,"Release/opencv_imgproc2412.lib")
//#pragma comment(lib,"Release/opencv_video2412.lib")
#endif /* _DEBUG */

/*****************************************************************************/
/* 名前空間解決                                                              */
/*****************************************************************************/
using namespace cv;

/*****************************************************************************/
/* 静的メンバ関数変数宣言                                                    */
/*****************************************************************************/

/*****************************************************************************/
/* 定数定義                                                                  */
/*****************************************************************************/
// 暫定
#define NUM_TEAM  (10)
#define MAX_TEAM  (12)

/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/

CDrawMap::CDrawMap(HWND hWnd, int iAlloted)
{
	OutputDebugString(TEXT("CDrawMapコンストラクタ\n"));
	
	// 親ウィンドウの関連付け
	m_hWnd = hWnd;

	// 持ち時間の設定
	m_iAllotedTime = m_iLeftTime = iAlloted * 10;

	// 7seg画像の読込み
	bLoad7SegImage();

	return;
}

CDrawMap::~CDrawMap()
{
	OutputDebugString(TEXT("CDrawMapデストラクタ\n"));
	if (NULL != m_pimCanvas) {
		delete m_pimCanvas;
	}
	return;
}

void CDrawMap::vDestroyWindows(void)
{
	destroyAllWindows();

	return;
}

int CDrawMap::iStartDraw(void)
{
	// 局所変数宣言
	wchar_t* pwszWndName = NULL;
	size_t   sizeConv;
	int      iLenStr;
	LONG     lWndStyle;
	HWND     hWnd;

	// Canvasメモリの確保
	m_pimCanvas = new Mat(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3);
		if (NULL == m_pimCanvas) {
		return DRAW_STAT_ERR_NULL;
	}

	// ウインドウを作る
	cvNamedWindow(m_szNameCanvas, CV_WINDOW_AUTOSIZE);

	// ウインドウからボタンを消す
	iLenStr = strlen(m_szNameCanvas) + 1;
	pwszWndName = new wchar_t[iLenStr];

	setlocale(LC_ALL, "japanese");
	mbstowcs_s(&sizeConv, pwszWndName, iLenStr, m_szNameCanvas, _TRUNCATE);
	hWnd = FindWindow(0, pwszWndName);

	lWndStyle = GetWindowLong(hWnd, GWL_STYLE);
	lWndStyle &= ~WS_SYSMENU;
	SetWindowLong(hWnd, GWL_STYLE, lWndStyle);

	if (NULL != pwszWndName) {
		delete[] pwszWndName;
		pwszWndName = NULL;
	}
	//SetWindowLong(s_hWndTest, GWL_STYLE, ~WS_VISIBLE);

	*m_pimCanvas = Scalar(255.0, 255.0, 255.0);

	//iDrawGrid();
	iDrawTempl(false);
	
	//iDrawGrid();

	imshow(m_szNameCanvas, *m_pimCanvas);

	waitKey(1);

	//destroyWindow(m_szNameCanvas);


	return DRAW_STAT_OK;
}

bool CDrawMap::bLoad7SegImage(void)
{
	// 局所変数宣言
	int i;               // ループ変数
	char szFileName[31];

	for (i = 0; LED7SEG_NUM > i; ++i) {
		// 橙色のLED
		sprintf_s(szFileName, "7seg_s/o%d.bmp", i);
		m_im7SegOra[i] = imread(szFileName, -1);
		resize(m_im7SegOra[i], m_im7SegOra[i], Size(), 0.35, 0.35, INTER_CUBIC);

		// 赤色のLED
		sprintf_s(szFileName, "7seg_s/r%d.bmp", i);
		m_im7SegRed[i] = imread(szFileName, -1);
		resize(m_im7SegRed[i], m_im7SegRed[i], Size(), 0.35, 0.35, INTER_CUBIC);
	}

	m_imSymbOra[COLON]  = imread("7seg_s/oc.bmp", -1);
	m_imSymbOra[PERIOD] = imread("7seg_s/op.bmp", -1);
	resize(m_imSymbOra[COLON],  m_imSymbOra[COLON],  Size(), 0.35, 0.35, INTER_CUBIC);
	resize(m_imSymbOra[PERIOD], m_imSymbOra[PERIOD], Size(), 0.35, 0.35, INTER_CUBIC);

	m_imSymbRed[COLON] = imread("7seg_s/rc.bmp", -1);
	m_imSymbRed[PERIOD] = imread("7seg_s/rp.bmp", -1);
	resize(m_imSymbRed[COLON], m_imSymbRed[COLON], Size(), 0.35, 0.35, INTER_CUBIC);
	resize(m_imSymbRed[PERIOD], m_imSymbRed[PERIOD], Size(), 0.35, 0.35, INTER_CUBIC);

	return true;
}

int CDrawMap::iDrawTempl(bool bCompet)
{
	// 局所変数宣言
	Scalar scColorGray(95.0, 95.0, 95.0);
	int iVertX = 500;
	int iHorzYL = 100;
	int iHorzYR = 200;
	Mat imChara;
	Rect rect;
	Mat imCanvasRoi;


	// 競技終了までの貼り付け
	imChara = imread("chara/until.bmp", -1);
	rect = Rect(10, 10, imChara.cols, imChara.rows);
	imCanvasRoi = (*m_pimCanvas)(rect);
	imChara.copyTo(imCanvasRoi);
	imChara.release();


	// もち時間の表示
	vResetTime();
	//vDrawTimeRemain(2, 0, 0, 0, true);

	// 今何回目
	putText(*m_pimCanvas, "1",
		cv::Point(400, 75), CV_FONT_HERSHEY_SIMPLEX,
		1.0, RGB(0, 0, 0), 2, CV_AA);


	imChara = imread("chara/times.bmp", -1);
	rect = Rect(430, 60, imChara.cols, imChara.rows);
	imCanvasRoi = (*m_pimCanvas)(rect);
	imChara.copyTo(imCanvasRoi);
	imChara.release();


	// 得点
	imChara = imread("chara/score.bmp", -1);
	rect = Rect(520, 20, imChara.cols, imChara.rows);
	imCanvasRoi = (*m_pimCanvas)(rect);
	imChara.copyTo(imCanvasRoi);
	imChara.release();

	putText(*m_pimCanvas, "0",
		cv::Point(570, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 255), 2, CV_AA);



	// 順位
	imChara = imread("chara/prize.bmp", -1);
	rect = Rect(670, 20, imChara.cols, imChara.rows);
	imCanvasRoi = (*m_pimCanvas)(rect);
	imChara.copyTo(imCanvasRoi);
	imChara.release();

	putText(*m_pimCanvas, "-",
		cv::Point(720, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 255), 2, CV_AA);

	if (true == bCompet) {

		// チーム名
		imChara = imread("chara/team.bmp", -1);
		rect = Rect(520, 100, imChara.cols, imChara.rows);
		imCanvasRoi = (*m_pimCanvas)(rect);
		imChara.copyTo(imCanvasRoi);
		imChara.release();

		putText(*m_pimCanvas, "Toppers",
			cv::Point(550, 170), CV_FONT_HERSHEY_SIMPLEX,
			1.5, RGB(0, 0, 255), 2, CV_AA);

		// 順位表
		int x = 530;
		int y = 240;
		const char* szPrizes[MAX_TEAM] = {
			"1st", "2nd", "3rd", "4th", "5th",
			"6th", "7th", "8th", "9th", "10th", "11th", "12th" };

		for (int i = 0; i < NUM_TEAM; i++) {
			putText(*m_pimCanvas, szPrizes[i],
				cv::Point(x, y), CV_FONT_HERSHEY_SIMPLEX,
				0.5, RGB(0, 0, 0), 1, CV_AA);
			y += 30;
		}

		x = 730;
		y = 240;
		for (int i = 0; i < NUM_TEAM; i++) {
			putText(*m_pimCanvas, "1234",
				cv::Point(x, y), CV_FONT_HERSHEY_SIMPLEX,
				0.5, RGB(0, 0, 0), 1, CV_AA);
			y += 30;
		}
		y -= 20;

		// 表の横線（上）
		line(*m_pimCanvas,
			Point(iVertX + 20, iHorzYR + 20),
			Point(CANVAS_WIDTH - 20, iHorzYR + 20),
			scColorGray,
			1
		);
		// 表の横線（下）
		line(*m_pimCanvas,
			Point(iVertX + 20, y),
			Point(CANVAS_WIDTH - 20, y),
			scColorGray,
			1
		);

		// 表の縦線（左から1番目）
		line(*m_pimCanvas,
			Point(iVertX + 20, iHorzYR + 20),
			Point(iVertX + 20, y),
			scColorGray,
			1
		);
		// 表の縦線（左から2番目）
		line(*m_pimCanvas,
			Point(iVertX + 80, iHorzYR + 20),
			Point(iVertX + 80, y),
			scColorGray,
			1
		);
		// 表の縦線（左から3番目）
		line(*m_pimCanvas,
			Point(iVertX + 220, iHorzYR + 20),
			Point(iVertX + 220, y),
			scColorGray,
			1
		);
		// 表の縦線（左から4番目）
		line(*m_pimCanvas,
			Point(iVertX + 280, iHorzYR + 20),
			Point(iVertX + 280, y),
			scColorGray,
			1
		);
	}
	else {
		const int POS_X1 = 530;
		const int POS_X2 = POS_X1 + 240;
		const int POS_Y1 = 110;
		const int POS_Y2 = POS_Y1 + 60;

		imChara = imread("chara/trial.bmp", -1);
		rect = Rect(POS_X1, POS_Y1, imChara.cols, imChara.rows);
		imCanvasRoi = (*m_pimCanvas)(rect);
		imChara.copyTo(imCanvasRoi);
		imChara.release();


		rectangle(*m_pimCanvas, Point(POS_X1, POS_Y1), Point(POS_X2, POS_Y2),
			Scalar(0, 0, 0, 0));
		rectangle(*m_pimCanvas, Point(POS_X1 + 2, POS_Y1 + 2), Point(POS_X2 - 2, POS_Y2 - 2),
			Scalar(0, 0, 0, 0));
	}

	/* *** 分割線の描画 *** */
	// 縦の分割線
	line(*m_pimCanvas,
		Point(iVertX, 0),
		Point(iVertX, CANVAS_HEIGHT - 1),
		scColorGray,
		2
	);

	// 横の分割線（左側）
	line(*m_pimCanvas,
		Point(0, iHorzYL),
		Point(iVertX, iHorzYL),
		scColorGray,
		2
	);

	// 横の分割線（右側）
	line(*m_pimCanvas,
		Point(iVertX, iHorzYR),
		Point(CANVAS_WIDTH - 1, iHorzYR),
		scColorGray,
		2
	);
	

	// 実況図貼り付け
	Mat imTemplMap = imread(TOP_VIEW_MAP_FILE, -1);
	rect = Rect(10, 110, 480, 480);

	// ROI指定
	imCanvasRoi = (*m_pimCanvas)(rect);
	imTemplMap.copyTo(imCanvasRoi);

	
	// アルファベットの印字
	//Scalar scColorGray(127, 127, 127, 255);
	Scalar scColorGreen(0, 255, 0, 255);

	putText(*m_pimCanvas, "A",
		cv::Point(200, 130), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);
	putText(*m_pimCanvas, "B",
		cv::Point(460, 240), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);
	putText(*m_pimCanvas, "C",
		cv::Point(460, 470), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);
	putText(*m_pimCanvas, "D",
		cv::Point(290, 580), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);
	putText(*m_pimCanvas, "E",
		cv::Point(20, 470), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);
	putText(*m_pimCanvas, "F",
		cv::Point(20, 240), CV_FONT_HERSHEY_DUPLEX,
		0.8, scColorGray, 1, CV_AA);


	vDrawCornPoints(INIT_POSITION);

	return DRAW_STAT_OK;
}

// cPrevPosは描画更新前のcreateの位置
void CDrawMap::vDrawCornPoints(char cPrevPos)
{
	// 局所変数宣言
	int i;
	char szPoint[2];
	Scalar scColorOrange(63, 127, 255, 255);
	Scalar scColorBlue(255, 31, 0, 255);
	Scalar scColorBlack(31, 31, 31, 255);
	Scalar scColor;

	// 白地で消す
	for (i = 0; CORNER_NUM > i; ++i) {
		rectangle(*m_pimCanvas,
			//m_ptPoints[i],
			Point(m_ptPoints[i].x - 10, m_ptPoints[i].y + 10),
			Point(m_ptPoints[i].x + 30, m_ptPoints[i].y - 30),
			Scalar(255, 255, 255, 0),
			CV_FILLED
			);
	}
	circle(*m_pimCanvas,
		m_ptMachine[(int)(cPrevPos - 'a')],
		30,
		Scalar(255, 255, 255, 255),
		CV_FILLED);

	// 各コーナーのポイントを印字
	for (i = 0; CORNER_NUM > i; ++i) {
		// 0はcreateの現在地
		if (0 == m_uiCurrPoint[i]) {
			circle(*m_pimCanvas,
				m_ptMachine[i],
				30,
				Scalar(0, 255, 0, 255),
				CV_FILLED);

			continue;
		}

		// Colourの決定
		scColor = (3 == m_uiCurrPoint[i]) ? scColorOrange
			    : (2 == m_uiCurrPoint[i]) ? scColorBlue : scColorBlack;
		
		sprintf_s(szPoint, "%d", m_uiCurrPoint[i]);
		putText(*m_pimCanvas, szPoint,
			m_ptPoints[i], CV_FONT_HERSHEY_DUPLEX,
			1.0, scColor, 1, CV_AA);
	}

	return;
}

// もち時間のリセット
void CDrawMap::vResetTime(void)
{
	int iDigit[4];   // 各桁で表示する数字
	int iRemain;     // 残り（計算用）

	//m_iLeftTime = INITIAL_TIME * 10;
	m_iLeftTime = m_iAllotedTime;

	// 桁を分解する
	iDigit[3] = (m_iLeftTime / 10) / 60;
	iRemain = (m_iLeftTime / 10) % 60;
	iDigit[2] = iRemain / 10;
	iDigit[1] = iRemain % 10;
	iDigit[0] = m_iLeftTime % 10;

	vDrawTimeRemain(iDigit[3], iDigit[2], iDigit[1], iDigit[0], true);

	// TODO: できればきれいに分けたい（機能）
	m_iTotalScore = 0;
	vDrawTotalScore();
	m_cPrevPos = INIT_POSITION;

	return;
}

// タイマーのコールバック関数
bool CDrawMap::bUpdateTimer(void)
{
	// 局所変数宣言
	int iSec = 0;  // 秒
	int iMin = 0;  // 分
	int iCS  = 0;  // 0.1秒

	// 残り時間の更新
	--m_iLeftTime;
	if (0 > m_iLeftTime) {
		return false;
	}

	// 各桁の計算
	iMin = m_iLeftTime / (60 * 10);
	iSec = (m_iLeftTime - (iMin * 600)) / 10;
	iCS  = (m_iLeftTime - (iMin * 600)) % 10;


	int iSec2[2];   // 秒 [1: 2桁目, 0: 1桁目]

	iSec2[1] = iSec / 10;
	iSec2[0] = iSec % 10;

	vDrawTimeRemain(iMin, iSec2[1], iSec2[0], iCS, false);

	// 画面更新
	imshow(m_szNameCanvas, *m_pimCanvas);
	waitKey(1);

	return true;

}

// 残り時間の表示
void CDrawMap::vDrawTimeRemain(
	int iDigit3, 
	int iDigit2, 
	int iDigit1, 
	int iDigit0, 
	bool bForce
	)
{
	static int s_iDigit[4] = { 0 };
	Mat imCanvasRoi;
	const int POS_BASE_LEFT = 150;
	const int POS_COL[] = {
		POS_BASE_LEFT, 
		POS_BASE_LEFT + 60, 
		POS_BASE_LEFT + 100, 
		POS_BASE_LEFT + 160
	};
	const int POS_COL_EX[] = { 
		POS_BASE_LEFT + 40, 
		POS_BASE_LEFT + 140
	};
	const int POS_ROW = 20;
	bool bRedraw = false;

	Mat* pim7Seg;  // 数字のデータ
	Mat* pimSymb;  // 記号のデータ
	
	if ((iDigit3 == 0) && (iDigit2 <= 2)) {
		pim7Seg = m_im7SegRed;
		pimSymb = m_imSymbRed;
		bRedraw = true;
	}
	else {
		pim7Seg = m_im7SegOra;
		pimSymb = m_imSymbOra;
	}

	// 強制表示
	if ((true == bForce) || (true == bRedraw)) {
		s_iDigit[3] = iDigit3;
		s_iDigit[2] = iDigit2;
		s_iDigit[1] = iDigit1;
		s_iDigit[0] = iDigit0;

		// デリミタの印字
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL_EX[0], POS_ROW, pimSymb[COLON].cols, pimSymb[COLON].rows));
		pimSymb[COLON].copyTo(imCanvasRoi);
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL_EX[1], POS_ROW, pimSymb[PERIOD].cols, pimSymb[PERIOD].rows));
		pimSymb[PERIOD].copyTo(imCanvasRoi);

		// 3桁目の表示
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[0], POS_ROW, pim7Seg[iDigit3].cols, pim7Seg[iDigit3].rows));
		pim7Seg[iDigit3].copyTo(imCanvasRoi);

		// 2桁目の表示
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[1], POS_ROW, pim7Seg[iDigit2].cols, pim7Seg[iDigit2].rows));
		pim7Seg[iDigit2].copyTo(imCanvasRoi);

		// 1桁目の表示
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[2], POS_ROW, pim7Seg[iDigit1].cols, pim7Seg[iDigit1].rows));
		pim7Seg[iDigit1].copyTo(imCanvasRoi);

		// 0桁目の表示
		imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[3], POS_ROW, pim7Seg[iDigit0].cols, pim7Seg[iDigit0].rows));
		pim7Seg[iDigit0].copyTo(imCanvasRoi);
	}

	// 数字が変化したときの再描画
	else {
		// 3桁目
		if (s_iDigit[3] != iDigit3) {
			imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[0], POS_ROW, pim7Seg[iDigit3].cols, pim7Seg[iDigit3].rows));
			pim7Seg[iDigit3].copyTo(imCanvasRoi);

			s_iDigit[3] = iDigit3;
		}
		if (s_iDigit[2] != iDigit2) {
			imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[1], POS_ROW, pim7Seg[iDigit2].cols, pim7Seg[iDigit2].rows));
			pim7Seg[iDigit2].copyTo(imCanvasRoi);

			s_iDigit[2] = iDigit2;
		}
		if (s_iDigit[1] != iDigit1) {
			imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[2], POS_ROW, pim7Seg[iDigit1].cols, pim7Seg[iDigit1].rows));
			pim7Seg[iDigit1].copyTo(imCanvasRoi);

			s_iDigit[1] = iDigit1;
		}
		if (s_iDigit[0] != iDigit0) {
			imCanvasRoi = (*m_pimCanvas)(Rect(POS_COL[3], POS_ROW, pim7Seg[iDigit0].cols, pim7Seg[iDigit0].rows));
			pim7Seg[iDigit0].copyTo(imCanvasRoi);

			s_iDigit[0] = iDigit0;
		}
	}


	return;
}

// グリッドの描画
int CDrawMap::iDrawGrid(void)
{
	// 局所変数宣言
	int iCntX, iCntY;
	const int StepWidth  = 10;
	const int StepHeight = 10;
	Scalar scColorOrange(63.0, 127.0, 255.0);
	Scalar scColorRed(0.0, 0.0, 255.0);
	Scalar scColorGreen(0.0, 223.0, 0.0);

	// 縦のグリッド
	for (iCntX = StepWidth; CANVAS_WIDTH > iCntX; iCntX += StepWidth) {
		line(*m_pimCanvas,
			Point(iCntX, 0),
			Point(iCntX, CANVAS_HEIGHT - 1),
			(iCntX % 100 == 0) ? scColorGreen : scColorOrange,
			1
		);
	}

	// 横のグリッド
	for (iCntY = StepHeight; CANVAS_HEIGHT > iCntY; iCntY += StepHeight) {
		
		line(*m_pimCanvas,
			Point(0, iCntY),
			Point(CANVAS_WIDTH - 1, iCntY),
			(iCntY % 100 == 0) ? scColorGreen : scColorOrange,
			1
		);
	}

	return DRAW_STAT_OK;
}


// 点数表の更新
// コーナーポイントの更新
// cPosはcreate2の現在位置
void CDrawMap::vUpdatePoints(char cPos, unsigned int uiPoints[])
{
	// 局所変数宣言
	int i;    // ループカウンタ


	if ((cPos != m_cPrevPos) && (false != m_bPlay)) {

		// 総得点の更新
		m_iTotalScore += m_uiCurrPoint[(int)(cPos - FIRST_CORNER)];

		// 点数表の更新
		for (i = 0; CORNER_NUM > i; ++i) {
			m_uiCurrPoint[i] = uiPoints[i];
		}
		m_uiCurrPoint[(int)(cPos - FIRST_CORNER)] = 0;

		
		// 更新スコアの表示
		vDrawCornPoints(m_cPrevPos);
		vDrawTotalScore();

		imshow(m_szNameCanvas, *m_pimCanvas);
		waitKey(1);

		// 過去位置の更新
		m_cPrevPos = cPos;
	}

	return;
}

void CDrawMap::vSetPlayStatus(bool bFlag)
{
	m_bPlay = bFlag;

	return;
}



// 総得点の更新
void CDrawMap::vDrawTotalScore(void)
{
	// 局所変数宣言
	char szTotal[7];

	sprintf_s(szTotal, "%d", m_iTotalScore);

	// 白地を作る
	rectangle(*m_pimCanvas,
		Point(570, 40),
		Point(650, 80),
		Scalar(255, 255, 255, 0),
		CV_FILLED
		);

	putText(*m_pimCanvas, szTotal,
		cv::Point(570, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 255), 2, CV_AA);
}

void CDrawMap::vSetCurrPoints(unsigned int uiPoints[])
{
	// 局所変数宣言
	int i;     // ループカウンタ


	// 点数表の更新
	for (i = 0; CORNER_NUM > i; ++i) {
		m_uiCurrPoint[i] = uiPoints[i];
	}

	return;
}


// テスト用暫定
void CDrawMap::vWindowTestOn(void)
{
	cvNamedWindow("テストウインドウ", CV_WINDOW_AUTOSIZE);
	//m_hWndTest = (HWND)cvGetWindowHandle("テストウインドウ");
	cvResizeWindow("テストウインドウ", 300, 200);

#if 0
	s_hWndTest = FindWindow(0, TEXT("テストウインドウ"));
	//s_hWndTest = (HWND)cvGetWindowHandle("テストウインドウ");

	LONG lStyle = GetWindowLong(s_hWndTest, GWL_STYLE);
	lStyle &= ~WS_SYSMENU;
	SetWindowLong(s_hWndTest, GWL_STYLE, lStyle);
	//SetWindowLong(s_hWndTest, GWL_STYLE, ~WS_VISIBLE);

	//imshow("テストウインドウ", NULL);

	waitKey(0);
#endif
	return;
}
void CDrawMap::vWindowTestOff(void)
{
	destroyWindow("テストウインドウ");
	return;
}