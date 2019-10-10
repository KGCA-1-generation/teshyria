// MapMng.h: interface for the CMapMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPMNG_H__C0828101_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
#define AFX_MAPMNG_H__C0828101_08FB_11D6_B3AA_0050DA898E67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Map.h"
#include "MapImg.h"
#include "Image.h"
#include "NeedStruct.h"

class CMapMng  
{
protected :

	// 미션정보와 관련된 변수들 ------------------------------------------
	char* m_strStageFile[6];
	char* (m_pMapFile[3]);		// 각 스토리별 맵 파일 명 
	int m_nStory;				// 스토리 일련번호 (1~3)
	int m_nStage;				// 미션 일련번호 (1~6)
	
	// 타일등의 사이즈 변수들 --------------------------------------------
	int m_nTileW, m_nTileH;		// 타일 사이즈(픽셀단위)
	int m_nAttrW, m_nAttrH;		// 속성타일 사이즈( 픽셀단위)

	// 맵 이미지 관련 변수들 -----------------------------------------------------
	int m_nObjTotal;			// 사용된 오브젝트 이미지 갯수
	int m_nCliffTotal;			// 사용된 절벽 타일 이미지 갯수 
	int m_nTileTotal;			// 사용된 타일 이미지 갯수

	CMapImg* m_pTileImg;
	CMapImg* m_pCliffImg;
	CMapImg* m_pObjImg;
	
	// 시야처리 관련 이미지 변수들 -----------------------------------------------
//	CImage m_Black;
//	CImage m_Fog;

public:
	CMap* m_pMap;													// 맵 객체 
	char m_MapFile[30];
public:
	CImage m_SightImg;
	void DrawSightMask(WORD* pScreen);

	int GetMapAttrP(int Xp, int Yp);
	int	GetMapAttrN(int Xn, int Yn);
//	void DrawFog(WORD* pScreen);									// 시야권 출력 함수 
	void DrawObjectMap(WORD *pScreen, int nObj, int nX, int nY);	// 오브젝트 맵 출력함수 
	void DrawCliffMap(WORD* pScreen, bool Alpha);					// 절벽맵 출력함수 
	void DrawTileMap(WORD* pScreen);								// 바닥 타일맵 출력함수 

	bool LoadStage(int nStage, StageInfo* pStg, point* Teshy, BOOL b565Mode);									// 미션파일 로딩 함수 
	void LoadMap();										// 맵파일 로딩함수 
	bool LoadMapImg(BOOL b565Mode);												// 맵 이미지 파일 로딩함수 
	void InitMapImg();												// 맵 이미지 초기화 함수 
	
	CMapMng();
	virtual ~CMapMng();
};

#endif // !defined(AFX_MAPMNG_H__C0828101_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
