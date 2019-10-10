// CharObj.h: interface for the CCharObj class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHAROBJ_H__5DE75E61_16A4_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_CHAROBJ_H__5DE75E61_16A4_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NeedStruct.h"

class CCharMng;
class CAnimation;
class CCharObj  
{
public:
	int  m_bEnd;
	void Draw(unsigned short* Screen, int TeX, int TeY);
	int  GetType() { return m_nType; }

protected:
	int  m_sx, m_sy;  // 오브젝트 생성의 스타트 좌표
	int  m_x, m_y;
	int  m_tx, m_ty;  // 타겟점 좌표.
	int  m_gx, m_gy;  // 사각충돌영역의 보정치
	int  m_nType;
	int  m_nMotion, m_nFrame, m_nCntLimit;
	int  m_nSpeed;
	int  m_nRange, m_nDistance;
	int  m_bEogumTurn;
	rect m_rtObject;

	double m_dUnitVeX, m_dUnitVeY;

	CAnimation* m_pAni;
	CCharMng*   m_pCharMng;

	bool DestroyMe();
	void TypeToMotion();
	void CDCheck();
	void CalUnitVector();
	void SetFrmRect();
	void EogumTurn();
	int  GetDirection16();

public:
	CCharObj(CAnimation* p, CCharMng* cp, int type, int x, int y, int tx, int ty);
	virtual ~CCharObj();
};

#endif // !defined(AFX_CHAROBJ_H__5DE75E61_16A4_11D6_B3AA_0050DA898F90__INCLUDED_)
