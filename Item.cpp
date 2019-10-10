// Item.cpp: implementation of the CItem class.
//
//////////////////////////////////////////////////////////////////////

#include "Item.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define		WP_SAINT	7
#define		AM_SAINT	7

CItem::CItem()
{
	m_aWeapon[0].nWeight = 4;
	m_aWeapon[1].nWeight = 4;
	m_aWeapon[2].nWeight = 6;
	m_aWeapon[3].nWeight = 6;
	m_aWeapon[4].nWeight = 8;
	m_aWeapon[5].nWeight = 10;
	m_aWeapon[6].nWeight = 4;
	m_aWeapon[7].nWeight = 10;

	m_aWeapon[0].nDelay  = 400;
	m_aWeapon[1].nDelay  = 200;
	m_aWeapon[2].nDelay  = 600;
	m_aWeapon[3].nDelay  = 800;
	m_aWeapon[4].nDelay  = 600;
	m_aWeapon[5].nDelay  = 1000;
	m_aWeapon[6].nDelay  = 200;
	m_aWeapon[7].nDelay  = 400;

	m_aWeapon[0].nMana  = 2;
	m_aWeapon[1].nMana  = 4;
	m_aWeapon[2].nMana  = 6;
	m_aWeapon[3].nMana  = 8;
	m_aWeapon[4].nMana  = 9;
	m_aWeapon[5].nMana  = 10;
	m_aWeapon[6].nMana  = 14;
	m_aWeapon[7].nMana  = 4;

	
	m_aWeapon[0].nAttack  = 20;
	m_aWeapon[1].nAttack  = 20;
	m_aWeapon[2].nAttack  = 30;
	m_aWeapon[3].nAttack  = 40;
	m_aWeapon[4].nAttack  = 40;
	m_aWeapon[5].nAttack  = 50;
	m_aWeapon[6].nAttack  = 30;
	m_aWeapon[7].nAttack  = 40;

	m_aArmor[0].nWeight = 8;
	m_aArmor[1].nWeight = 10;
	m_aArmor[2].nWeight = 12;
	m_aArmor[3].nWeight = 14;
	m_aArmor[4].nWeight = 16;
	m_aArmor[5].nWeight = 18;
	m_aArmor[6].nWeight = 12;
	m_aArmor[7].nWeight = 20;

	m_aArmor[0].nDefense = 10;
	m_aArmor[1].nDefense = 15;
	m_aArmor[2].nDefense = 20;
	m_aArmor[3].nDefense = 25;
	m_aArmor[4].nDefense = 30;
	m_aArmor[5].nDefense = 35;
	m_aArmor[6].nDefense = 30;
	m_aArmor[7].nDefense = 30;

	m_nPtWeight = 2;
	m_nPtRecovery = 25;
	m_nKnifeMax = 60;

	m_nSaintAttack = 60;
	m_nSaintDefense = 60;

	m_nStmReduce = 3;
	m_nConReduce = 10;
	m_nDlyReduce = 1;
}

CItem::~CItem()
{

}

void CItem::SetCurItem(int nWpKinds, int nAmKinds, int nKnCnt, int nHPCnt, int nMPCnt, int nSPCnt)
{
	m_CurItem.nWeapon = nWpKinds;
	m_CurItem.nArmor = nAmKinds;
	m_CurItem.nKnifeCnt = nKnCnt;
	m_CurItem.nHPCnt = nHPCnt;
	m_CurItem.nMPCnt = nMPCnt;
	m_CurItem.nSPCnt = nSPCnt;
}

void CItem::AccountTotalWeight()
{
	m_nTotalWeight = m_aWeapon[m_CurItem.nWeapon].nWeight									// 公扁 公霸 
					+m_aArmor[m_CurItem.nArmor].nWeight										// 规绢备 公霸 
					+(m_CurItem.nHPCnt+m_CurItem.nMPCnt+m_CurItem.nSPCnt)*m_nPtWeight;		// 器记 公霸 
}

int CItem::GetCurAttack(bool bDevil)
{
	if( bDevil && m_CurItem.nWeapon ==  WP_SAINT )
		return m_nSaintAttack;
	
	else return m_aWeapon[m_CurItem.nWeapon].nAttack;	
}

int CItem::GetCurDefense(bool bDevil)
{
	if( bDevil && m_CurItem.nArmor ==  AM_SAINT )
		return m_nSaintDefense;
	
	else return m_aArmor[m_CurItem.nArmor].nDefense;	
}

void CItem::AccountItemEffect()
{
	if( m_nTotalWeight < 30 )
		m_nPenalty = 0;
	else {
		if( m_nTotalWeight < 40 )		m_nPenalty = 1; 
		else if( m_nTotalWeight < 50 )	m_nPenalty = 2; 
		else if( m_nTotalWeight < 60 )	m_nPenalty = 3; 
		else if( m_nTotalWeight < 70 )	m_nPenalty = 4; 
		else if( m_nTotalWeight < 80 )	m_nPenalty = 5; 
	}

	m_nRunStmDec = m_nPenalty * m_nStmReduce;
	m_nConStmDec = m_nPenalty * m_nConReduce;
	m_nDlyDec = m_nPenalty * m_nDlyReduce;
}
