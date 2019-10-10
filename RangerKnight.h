// RangerKnight.h: interface for the CRangerKnight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGERKNIGHT_H__49575F41_11F9_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_RANGERKNIGHT_H__49575F41_11F9_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
#include "Enemy.h"
#include "NeedStruct.h"

class CCharMng;
class CAnimation;
class CRangerKnight : public CEnemy
{
public:
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir, int What);

protected:
	TeamInfo*   m_pTeam;

	int  PreDefineAni();
	void Think();
	void GuardTo();

public:
	BOOL SetAttackPos(int dir);
	CRangerKnight(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CRangerKnight();

};

#endif // !defined(AFX_RANGERKNIGHT_H__49575F41_11F9_11D6_B3AA_0050DA898F90__INCLUDED_)
