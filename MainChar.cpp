// MainChar.cpp: implementation of the CMainChar class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>
#include <dsound.h>
#include <math.h>
#include <windows.h>
#include <mmsystem.h>
#include "Dx7Sound.h"
#include "Animation.h"
#include "function.h"
#include "CharMng.h"
#include "MainChar.h"
#include "NeedStruct.h"

#define WORD    unsigned short
#define NULL    0
#define BOOL    int
//---------------------------------------------------------
// 테시리아(주캐릭)의 모션 Define
//---------------------------------------------------------
#define Stand	0
#define Run		1
#define Run_M	2
#define NA		3
#define NA_M	4
#define Concen	5
#define Absorb	6
#define Sitdown	7
#define Defense	8
#define Beaten	9
#define Eogum	10
#define Bido	11
#define Suphon	12
#define Valk	13
#define Dragon	14
#define Sword	15
#define Die		16
#define Success	17
//---------------------------------------------------------
// 테시리아(주캐릭)의 내부수치 변화 요소 Define
//---------------------------------------------------------
#define AT    100    // 공격력
#define DF    100    // 방어력
#define	MAX_HP	1000	
#define	MAX_MP	1000
#define	MAX_SP	1000

#define SD    100    // 스피드
#define Madd   50    // 마나 가중치
#define MEa    20    // 마나 소모량, 초당(공격시)
#define MEd    20    //         "        (방어시)
#define MEs    10    //         "        (이동시)
#define MAp    80    // 마나 흡수 퍼센트
#define MAs     6    // 마나 흡수시 걸리는 시간
#define SEc   20    // 집중시 스태미너 소모량
#define SEs    10    // 이동시 스태미너 소모량
#define SAq    40    // 스태미너 흡수량 (초당)
#define CL   1400    // 집중 영향 거리(per pixel)
#define CT      4    // 집중 소모 시간
#define VP     20    // 회피 확률
#define WL     30    // 착용 제한 무게
#define WLMAX  80    // 착용 제한 최대 무게

//-------------------------------------------------------------------
// Construction/Destruction
//-------------------------------------------------------------------
CMainChar::CMainChar(CAnimation *p, CCharMng* cp)
{
	m_pAni = p;
	m_pCharMng = cp;
	m_bSitdown = m_bAniStop = false;
	m_nAttack = -1;
	m_nNowMotion = m_nNowFrame = m_nFrmCntNow = 0;
	m_nX = m_nY = 0;
	m_nCharStatus = 0;
	SetAniData(false, 8, "Stand_0");
	m_bConcent = false;


	//테시리아 수치들 
	m_HP = MAX_HP;
	m_MP = MAX_MP;
	m_SP = MAX_SP;
	
	m_IncMP =0;
	m_IncSP =0;

	m_DF = DF;
	m_nDFPenalty = 1;

	memset(m_aManaWaste, 0, 17*4);

	// 아이템 클래스에서 세팅해주어야 하는 함수 
	SetDefense(100);
	SetManaWaste(30);
	SetSPWaste(3);
}

CMainChar::~CMainChar()
{
}

