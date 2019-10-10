// Animal.cpp: implementation of the CAnimal class.
//
//////////////////////////////////////////////////////////////////////
#include <math.h>
//#include <windows.h>
#include "Animation.h"
#include "CharMng.h"
#include "Animal.h"

#define Stand    0
#define Run      1
#define Fright	 2

#define Rabbit  10
#define Wolf    11
#define Bear    12

#define SiR    300    // �þ߹ݰ�
#define SiR2   800    // �þ߹ݰ�
//--------------------------------------------------------------------
// Construction/Destruction
//--------------------------------------------------------------------
CAnimal::CAnimal(CAnimation* p, CCharMng* cp, int x, int y, int team, int type)
{
	m_pTeam        = NULL; 
	m_pAni         = p;
	m_pCharMng     = cp;
	m_HP           = 1;
	m_nType        = type;
	switch(type) {
	case Rabbit : m_nSpeed = 15;	break;
	case Wolf   : m_nSpeed = 10;	break;
	case Bear   : m_nSpeed = 7;	break;
	}
	m_X            = x;
	m_Y            = y;
	m_nMoveX	   = m_X/32;
	m_nMoveY	   = m_Y/32;
	SetAniData(false, 10, "Stand_0");
	m_pCharMng->MarkPos(m_X, m_Y, 0, true);
	m_nTeam        = team;
	m_bFright	   = false;
}

CAnimal::~CAnimal()
{
}

//----------------------------------------------------------
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return true  : ���� ���
//        false : ���� ���X (1 ���� �׸�)
//----------------------------------------------------------
int CAnimal::PreDefineAni()
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 20, "Stand_0");		return true;
	case Run :    // �޸���
		if(m_nType==Rabbit) {
			SetAniData(false, 3, "Run_0");		return true;
		}
		else {
			SetAniData(false, 6, "Run_0");		return true;
		}
	case Fright :
		if(m_nType==Rabbit)	{
			SetAniData(true, 2, "Fright");		return false;
		}
		else {
			SetAniData(true, 8, "Fright_0");	return true;
		}
	}
	return false;
}

//----------------------------------------------------------------------
// �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
//----------------------------------------------------------------------
void CAnimal::DrawChar(unsigned short* pScreen, int bOutLine)
{
	m_pAni->DrawAni(m_nScrX, m_nScrY, m_nNowMotion, m_nNowFrame, pScreen);

	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		m_nNowFrame++;
		if(m_nCharStatus==Run) MoveTo(); // �̵��ô� pixel�� ���ϹǷ�
		m_nFrmCntNow = 0;
	}
	
	if(IsEndFrm(m_nCharStatus))  // �ִϰ� ���ٸ�
	{
		m_nCharStatus = Stand;
		m_nNowMotion = DecideAni();
	}

	m_nNowFrame %= m_pAni->GetMotFrmCount(m_nNowMotion);
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CAnimal::SetStatus(point* Teshy, TeamInfo* TInfo)
{
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	m_pTeam = TInfo;
	m_nDistance  = GetDistance (m_nTeX-m_X, m_nTeY-m_Y);
	m_nDirection = GetDirection(m_nTeX-m_X, m_Y-m_nTeY);
	Think();
	m_nScrX = m_X-m_nTeX+400;
	m_nScrY = m_Y-m_nTeY+300;
	m_nNowMotion = DecideAni();
}

//----------------------------------------------------------------------
// ĳ���� ���� ���� �Լ�
//----------------------------------------------------------------------
void CAnimal::Think()
{
	if( m_nDistance < SiR ) 
	{
		if( !m_bFright ) {
			m_bFright = true;
			m_nCharStatus = Fright;
		}
		else {
			if( !m_bAniLock )	AnimalRun();
		}
	}
	else {
		if( m_nDistance > SiR2 && m_bFright ) m_bFright = false;
		AnimalRun();
	}
	m_nDirection = GetDirection(m_nTarX-m_X, m_Y-m_nTarY);
}

void CAnimal::AnimalRun()
{
	if( m_pTeam->nWpNum == m_pTeam->nWPCnt ) m_pTeam->nWpNum = 0;
			
	m_nTarX = m_pTeam->pWpX[m_pTeam->nWpNum];
	m_nTarY = m_pTeam->pWpY[m_pTeam->nWpNum];
	if( (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000010) || (m_pCharMng->GetMapAttrP(m_nTarX, m_nTarY) & 0x00000001) )
	{
		m_pTeam->nWpNum++;
	}

	if(abs(m_nTarX-m_X)<10 && abs(m_nTarY-m_Y)<10) {		// ��ǥ������ �� ������
		m_nCharStatus = Stand;								// 
		m_pTeam->nWpNum++;
	}
	else													// �ƴҶ� 
	{
		m_nCharStatus = Run;
	}
}
