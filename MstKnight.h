// MstKnight.h: interface for the CMstKnight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MSTKNIGHT_H__B95A7315_6A87_4C13_AB94_DEF8EF6E3F51__INCLUDED_)
#define AFX_MSTKNIGHT_H__B95A7315_6A87_4C13_AB94_DEF8EF6E3F51__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
#include "Enemy.h"
#include "NeedStruct.h"

class CCharMng;
class CAnimation;
class CMstKnight : public CEnemy
{
public:
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir, int What);

protected:
	TeamInfo*   m_pTeam;
	int			m_nWp;

	int  PreDefineAni();
	void Think();
	void GuardTo();

public:
	BOOL SetAttackPos(int dir);
	CMstKnight(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CMstKnight();
};
  
#endif // !defined(AFX_MSTKNIGHT_H__B95A7315_6A87_4C13_AB94_DEF8EF6E3F51__INCLUDED_)