//----------------------------------------------------------
// 방향과 캐릭의 상태의 변화시 값을 지정
//----------------------------------------------------------
void CMainChar::SetStatus(int* dir, int status)
{
	m_nDirection = (*dir);
	int OldStatus = m_nCharStatus;

	// 수치코딩 1: 입력된 행동을 각 수치를 고려해 판단 --------------------------------
	status = DecideStatus(status, OldStatus);					
	//----------------------------------------------------------------------------------

	if(m_nCharStatus == Die) {
		m_nNowMotion = DecideAni();
		return;
	}
	
	if(m_bAniLock==false) // 애니락이 false
	{
		if(status!=Concen && status!=Absorb) {
			//m_nDirection = (*dir);
			//if((*status)==Sitdown) { m_bSitdown = true; }
			if(m_bSitdown) {
				m_nInputStatus = Sitdown;
			
			}
			else {
				m_bSPAbsorb = false;
				m_nInputStatus = status;
			}

			InputProcess();
		}
	}
	else // 애니락이 true
	{
		if(m_nCharStatus==Sitdown) { // 앉기 상태일때

			//수치코딩 2: 스테미너 흡수시 수치 계산 -------------------------------------------------------
			if( !m_bSPAbsorb )	{ m_bSPAbsorb = true; m_dwTime = timeGetTime(); }
			AcountSPAbsorb();
			//----------------------------------------------------------------------------------
			//m_nDirection = (*dir);
			//m_nInputStatus = (*status);
			switch( status )
			{
			case Sitdown :	m_nInputStatus = Stand;		m_bSitdown = false;	break;
			case Concen :	m_nInputStatus = Concen;	break;
			case Absorb :	m_nInputStatus = Absorb;	break;
			case Run :		m_nInputStatus = Run;		m_bSitdown = false;	break;
			case NA :		m_nInputStatus = NA;		m_bSitdown = false;	break;
			}
			InputProcess();
		}
	}
	if(OldStatus != m_nCharStatus ) m_nNowFrame = 0;
	
	//수치코딩 3: 마나 흡수시 수치 계산 ------------------------------------------------------
	if( m_nCharStatus == Absorb )
	{
		if( !m_bMPAbsorb ) { m_bMPAbsorb = true; m_dwTime = timeGetTime(); m_IncMP = m_MP; }
		AcountMPAbsorb();
	}
	else {
		m_bMPAbsorb = false;
		m_IncMP = m_MP;
	}
	//-----------------------------------------------------------------------------------------
	//수치코딩 4: 집중시 수치 계산 ------------------------------------------------------------
	if( m_nCharStatus == Concen )
	{
		m_nDFPenalty = 3;
		AcountSPWaste();
	}
	else m_nDFPenalty = 1;
	//-----------------------------------------------------------------------------------------
	
}

//----------------------------------------------------------
// Input키값의 의한 캐릭의 상태결정.
// 키를 복수개 눌렀을 때를 대비.
//----------------------------------------------------------
void CMainChar::InputProcess()
{
	m_nCharStatus = m_nInputStatus;
	
	m_nNowMotion = DecideAni();
}

