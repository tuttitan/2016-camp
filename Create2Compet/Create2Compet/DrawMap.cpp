﻿/*****************************************************************************/
/* ファイルインクルード                                                      */
/*****************************************************************************/
#include "stdafx.h"
#include "DrawMap.h"
#include "windows.h"

#include "opencv2/imgproc/imgproc.hpp"


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
CConMQTT* CDrawMap::s_pConMQTT = NULL;

/*****************************************************************************/
/* メンバ関数定義                                                            */
/*****************************************************************************/

CDrawMap::CDrawMap()
{
	OutputDebugString(TEXT("CDrawMapコンストラクタ\n"));
	
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

int CDrawMap::iStartDraw(void)
{

	m_pimCanvas = new Mat(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3);


	if (NULL == m_pimCanvas) {
		return DRAW_STAT_ERR_NULL;
	}
	namedWindow(m_szNameCanvas, CV_WINDOW_AUTOSIZE);

	*m_pimCanvas = Scalar(255.0, 255.0, 255.0);

	//iDrawGrid();
	iDrawTempl();
	
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

int CDrawMap::iDrawTempl(void)
{
	// 局所変数宣言
	Scalar scColorGray(95.0, 95.0, 95.0);
	int iVertX = 500;
	int iHorzYL = 100;
	int iHorzYR = 200;

	// 7seg画像の読込み
	bLoad7SegImage();

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

	// 残り時間
	putText(*m_pimCanvas, "Remaining", 
		cv::Point(30, 40), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);
	putText(*m_pimCanvas, "Time",
		cv::Point(70, 70), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);

	// もち時間の表示
	vDrawTimeRemain(2, 0, 0, 0, true);

	// 今何回目
	putText(*m_pimCanvas, "2",
		cv::Point(370, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 0), 2, CV_AA);

	putText(*m_pimCanvas, "chal.",
		cv::Point(400, 70), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);

	// 得点
	putText(*m_pimCanvas, "Score",
		cv::Point(520, 30), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);
	putText(*m_pimCanvas, "0",
		cv::Point(570, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 255), 2, CV_AA);

	// 順位
	putText(*m_pimCanvas, "Prize",
		cv::Point(670, 30), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);
	putText(*m_pimCanvas, "4",
		cv::Point(720, 70), CV_FONT_HERSHEY_SIMPLEX,
		1.2, RGB(0, 0, 255), 2, CV_AA);

	// チーム名
	putText(*m_pimCanvas, "Team",
		cv::Point(520, 110), CV_FONT_HERSHEY_SIMPLEX,
		0.5, RGB(0, 0, 0), 1, CV_AA);
	putText(*m_pimCanvas, "Toppers",
		cv::Point(550, 170), CV_FONT_HERSHEY_SIMPLEX,
		1.5, RGB(0, 0, 255), 2, CV_AA);


	// 実況図貼り付け
	Mat imTemplMap = imread(TOP_VIEW_MAP_FILE, -1);
	Rect rect(10, 110, 480, 480);

	// ROI指定
	Mat imCanvasRoi = (*m_pimCanvas)(rect);
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

	// TODO: 定数に置き換える
	vDrawCornPoints('a');

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
	m_iLeftTime = INITIAL_TIME;
	vDrawTimeRemain(2, 0, 0, 0, true);
	return;
}

// タイマーのコールバック関数
bool CDrawMap::bUpdateTimer(void)
{
	// 局所変数宣言
	char szTime[15];
	int iSec = 0;
	int iMin = 0;
	int iCS = 0;

	// 残り時間の更新
	--m_iLeftTime;
	if (0 > m_iLeftTime) {
		return false;
	}

	// 各桁の計算
	iMin = m_iLeftTime / (60 * 10);
	iSec = (m_iLeftTime - (iMin * 600)) / 10;
	iCS = (m_iLeftTime - (iMin * 600)) % 10;

#if 0
	// 白地を作る
	rectangle(*m_pimCanvas,
		Point(150, 20),
		Point(400, 80),
		Scalar(255, 255, 255, 0),
		CV_FILLED
	);

	//sprintf_s(szTime, "%02d:%02d", iSec, iDesi);
	sprintf_s(szTime, "%d:%02d.%d", iMin, iSec, iCS);

	putText(*m_pimCanvas, szTime,
		cv::Point(160, 70), CV_FONT_HERSHEY_SIMPLEX,
		2.0, RGB(0, 0, 0), 2, CV_AA);
#else /* 0 */
	int iSec2[2];

	iSec2[1] = iSec / 10;
	iSec2[0] = iSec % 10;

	vDrawTimeRemain(iMin, iSec2[1], iSec2[0], iCS, false);

#endif /* 0 */
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
	const int POS_COL[] = { 120, 180, 220, 280 };
	const int POS_COL_EX[] = { 160, 260 };
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

	else {
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

void CDrawMap::vRelateObject(CConMQTT* pConMQTT)
{
	if (NULL != pConMQTT) {
		s_pConMQTT = pConMQTT;
	}
}

// コーナーポイントの更新
// cPosはcreate2の現在位置
#if 0
void CDrawMap::vUpdatePoints(char cPos)
{
	int i;    // ループカウンタ
	unsigned int uiCurrPoints[CORNER_NUM] = { 0 };

	if (cPos != m_cPrevPos) {

		// 総得点の更新
		m_iTotalScore += m_uiCurrPoint[(int)(cPos - 'a')];

		// m_uiCurrPoint[]のコピーをとる
		for (i = 0; CORNER_NUM > i; ++i) {
			uiCurrPoints[i] = m_uiCurrPoint[i];
		}

		// 点数をシフトさせる
		for (i = 0; CORNER_NUM > i; ++i) {
			m_uiCurrPoint[(CORNER_NUM + (int)(cPos - m_cPrevPos) + i) % CORNER_NUM] = uiCurrPoints[i];
		}


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
#else  /* 0 */

// 点数表の更新
void CDrawMap::vUpdatePointTable(unsigned int uiPoints[])
{
	// 局所変数宣言
	int i;      // ループ変数


	for (i = 0; CORNER_NUM > i; ++i) {
		m_uiCurrPoint[i] = uiPoints[i];
	}
}

void CDrawMap::vUpdatePoints(char cPos, unsigned int uiPoints[])
{
	int i;    // ループカウンタ
	int j;    // インデックス
	unsigned int uiCurrPoints[CORNER_NUM] = { 0 };

	if (cPos != m_cPrevPos) {

		// 総得点の更新
		m_iTotalScore += m_uiCurrPoint[(int)(cPos - 'a')];

		// m_uiCurrPoint[]のコピーをとる
		//for (i = 0; CORNER_NUM > i; ++i) {
		//	uiCurrPoints[i] = m_uiCurrPoint[i];
		//}

		// 点数を更新する
		j = 0;
		for (i = 0; CORNER_NUM > i; ++i) {
			if ((int)(cPos - 'a') == i) {
				m_uiCurrPoint[i] = 0;
			}
			else {
				m_uiCurrPoint[i] = uiPoints[j];
				
			}
			++j;

		}


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
#endif /* 0 */


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