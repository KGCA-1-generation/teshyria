// RangerMage.h: interface for the CRangerMage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RANGERMAGE_H__FBF11FA1_1277_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_RANGERMAGE_H__FBF11FA1_1277_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class CRangerMage : public CEnemy  
{
public:
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir, int What);
	TeamInfo*   m_pTeam;

protected:


	int  PreDefineAni();
	void Think();

public:
	void GuardTo();
	CRangerMage(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CRangerMage();
};

#endif // !defined(AFX_RANGERMAGE_H__FBF11FA1_1277_11D6_B3AA_0050DA898F90__INCLUDED_)
