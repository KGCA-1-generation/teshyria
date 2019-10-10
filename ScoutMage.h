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

	DWORD m_dwCommTime;	// time ���� ���� 
	int m_nCommTime;	// ��Ÿ��� �ð�
	BOOL m_bComm;		// ��Ÿ��� �������� 

	BOOL m_bBeaten;		// �¾Ҵ��� ���� 
	BOOL m_bPattern;	// ����������, ��������� ���� ���� 
	int m_nPattern;		// �����ϰ� ������ �ൿ ������ �����ϴ� ���� 

	CScoutMage(CAnimation* p, CCharMng* cp, int x, int y, int team);
	virtual ~CScoutMage();
};

#endif // !defined(AFX_SCOUTMAGE_H__B268D58C_76E8_4BD3_9860_0D9616E9403B__INCLUDED_)
