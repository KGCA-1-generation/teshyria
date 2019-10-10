// Enemy.cpp: implementation of the CEnemy class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <math.h>
#include <windows.h>
#include "CharMng.h"
#include "Animation.h"
#include "function.h"
#include "PathFinder.h"
#include "Enemy.h"

#define POW(a) ( (a)*(a) )

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CEnemy::CEnemy()
{
	m_nFrmCntNow  = 0;
	m_nNowMotion  = 0; 
	m_nNowFrame   = 0;
	m_nDirection  = 0;
	m_nCharStatus = 0;
	m_nGapX = m_nGapY = 0;
	m_pPath = 0;
	m_bDead = m_bLeader = false;
	m_rtRect.x1= m_rtRect.x2= m_rtRect.y1= m_rtRect.y2= 0;
}

CEnemy::~CEnemy()
{
}

//----------------------------------------------------------------------
// 애니메이션 그림 그려 주는 함수
//----------------------------------------------------------------------
void CEnemy::DrawChar(unsigned short* pScreen, BOOL bOutline)
{
}

//----------------------------------------------------------------------
// 캐릭의 상태 설정 함수
//----------------------------------------------------------------------
void CEnemy::SetStatus(point* Teshy, TeamInfo* TInfo)
{
}

//----------------------------------------------------------
// 상태를 파악해서 캐릭의 애니를 정해줌
// return true  : 방향 고려
//        false : 방향 고려X (1방향 애니)
//----------------------------------------------------------
int CEnemy::PreDefineAni()
{
	return 0;
}

void CEnemy::Think()
{
}

//----------------------------------------------------------------------
// 내가 맞았나를 체크
//----------------------------------------------------------------------
void CEnemy::AmIBeaten(int *dir, int What)
{
}
//----------------------------------------------------------------------
// 캐릭의 이동 좌표 설정 함수
// dx, dy는 이동하고자 하는 목표 점.
//----------------------------------------------------------------------
void CEnemy::MoveTo() 
{	
	m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	bool bFind = false;
	bool bMove = false;

	if( m_pPath != NULL )
	{
		double VeX, VeY, dist, UnitVeX, UnitVeY;

	
		if( m_nTarX/32 != m_X/32 || m_nTarY/32 != m_Y/32)
		{
			VeX = (m_nMoveX*32+m_nTarX%32) - m_X;
			VeY = (m_nMoveY*32+m_nTarY%32) - m_Y;

			dist = sqrt(POW(VeX)+POW(VeY));

			UnitVeX = VeX / dist;
			UnitVeY = VeY / dist;

			UnitVeX *= m_nSpeed;
			UnitVeY *= m_nSpeed;

		//	if( !( m_pCharMng->GetMapAttrP((m_X+(int)UnitVeX), (m_Y+(int)UnitVeY)) & 0x00000010 ) )
		//		bMove = true;
		//	else { bMove = false; bFind = true; }

		//	if( bMove )
		//	{
				m_X += (int)UnitVeX;
				m_Y += (int)UnitVeY;
		//	}
			
		//	if( (m_X)/32 == m_nMoveX && (m_Y)/32 == m_nMoveY ) bFind = true;
			bFind = true;
			if( bFind )
			{
				if ( m_pPath->NewPath(m_X/32, m_Y/32, m_nTarX/32, m_nTarY/32) )  
				{
					m_pPath->PathNextNode();      
					m_nMoveX = m_pPath->NodeGetX();  
					m_nMoveY = m_pPath->NodeGetY(); 
					
					
				}
				else{
					m_pCharMng->MarkPos(m_X, m_Y, 0, true);
					return;
				}
				bFind = false; 
				
			}
		}
		else if( m_nTarX/32 == m_X/32 && m_nTarY/32 == m_Y/32 )
		{
			m_X = m_nTarX;
			m_Y = m_nTarY;
			m_pCharMng->MarkPos(m_X, m_Y, 0, true);
			return ;
		}

	}
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);
}

//----------------------------------------------------------------------
// 이동 가능한 지 체크(타일을 체크)
// return true; 가능  
//----------------------------------------------------------------------
BOOL CEnemy::IsMovable(int dir)
{
	int check[3], ax=0, ay=0;

	check[0] = abs(dir-1);
	check[1] = dir;
	check[2] = (dir+1)%8;

	for(int i=0; i<3; i++)
	{
		switch(check[i]) {
		case 0 : ax=1; ay=0;	break;
		case 1 : ax=1; ay=-1; 	break;
		case 2 : ax=0; ay=-1;	break;
		case 3 : ax=-1; ay=-1;	break;
		case 4 : ax=-1; ay=0;	break;
		case 5 : ax=-1; ay=1;	break;
		case 6 : ax=0; ay=1;	break;
		case 7 : ax=1; ay=1;
		}
		if(m_pCharMng->GetMapAttrP(m_X+(ax*32), m_Y+(ay*32)) & 0x00000030 )
		{
			return false;
		}
	}
	return true;
}

