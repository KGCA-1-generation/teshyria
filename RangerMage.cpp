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
#define RgMg   80    // ��������� ���ݰŸ�
//--------------------------------------------------------------------
// ������ ������
//--------------------------------------------------------------------
#define AT    220    // ���ݷ�
#define DF     10    // ����
#define VP     20    // ȸ�� Ȯ��
#define SiR   650    // �þ߹ݰ�
#define AC    200    // ���ݹݰ�
#define AL    260    // ���� ���� �Ÿ�
#define AtR   250    // ���ݹݰ�
#define CT     10    // ��� �ҿ� �ð�
#define CD      2    // ĳ���� ������

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
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return TRUE  : ���� ���
//        FALSE : ���� ���X (1 ���� �׸�)
//----------------------------------------------------------
int CRangerMage::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(FALSE, 12, "Stand_0");		return TRUE;
	case Run :    // �޸���
		SetAniData(FALSE, 5, "Run_0");			return TRUE;
	case Attack : // �����ַο� ����
		SetAniData(TRUE, 4, "Attack_0");		return TRUE;
	case Beaten : // ���� �ޱ�.
		SetAniData(TRUE, 3, "Beaten_0");		return TRUE;
	case Die :    // �ױ�
		SetAniData(TRUE, 4, "Die");				return FALSE;
	}
	return FALSE;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
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
		if(m_nCharStatus==Run) MoveTo(); // �̵��ô� pixel�� ���ϹǷ�
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
	if(IsEndFrm(m_nCharStatus)) { // �ִϰ� ���ٸ�
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
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
		m_nCharStatus = Die; // �ױ�
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
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CRangerMage::Think()
{	
	if( m_nDistance < SiR)			// �þ� �ݰ�ȿ� ������ ���
	{
		if( m_nDistance < AtR ){		// ���� �����ȿ� ������ ���
			m_nCharStatus = Attack;
		}
		else {						// ���ݿ��� �ۿ� ���� ��� 
			m_nTarX = m_nTeX;
			m_nTarY = m_nTeY;
			m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			m_nCharStatus = Run;		
		}
	}
	else{							// �þ� �ݰ� �ۿ� ������
		if( m_bLeader )	{
			
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) {		// ��ǥ������ �� ������
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
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) 	m_nCharStatus = Stand;	// ��ǥ������ �� ������
			else {
				m_nCharStatus = Run;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			}
			
		}
	}
}

//----------------------------------------------------------------------
// ���� �¾ҳ��� üũ
//----------------------------------------------------------------------
void CRangerMage::AmIBeaten(int *dir, int What)
{
	if(m_bDead) return;
	if(m_HP>0) {  // �¾Ҵٸ�) {
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

