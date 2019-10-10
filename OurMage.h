// OurMage.h: interface for the COurMage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OURMAGE_H__3ECCA7C1_1405_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_OURMAGE_H__3ECCA7C1_1405_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "function.h"
#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class COurMage : public CEnemy  
{
public:	
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir);

protected:
	int  PreDefineAni();
	void Think();

public:
	COurMage(CAnimation* p, CCharMng* cp, int x, int y);
	virtual ~COurMage();

};

#endif // !defined(AFX_OURMAGE_H__3ECCA7C1_1405_11D6_B3AA_0050DA898F90__INCLUDED_)