//----------------------------------------------------------------------
// 실제 해당 모션 ID값과 방향, 상황 등을 판단 해서 
// 그려줄 애니의 모션 값을 리턴한다.
//----------------------------------------------------------------------
int CEnemy::DecideAni()
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

//----------------------------------------------------------------------
// 캐릭을 현 화면에 그릴 것인지 정하는 함수. 주캐릭과 거리차이.
// return true  : --> 그려라.
//        false : --> 그리지 마라.
//----------------------------------------------------------------------
BOOL CEnemy::IsDraw() 
{	
	int a = abs(m_nTeX-m_X);
	int b = abs(m_nTeY-m_Y);

	if( a>800 || b>800 ) return false;

	return true; 
}

//----------------------------------------------------------------------
// 현 모션의 마지막 프레임을 만났는지.
// return true : 마지막 프레임시
//----------------------------------------------------------------------
BOOL CEnemy::IsEndFrm(int AniStatus)
{
	if((m_nCharStatus==AniStatus) && 
	   (m_nNowFrame==m_pAni->GetMotFrmCount(m_nNowMotion)) ) return true;
	return false;
}

//----------------------------------------------------------------------
// 현 모션의 중간 프레임을 만났는지.
// return true : 마지막 프레임시
//----------------------------------------------------------------------
BOOL CEnemy::IsMidFrm(int AniStatus)
{
	if((m_nCharStatus==AniStatus) && 
	   (m_nNowFrame==(m_pAni->GetMotFrmCount(m_nNowMotion)>>1))) return true;
	return false;
}

//----------------------------------------------------------------------
// 인자의 x, y가 현재 모션프레임 사각형 안에 들어 있는지
// return true : 들어 있을 때, false : 아닐 때
//----------------------------------------------------------------------
bool CEnemy::IsFrmRect(int x, int y)
{
	SetFrmRect();

	if(x>m_rtRect.x1 && x<m_rtRect.x2 && y>m_rtRect.y1 && y<m_rtRect.y2) return true;
	return false;
}

//----------------------------------------------------------------------
// 인자의 rect가 현재 모션프레임 사각형 안에 들어 있는지
// return true : 들어 있을 때, false : 아닐 때
//----------------------------------------------------------------------
bool CEnemy::IsFrmRect(rect* rt)
{
	SetFrmRect();

	if(m_bDead) return false;
	if((m_rtRect.x1 < rt->x2) && (m_rtRect.x2 > rt->x1) &&
	   (m_rtRect.y1 < rt->y2) && (m_rtRect.y2 > rt->y1) ) return true;
	return false;
}

//----------------------------------------------------------------------
// 현 프레임의 rect값을 Setting
//----------------------------------------------------------------------
void CEnemy::SetFrmRect()
{
	m_rtRect.x1 = m_X - m_pAni->m_pCX[m_pAni->GetFrmData(m_nNowMotion, m_nNowFrame)];
	m_rtRect.x2 = m_rtRect.x1 + m_pAni->m_pFrmWidth[m_pAni->GetFrmData(m_nNowMotion, m_nNowFrame)];
	m_rtRect.y1 = m_Y - m_pAni->m_pCY[m_pAni->GetFrmData(m_nNowMotion, m_nNowFrame)];
	m_rtRect.y2 = m_rtRect.y1 + m_pAni->m_pFrmHeight[m_pAni->GetFrmData(m_nNowMotion, m_nNowFrame)];
}

void CEnemy::SetAniData(int lock, int cnt, char* str)
{
	m_bAniLock     = lock;
	m_nFrmCntLimit = cnt;
	strcpy(m_sMotID, str);
}

//-----------------------------------------------------------
//  팀에서의 위치만 넘겨주면 리더와의 x, y 간격을 세팅
//-----------------------------------------------------------
void CEnemy::SetGap(int num)
{
	switch(num)
	{
	case 0 : m_nGapX =   0; m_nGapY =   0; break;
	case 1 : m_nGapX = -80; m_nGapY =   0; break;
	case 2 : m_nGapX =  80; m_nGapY =   0; break;
	case 3 : m_nGapX =   0; m_nGapY = -80; break;
	case 4 : m_nGapX =   0; m_nGapY =  80; break;
	case 5 : m_nGapX = -80; m_nGapY = -80; break;
	case 6 : m_nGapX =  80; m_nGapY = -80; break;
	case 7 : m_nGapX = -80; m_nGapY =  80; break;
	case 8 : m_nGapX =  80; m_nGapY =  80; break;
	}
}