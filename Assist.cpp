// Assist.cpp: implementation of the CAssist class.
//
//////////////////////////////////////////////////////////////////////
#include "Animation.h"
#include "Assist.h"

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CAssist::CAssist(CAnimation* p)
{
	m_pAni = p;
	m_nCount = 0;
}

CAssist::~CAssist()
{
}

//--------------------------------------------------------------------
// 정찰대 방향의 화살표 그리기
//-------------------------------------------------------------------- 
void CAssist::DrawArrow(unsigned short* pScreen, int dir)
{
	static fr = 0, count=0;
	if(dir != -1)
	{
		m_nDir = dir; 
		SetArrowXY();
		if(m_nCount<10)
			m_pAni->DrawAni(m_x, m_y, m_nDir, 0, pScreen);
		m_pAni->DrawAni(400, 200, 8, fr, pScreen);

		if(count==10) { fr++; count=0; }
		count ++;
		fr = fr%3;
		m_nCount++;
		m_nCount = m_nCount%25;
	}
}

void CAssist::SetArrowXY()
{
	switch(m_nDir)
	{
	case 0:
		m_x = 500;
		m_y = 255;
		break;
	case 1:
		m_x = 470;
		m_y = 180;
		break;
	case 2:
		m_x = 400;
		m_y = 160;
		break;
	case 3:
		m_x = 330;
		m_y = 180;
		break;
	case 4:
		m_x = 310;
		m_y = 255;
		break;
	case 5:
		m_x = 330;
		m_y = 330;
		break;
	case 6:
		m_x = 400;
		m_y = 370;
		break;
	case 7:
		m_x = 500;
		m_y = 330;
		break;
	}
}