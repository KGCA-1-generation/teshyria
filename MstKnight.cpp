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
// ������ ������
//--------------------------------------------------------------------
#define AT    240    // ���ݷ�
#define DF     20    // ����
#define SpD   30    // ���ǵ�
#define VP     25    // ȸ�� Ȯ��
#define SiR   600    // �þ߹ݰ�
#define AtR   250    // ���ݹݰ�
#define AtCk  120    // ���� �浹 üũ
#define RgMg   80    // ��������� ���ݰŸ�

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
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return true  : ���� ���
//        false : ���� ���X (1���� �ִ�)
//----------------------------------------------------------
int CMstKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 16, "Stand");		return false;
	case Run :    // �޸���
		SetAniData(false, 3, "Run_0");		return true;
	case Attack : // ����
		SetAniData(true, 3, "Attack_0");	return true;
	case Beaten : // ���� �ޱ�
		SetAniData(true, 3, "Beaten_0");	return true;
	case Die :    // �׾ ��������
		SetAniData(true, 4, "Die");			return false;
	}
	return false;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
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
		if(m_nCharStatus==Run) MoveTo(); // �̵��ô� pixel�� ���ϹǷ�
		m_nFrmCntNow = 0;
	}
	// ������ �߰� �������� ������ ��
	if(IsMidFrm(Attack)) {
		// �߰� �����ӽ� �ѹ��� �׽ø��ƿ��� �Ÿ��� ����Ѵ�.
		m_nDistance  = GetDistance(abs(m_nTeX-m_X), abs(m_nTeY-m_Y));
		if(m_nDistance<AtCk) { m_pCharMng->BeatenTeshy(AT); } 
	}
	// �¾��� �� ������ �������� ������ ��
	if(IsEndFrm(Beaten)) {
		m_HP -= 100;		
	}
	// �׾��� �� ������ �������� ������
	if(IsEndFrm(Die)) {
		m_bDead = true;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
	if(IsEndFrm(m_nCharStatus)) { // �ִϰ� ���ٸ�
		m_nCharStatus  = Stand;
		m_nNowMotion = DecideAni();
	}
	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
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
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CMstKnight::Think()
{
	BOOL bSearch = false;
	if( m_nDistance < SiR)			// �þ� �ݰ�ȿ� ������ ���
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
	else{							// �þ� �ݰ� �ۿ� ������
		if( m_bLeader )	{
			
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) {		// ��ǥ������ �� ������
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
			if(abs(m_nTarX-m_X)<20 && abs(m_nTarY-m_Y)<20) 	m_nCharStatus = Stand;	// ��ǥ������ �� ������
			else {
				m_nCharStatus = Run;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
			}			
		}
	}
}

//----------------------------------------------------------------------
// ĳ���� ȣ�� ��� �� ��ǥ�� ���� �Լ�
// ��ǥ���� ������ ������� �׽ø��� ���̿��� ����κ��� �Ÿ� RgMg��ġ
//----------------------------------------------------------------------
void CMstKnight::GuardTo() 
{	
	m_nTarX = RgMg*(m_nTeX-m_pTeam->LeaderX)/m_pTeam->Distance + m_pTeam->LeaderX;
	m_nTarY = RgMg*(m_nTeY-m_pTeam->LeaderY)/m_pTeam->Distance + m_pTeam->LeaderY;
}

//----------------------------------------------------------------------
// ���� �¾ҳ��� üũ
//----------------------------------------------------------------------
void CMstKnight::AmIBeaten(int *dir, int What)
{
	if(m_bDead) return;
	if(m_HP>0) {  // �¾Ҵٸ�
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