// Map.h: interface for the CMap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAP_H__C0828102_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
#define AFX_MAP_H__C0828102_08FB_11D6_B3AA_0050DA898E67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <windows.h>

#define WORD    unsigned short
#define UINT    unsigned int
#define UNITCNT 100
#include "NeedStruct.h"

class CMapImg;
class CMap  
{
protected:
	// 맵 이미지 관련 변수들 ----------------------------------------------
	CMapImg* m_pTileImg;			// 타일 이미지 포인터 
	CMapImg* m_pObjImg;				// 오브젝트 이미지 포인터 
	CMapImg* m_pCliffImg;			// 절벽 이미지 포인터 

	// 맵 배열 -------------------------------------------------------------
	int* m_pTileMap;				// 타일 맵 배열
	int* m_pCliffMap;				// 절벽 맵 배열
	Object* m_pObjMap;				// 오브젝트 맵 배열

	// 맵과 관련 사이즈 변수들 --------------------------------------------
	int m_nWidth, m_nHeight;		// 실제 맵 사이즈 (픽셀단위)
	int m_nTileW, m_nTileH;			// 타일 사이즈(픽셀단위)
	int m_nAttrW, m_nAttrH;			// 속성타일 사이즈( 픽셀단위)
	int m_nMapW, m_nMapH;			// 맵 사이즈 (타일 단위)
	int m_nAttrMapW, m_nAttrMapH;	// 속성맵 사이즈 (타일 단위)

	// 맵 출력 관련된 변수들 ------------------------------------------------
	int m_nDrawXs, m_nDrawYs;		// 화면에 그려질 맵의 시작 좌표 (타일단위)
	int m_nMDrawXs, m_nMDrawYs;		// 미니맵에 그려질 맵의 시작 좌표 (타일단위)

	int m_nXscl, m_nYscl;			// 스크롤 좌표 	: 테시리아 맵 좌표 


	// 스크롤 관련 변수들 -------------------------------------------------------
	float m_fYerror;				// 스크롤시 오차 X
	float m_fXerror;				// 스크롤시 오차 Y
	float m_fYvec;					// 스크롤할 단위벡터 X
	float m_fXvec;					// 스크롤할 단위벡터 Y
	int m_nYmove;					// 스크롤할 거리 X
	int m_nXmove;					// 스크롤할 거리 Y

	// 기타 변수들 ---------------------------------------------------------------
	unsigned short m_Col[6];		// 미니맵 칼라 
	BOOL m_bEvent;					// 이벤트 번호 


public:
	void DrawObjectMap(unsigned short *pScreen);
	bool IsPassableP(int x, int y);
	bool IsPassable(int x, int y);

	BOOL IsEvent() { return m_bEvent;}					// 이벤트 번호 얻는 함수 
	int* m_pAttrMap;				// 속성 맵 배열

	int m_nEnemyCnt;
	int m_nConEnemyCnt;
	point m_nEnemyPos[UNITCNT];		// 집중시 미니맵에 적캐릭터위치를 표시하기위한 변수
	point m_nConEnemyPos[UNITCNT];

	bool SetConEnemyPos(SORTY* pSort, int nCount);
//	bool SetEnemyPos();

	int GetMapXPos() { return m_nXscl; }							// 스크롤 좌표 얻는 함수 (테시리아 맵 좌표) X
	int GetMapYPos() { return m_nYscl; }							// 스크롤 좌표 얻는 함수 (테시리아 맵 좌표) Y	
	int GetMapWidth() { return m_nMapW; }							// 맵 크기 얻는 함수 
	int GetMapHeight() { return m_nMapH; }
	int GetAttrMapWidth() { return m_nAttrMapW; }					// 속성맵 크기 얻는 함수 
	int GetAttrMapHeight() { return m_nAttrMapH; }
	
	void SetScrInfo(point* Teshy)                   // 스크롤 좌표 세팅 함수 
	{ m_nXscl = (Teshy->x)-400; m_nYscl = (Teshy->y)-300; }	
																	

	void SetMapImg(CMapImg* pTileImg, CMapImg* pCliffImg, CMapImg* pObjImg);	
																	// 맵 이미지 포인터 세팅하는 함수 

	int SetObjectSortList(SORTY* pSort, int i);					// Y정렬을 위해 화면상의 오브젝트 정보를 세팅하는 함수 
	int SortObjectCount();											// Y정렬을 위해 화면상의 오브젝트 갯수를 얻는 함수 
	void SetEventArea(rect rtEvent, int nEvent);			// 이벤트 영역 세팅 함수 


	void DrawMinimap(WORD *pScreen, int tX, int tY);				// 미니맵 출력 함수 
	void DrawObjectMap(WORD *pScreen, int nObj, int nX, int nY );	// 오브젝트 출력 함수 
	void DrawCliffMap(WORD *pScreen, bool Alpha);					// 절벽 출력 함수 
	void DrawTileMap(WORD* pScreen);								// 타일 출력 함수 

	void SetScroll(point* Te, point* Cur, int nDir, int nSpeed);		
																	// 스크롤 계산 함수 
	void SetTileSize(int x, int y, int ax, int ay);					// 타일등의 사이즈 세팅하는 함수 
		
	bool InitMap();													// 맵 초기화 함수 

	bool LoadMap(char* pFile);										// 맵 파일 로딩함수 

	CMap();													
	virtual ~CMap();

};

#endif // !defined(AFX_MAP_H__C0828102_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
