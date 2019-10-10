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
// �׽ø���(��ĳ��)�� ��� Define
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
// �׽ø���(��ĳ��)�� ���μ�ġ ��ȭ ��� Define
//---------------------------------------------------------
#define AT    100    // ���ݷ�
#define DF    100    // ����
#define	MAX_HP	1000	
#define	MAX_MP	1000
#define	MAX_SP	1000

#define SD    100    // ���ǵ�
#define Madd   50    // ���� ����ġ
#define MEa    20    // ���� �Ҹ�, �ʴ�(���ݽ�)
#define MEd    20    //         "        (����)
#define MEs    10    //         "        (�̵���)
#define MAp    80    // ���� ��� �ۼ�Ʈ
#define MAs     6    // ���� ����� �ɸ��� �ð�
#define SEc   20    // ���߽� ���¹̳� �Ҹ�
#define SEs    10    // �̵��� ���¹̳� �Ҹ�
#define SAq    40    // ���¹̳� ����� (�ʴ�)
#define CL   1400    // ���� ���� �Ÿ�(per pixel)
#define CT      4    // ���� �Ҹ� �ð�
#define VP     20    // ȸ�� Ȯ��
#define WL     30    // ���� ���� ����
#define WLMAX  80    // ���� ���� �ִ� ����

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


	//�׽ø��� ��ġ�� 
	m_HP = MAX_HP;
	m_MP = MAX_MP;
	m_SP = MAX_SP;
	
	m_IncMP =0;
	m_IncSP =0;

	m_DF = DF;
	m_nDFPenalty = 1;

	memset(m_aManaWaste, 0, 17*4);

	// ������ Ŭ�������� �������־�� �ϴ� �Լ� 
	SetDefense(100);
	SetManaWaste(30);
	SetSPWaste(3);
}

CMainChar::~CMainChar()
{
}

