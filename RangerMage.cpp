// RangerMage.cpp: implementation of the CRangerMage class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <windows.h>
#include "CharMng.h"
#include "Animation.h"
#include "function.h"
#include "RangerMage.h"

#define Stand  0
#define Run    1
#define Attack 2
#define Beaten 3
#define Die    4
#define RgMg   80    // 마법사와의 간격거리
//--------------------------------------------------------------------
// 순찰대 마법사
//--------------------------------------------------------------------
#define AT    220    // 공격력
#define DF     10    // 방어력
#define VP     20    // 회피 확률
#define SiR   650    // 시야반경
#define AC    200    // 공격반경
#define AL    260    // 마법 사정 거리
#define AtR   250    // 공격반경
#define CT     10    // 통신 소요 시간
#define CD      2    // 캐스팅 딜레이

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CRangerMage::CRangerMage(CAnimation* p, CCharMng* cp, int x, int y, int team)
{
	m_pAni         = p;
	m_pCharMng     = cp;
	m_pTeam        = NULL; 
	m_HP           = 130;
	m_nSpeed       = 20;
	m_X            = x;
	m_Y            = y;	
	m_nMoveX		= m_X/32;
	m_nMoveY		= m_Y/32;
	m_nTeam        = team;
	SetAniData(FALSE, 12, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, TRUE);
}

CRangerMage::~CRangerMage()
{

}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return TRUE  : 방향 고려
//        FALSE : 방향 고려X (1 장의 그림)
//----------------------------------------------------------
int CRangerMage::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(FALSE, 12, "Stand_0");		return TRUE;
	case Run :    // 달리기
		SetAniData(FALSE, 5, "Run_0");			return TRUE;
	case Attack : // 매직애로우 공격
		SetAniData(TRUE, 4, "Attack_0");		return TRUE;
	case Beaten : // 공격 받기.
		SetAniData(TRUE, 3, "Beaten_0");		return TRUE;
	case Die :    // 죽기
		SetAniData(TRUE, 4, "Die");				return FALSE;
	}
	return FALSE;
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//----------------------------------------------------------------------
void CRangerMage::DrawChar(unsigned short* pScreen, BOOL bOutLine)
{
	if(bOutLine) {
		m_pAni->DrawAniOL(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	}
	else 
		m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bDead) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		if(!m_bDead) m_nNowFrame++;
		if(m_nCharStatus==Run) MoveTo(); // 이동시는 pixel이 변하므로
		m_nFrmCntNow = 0;
	}
	if(IsEndFrm(Beaten)) { 
		m_HP -=100;			
	}
	if(IsEndFrm(Die)) {
		m_bDead = TRUE;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
	if(IsEndFrm(m_nCharStatus)) { // 애니가 잠겼다면
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// 캐릭의 상태 설정 함수
//----------------------------------------------------------------------
void CRangerMage::SetStatus(point* Teshy, TeamInfo* TInfo)
{	
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_pTeam = TInfo;

	if(m_HP>0) {
		m_nDistance = GetDistance( m_nTeX-m_X, m_nTeY-m_Y );
		m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
		if(!m_bAniLock) Think();
	}
	else { 
		m_nCharStatus = Die; // 죽기
	}

	m_nScrX = m_X-m_nTeX + 400;
	m_nScrY = m_Y-m_nTeY + 300;	
	
	if( m_bLeader )	{
		if(!m_bDead) {
			m_pTeam->LeaderX  = m_X; 
			m_pTeam->LeaderY  = m_Y;
			m_pTeam->Distance = m_nDistance;
		}
	}
	
	if(!m_bDead) m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// 캐릭의 패턴 설정 함수
//----------------------------------------------------------------------
void CRangerMage::Think()
{	
	if( m_nDistance < SiR)			// 시야 반경안에 들어왔을 경우
	{
		if( m_nDistance < AtR ){		// 공격 영역안에 들어왔을 경우
			m_nCharStatus = Attack;
		}
		else {						// 공격영역 밖에 있을 경우 
			m_nTarX = m_nTeX;
			m_nTarY = m_nTeY;
			m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			m_nCharStatus = Run;		
		}
	}
	else{							// 시야 반경 밖에 있을때
		if( m_bLeader )	{
			
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) {		// 목표지점에 다 왔을때
				m_nCharStatus = Stand;								// 
				m_pTeam->nWpNum++;
			}
			if( m_pTeam->nWpNum == m_pTeam->nWPCnt )	m_pTeam->nWpNum = 0;	
			m_nTarX = m_pTeam->pWpX[m_pTeam->nWpNum];
			m_nTarY = m_pTeam->pWpY[m_pTeam->nWpNum];
			if( (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000010) || (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000001) )
				m_pTeam->nWpNum++;
			m_nCharStatus = Run;
			m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
		}
	
		else {
			m_nTarX = m_pTeam->LeaderX+GetGapX();
			m_nTarY = m_pTeam->LeaderY+GetGapY();
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) 	m_nCharStatus = Stand;	// 목표지점에 다 왔을때
			else {
				m_nCharStatus = Run;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			}
			
		}
	}
}

//----------------------------------------------------------------------
// 내가 맞았나를 체크
//----------------------------------------------------------------------
void CRangerMage::AmIBeaten(int *dir, int What)
{
	if(m_bDead) return;
	if(m_HP>0) {  // 맞았다면) {
		if(m_nDistance<90 && (What==1 || What==2) && m_nCharStatus!=Beaten) {
			m_nCharStatus = Beaten;
			m_nNowFrame = 0;
		}
		else {
			if(What!=-1 && m_nCharStatus!=Beaten)
			{
				m_nCharStatus = Beaten;
				m_nNowFrame = 0;
			}
		}
	}
	m_nNowMotion = DecideAni();
}
void CRangerMage::GuardTo()
{
	m_nTarX = RgMg*(m_nTeX-m_pTeam->LeaderX)/m_pTeam->Distance + m_pTeam->LeaderX;
	m_nTarY = RgMg*(m_nTeY-m_pTeam->LeaderY)/m_pTeam->Distance + m_pTeam->LeaderY;
}

