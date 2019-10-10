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
	// �� �̹��� ���� ������ ----------------------------------------------
	CMapImg* m_pTileImg;			// Ÿ�� �̹��� ������ 
	CMapImg* m_pObjImg;				// ������Ʈ �̹��� ������ 
	CMapImg* m_pCliffImg;			// ���� �̹��� ������ 

	// �� �迭 -------------------------------------------------------------
	int* m_pTileMap;				// Ÿ�� �� �迭
	int* m_pCliffMap;				// ���� �� �迭
	Object* m_pObjMap;				// ������Ʈ �� �迭

	// �ʰ� ���� ������ ������ --------------------------------------------
	int m_nWidth, m_nHeight;		// ���� �� ������ (�ȼ�����)
	int m_nTileW, m_nTileH;			// Ÿ�� ������(�ȼ�����)
	int m_nAttrW, m_nAttrH;			// �Ӽ�Ÿ�� ������( �ȼ�����)
	int m_nMapW, m_nMapH;			// �� ������ (Ÿ�� ����)
	int m_nAttrMapW, m_nAttrMapH;	// �Ӽ��� ������ (Ÿ�� ����)

	// �� ��� ���õ� ������ ------------------------------------------------
	int m_nDrawXs, m_nDrawYs;		// ȭ�鿡 �׷��� ���� ���� ��ǥ (Ÿ�ϴ���)
	int m_nMDrawXs, m_nMDrawYs;		// �̴ϸʿ� �׷��� ���� ���� ��ǥ (Ÿ�ϴ���)

	int m_nXscl, m_nYscl;			// ��ũ�� ��ǥ 	: �׽ø��� �� ��ǥ 


	// ��ũ�� ���� ������ -------------------------------------------------------
	float m_fYerror;				// ��ũ�ѽ� ���� X
	float m_fXerror;				// ��ũ�ѽ� ���� Y
	float m_fYvec;					// ��ũ���� �������� X
	float m_fXvec;					// ��ũ���� �������� Y
	int m_nYmove;					// ��ũ���� �Ÿ� X
	int m_nXmove;					// ��ũ���� �Ÿ� Y

	// ��Ÿ ������ ---------------------------------------------------------------
	unsigned short m_Col[6];		// �̴ϸ� Į�� 
	BOOL m_bEvent;					// �̺�Ʈ ��ȣ 


public:
	void DrawObjectMap(unsigned short *pScreen);
	bool IsPassableP(int x, int y);
	bool IsPassable(int x, int y);

	BOOL IsEvent() { return m_bEvent;}					// �̺�Ʈ ��ȣ ��� �Լ� 
	int* m_pAttrMap;				// �Ӽ� �� �迭

	int m_nEnemyCnt;
	int m_nConEnemyCnt;
	point m_nEnemyPos[UNITCNT];		// ���߽� �̴ϸʿ� ��ĳ������ġ�� ǥ���ϱ����� ����
	point m_nConEnemyPos[UNITCNT];

	bool SetConEnemyPos(SORTY* pSort, int nCount);
//	bool SetEnemyPos();

	int GetMapXPos() { return m_nXscl; }							// ��ũ�� ��ǥ ��� �Լ� (�׽ø��� �� ��ǥ) X
	int GetMapYPos() { return m_nYscl; }							// ��ũ�� ��ǥ ��� �Լ� (�׽ø��� �� ��ǥ) Y	
	int GetMapWidth() { return m_nMapW; }							// �� ũ�� ��� �Լ� 
	int GetMapHeight() { return m_nMapH; }
	int GetAttrMapWidth() { return m_nAttrMapW; }					// �Ӽ��� ũ�� ��� �Լ� 
	int GetAttrMapHeight() { return m_nAttrMapH; }
	
	void SetScrInfo(point* Teshy)                   // ��ũ�� ��ǥ ���� �Լ� 
	{ m_nXscl = (Teshy->x)-400; m_nYscl = (Teshy->y)-300; }	
																	

	void SetMapImg(CMapImg* pTileImg, CMapImg* pCliffImg, CMapImg* pObjImg);	
																	// �� �̹��� ������ �����ϴ� �Լ� 

	int SetObjectSortList(SORTY* pSort, int i);					// Y������ ���� ȭ����� ������Ʈ ������ �����ϴ� �Լ� 
	int SortObjectCount();											// Y������ ���� ȭ����� ������Ʈ ������ ��� �Լ� 
	void SetEventArea(rect rtEvent, int nEvent);			// �̺�Ʈ ���� ���� �Լ� 


	void DrawMinimap(WORD *pScreen, int tX, int tY);				// �̴ϸ� ��� �Լ� 
	void DrawObjectMap(WORD *pScreen, int nObj, int nX, int nY );	// ������Ʈ ��� �Լ� 
	void DrawCliffMap(WORD *pScreen, bool Alpha);					// ���� ��� �Լ� 
	void DrawTileMap(WORD* pScreen);								// Ÿ�� ��� �Լ� 

	void SetScroll(point* Te, point* Cur, int nDir, int nSpeed);		
																	// ��ũ�� ��� �Լ� 
	void SetTileSize(int x, int y, int ax, int ay);					// Ÿ�ϵ��� ������ �����ϴ� �Լ� 
		
	bool InitMap();													// �� �ʱ�ȭ �Լ� 

	bool LoadMap(char* pFile);										// �� ���� �ε��Լ� 

	CMap();													
	virtual ~CMap();

};

#endif // !defined(AFX_MAP_H__C0828102_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