//----------------------------------------------------------
// ����� ĳ���� ������ ��ȭ�� ���� ����
//----------------------------------------------------------
void CMainChar::SetStatus(int* dir, int status)
{
	m_nDirection = (*dir);
	int OldStatus = m_nCharStatus;

	// ��ġ�ڵ� 1: �Էµ� �ൿ�� �� ��ġ�� ����� �Ǵ� --------------------------------
	status = DecideStatus(status, OldStatus);					
	//----------------------------------------------------------------------------------

	if(m_nCharStatus == Die) {
		m_nNowMotion = DecideAni();
		return;
	}
	
	if(m_bAniLock==false) // �ִ϶��� false
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
	else // �ִ϶��� true
	{
		if(m_nCharStatus==Sitdown) { // �ɱ� �����϶�

			//��ġ�ڵ� 2: ���׹̳� ����� ��ġ ��� -------------------------------------------------------
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
	
	//��ġ�ڵ� 3: ���� ����� ��ġ ��� ------------------------------------------------------
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
	//��ġ�ڵ� 4: ���߽� ��ġ ��� ------------------------------------------------------------
	if( m_nCharStatus == Concen )
	{
		m_nDFPenalty = 3;
		AcountSPWaste();
	}
	else m_nDFPenalty = 1;
	//-----------------------------------------------------------------------------------------
	
}

//----------------------------------------------------------
// InputŰ���� ���� ĳ���� ���°���.
// Ű�� ������ ������ ���� ���.
//----------------------------------------------------------
void CMainChar::InputProcess()
{
	m_nCharStatus = m_nInputStatus;
	
	m_nNowMotion = DecideAni();
}

//------------------------------------------------------------
// ĳ���� ��ü �ִϸ��̼� �׸� �׷� �ִ� �Լ�
// DecideAni()�Լ����� �ִ��� ���������� üũ �ް� ����� ����
//------------------------------------------------------------
void CMainChar::DrawChar(WORD* pScreen)
{
	m_pAni->DrawAni(400+m_nX, 300+m_nY, m_nNowMotion, m_nNowFrame, pScreen);
	if(m_bAniStop) return;
	if((m_nFrmCntNow++) >= m_nFrmCntLimit) {
		m_nNowFrame++;	m_nFrmCntNow = 0;
	}
	
	//��ġ�ڵ� 5: �� �ൿ�� ���� �������ҷ� ��� ----------------------------------------
	if( IsEndFrm( m_nCharStatus ) )	{		// ���� �Ҹ� ��� 
		AcountManaWaste(m_nCharStatus);
	}
	//-------------------------------------------------------------------------------------

	//��ġ�ڵ� 6: �޸���� ���׹̳� ���ҷ� ��� ----------------------------------------
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

	if(m_bAniLock && IsEndFrm(m_nCharStatus))  // �ִϰ� ���ٸ�
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
// ���¸� �ľ��ؼ� ĳ���� �ִϸ� ������
// return true  : ���� ���
//        false : ���� ���X (1���� �׸�)
//----------------------------------------------------------
BOOL CMainChar::PreDefineAni(void) 
{
	switch(m_nCharStatus)
	{
	case Stand :  // �� �ֱ�
		SetAniData(false, 8, "Stand_0");	return true;		
	case Run :  // �޸���
		SetAniData(false, 3, "Run_0");		return true;
	case Run_M :  // ���� �޸���
		SetAniData(false, 1, "Run&M_0");	return true;
	case NA :  // �Ϲ� ����
		m_nAttack  = 1;
		SetAniData(true, 1, "NA_0");		return true;
	case NA_M :  // ������� ����
		m_nAttack  = 2;
		SetAniData(true, 1, "NA&M_0");		return true;
	case Concen :  // �����ϱ�
		SetAniData(false, 1, "Concen");		return false;
	case Absorb :  // ���� ���
		SetAniData(false, 1, "Absorb");		return false;
	case Sitdown :  // �ɱ�
		SetAniData(true, 3, "Sitdown_0");	return true;
	case Defense :  // ���
		SetAniData(true, 1, "Defense_0");	return true;
	case Beaten :  // ���� ��
		SetAniData(true, 2, "Beaten_0");	return true;
	case Eogum : // ��˼�
		m_nAttack  = 3;
		SetAniData(true, 1, "Eogum_0");		return true;
	case Bido : // �񵵼�
		m_nAttack  = 4;
		SetAniData(true, 1, "Bido_0");		return true;
	case Suphon : // ��ȥ��
		SetAniData(true, 2, "Suphon_0");	return true;
	case Valk : // ��Ű��
		SetAniData(true, 3, "Valk");		return false;
	case Dragon : // �巡���Ǿ�
		m_nAttack  = true;
		SetAniData(true, 2, "Dragon_0");	return true;
	case Sword : // �ҵ�극��Ŀ
		m_nAttack  = true;
		SetAniData(true, 1, "Sword_0");		return true;
	case Die : // �ױ�
		SetAniData(true, 6, "Die");			return false;
	case Success : // �̼Ǽ���
		SetAniData(true, 5, "Win");			return false;
	}
	return false;
}

//----------------------------------------------------------
// status���� ���Ͼ��� ��� ID�� ���Ͽ� ���Offset�� ����
//----------------------------------------------------------
int CMainChar::DecideAni()
{
	BOOL bDir;
	bDir = PreDefineAni();
	for(int i=0; i<m_pAni->GetMotCount(); i++) // ��� ����ŭ ����
	{
		// ID���� ���� ���� �ִ��� ��
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
	// ������ ����� Ȯ�� 20%
	if(m_nCharStatus != Beaten && m_nCharStatus != Defense  && m_nCharStatus!=Die ) {		// ó�� �¾����� 
		if(rand()%5) {
			m_nCharStatus = Beaten;

			//��ġ�ڵ� 6: Ÿ���� �޾����� ü�� ���ҷ� ��� ----------------------------------------
			AcountHPWaste(nPower);
			//-------------------------------------------------------------------------------------

		}
		else m_nCharStatus = Defense;
		m_nNowFrame = 0;
		m_nNowMotion = DecideAni();
	}	
	//��ġ�ڵ� 7: Ÿ�� �޴� ���� �ٽ� Ÿ���� �޾����� ü�� ���ҷ� ��� ------------------------------
//	if(m_nCharStatus == Beaten)	{									// �´� ���� ���߿� �� �¾����� 					
//		if( rand()%5 && m_nCharStatus!=Die ) AcountHPWaste(nPower);
//	}
	//----------------------------------------------------------------------------------------------

}

//----------------------------------------------------------------------
// ���� ����� ������ �������� ��������.
// return true : ������ �����ӽ�
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
// ���� �����ϰ��� �ϴ� ���� ���ɿ��θ� �Ǵ��ϴ� �Լ� ( SetStatus���÷� ȣ�� )
// ����  : ���� Ű�Էµ� ���� 
// ���ϰ� :	�Ǵ��� ������ ���� 	
// : �������� - ����� ������ ������� �Ǵ�
// : �񵵼� - ����� �ܵ������� ������� �Ǵ� 
//--------------------------------------------------------------------------------
int CMainChar::DecideStatus(int nStatus, int nOld)
{
	if( nStatus == Bido ) {					// �ܵ����� ���� 


	}
	if( m_MP < m_aManaWaste[nStatus] ){		// ������ ������� ������ 
		switch(nStatus)
		{
		case NA_M :		return NA;			// ������� ����
		case Run_M:		return Run;			// �����޸��� 

		default:		return nOld;
		}
	}
	else return nStatus;					// ������ ����ϸ� 
}
// ---------------------------------------------------------
// Ÿ���� �޾����� ü�� ��� ( AmIBeaten() �϶� ȣ�� )
// : ü�� = ü�� - Ÿ��ġ - ���� 
//----------------------------------------------------------
void CMainChar::AcountHPWaste(int nPower)
{
	if( m_HP > 0 )
		m_HP -= (nPower-m_DF/m_nDFPenalty);
	else {					// �׽ø��� Die, GameOver
		m_nCharStatus = Die;
	}
}

// ---------------------------------------------------------------
// ���� ��� ���ݽ� ���� �Ҹ� ��� ( AmIBeaten() �϶� ȣ�� )
// : ������ = ������ - �� ������ ���� �Ҹ�  
//----------------------------------------------------------------
void CMainChar::AcountManaWaste(int nStatus)
{
	m_MP -= m_aManaWaste[nStatus];
	if( m_MP < 0 ) m_MP = 0;
}
// ---------------------------------------------------------------
// ���׹̳� �Ҹ� ��� 
// �޸���� ���׹̳� �Ҹ� = ���׹̳ʷ� - �ѵ���   
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
// ���׹̳� ����� ���׹̳ʷ� ���
// : ���׹̳ʷ� = ���͹̳� + �⺻ ���׹̳� ������ (�ʴ�)
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
// ���� ����� ������ ���
// : ������ = ������ +  �����Ҹ��� 80%( ������� ������ )
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
// ���� �׽ø����� ���� ��� ( ���� �ٲܶ����� ȣ�� )
// : ���� = �⺻ ���� + ���� ���� 
//-------------------------------------------------------------------
void CMainChar::SetDefense(int nDF)	
{
	m_DF = DF+nDF;
}

// -------------------------------------------------------------------
// �� ������ ���� �Ҹ� ���� ( ���⸦ �ٲܶ����� ȣ�� )
// ���� : ������ ���� �Ҹ� 
// : �� ������ �����Ҹ� + ������ �����Ҹ� 
//---------------------------------------------------------------------

void CMainChar::SetManaWaste(int nWpMana)
{
	m_aManaWaste[Run_M]		= 10;						// �����޸��� 
	m_aManaWaste[NA_M]		= 20	+ nWpMana;			// ��������
	m_aManaWaste[Eogum]		= 100	+ nWpMana;			// ��˼� 
	m_aManaWaste[Bido]		= 60	+ nWpMana;			// �񵵼�
	m_aManaWaste[Suphon]	= 40	+ nWpMana;			// ��ȥ��
	m_aManaWaste[Valk]		= 90	+ nWpMana;			// ��Ű�� 
	m_aManaWaste[Dragon]	= 40	+ nWpMana;			// �巡���Ǿ�
	m_aManaWaste[Sword]		= 80	+ nWpMana;			// �ҵ�극��Ŀ
}

// -------------------------------------------------------------------
// ���׹̳� �Ҹ� ���� ( ���⸦ �ٲܶ����� ȣ�� )
// ���� : ������ �г�Ƽ 
// : �� �ൿ�� �⺻ ���׹̳� �Ҹ� + ���� �г�Ƽ * ����ġ  
//---------------------------------------------------------------------
void CMainChar::SetSPWaste(int nPenalty)
{
	m_nSPWasteR = SEs+nPenalty*3;		
	m_nSPWasteC = SEc+nPenalty*10;	
}


