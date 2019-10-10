// Animal.cpp: implementation of the CAnimal class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
//#include <windows.h>
#include "Animation.h"
#include "CharMng.h"
#include "Animal.h"

#define Stand    0
#define Run      1
#define Fright	 2

#define Rabbit  10
#define Wolf    11
#define Bear    12

#define SiR    300    // 시야반경
#define SiR2   800    // 시야반경
//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CAnimal::CAnimal(CAnimation* p, CCharMng* cp, int x, int y, int team, int type)
{
	m_pTeam        = NULL; 
	m_pAni         = p;
	m_pCharMng     = cp;
	m_HP           = 1;
	m_nType        = type;
	switch(type) {
	case Rabbit : m_nSpeed = 15;	break;
	case Wolf   : m_nSpeed = 10;	break;
	case Bear   : m_nSpeed = 7;	break;
	}
	m_X            = x;
	m_Y            = y;
	m_nMoveX	   = m_X/32;
	m_nMoveY	   = m_Y/32;
	SetAniData(false, 10, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);
	m_nTeam        = team;
	m_bFright	   = false;
}

CAnimal::~CAnimal()
{
}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return true  : 방향 고려
//        false : 방향 고려X (1 장의 그림)
//----------------------------------------------------------
int CAnimal::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(false, 20, "Stand_0");		return true;
	case Run :    // 달리기
		if(m_nType==Rabbit) {
			SetAniData(false, 3, "Run_0");		return true;
		}
		else {
			SetAniData(false, 6, "Run_0");		return true;
		}
	case Fright :
		if(m_nType==Rabbit)	{
			SetAniData(true, 2, "Fright");		return false;
		}
		else {
			SetAniData(true, 8, "Fright_0");	return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//----------------------------------------------------------------------
void CAnimal::DrawChar(unsigned short* pScreen, int bOutLine)
{
	m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);

	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		m_nNowFrame++;
		if(m_nCharStatus==Run) MoveTo(); // 이동시는 pixel이 변하므로
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
void CAnimal::SetStatus(point* Teshy, TeamInfo* TInfo)
{
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_pTeam = TInfo;
	m_nDistance  = GetDistance (m_nTeX-m_X, m_nTeY-m_Y);
	m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
	Think();
	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// 캐릭의 패턴 설정 함수
//----------------------------------------------------------------------
void CAnimal::Think()
{
	if( m_nDistance < SiR ) 
	{
		if( !m_bFright ) {
			m_bFright = true;
			m_nCharStatus = Fright;
		}
		else {
			if( !m_bAniLock )	AnimalRun();
		}
	}
	else {
		if( m_nDistance > SiR2 && m_bFright ) m_bFright = false;
		AnimalRun();
	}
	m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
}

void CAnimal::AnimalRun()
{
	if( m_pTeam->nWpNum == m_pTeam->nWPCnt ) m_pTeam->nWpNum = 0;
			
	m_nTarX = m_pTeam->pWpX[m_pTeam->nWpNum];
	m_nTarY = m_pTeam->pWpY[m_pTeam->nWpNum];
	if( (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000010) || (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000001) )
	{
		m_pTeam->nWpNum++;
	}

	if(abs(m_nTarX-m_X)<10 && abs(m_nTarY-m_Y)<10) {		// 목표지점에 다 왔을때
		m_nCharStatus = Stand;								// 
		m_pTeam->nWpNum++;
	}
	else													// 아닐때 
	{
		m_nCharStatus = Run;
	}
}
