// Item.h: interface for the CItem class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_)
#define AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ���� ����ü 
struct Weapon{
	int nAttack;		// ���ݷ�
	int nWeight;		// ���� 
	int nMana;			// ��� ���� 
	int nDelay;			// �� 
};

// �� ����ü 
struct Armor{
	int nDefense;		// ����
	int nWeight;		// ���� 
};

// ���� ���� ������ ����ü 
struct CurItem{
	int nWeapon;		// ���� ������ ���� 
	int nArmor;			// ���� ������ �� 
	int nKnifeCnt;		// ���� ������ �ܵ� ���� 
	int nHPCnt;			// ���� ������ �������� ���� 
	int nMPCnt;			// ���� ������ �������� ���� 
	int nSPCnt;			// ���� ������ ���׹̳� ���� ���� 
};

class CItem  
{
public:
	void AccountItemEffect();			// ������ ����Ƽ�� �� ��ġ ���Һ��� ����ϴ� �Լ� 
	void AccountTotalWeight();			// ���� ������ �������� �� ���Ը� ����ϴ� �Լ� 
	
	int GetCurDefense(bool bDevil);		// ���� ������ ��� �Լ� 
	int GetCurAttack(bool bDevil);		// ���� ���ݷ��� ��� �Լ� 


	int GetStmDec(bool bConCent) { if( bConCent ) return m_nConStmDec; else return m_nRunStmDec; }
										// ���� ���׹̳� ���Һ��� ��� �Լ� 
	int	GetDlyDec()		{ return m_nDlyDec; }	// ���� ���� �����̸� ��� �Լ� 

	void SetCurItem(int nWpKinds, int nAmKinds, int nKnCnt, int nHPCnt, int nMPCnt, int nSPCnt);
										// ���� �������� �Ѳ����� �����ϴ� �Լ� 

	void SetCurWeapon(int nWpKinds)		{ m_CurItem.nWeapon = nWpKinds; }		// ���� ���⸦ ����
	void SetCurArmor(int nAmKinds)		{ m_CurItem.nArmor = nAmKinds; }		// ���� ���� ���� 
	void SetCurKnifeCnt(int nKnCnt)		{ m_CurItem.nKnifeCnt = nKnCnt; }		// ���� �ܵ������� ���� 
	void SetCurHPCnt(int nHPCnt)		{ m_CurItem.nHPCnt = nHPCnt; }			// ���� �������� ������ ����
	void SetCurMPCnt(int nMPCnt)		{ m_CurItem.nMPCnt = nMPCnt; }			// ���� �������� ������ ����
	void SetCurSPCnt(int nSPCnt)		{ m_CurItem.nSPCnt = nSPCnt; }			// ���� ���׹̳����� ������ ����
	
	void GetCurItem(CurItem* pCurItem)  { pCurItem = &m_CurItem; }			// ���� ���� �����۱���ü�� �����͸� ��� �Լ� 
	
	int	GetCurWeapon()				{ return m_CurItem.nWeapon; }			// ���� ������ ���⸦ ��� �Լ� 
	int	GetCurArmor()				{ return m_CurItem.nArmor; }			// ���� ������ ���� ��� �Լ� 
	int	GetCurKnifeCnt()			{ return m_CurItem.nKnifeCnt; }			// ���� ������ �ܵ� ������ ��� �Լ� 
	int	GetCurHPCnt()				{ return m_CurItem.nHPCnt; }			// ���� ������ �������� ������ ��� �Լ� 
	int	GetCurMPCnt()				{ return m_CurItem.nMPCnt; }			// ���� ������ �̴����� ������ ��� �Լ� 
	int	GetCurSPCnt()				{ return m_CurItem.nSPCnt; }			// ���� ������ ���׹̳����� ������ ��� �Լ� 

	int GetKnifeMax()				{ return m_nKnifeMax; }					// �ܵ� ���Ѱ��� ��� �Լ� 
	int GetnPtRecovery()			{ return m_nPtRecovery; }				// ������ ȸ��ġ�� ��� �Լ� 


protected:
	Weapon	m_aWeapon[8];		// ���� ����ü �迭
	Armor	m_aArmor[8];		// �� ����ü �迭
	CurItem m_CurItem;			// ���� ������ �������� ������ ������ ������ ���� 

	int		m_nPtRecovery;		// ���� ȸ��ġ (%)
	int		m_nPtWeight;		// ���� ���� 
	int		m_nKnifeMax;		// �ܵ� ���� �Ѱ� 

	int		m_nSaintAttack;		// ����Ʈ ī������ ���ݷ�(���� ���ݽ�)
	int		m_nSaintDefense;	// ����Ʈ ũ����Ż �Ƹ��� ����(���� ����) 

	int		m_nStmReduce;		// �г�Ƽ�� ���� �޸���� ���׹̳��� ���Һ� 
	int		m_nConReduce;		// �г�Ƽ�� ���� ���߽� ���׹̳��� ���Һ�
	int		m_nDlyReduce;		// �г�Ƽ�� ���� ���� ���Һ� 

	int		m_nPenalty;			// �������� �� ���Կ� ���� �г�Ƽ 
	int		m_nRunStmDec;		// �޸���� ���׹̳� ���Һ�
	int		m_nConStmDec;		// ���߽� ���׹̳� ���Һ� 
	int		m_nDlyDec;			// ����� ���Һ� 
	
	int		m_nTotalWeight;		// ���� ������ �������� �� ����

	CItem();
	virtual ~CItem();

};

#endif // !defined(AFX_ITEM_H__380B037F_C060_4BD2_A760_80CF70B05B1C__INCLUDED_)
