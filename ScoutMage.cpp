// ScoutMage.cpp: implementation of the CScoutMage class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
//#include <mmsystem.h>
#include <windows.h>
#include "Animation.h"
#include "CharMng.h"
#include "ScoutMage.h"

#define Stand    0
#define Run      1
#define Attack   2
#define Beaten   3
#define Comm     4
#define Die      5
//--------------------------------------------------------------------
// ������ ������
//--------------------------------------------------------------------
#define AT    220    // ���ݷ�
#define DF     10    // ����
#define VP     20    // ȸ�� Ȯ��
#define SiR   800    // �þ߹ݰ�
#define AC    200    // ���ݹݰ�
#define AL    260    // ���� ���� �Ÿ�
#define CT     10    // ��� �ҿ� �ð�
#define CD      2    // ĳ���� ������
#define SUCCTIME		20		// ��� ���� �ð� 
#define OVERTIME		6		// ��� ������ �̼� ���� �ð� 


#define	FIND		0x00000001		// �׽ø��� �߰�
#define SUCC		0x00000002		// ��Ÿ��� ����
#define NOGUARD		0x00000004		// ��� ���� ����
#define BEATEN		0x00000008		// �¾Ҵ��� ���� 
#define SIGHT_R		0x00000010		// �þ߹����ȿ� �ִ��� ���� 
#define ATTACK_R	0x00000020		// ���ݹ����ȿ� �ִ��� ���� 
#define FIND_OX		0x00010000		// �߰߿��� �÷��� 
#define ATTACK_OX	0x00020000		// ���ݿ��� �÷��� 
#define ATTACK_OX2	0x00040000		// ���ݿ��� �÷��� 2

//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CScoutMage::CScoutMage(CAnimation* p, CCharMng* cp, int x, int y, int team)
{
	m_pAni         = p;
	m_pCharMng     = cp;
	m_pTeam        = NULL; 
	m_HP           = 130;
	m_nSpeed       = 15;
	m_X            = x;
	m_Y            = y;
	m_nMoveX		= m_X/32;
	m_nMoveY		= m_Y/32;
	m_nTeam         = team;
	m_nCommTime		= 0;
	m_bPattern		= false;
	m_bBeaten		= false;
	m_bComm			= false;		// ��Ÿ��� ���� ���� 
	m_nPattern		= 0;

	SetAniData(false, 12, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);
}

CScoutMage::~CScoutMage()
{
}

//----------------------------------------------------------
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return TRUE  : ���� ���
//        FALSE : ���� ���X (1 ���� �׸�)
//----------------------------------------------------------
int CScoutMage::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 12, "Stand_0");		return true;
	case Run :    // �޸���
		SetAniData(false, 5, "Run_0");			return true;
	case Attack : // �����ַο� ����
		SetAniData(true, 4, "Attack_0");		return true;
	case Beaten : // ���� �ޱ�.
		SetAniData(true, 3, "Beaten_0");		return true;
	case Comm :   // ��Ÿ��� ����
		SetAniData(true, 3, "Comm");			return false;
	case Die :    // �ױ�
		SetAniData(true, 4, "Die");				return false;
	}
	return false;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
