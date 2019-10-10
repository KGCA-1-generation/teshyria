// ScoutKnight.cpp: implementation of the CScoutKnight class.
//
//////////////////////////////////////////////////////////////////////
#include <stdlib.h>
//#include <math.h>
#include "CharMng.h"
#include "Animation.h"
#include "function.h"
#include "ScoutKnight.h"

//#define POW(a) ( (a)*(a) )
#define Stand   0
#define Run     1
#define Attack1 2
#define Attack2 3
#define Beaten  4
#define Die1    5
#define Die2    6
//--------------------------------------------------------------------
// 정찰대 기사
//--------------------------------------------------------------------
#define AT    240    // 공격력
#define DF     20    // 방어력
#define SpD  20.0f   // 스피드
#define VP     25    // 회피 확률
#define SiR   750    // 시야반경
#define AtR   250    // 공격반경
#define AtCk  150    // 공격 충돌 체크
#define RgMg   80    // 마법사와의 간격거리

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CScoutKnight::CScoutKnight(CAnimation* p, CCharMng* cp, int x, int y, int team)
{
	m_pAni         = p;
	m_pCharMng     = cp;
	m_pTeam        = NULL;  
	m_HP           = 400;
	m_nSpeed       = 20;
	m_X            = x;
	m_Y            = y;
	m_nMoveX       = m_X/32;
	m_nMoveY       = m_Y/32;
	m_nTeam        = team;
	SetAniData(FALSE, 16, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);

	//방향에 따른 공격충돌범위  
	m_aAtCk[0] = 150;
	m_aAtCk[1] = 130;
	m_aAtCk[2] = 80;
	m_aAtCk[3] = 130;
	m_aAtCk[4] = 150;
	m_aAtCk[5] = 130;
	m_aAtCk[6] = 80;
	m_aAtCk[7] = 130;
}

CScoutKnight::~CScoutKnight()
{
}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return TRUE  : 방향 고려
//        FALSE : 방향 고려X (1방향 애니)
//----------------------------------------------------------
int CScoutKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(false, 16, "Stand_0");		return true;
	case Run :    // 달리기
		SetAniData(false, 3, "Run_0");			return true;
	case Attack1 : // 공격
		SetAniData(true, 3, "Attack1_0");		return true;
	case Attack2 : // 공격
		SetAniData(true, 3, "Attack2_0");		return true;
	case Beaten : // 공격 받기
		SetAniData(true, 3, "Beaten_0");		return true;
	case Die1 :   // 죽기 1
		SetAniData(true, 4, "Die1");			return false;
	case Die2 :   // 죽기 2
		SetAniData(true, 4, "Die2");			return false;
	}
	return FALSE;
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//----------------------------------------------------------------------
void CScoutKnight::DrawChar(unsigned short* pScreen, int bOutLine)
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
	// 공격자 중간 프레임을 만났을 때
	if(IsMidFrm(Attack1) || IsMidFrm(Attack2)) {
		// 중간 프레임시 한번더 테시리아와의 거리를 계산한다.
		m_nDistance  = GetDistance(abs(m_nTeX-m_X), abs(m_nTeY-m_Y));
		if(m_nDistance<AtCk) { m_pCharMng->BeatenTeshy(AT); } 
	}
	// 맞았을 때 마지막 프레임을 만났을 때
	if(IsEndFrm(Beaten)) {
		m_HP -= 100;			
	}
	// 죽었을 때 마지막 프레임을 만났을 때
	if(IsEndFrm(Die1) || IsEndFrm(Die2)) {
		m_bDead = TRUE;
		m_pTeam->nDeadCnt++;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
	// 애니가 잠겼다면
	if(IsEndFrm(m_nCharStatus)) { 
		m_nCharStatus  = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// 캐릭의 상태 설정 함수
//----------------------------------------------------------------------
void CScoutKnight::SetStatus(point* Teshy, TeamInfo* TInfo)
{	
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_pTeam = TInfo;
	
	if(m_HP>0) {
		m_nDistance  = GetDistance(abs(m_nTeX-m_X), abs(m_nTeY-m_Y));
		m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
		if(!m_bAniLock) Think();
	}
	else {
		if(m_nCharStatus!=Die1 && m_nCharStatus!=Die2) 
			m_nCharStatus=Die1+(rand()%2);
	}

	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
		
	if(!m_bDead) m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// 캐릭의 패턴 설정 함수
//----------------------------------------------------------------------
void CScoutKnight::Think()
{	
	if( m_nDistance<SiR ) // 시야 반경안에 들어왔을 경우
	{
		m_pTeam->bFind = TRUE;
		if( m_nDistance < AtR ) // 공격 영역안에 들어왔을 경우
		{
			m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
			if(m_nDistance > m_aAtCk[m_nDirection] ) // 칼범위 밖에 있을 때
			{ 				
				m_nCharStatus = Run;
				m_nTarX = m_nTeX;
				m_nTarY = m_nTeY;
			} 
			else                   // 칼범위 안에 있을 때
			{
				if(m_nCharStatus != Attack1 && m_nCharStatus != Attack2)
					m_nCharStatus = Attack1 + (rand()%2);
			}
		}
		else                   // 공격 영역이 아닌 경우
		{           
			if(m_pTeam->bLeader) // 마법사가 살아있을 때 (호위모드)
			{
				m_nCharStatus = Run;
				GuardTo();
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
				// 목표지점에 왔을 때
				if(abs(m_nTarX-m_X)<10 && abs(m_nTarY-m_Y)<10) {
					m_nCharStatus = Stand;
					m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
				}
			}
			else               // 마법사가 죽었을 때
			{
				m_nTarX = 2000;
				m_nTarY = 2000;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
				m_nCharStatus = Run;
			}
		}
	}
	else // 시야 반경 밖에 있을 경우
	{
		//if(m_pTeam->bFind) m_nCharStatus = 0;
	}
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// 내가 맞았나를 체크
//----------------------------------------------------------------------
void CScoutKnight::AmIBeaten(int *dir, int What)
{
	if(m_bDead) return;
	if(m_HP>0) {  // 맞았다면
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

//----------------------------------------------------------------------
// 캐릭의 호위 모드 시 목표점 설정 함수
// 목표점은 정찰대 마법사와 테시리아 사이에서 법사로부터 거리 RgMg위치
//----------------------------------------------------------------------
void CScoutKnight::GuardTo() 
{	
	m_nTarX = RgMg*(m_nTeX-m_pTeam->LeaderX)/m_pTeam->Distance + m_pTeam->LeaderX;
	m_nTarY = RgMg*(m_nTeY-m_pTeam->LeaderY)/m_pTeam->Distance + m_pTeam->LeaderY;
}