// Assist.h: interface for the CAssist class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASSIST_H__F3B83901_14D3_11D6_B3AA_0050DA898F90__INCLUDED_)
#define AFX_ASSIST_H__F3B83901_14D3_11D6_B3AA_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAnimation;
class CAssist  
{
protected:
	int m_x, m_y;
	int m_nDir;
	int m_nCount;
	CAnimation* m_pAni;
	void SetArrowXY();

public:
	void DrawArrow(unsigned short* pScreen, int dir);
	CAssist(CAnimation* p);
	virtual ~CAssist();

};

#endif // !defined(AFX_ASSIST_H__F3B83901_14D3_11D6_B3AA_0050DA898F90__INCLUDED_)
