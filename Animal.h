// Animal.h: interface for the CAnimal class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMAL_H__1D5387C1_1424_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_ANIMAL_H__1D5387C1_1424_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "function.h"
#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class CAnimal : public CEnemy  
{
public:	
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);

	TeamInfo*	m_pTeam;	
protected:
	int  m_nType;  // µøπ∞ ≈∏¿‘ 10:≈‰≥¢, 11:¥¡¥Î, 12:∞ıµπ¿Ã
	int  PreDefineAni();
	void Think();

public:
	void AnimalRun();
	BOOL m_bFright;
	CAnimal(CAnimation* p, CCharMng* cp, int x, int y, int team, int type);
	virtual ~CAnimal();
};

#endif // !defined(AFX_ANIMAL_H__1D5387C1_1424_11D6_B3AA_0050DA898F90__INCLUDED_)
