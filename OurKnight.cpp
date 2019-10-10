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
// 상태를 파악해서 캐릭의 애니를 정해줌
// return true  : 방향 고려
//        false : 방향 고려X (1 장의 그림)
//----------------------------------------------------------
int COurKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(false, 20, "Stand_0");		return true;
	case Beaten : // 공격 받기.
		SetAniData(true, 3, "Beaten_0");		return true;
	case Die :    // 죽기
		SetAniData(true, 4, "Die");				return false;
	}
	return false;
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//----------------------------------------------------------------------
void COurKnight::DrawChar(unsigned short* pScreen, BOOL bOutLine)
{
	m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bDead) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		if(!m_bDead) m_nNowFrame++;
		m_nFrmCntNow = 0;
	}
	
	if(IsEndFrm(m_nCharStatus))  // 애니가 잠겼다면
	{
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// 캐릭의 상태 설정 함수
//----------------------------------------------------------------------
void COurKnight::SetStatus(point* Teshy, TeamInfo* TInfo)
{
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
}

//----------------------------------------------------------------------
// 캐릭의 패턴 설정 함수
//----------------------------------------------------------------------
void COurKnight::Think()
{	
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// 내가 맞았나를 체크
//----------------------------------------------------------------------
void COurKnight::AmIBeaten(int *dir)
{	
}