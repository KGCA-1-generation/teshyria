// CharObj.cpp: implementation of the CCharObj class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include "Animation.h"
#include "CharMng.h"
#include "CharObj.h"

#define POW(a) ( (a)*(a) )
#define Eogum  3
#define Bido   4
#define Suphon 5
#define Dragon 6
#define Sword  7
#define Valk   8
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCharObj::CCharObj(CAnimation* p, CCharMng* cp, int type, int x, int y, int tx, int ty)
{
	m_pAni = p;
	m_pCharMng = cp;
	m_sx = m_x = x;
	m_sy = m_y = y;
	m_tx = tx;
	m_ty = ty;
	m_rtObject.x1= m_rtObject.x2= m_rtObject.y1= m_rtObject.y2 = 0;
	m_bEnd = false;
	m_bEogumTurn = false;
	m_nType = type;
	m_nFrame = 0;
	m_nCntLimit = 1;
	
	TypeToMotion();
	if(m_nType!=Valk) CalUnitVector();
}

CCharObj::~CCharObj()
{
}

void CCharObj::Draw(unsigned short* Screen, int TeX, int TeY)
{
	static cnt = 0;
	if(DestroyMe()) return;

	if(m_nType!=Valk)
	{
		m_x += (int)m_dUnitVeX;
		m_y += (int)m_dUnitVeY;

		CDCheck();
		m_pAni->DrawAni(m_x-TeX+400, m_y-TeY+300, m_nMotion, m_nFrame, Screen);
	}
	else {
		m_pAni->DrawAni(400, 300, m_nMotion, m_nFrame, Screen);
	}

	if(cnt>=m_nCntLimit) { m_nFrame++; cnt=0; }
	cnt++;
	m_nFrame %= m_pAni->GetMotFrmCount(m_nMotion);
}

void CCharObj::TypeToMotion()
{
	switch(m_nType)
	{
	case Eogum :  // 어검술
		m_nMotion = 0; m_nSpeed = 15; m_nRange = 300;
		m_gx=13; m_gy=8;
		break;
	case Bido :   // 비도술
		m_nMotion = 1+GetDirection16(); m_nSpeed = 30; m_nRange = 600;
		m_gx=3;  m_gy=3;
		break;
	case Suphon : // 섭혼술
		m_nMotion = 17; m_nSpeed = 10; m_nRange = 500; m_nCntLimit = 5;
		m_gx=12; m_gy=10;
		break;
	case Dragon : // 드래곤피어
		m_nMotion = 19; m_nSpeed = 10; m_nRange = 500; m_nCntLimit = 5;
		m_gx=12; m_gy=10;
		break;
	case Sword :  // 소드 브레이커
		m_nMotion = 20+GetDirection16(); m_nSpeed = 15; m_nRange = 500; m_nCntLimit = 5;
		m_gx=7;  m_gy=7;
		break;
	case Valk :   // 발키리
		m_nMotion = 18; m_nSpeed = 15; m_nCntLimit = 4;
		//m_gx=7;  m_gy=7;
		break;
	}
}

//---------------------------------------------------------------
// 유닛 벡터 계산(속도까지)
//---------------------------------------------------------------
void CCharObj::CalUnitVector()
{
	int x = m_tx-m_x;
	int y = m_ty-m_y;
	double distance = sqrt(POW(x)+POW(y));
	m_dUnitVeX = (x / distance)*m_nSpeed;
	m_dUnitVeY = (y / distance)*m_nSpeed;
}

//---------------------------------------------------------------
// 소멸 처리. (처리 자체는 캐릭매니저에서 한다)
//---------------------------------------------------------------
bool CCharObj::DestroyMe()
{
	int x = m_sx-m_x;
	int y = m_sy-m_y;
	m_nDistance = (int)sqrt(pow(x,2)+pow(y,2));
	if(m_nDistance>m_nRange) 
	{
		if(m_nType==Eogum) {
			EogumTurn();
			return false;
		}
		return (m_bEnd=true);
	}
	else {
		if(m_bEogumTurn && m_nDistance<20) return (m_bEnd=true);
	}
	return false;
}

void CCharObj::EogumTurn()
{
	m_dUnitVeX = -m_dUnitVeX;
	m_dUnitVeY = -m_dUnitVeY;
	m_bEogumTurn = true;
}

//---------------------------------------------------------------
// 16방향 체크
//---------------------------------------------------------------
int CCharObj::GetDirection16()
{
	double pi = 3.1415926535, rad, Angle;
	int nJump = 0;

	double x = (double)(m_tx-m_sx);
	double y = (double)(m_sy-m_ty);
	rad = y / x;

	if( y<0 ) nJump = 8;  // 아랫부분

	Angle = atan(rad) * 57.2958;  // (180 / pi) = 57.2958
	if( Angle > 0.0   && Angle < 11.25 ) return (0+nJump);
	if( Angle > 11.25 && Angle < 33.75 ) return (1+nJump);
	if( Angle > 33.75 && Angle < 56.25 ) return (2+nJump);
	if( Angle > 56.25 && Angle < 78.75 ) return (3+nJump);
	if( Angle > 78.75 && Angle < 90.0  ) return (4+nJump);

	if( Angle >-90.0  && Angle <-78.75 ) return (4+nJump);
	if( Angle >-78.75 && Angle <-56.25 ) return (5+nJump);
	if( Angle >-56.25 && Angle <-33.75 ) return (6+nJump);
	if( Angle >-33.75 && Angle <-11.25 ) return (7+nJump);
	if( Angle >-11.25 && Angle < 0.0   ) return (8+nJump)&15;

	if( x==0 && y>0 ) return 4;
	if( y==0 && x<0 ) return 8;
	if( x==0 && y<0 ) return 12;	
	return 0;
}

//----------------------------------------------------------------------
// 현 프레임의 rect값을 Setting
//----------------------------------------------------------------------
void CCharObj::SetFrmRect()
{
	m_rtObject.x1 = m_x - m_pAni->m_pCX[m_pAni->m_nRealFrame] + m_gx;
	m_rtObject.y1 = m_y - m_pAni->m_pCY[m_pAni->m_nRealFrame] + m_gy;
	m_rtObject.x2 = m_rtObject.x1 + m_pAni->GetWidth(m_pAni->m_nRealFrame) - (m_gx<<1);
	m_rtObject.y2 = m_rtObject.y1 + m_pAni->GetHeight(m_pAni->m_nRealFrame)- (m_gy<<1);
}

void CCharObj::CDCheck()
{
	SetFrmRect();
	int SChar = m_pCharMng->TObjectAndEnemy(&m_rtObject);
	if(SChar!=-1) {
		m_pCharMng->HitEnemy(SChar, 0, m_nType);
		if(m_bEogumTurn==false && m_nType==Eogum && m_nDistance>20) EogumTurn();
		else m_bEnd=true;
	}
}
