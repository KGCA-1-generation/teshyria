// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_)
#define AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 무기 구조체 
struct Weapon{
	int nAttack;		// 공격력
	int nWeight;		// 무게 
	int nMana;			// 사용 마나 
	int nDelay;			// 빈도 
};

// 방어구 구조체 
struct Armor{
	int nDefense;		// 방어력
	int nWeight;		// 무게 
};

// 현재 착용 아이템 구조체 
struct CurItem{
	int nWeapon;		// 현재 소지한 무기 
	int nArmor;			// 현재 착용한 방어구 
	int nKnifeCnt;		// 현재 소지한 단도 갯수 
	int nHPCnt;			// 현재 소지한 힐링포션 갯수 
	int nMPCnt;			// 현재 소지한 마나포션 갯수 
	int nSPCnt;			// 현재 소지한 스테미나 포션 갯수 
};

class CItem  
{
public:
	void AccountItemEffect();			// 아이템 패털티와 각 수치 감소분을 계산하는 함수 
	void AccountTotalWeight();			// 현재 소지한 아이템의 총 무게를 계산하는 함수 
	
	int GetCurDefense(bool bDevil);		// 현재 방어력을 얻는 함수 
	int GetCurAttack(bool bDevil);		// 현재 공격력을 얻는 함수 


	int GetStmDec(bool bConCent) { if( bConCent ) return m_nConStmDec; else return m_nRunStmDec; }
										// 현재 스테미너 감소분을 얻는 함수 
	int	GetDlyDec()		{ return m_nDlyDec; }	// 현재 무기 딜레이를 얻는 함수 

	void SetCurItem(int nWpKinds, int nAmKinds, int nKnCnt, int nHPCnt, int nMPCnt, int nSPCnt);
										// 현재 아이템을 한꺼번에 세팅하는 함수 

	void SetCurWeapon(int nWpKinds)		{ m_CurItem.nWeapon = nWpKinds; }		// 현재 무기를 세팅
	void SetCurArmor(int nAmKinds)		{ m_CurItem.nArmor = nAmKinds; }		// 현재 방어구를 세팅 
	void SetCurKnifeCnt(int nKnCnt)		{ m_CurItem.nKnifeCnt = nKnCnt; }		// 현재 단도갯수를 세팅 
	void SetCurHPCnt(int nHPCnt)		{ m_CurItem.nHPCnt = nHPCnt; }			// 현재 힐링포션 갯수를 세팅
	void SetCurMPCnt(int nMPCnt)		{ m_CurItem.nMPCnt = nMPCnt; }			// 현재 마나포션 갯수를 세팅
	void SetCurSPCnt(int nSPCnt)		{ m_CurItem.nSPCnt = nSPCnt; }			// 현재 스테미나포션 갯수를 세팅
	
	void GetCurItem(CurItem* pCurItem)  { pCurItem = &m_CurItem; }			// 현재 착용 아이템구조체의 포인터를 얻는 함수 
	
	int	GetCurWeapon()				{ return m_CurItem.nWeapon; }			// 현재 소지한 무기를 얻는 함수 
	int	GetCurArmor()				{ return m_CurItem.nArmor; }			// 현재 착용한 방어구를 얻는 함수 
	int	GetCurKnifeCnt()			{ return m_CurItem.nKnifeCnt; }			// 현재 소지한 단도 갯수를 얻는 함수 
	int	GetCurHPCnt()				{ return m_CurItem.nHPCnt; }			// 현재 소지한 힐링포션 갯수를 얻는 함수 
	int	GetCurMPCnt()				{ return m_CurItem.nMPCnt; }			// 현재 소지한 미니포션 갯수를 얻는 함수 
	int	GetCurSPCnt()				{ return m_CurItem.nSPCnt; }			// 현재 소지한 스테미너포션 갯수를 얻는 함수 

	int GetKnifeMax()				{ return m_nKnifeMax; }					// 단도 제한갯수 얻는 함수 
	int GetnPtRecovery()			{ return m_nPtRecovery; }				// 포션의 회복치를 얻는 함수 


protected:
	Weapon	m_aWeapon[8];		// 무기 구조체 배열
	Armor	m_aArmor[8];		// 방어구 구조체 배열
	CurItem m_CurItem;			// 현재 소지한 아이템의 종류와 갯수를 저장할 변수 

	int		m_nPtRecovery;		// 포션 회복치 (%)
	int		m_nPtWeight;		// 포션 무게 
	int		m_nKnifeMax;		// 단도 소지 한계 

	int		m_nSaintAttack;		// 세인트 카이저의 공격력(마족 공격시)
	int		m_nSaintDefense;	// 세인트 크리스탈 아머의 방어력(마족 방어시) 

	int		m_nStmReduce;		// 패널티에 대한 달리기시 스테미너의 감소분 
	int		m_nConReduce;		// 패널티에 대한 집중시 스테미너의 감소분
	int		m_nDlyReduce;		// 패널티에 대한 빈도의 감소분 

	int		m_nPenalty;			// 아이템의 총 무게에 대한 패널티 
	int		m_nRunStmDec;		// 달리기시 스테미너 감소분
	int		m_nConStmDec;		// 집중시 스테미너 감소분 
	int		m_nDlyDec;			// 무기빈도 감소분 
	
	int		m_nTotalWeight;		// 현재 소지한 아이템의 총 무게

	CItem();
	virtual ~CItem();

};

#endif // !defined(AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_)