//----------------------------------------------------------------------
void CScoutMage::DrawChar(unsigned short* pScreen, BOOL bOutLine)
{
	if(bOutLine) {
		m_pAni->DrawAniOL(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	}
	else 
		m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bDead) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		if(!m_bDead) m_nNowFrame++;
		if(m_nCharStatus==Run) MoveTo(); // �̵��ô� pixel�� ���ϹǷ�
		m_nFrmCntNow = 0;
	}
	if(IsEndFrm(Beaten)) {
		m_HP -=100;
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}
	if(IsEndFrm(Die)) {
		m_bDead = TRUE;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
//	if(IsEndFrm(m_nCharStatus)) { // �ִϰ� ���ٸ�
//		m_nNowMotion = DecideAni();
//	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CScoutMage::SetStatus(point* Teshy, TeamInfo* TInfo)
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
		m_pTeam->bLeader = false;
		m_nCharStatus = Die;
	}

	m_nScrX = m_X-m_nTeX + 400;
	m_nScrY = m_Y-m_nTeY + 300;

	if(!m_bDead) {
		m_pTeam->LeaderX  = m_X; 
		m_pTeam->LeaderY  = m_Y;
		m_pTeam->Distance = m_nDistance;
	}	
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CScoutMage::Think()
{	
	if( m_nDistance < SiR ) m_nPattern |= SIGHT_R; 
	else m_nPattern &= (~SIGHT_R);
	if( m_nDistance < AC  )	m_nPattern |= ATTACK_R; 
	else m_nPattern &= (~ATTACK_R);
	if( m_pTeam->nDeadCnt == 2 ) m_nPattern |= NOGUARD;

	if( m_nPattern == 0 ) { m_nCharStatus = Stand; return; }
	if( m_nPattern & SUCC ){
		if( m_nPattern & SIGHT_R )
		{
			if( m_nPattern & ATTACK_R )	m_nCharStatus = Attack;
			else { 
				m_nCharStatus = Run;
				m_nTarX = m_nTeX; m_nTarY = m_nTeY; 
			}
		}
		else m_nCharStatus = Stand;
	}
	else {
		if( m_nPattern & NOGUARD )
		{
			if( m_nCommTime > SUCCTIME-4 ) m_nCharStatus = Comm;
			else {
				m_nCharStatus = Run;
				m_nTarX = 2000; m_nTarY = 2000;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			}
			return;
		}
		if( m_nPattern & SIGHT_R )
		{
			if( !(m_nPattern & FIND) ) {
				m_nPattern |= FIND;
				m_dwCommTime = timeGetTime();
			}
			if( m_nPattern & ATTACK_R )	
			{
				if( m_nPattern & BEATEN ) {
					if( m_nPattern & ATTACK_OX2 ) m_nCharStatus = Attack;
					else {
						if( rand()%10 < 4 ) m_nCharStatus = Attack;
						else m_nCharStatus = Comm;
					}
					m_nPattern &= (~BEATEN);
				}
				else {
					if( m_nPattern & ATTACK_OX ){
						
					}
					else if( !(m_nPattern & ATTACK_OX) ) {
						m_nPattern |= ATTACK_OX;
						if( rand()%10 < 1 ) m_nCharStatus = Attack;
						else m_nCharStatus = Comm;
					}
				}				
			}

			else { 
				if( m_nPattern & ATTACK_OX ) m_nPattern &= (~ATTACK_OX);
				m_nCharStatus = Comm;
			}
		}
		else {
			if( m_nPattern & FIND ) m_nCharStatus = Comm;
			else m_nCharStatus = Stand;

		}
	}

	if( m_nCharStatus == Comm ) 
	{
		if( m_nCommTime > SUCCTIME ) {
			m_nPattern |= SUCC;
			return;
		}
		if( timeGetTime() - m_dwCommTime > 1000 ) {
			m_nCommTime++;
			m_dwCommTime = timeGetTime();
		}
	}	
	else { m_nCommTime = 0;	m_dwCommTime = 0; }

	if( m_nPattern & SUCC )
	{
		if( m_nCommTime > OVERTIME ) {
			return;
		}
		if( timeGetTime() - m_dwCommTime > 1000 ) {
			m_nCommTime++;
			m_dwCommTime = timeGetTime();
		}
	}	
}

//----------------------------------------------------------------------
// ���� �¾ҳ��� üũ
//----------------------------------------------------------------------
void CScoutMage::AmIBeaten(int *dir, int What)
{
	if(m_bDead) return;
	if(m_HP>0) { // �¾Ҵٸ�
		if(m_nDistance<90 && (What==1 || What==2)) 
		{
			if( m_nCharStatus == Attack ) m_nPattern |= ATTACK_OX2;
			else m_nPattern &= (~ATTACK_OX2);
			m_nCharStatus = Beaten;
			m_nPattern |= BEATEN;
			m_bBeaten = true;
			m_nNowFrame = 0;
		}
		else 
		{
			if(What!=-1 && m_nCharStatus!=Beaten) {
				if( m_nCharStatus == Attack ) m_nPattern |= ATTACK_OX2;
				else m_nPattern &= (~ATTACK_OX2);
				m_nCharStatus = Beaten;
				m_nPattern |= BEATEN;
				m_bBeaten = true;
				m_nNowFrame = 0;
			}
		}
	}
	m_nNowMotion = DecideAni();
}