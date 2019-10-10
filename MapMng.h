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

	// �̼������� ���õ� ������ ------------------------------------------
	char* m_strStageFile[6];
	char* (m_pMapFile[3]);		// �� ���丮�� �� ���� �� 
	int m_nStory;				// ���丮 �Ϸù�ȣ (1~3)
	int m_nStage;				// �̼� �Ϸù�ȣ (1~6)
	
	// Ÿ�ϵ��� ������ ������ --------------------------------------------
	int m_nTileW, m_nTileH;		// Ÿ�� ������(�ȼ�����)
	int m_nAttrW, m_nAttrH;		// �Ӽ�Ÿ�� ������( �ȼ�����)

	// �� �̹��� ���� ������ -----------------------------------------------------
	int m_nObjTotal;			// ���� ������Ʈ �̹��� ����
	int m_nCliffTotal;			// ���� ���� Ÿ�� �̹��� ���� 
	int m_nTileTotal;			// ���� Ÿ�� �̹��� ����

	CMapImg* m_pTileImg;
	CMapImg* m_pCliffImg;
	CMapImg* m_pObjImg;
	
	// �þ�ó�� ���� �̹��� ������ -----------------------------------------------
//	CImage m_Black;
//	CImage m_Fog;

public:
	CMap* m_pMap;													// �� ��ü 
	char m_MapFile[30];
public:
	CImage m_SightImg;
	void DrawSightMask(WORD* pScreen);

	int GetMapAttrP(int Xp, int Yp);
	int	GetMapAttrN(int Xn, int Yn);
//	void DrawFog(WORD* pScreen);									// �þ߱� ��� �Լ� 
	void DrawObjectMap(WORD *pScreen, int nObj, int nX, int nY);	// ������Ʈ �� ����Լ� 
	void DrawCliffMap(WORD* pScreen, bool Alpha);					// ������ ����Լ� 
	void DrawTileMap(WORD* pScreen);								// �ٴ� Ÿ�ϸ� ����Լ� 

	bool LoadStage(int nStage, StageInfo* pStg, point* Teshy, BOOL b565Mode);									// �̼����� �ε� �Լ� 
	void LoadMap();										// ������ �ε��Լ� 
	bool LoadMapImg(BOOL b565Mode);												// �� �̹��� ���� �ε��Լ� 
	void InitMapImg();												// �� �̹��� �ʱ�ȭ �Լ� 
	
	CMapMng();
	virtual ~CMapMng();
};

#endif // !defined(AFX_MAPMNG_H__C0828101_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
