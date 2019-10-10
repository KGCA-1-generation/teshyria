// Enemy.h: interface for the CEnemy class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENEMY_H__1EFB8E01_1259_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_ENEMY_H__1EFB8E01_1259_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
#include "NeedStruct.h"

class CPathFinder;
class CCharMng;
class CAnimation;
class CEnemy  
{
public:
	// public 변수
	int  m_nDistance;
	int  m_nMoveX, m_nMoveY;	// 길찾기 추가코딩 4-1(ScoutKnight.h)
	int  m_aAtCk[8];
	CAnimation* m_pAni;
	// public 함수
	int  GetXPos()    { return m_X; }
	int  GetYPos()    { return m_Y; }
	int  GetTeamNum() { return m_nTeam; }
	int  GetCharDir() { return m_nDirection; }
	int  GetNMotion() { return m_nNowMotion; }
	int  GetNFrame()  { return m_nNowFrame; }
	int  IsLock()     { return m_bAniLock; }
	BOOL IsDead()     { return m_bDead; }
	BOOL IsDraw();
	BOOL IsEndFrm(int AniStatus);
	BOOL IsMidFrm(int AniStatus);
	bool IsFrmRect(int x, int y);
	bool IsFrmRect(rect* rt);
	virtual BOOL IsMovable(int dir);
	virtual void DrawChar(unsigned short* pScreen, int bOutline);
	virtual void SetStatus(point* Teshy, TeamInfo* TInfo);
	virtual void AmIBeaten(int *dir, int What);
	void SetPathFinder(CPathFinder* pPath){ m_pPath = pPath; }
	void SetGap(int num);
	void IamNotLeader() { m_bLeader = false; } // 난 대장이 아니다.
	void IamLeader() { m_bLeader = true; }     // 난 이제부터 대장이다.

	// 임시용. 난중에 지워도 됨.
	int GetGapX() { return m_nGapX; }
	int GetGapY() { return m_nGapY; }
	BOOL m_bAniLock, m_bDraw, m_bDead, m_bLeader;
	int  m_nTarX, m_nTarY; // 이동시 목표 점.
protected:
	// protected 변수
	int  m_nNowMotion, m_nNowFrame, m_nCharStatus;
	int  m_nDirection; //, m_nDistance;
	int  m_nScrX, m_nScrY, m_nTeam;

	int  m_nTeX,  m_nTeY;  // 테시리아의 좌표
	int  m_nGapX, m_nGapY; // 리더와의 간격정도
	int  m_nFrmCntLimit, m_nFrmCntNow;
	rect m_rtRect;

	char m_sMotID[12];

	CCharMng*    m_pCharMng;
	CPathFinder* m_pPath;

	// 헝가리안 표기법을 따르지 않은 캐릭 상태 변수들
public:
	int  m_HP;
protected:
	int  m_nSpeed;
	int  m_X, m_Y; // 맵상에서의 X, Y 좌표	

	// protected 함수
	virtual int  PreDefineAni();
	virtual void Think();
	virtual void MoveTo();
	int  DecideAni();
	void SetFrmRect();
	void SetAniData(BOOL lock, int cnt, char* str);

public:
	CEnemy();
	virtual ~CEnemy();
};

#endif // !defined(AFX_ENEMY_H__1EFB8E01_1259_11D6_B3AA_0050DA898F90__INCLUDED_)
