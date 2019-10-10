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
// ������ ���
//--------------------------------------------------------------------
#define AT    240    // ���ݷ�
#define DF     20    // ����
#define SpD  20.0f   // ���ǵ�
#define VP     25    // ȸ�� Ȯ��
#define SiR   750    // �þ߹ݰ�
#define AtR   250    // ���ݹݰ�
#define AtCk  150    // ���� �浹 üũ
#define RgMg   80    // ��������� ���ݰŸ�

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

	//���⿡ ���� �����浹����  
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
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return TRUE  : ���� ���
//        FALSE : ���� ���X (1���� �ִ�)
//----------------------------------------------------------
int CScoutKnight::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 16, "Stand_0");		return true;
	case Run :    // �޸���
		SetAniData(false, 3, "Run_0");			return true;
	case Attack1 : // ����
		SetAniData(true, 3, "Attack1_0");		return true;
	case Attack2 : // ����
		SetAniData(true, 3, "Attack2_0");		return true;
	case Beaten : // ���� �ޱ�
		SetAniData(true, 3, "Beaten_0");		return true;
	case Die1 :   // �ױ� 1
		SetAniData(true, 4, "Die1");			return false;
	case Die2 :   // �ױ� 2
		SetAniData(true, 4, "Die2");			return false;
	}
	return FALSE;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
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
		if(m_nCharStatus==Run) MoveTo(); // �̵��ô� pixel�� ���ϹǷ�
		m_nFrmCntNow = 0;
	}
	// ������ �߰� �������� ������ ��
	if(IsMidFrm(Attack1) || IsMidFrm(Attack2)) {
		// �߰� �����ӽ� �ѹ��� �׽ø��ƿ��� �Ÿ��� ����Ѵ�.
		m_nDistance  = GetDistance(abs(m_nTeX-m_X), abs(m_nTeY-m_Y));
		if(m_nDistance<AtCk) { m_pCharMng->BeatenTeshy(AT); } 
	}
	// �¾��� �� ������ �������� ������ ��
	if(IsEndFrm(Beaten)) {
		m_HP -= 100;			
	}
	// �׾��� �� ������ �������� ������ ��
	if(IsEndFrm(Die1) || IsEndFrm(Die2)) {
		m_bDead = TRUE;
		m_pTeam->nDeadCnt++;
		m_nNowFrame--;
		m_pCharMng->MarkPos(m_X, m_Y, 0, false);
	}
	// �ִϰ� ���ٸ�
	if(IsEndFrm(m_nCharStatus)) { 
		m_nCharStatus  = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
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
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CScoutKnight::Think()
{	
	if( m_nDistance<SiR ) // �þ� �ݰ�ȿ� ������ ���
	{
		m_pTeam->bFind = TRUE;
		if( m_nDistance < AtR ) // ���� �����ȿ� ������ ���
		{
			m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
			if(m_nDistance > m_aAtCk[m_nDirection] ) // Į���� �ۿ� ���� ��
			{ 				
				m_nCharStatus = Run;
				m_nTarX = m_nTeX;
				m_nTarY = m_nTeY;
			} 
			else                   // Į���� �ȿ� ���� ��
			{
				if(m_nCharStatus != Attack1 && m_nCharStatus != Attack2)
					m_nCharStatus = Attack1 + (rand()%2);
			}
		}
		else                   // ���� ������ �ƴ� ���
		{           
			if(m_pTeam->bLeader) // �����簡 ������� �� (ȣ�����)
			{
				m_nCharStatus = Run;
				GuardTo();
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
				// ��ǥ������ ���� ��
				if(abs(m_nTarX-m_X)<10 && abs(m_nTarY-m_Y)<10) {
					m_nCharStatus = Stand;
					m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
				}
			}
			else               // �����簡 �׾��� ��
			{
				m_nTarX = 2000;
				m_nTarY = 2000;
				m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
				m_nCharStatus = Run;
			}
		}
	}
	else // �þ� �ݰ� �ۿ� ���� ���
	{
		//if(m_pTeam->bFind) m_nCharStatus = 0;
	}
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// ���� �¾ҳ��� üũ
//----------------------------------------------------------------------
void CScoutKnight::AmIBeaten(int *dir, int What)
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

//----------------------------------------------------------------------
// ĳ���� ȣ�� ��� �� ��ǥ�� ���� �Լ�
// ��ǥ���� ������ ������� �׽ø��� ���̿��� ����κ��� �Ÿ� RgMg��ġ
//----------------------------------------------------------------------
void CScoutKnight::GuardTo() 
{	
	m_nTarX = RgMg*(m_nTeX-m_pTeam->LeaderX)/m_pTeam->Distance + m_pTeam->LeaderX;
	m_nTarY = RgMg*(m_nTeY-m_pTeam->LeaderY)/m_pTeam->Distance + m_pTeam->LeaderY;
}