// OurKnight.h: interface for the COurKnight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OURKNIGHT_H__3ECCA7C2_1405_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_OURKNIGHT_H__3ECCA7C2_1405_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "function.h"
#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class COurKnight : public CEnemy  
{
public:	
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir);

protected:
	int  PreDefineAni();
	void Think();

public:
	COurKnight(CAnimation* p, CCharMng* cp, int x, int y);
	virtual ~COurKnight();

};

#endif // !defined(AFX_OURKNIGHT_H__3ECCA7C2_1405_11D6_B3AA_0050DA898F90__INCLUDED_)
