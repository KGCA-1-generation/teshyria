// CharMng.h: interface for the CCharMgr class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARMNG_H__69C3E419_DBCC_45FA_A6E6_A6CB04A6424B__INCLUDED_)
#define AFX_CHARMNG_H__69C3E419_DBCC_45FA_A6E6_A6CB04A6424B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAXNUM      20   // 적들의 각캐릭의 수
#define SORTCHAR    200  // 보여지는 화면상의 오브젝트 및 캐릭 개수
#define ENEMYCNT    100  // 적캐릭의 수
#define NPCCNT      100  // 몬스터와 야생동물 수
#define OBJCNT      32   // 발사되는 오브젝트 수
#define ANILIST     19   // 애니메이션 개수
#define ALLCharTYPE 16   // 캐릭 타입 수(테시리아를 제외)
#define TEAMNUM     20   // 팀 수

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
// protected 변수들
protected :	
public:
	int  m_nTeX, m_nTeY;		    // 테시리아의 좌표
	int  m_nCurX, m_nCurY;          // 커서 좌표
	int  m_nTeamCnt, m_nCharCnt;    // 화면상의 팀 밑 객체 수 ( 적 캐릭터들+테시리아+오브젝트 수 )
	int  m_aUnitSum[ALLCharTYPE];   // 1~16까지의 캐릭의 누적수
	int  m_nLoadedCharCnt;          // 현재 로딩된 적캐릭 수(임시)
	int  m_nArrowDir;               // 화살표 방향. 기본값 -1 
	int  m_nSelectedChar;
	int  m_nObjectNum;
	char m_strAniFile[ANILIST][40]; // 애니 화일명(40자)

	CMapMng*   m_pMapMng;		    // 맵매니저 포인터 
	SORTY      m_aSort[SORTCHAR];	// Y정렬을 위한 구조체 배열
	StageInfo* m_pStageInfo;        // 스테이지 포인터
	
// protected 함수들
	void LoadAnis();
	void CreateChar();
	bool CreateObj(int num, int x, int y, int tx, int ty);
	bool DeleteObj(int num);
	void DeleteChar();
	void ResetTeamInfo(int num);
	void SettingCharAttr();
// public 변수들
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

// public 함수들
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