//------------------------------------------------------------
// 캐릭터 자체 애니메이션 그림 그려 주는 함수
// DecideAni()함수에서 애니의 변동사항을 체크 받고 모션을 결정
//------------------------------------------------------------
void CMainChar::DrawChar(WORD* pScreen)
{
	m_pAni->DrawAni(400+m_nX, 300+m_nY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bAniStop) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		m_nNowFrame++;	m_nFrmCntNow = 0;
	}
	
	//수치코딩 5: 각 행동에 따른 마나감소량 계산 ----------------------------------------
	if( IsEndFrm( m_nCharStatus ) )	{		// 마나 소모량 계산 
		AcountManaWaste(m_nCharStatus);
	}
	//-------------------------------------------------------------------------------------

	//수치코딩 6: 달리기시 스테미너 감소량 계산 ----------------------------------------
	if(IsEndFrm(Run))
	{
		AcountSPWaste();
	}
	//-------------------------------------------------------------------------------------
	if(IsEndFrm(Eogum))
	{
		m_pCharMng->CreateObj(3, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
		m_bAniStop = true;
		m_nNowFrame--;
		//m_CSound.PlaySound("0001.WAV",0,0);
		return;
	}
	if(IsEndFrm(Bido)) 
	{
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
		BidoEtc();
	}
	if(IsEndFrm(Suphon))
	{
		m_pCharMng->CreateObj(5, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
	}
	if(IsEndFrm(Dragon))
	{
		m_pCharMng->CreateObj(6, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
	}
	if(IsEndFrm(Sword))
	{
		m_pCharMng->CreateObj(7, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
		m_bAniStop = true;
		m_nNowFrame--;
		return;
	}
	if(IsEndFrm(Valk))
	{
		m_pCharMng->CreateObj(8, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY, 
		                         m_pCharMng->m_nCurX, m_pCharMng->m_nCurY);
	}
	if(IsEndFrm(Die))
	{
		m_bAniStop = true;
		m_nNowFrame--;
		return;
	}

	if(m_bAniLock && IsEndFrm(m_nCharStatus))  // 애니가 잠겼다면
	{
		m_nAttack = -1;
		if(m_nCharStatus==Sitdown) {
			m_bSitdown = true;
			m_nNowFrame=m_pAni->GetMotFrmCount(m_nNowMotion)-1;
		}
		else m_nCharStatus = Stand;
		m_nNowMotion	= DecideAni();
	}
	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return true  : 방향 고려
//        false : 방향 고려X (1방향 그림)
//----------------------------------------------------------
BOOL CMainChar::PreDefineAni(void) 
{
	switch(m_nCharStatus)
	{
	case Stand :  // 서 있기
		SetAniData(false, 8, "Stand_0");	return true;		
	case Run :  // 달리기
		SetAniData(false, 3, "Run_0");		return true;
	case Run_M :  // 마나 달리기
		SetAniData(false, 1, "Run&M_0");	return true;
	case NA :  // 일반 공격
		m_nAttack  = 1;
		SetAniData(true, 1, "NA_0");		return true;
	case NA_M :  // 마나사용 공격
		m_nAttack  = 2;
		SetAniData(true, 1, "NA&M_0");		return true;
	case Concen :  // 집중하기
		SetAniData(false, 1, "Concen");		return false;
	case Absorb :  // 마나 흡수
		SetAniData(false, 1, "Absorb");		return false;
	case Sitdown :  // 앉기
		SetAniData(true, 3, "Sitdown_0");	return true;
	case Defense :  // 방어
		SetAniData(true, 1, "Defense_0");	return true;
	case Beaten :  // 맞을 때
		SetAniData(true, 2, "Beaten_0");	return true;
	case Eogum : // 어검술
		m_nAttack  = 3;
		SetAniData(true, 1, "Eogum_0");		return true;
	case Bido : // 비도술
		m_nAttack  = 4;
		SetAniData(true, 1, "Bido_0");		return true;
	case Suphon : // 섭혼술
		SetAniData(true, 2, "Suphon_0");	return true;
	case Valk : // 발키리
		SetAniData(true, 3, "Valk");		return false;
	case Dragon : // 드래곤피어
		m_nAttack  = true;
		SetAniData(true, 2, "Dragon_0");	return true;
	case Sword : // 소드브레이커
		m_nAttack  = true;
		SetAniData(true, 1, "Sword_0");		return true;
	case Die : // 죽기
		SetAniData(true, 6, "Die");			return false;
	case Success : // 미션성공
		SetAniData(true, 5, "Win");			return false;
	}
	return false;
}

//----------------------------------------------------------
// status값을 파일안의 모션 ID와 비교하여 모션Offset로 리턴
//----------------------------------------------------------
int CMainChar::DecideAni()
{
	BOOL bDir;
	bDir = PreDefineAni();
	for(int i=0; i<m_pAni->GetMotCount(); i++) // 모션 수만큼 루프
	{
		// ID값이 같은 것이 있는지 비교
		if (!strcmp(m_sMotID, m_pAni->m_sMotID[i]))
		{
			if(bDir) return (i+m_nDirection);
			return i;
		}
	}
	return m_nNowMotion;
}

void CMainChar::SetAniData(int lock, int cnt, char* str)
{
	m_bAniLock     = lock;
	m_nFrmCntLimit = cnt;
	strcpy(m_sMotID, str);
}

void CMainChar::AmIBeaten(int nPower)
{
	// 지금은 방어할 확률 20%
	if(m_nCharStatus != Beaten && m_nCharStatus != Defense  && m_nCharStatus!=Die ) {		// 처음 맞았을때 
		if(rand()%5) {
			m_nCharStatus = Beaten;

			//수치코딩 6: 타격을 받았을때 체력 감소량 계산 ----------------------------------------
			AcountHPWaste(nPower);
			//-------------------------------------------------------------------------------------

		}
		else m_nCharStatus = Defense;
		m_nNowFrame = 0;
		m_nNowMotion = DecideAni();
	}	
	//수치코딩 7: 타격 받는 도중 다시 타격을 받았을때 체력 감소량 계산 ------------------------------
//	if(m_nCharStatus == Beaten)	{									// 맞는 동작 도중에 또 맞았을때 					
//		if( rand()%5 && m_nCharStatus!=Die ) AcountHPWaste(nPower);
//	}
	//----------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------
// 인자 모션의 마지막 프레임을 만났는지.
// return true : 마지막 프레임시
//----------------------------------------------------------------------
BOOL CMainChar::IsEndFrm(int AniStatus)
{
	if((m_nCharStatus==AniStatus) && 
	   (m_nNowFrame==m_pAni->GetMotFrmCount(m_nNowMotion)) ) return true;
	return false;
}

void CMainChar::BidoEtc()
{
	int gx = m_pCharMng->m_nCurX - m_pCharMng->m_nTeX;
	int gy = m_pCharMng->m_nTeY - 50 - m_pCharMng->m_nCurY;
	double dist = sqrt(pow(gx,2)+pow(gy,2));
	double xy = ((double)gy) / gx;
	int theta1 = (int)(atan(xy) * 57.2957995) - 15; // 57.2957995:180/pi 
	int theta2 = theta1 + 30;
	// 0.01745329 : pi/180
	
	if(gx>0)
	{
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX+(int)(cos(theta1*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50-(int)(sin(theta1*0.01745329)*dist));
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX+(int)(cos(theta2*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50-(int)(sin(theta2*0.01745329)*dist));
	}
	else if(gx==0)
	{
		if(gy>0)
		{
			theta1 = 75;
			theta2 = 105;
		}
		else
		{
			theta1 = 255;
			theta2 = 285;
		}
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX+(int)(cos(theta1*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50-(int)(sin(theta1*0.01745329)*dist));
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX+(int)(cos(theta2*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50-(int)(sin(theta2*0.01745329)*dist));
	}
	else
	{
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX-(int)(cos(theta1*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50+(int)(sin(theta1*0.01745329)*dist));
		m_pCharMng->CreateObj(4, m_pCharMng->m_nTeX, m_pCharMng->m_nTeY-50, 
								 m_pCharMng->m_nTeX-(int)(cos(theta2*0.01745329)*dist), 
								 m_pCharMng->m_nTeY-50+(int)(sin(theta2*0.01745329)*dist));
	}
}

//-------------------------------------------------------------------------------
// 현재 수행하고자 하는 공격 가능여부를 판단하는 함수 ( SetStatus수시로 호출 )
// 인자  : 현재 키입력된 상태 
// 리턴값 :	판단후 가능한 동작 	
// : 마나공격 - 사용할 마나가 충분한지 판단
// : 비도술 - 사용할 단도수량이 충분한지 판단 
//--------------------------------------------------------------------------------
int CMainChar::DecideStatus(int nStatus, int nOld)
{
	if( nStatus == Bido ) {					// 단도갯수 점검 


	}
	if( m_MP < m_aManaWaste[nStatus] ){		// 마나가 충분하지 않으면 
		switch(nStatus)
		{
		case NA_M :		return NA;			// 마나사용 공격
		case Run_M:		return Run;			// 마나달리기 

		default:		return nOld;
		}
	}
	else return nStatus;					// 마나가 충분하면 
}
// ---------------------------------------------------------
// 타격을 받았을때 체력 계산 ( AmIBeaten() 일때 호출 )
// : 체력 = 체력 - 타격치 - 방어력 
//----------------------------------------------------------
void CMainChar::AcountHPWaste(int nPower)
{
	if( m_HP > 0 )
		m_HP -= (nPower-m_DF/m_nDFPenalty);
	else {					// 테시리아 Die, GameOver
		m_nCharStatus = Die;
	}
}

// ---------------------------------------------------------------
// 마나 사용 공격시 마나 소모량 계산 ( AmIBeaten() 일때 호출 )
// : 마나량 = 마나량 - 각 공격의 마나 소모량  
//----------------------------------------------------------------
void CMainChar::AcountManaWaste(int nStatus)
{
	m_MP -= m_aManaWaste[nStatus];
	if( m_MP < 0 ) m_MP = 0;
}
// ---------------------------------------------------------------
// 스테미너 소모량 계산 
// 달리기시 스테미너 소모량 = 스테미너량 - 한동작   
//----------------------------------------------------------------
void CMainChar::AcountSPWaste()
{
	if( m_nCharStatus == Run ){
		m_SP -= m_nSPWasteR;
		if( m_SP < 0 ) m_SP = 0;
	}
	else if( m_nCharStatus == Concen ){
		if( timeGetTime() - m_dwTime > 1000 ){
			m_SP -= m_nSPWasteC;
			if( m_SP < 0 )	m_SP = 0;
			m_dwTime = timeGetTime();
		}
	}
}

// ---------------------------------------------------------------
// 스테미너 흡수시 스테미너량 계산
// : 스테미너량 = 스터미너 + 기본 스테미너 증가량 (초당)
//----------------------------------------------------------------
void CMainChar::AcountSPAbsorb()
{
	if( timeGetTime() - m_dwTime > 1000 ){
		m_SP += SAq;
		if( m_SP > MAX_SP )	m_SP = MAX_SP;
		m_dwTime = timeGetTime();
	}
}

// ---------------------------------------------------------------
// 마나 흡수시 마나량 계산
// : 마나량 = 마나량 +  마나소모량의 80%( 마나흡수 성공시 )
//----------------------------------------------------------------
void CMainChar::AcountMPAbsorb()
{
	if( timeGetTime() - m_dwTime > 1000 ){
		m_nAbsTime++;
		m_IncMP += ((MAX_MP-m_MP)*80/100)/MAs;
		if( m_nAbsTime > MAs ) 
		{
			m_MP += (MAX_MP-m_MP)*80/100;
			m_nAbsTime = 0;
			m_IncMP = m_MP;
		}
		m_dwTime = timeGetTime();
	}
}

//-------------------------------------------------------------------
// 현재 테시리아의 방어력 계산 ( 방어구를 바꿀때마다 호출 )
// : 방어력 = 기본 방어력 + 방어구의 방어력 
//-------------------------------------------------------------------
void CMainChar::SetDefense(int nDF)	
{
	m_DF = DF+nDF;
}

// -------------------------------------------------------------------
// 각 공격의 마나 소모량 세팅 ( 무기를 바꿀때마다 호출 )
// 인자 : 무기의 마나 소모량 
// : 각 공격의 마나소모량 + 무기의 마나소모량 
//---------------------------------------------------------------------

void CMainChar::SetManaWaste(int nWpMana)
{
	m_aManaWaste[Run_M]		= 10;						// 마나달리기 
	m_aManaWaste[NA_M]		= 20	+ nWpMana;			// 마나공격
	m_aManaWaste[Eogum]		= 100	+ nWpMana;			// 어검술 
	m_aManaWaste[Bido]		= 60	+ nWpMana;			// 비도술
	m_aManaWaste[Suphon]	= 40	+ nWpMana;			// 섭혼술
	m_aManaWaste[Valk]		= 90	+ nWpMana;			// 발키리 
	m_aManaWaste[Dragon]	= 40	+ nWpMana;			// 드래곤피어
	m_aManaWaste[Sword]		= 80	+ nWpMana;			// 소드브레이커
}

// -------------------------------------------------------------------
// 스테미너 소모량 세팅 ( 무기를 바꿀때마다 호출 )
// 인자 : 무기의 패널티 
// : 각 행동의 기본 스테미너 소모량 + 무기 패널티 * 가중치  
//---------------------------------------------------------------------
void CMainChar::SetSPWaste(int nPenalty)
{
	m_nSPWasteR = SEs+nPenalty*3;		
	m_nSPWasteC = SEc+nPenalty*10;	
}


