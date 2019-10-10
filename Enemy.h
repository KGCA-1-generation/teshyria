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
	// public ����
	int  m_nDistance;
	int  m_nMoveX, m_nMoveY;	// ��ã�� �߰��ڵ� 4-1(ScoutKnight.h)
	int  m_aAtCk[8];
	CAnimation* m_pAni;
	// public �Լ�
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
	void IamNotLeader() { m_bLeader = false; } // �� ������ �ƴϴ�.
	void IamLeader() { m_bLeader = true; }     // �� �������� �����̴�.

	// �ӽÿ�. ���߿� ������ ��.
	int GetGapX() { return m_nGapX; }
	int GetGapY() { return m_nGapY; }
	BOOL m_bAniLock, m_bDraw, m_bDead, m_bLeader;
	int  m_nTarX, m_nTarY; // �̵��� ��ǥ ��.
protected:
	// protected ����
	int  m_nNowMotion, m_nNowFrame, m_nCharStatus;
	int  m_nDirection; //, m_nDistance;
	int  m_nScrX, m_nScrY, m_nTeam;

	int  m_nTeX,  m_nTeY;  // �׽ø����� ��ǥ
	int  m_nGapX, m_nGapY; // �������� ��������
	int  m_nFrmCntLimit, m_nFrmCntNow;
	rect m_rtRect;

	char m_sMotID[12];

	CCharMng*    m_pCharMng;
	CPathFinder* m_pPath;

	// �밡���� ǥ����� ������ ���� ĳ�� ���� ������
public:
	int  m_HP;
protected:
	int  m_nSpeed;
	int  m_X, m_Y; // �ʻ󿡼��� X, Y ��ǥ	

	// protected �Լ�
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
