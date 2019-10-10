// MstKnight.cpp: implementation of the CMstKnight class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <windows.h>
#include "CharMng.h"
#include "Animation.h"
#include "function.h"
#include "MstKnight.h"
#define PI		3.1415926535
#define POW(a) ( (a)*(a) )
#define Stand  0
#define Run    1
#define Attack 2
#define Beaten 3
#define Die    4
//--------------------------------------------------------------------
// 순찰대 기사단장
//--------------------------------------------------------------------
#define AT    240    // 공격력
#define DF     20    // 방어력
#define SpD   30    // 스피드
#define VP     25    // 회피 확률
#define SiR   600    // 시야반경
#define AtR   250    // 공격반경
#define AtCk  120    // 공격 충돌 체크
#define RgMg   80    // 마법사와의 간격거리

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CMstKnight::CMstKnight(CAnimation* p, CCharMng* cp, int x, int y, int team)
{
	m_pAni         = p;
	m_pCharMng     = cp;
	m_pTeam        = NULL;  
	m_HP           = 400;
	m_nSpeed       = 15;
	m_X            = x;
	m_Y            = y;
	m_nMoveX		= m_X/32;
	m_nMoveY		= m_Y/32;
	m_nTeam        = team;
	SetAniData(false, 16, "Stand");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);	
	m_nWp			=0;
}

CMstKnight::~CMstKnight()
{

}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return true  : 방향 고려
//        false : 방향 고려X (1방향 애니)
//----------------------------------------------------------
int CMstKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(false, 16, "Stand");		return false;
	case Run :    // 달리기
		SetAniData(false, 3, "Run_0");		return true;
	case Attack : // 공격
		SetAniData(true, 3, "Attack_0");	return true;
	case Beaten : // 공격 받기
		SetAniData(true, 3, "Beaten_0");	return true;
	case Die :    // 죽어서 쓰러지기
		SetAniData(true, 4, "Die");			return false;
	}
	return false;
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//----------------------------------------------------------------------
void CMstKnight::DrawChar(unsigned short* pScreen, int bOutLine)
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
	if(IsMidFrm(Attack)) {
		// 중간 프레임시 한번더 테시리아와의 거리를 계산한다.
		m_nDistance  = GetDistance(abs(m_nTeX-m_X), abs(m_nTeY-m_Y));
		if(m_nDistance<AtCk) { m_pCharMng->BeatenTeshy(AT); } 
	}
	// 맞았을 때 마지막 프레임을 만났을 때
	if(IsEndFrm(Beaten)) {
		m_HP -= 100;		
	}
	// 죽었을 때 마지막 프레임을 만나면
	if(IsEndFrm(Die)) {
		m_bDead = true;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
	if(IsEndFrm(m_nCharStatus)) { // 애니가 잠겼다면
		m_nCharStatus  = Stand;
		m_nNowMotion = DecideAni();
	}
	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// 캐릭의 상태 설정 함수
//----------------------------------------------------------------------
void CMstKnight::SetStatus(point* Teshy, TeamInfo* TInfo)
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
		m_nCharStatus = Die;
	}

	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
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
void CMstKnight::Think()
{
	BOOL bSearch = false;
	if( m_nDistance < SiR)			// 시야 반경안에 들어왔을 경우
	{
		if( m_nDistance < AtR )
		{
			m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
			bSearch = SetAttackPos(m_nDirection);
			if( !bSearch ) {
				m_nTarX = m_nTeX;
				m_nTarY = m_nTeY;
			}

			if(m_nTarX==m_X && m_nTarY==m_Y) {
				m_nCharStatus = Attack;
			}
			else { 
				if( bSearch )	m_nCharStatus = Run;
				else m_nCharStatus = Stand;
			}
		}
		else {
			m_nCharStatus = Run;
			m_nTarX = m_nTeX;
			m_nTarY = m_nTeY;
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
			if( (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000010) 
				|| (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000001) )
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
// 캐릭의 호위 모드 시 목표점 설정 함수
// 목표점은 정찰대 마법사와 테시리아 사이에서 법사로부터 거리 RgMg위치
//----------------------------------------------------------------------
void CMstKnight::GuardTo() 
{	
	m_nTarX = RgMg*(m_nTeX-m_pTeam->LeaderX)/m_pTeam->Distance + m_pTeam->LeaderX;
	m_nTarY = RgMg*(m_nTeY-m_pTeam->LeaderY)/m_pTeam->Distance + m_pTeam->LeaderY;
}

//----------------------------------------------------------------------
// 내가 맞았나를 체크
//----------------------------------------------------------------------
void CMstKnight::AmIBeaten(int *dir, int What)
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

BOOL CMstKnight::SetAttackPos(int dir)
{
	int i, aPos[8][2], dir1;
	for(  i=0; i<8; i++)
	{
		dir1 = (dir+i)%8;
		aPos[i][0] = (int)(-(double)cos(dir1*45*PI/180)*(AtCk-10) + m_nTeX);
		aPos[i][1] = (int)((double)sin(dir1*45*PI/180)*(AtCk-10) + m_nTeY);
	}	
	
	for(  i=0; i<8; i++)
	{
		if( m_X/32 == aPos[i][0]/32 && m_Y/32 == aPos[i][1]/32 ) return true;
		else if( !(m_pCharMng->GetMapAttrP(aPos[i][0], aPos[i][1]) & 0x00000010) 
			&& !(m_pCharMng->GetMapAttrP(aPos[i][0], aPos[i][1]) & 0x00000001) )
		{
			m_nTarX = aPos[i][0];
			m_nTarY = aPos[i][1];
			return true;
		}
	}
	return false;	
}