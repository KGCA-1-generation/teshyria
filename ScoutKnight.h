// ScoutKnight.h: interface for the CScoutKnight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOUTKNIGHT_H__3080DFF0_E18A_4B88_A3DF_A2889465D982__INCLUDED_)
#define AFX_SCOUTKNIGHT_H__3080DFF0_E18A_4B88_A3DF_A2889465D982__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class CScoutKnight : public CEnemy
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
	CScoutKnight(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CScoutKnight();
};

#endif // !defined(AFX_SCOUTKNIGHT_H__3080DFF0_E18A_4B88_A3DF_A2889465D982__INCLUDED_)
