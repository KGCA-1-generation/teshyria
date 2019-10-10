// ScoutMage.h: interface for the CScoutMage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SCOUTMAGE_H__B268D58C_76E8_4BD3_9860_0D9616E9403B__INCLUDED_)
#define AFX_SCOUTMAGE_H__B268D58C_76E8_4BD3_9860_0D9616E9403B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "function.h"
#include "NeedStruct.h"
#include "Enemy.h"

class CCharMng;
class CAnimation;
class CScoutMage : public CEnemy
{
public:	
	void DrawChar(unsigned short* pScreen, BOOL bOutLine);
	void SetStatus(point* Teshy, TeamInfo* TInfo);
	void AmIBeaten(int *dir, int What);
	TeamInfo* m_pTeam;

protected:

	int PreDefineAni();
	void Think();

public:

	DWORD m_dwCommTime;	// time 저장 변수 
	int m_nCommTime;	// 통신마법 시간
	BOOL m_bComm;		// 통신마법 성공여부 

	BOOL m_bBeaten;		// 맞았는지 여부 
	BOOL m_bPattern;	// 공격형인지, 통신형인지 결정 여부 
	int m_nPattern;		// 랜덤하게 결정된 행동 패턴을 저장하는 변수 

	CScoutMage(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CScoutMage();
};

#endif // !defined(AFX_SCOUTMAGE_H__B268D58C_76E8_4BD3_9860_0D9616E9403B__INCLUDED_)
