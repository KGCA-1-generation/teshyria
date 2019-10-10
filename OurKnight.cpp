// OurKnight.cpp: implementation of the COurKnight class.
//
//////////////////////////////////////////////////////////////////////
#include "Animation.h"
#include "CharMng.h"
#include "OurKnight.h"

#define Stand    0
#define Beaten   1
#define Die      2
//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
COurKnight::COurKnight(CAnimation* p, CCharMng* cp, int x, int y)
{
	m_pAni         = p;
	m_pCharMng     = cp;
	m_HP           = 400;
	m_nSpeed       = 15;
	m_X            = x;
	m_Y            = y;
	SetAniData(false, 20, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);
}

COurKnight::~COurKnight()
{
}

//----------------------------------------------------------
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return true  : ���� ���
//        false : ���� ���X (1 ���� �׸�)
//----------------------------------------------------------
int COurKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 20, "Stand_0");		return true;
	case Beaten : // ���� �ޱ�.
		SetAniData(true, 3, "Beaten_0");		return true;
	case Die :    // �ױ�
		SetAniData(true, 4, "Die");				return false;
	}
	return false;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
//----------------------------------------------------------------------
void COurKnight::DrawChar(unsigned short* pScreen, BOOL bOutLine)
{
	m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bDead) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		if(!m_bDead) m_nNowFrame++;
		m_nFrmCntNow = 0;
	}
	
	if(IsEndFrm(m_nCharStatus))  // �ִϰ� ���ٸ�
	{
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void COurKnight::SetStatus(point* Teshy, TeamInfo* TInfo)
{
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void COurKnight::Think()
{	
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// ���� �¾ҳ��� üũ
//----------------------------------------------------------------------
void COurKnight::AmIBeaten(int *dir)
{	
}