// CharMng.h: interface for the CCharMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARMNG_H__69C3E419_DBCC_45FA_A6E6_A6CB04A6424B__INCLUDED_)
#define AFX_CHARMNG_H__69C3E419_DBCC_45FA_A6E6_A6CB04A6424B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXNUM      20   // ������ ��ĳ���� ��
#define SORTCHAR    200  // �������� ȭ����� ������Ʈ �� ĳ�� ����
#define ENEMYCNT    100  // ��ĳ���� ��
#define NPCCNT      100  // ���Ϳ� �߻����� ��
#define OBJCNT      32   // �߻�Ǵ� ������Ʈ ��
#define ANILIST     19   // �ִϸ��̼� ����
#define ALLCharTYPE 16   // ĳ�� Ÿ�� ��(�׽ø��Ƹ� ����)
#define TEAMNUM     20   // �� ��

#include "Map.h"
#include "NeedStruct.h"

class CAnimation;
class CMainChar;
class CEnemy;
class CScoutMage;
class CScoutKnight;
class CRangerMage;
class CRangerKnight;
class CMstKnight;
class COurMage;
class COurKnight;
class CAnimal;
class CAssist;
class CCharObj;
class CPathFinder;
class CMapMng;
class CCharMng  
{
// protected ������
protected :	
public:
	int  m_nTeX, m_nTeY;		    // �׽ø����� ��ǥ
	int  m_nCurX, m_nCurY;          // Ŀ�� ��ǥ
	int  m_nTeamCnt, m_nCharCnt;    // ȭ����� �� �� ��ü �� ( �� ĳ���͵�+�׽ø���+������Ʈ �� )
	int  m_aUnitSum[ALLCharTYPE];   // 1~16������ ĳ���� ������
	int  m_nLoadedCharCnt;          // ���� �ε��� ��ĳ�� ��(�ӽ�)
	int  m_nArrowDir;               // ȭ��ǥ ����. �⺻�� -1 
	int  m_nSelectedChar;
	int  m_nObjectNum;
	char m_strAniFile[ANILIST][40]; // �ִ� ȭ�ϸ�(40��)

	CMapMng*   m_pMapMng;		    // �ʸŴ��� ������ 
	SORTY      m_aSort[SORTCHAR];	// Y������ ���� ����ü �迭
	StageInfo* m_pStageInfo;        // �������� ������
	
// protected �Լ���
	void LoadAnis();
	void CreateChar();
	bool CreateObj(int num, int x, int y, int tx, int ty);
	bool DeleteObj(int num);
	void DeleteChar();
	void ResetTeamInfo(int num);
	void SettingCharAttr();
// public ������
public:	
	int TObjectAndEnemy(rect* rt);
	int m_nTest;
	CAnimation*    m_pAniArray[ANILIST];

	TeamInfo       m_pTeam[TEAMNUM];
	
	CMainChar*     m_pTeshy;
	CEnemy*        m_pEnemy[ENEMYCNT];
	CScoutMage*    m_pScMage[MAXNUM];
	CScoutKnight*  m_pScKnight[MAXNUM];
	CRangerMage*   m_pRgMage[MAXNUM];
	CRangerKnight* m_pRgKnight[MAXNUM];
	CMstKnight*    m_pMKnight[MAXNUM];
	COurMage*      m_pOurMage[MAXNUM];
	COurKnight*    m_pOurKnight[MAXNUM];
	CAnimal*       m_pAnimal[MAXNUM];

	CPathFinder*   m_pPath;
	CAssist*       m_pAssist;
	CCharObj*      m_pObject[OBJCNT]; 

// public �Լ���
	void SetInfo(CMapMng* pMap, StageInfo* Stage);
	void StatusIn(int* dir, int key, point* Teshy, point* Curs);
	void LocationIn(point* Teshy);
	void Draw(unsigned short* Screen, point* Curs);
	void BeatenTeshy(int npower);
	void HitEnemy(int SChar, int dir, int AtckType);
	void SetSortList();
	void SortYPos();
	void MarkPos(int x, int y, int type, BOOL bOn);
	int  GetDistance();
	int  GetMapAttrP(int Xp, int Yp);
	int  GetMapAttrN(int Xn, int Yn);
	int  CanScroll();

	CCharMng();
	virtual ~CCharMng();
};

#endif // !defined(AFX_CHARMNG_H__69C3E419_DBCC_45FA_A6E6_A6CB04A6424B__INCLUDED_)
