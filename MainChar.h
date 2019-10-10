// MainChar.h: interface for the CMainChar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINCHAR_H__A65AEE8A_ED09_4A1D_86A0_3433CE8A5670__INCLUDED_)
#define AFX_MAINCHAR_H__A65AEE8A_ED09_4A1D_86A0_3433CE8A5670__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
class CDx7Sound;
class CCharMng;
class CAnimation;
class CMainChar
{
public:
	void DrawChar(unsigned short* pScreen);
	void SetStatus(int* dir, int status);
	void AmIBeaten(int nPower);
	void SetMove(int x, int y) { m_nX = x; m_nY = y; }	
	int  IsLock()   { return m_bAniLock; }
	int  IsSitdown(){ return m_bSitdown; }
	int  WhatAttack() { return m_nAttack; }
	void AniContinue() { m_bAniStop = false; m_nCharStatus = 0;}
	void AttackNormalize() { m_nAttack = -1; }
	int  DecideAni();
	int  PreDefineAni(void);

	// �ӽ� �Լ�
	int  GetStatus() { return m_nCharStatus; }

	CMainChar(CAnimation* p, CCharMng* cp);
	virtual ~CMainChar();

public:      // �׽����� ����ġ ����ü
	BOOL m_bConcent;  // ���߽� �̴ϸ������ ���� �߰��� ����
	char m_sMotID[12];
	//int  m_X, m_Y;
	CAnimation* m_pAni;
	CCharMng*   m_pCharMng;


protected:
	void BidoEtc();
	void SetAniData(int lock, int cnt, char* str);
	void InputProcess();
	BOOL IsEndFrm(int AniStatus);

	int  m_nX, m_nY; // ����ġ
	int  m_nFrmCntLimit, m_nFrmCntNow;
	BOOL m_bAniLock, m_bSitdown, m_bAniStop;
	int  m_nInputStatus, m_nCharStatus, m_nDirection;
	int  m_nAttack;
	//CDx7Sound m_CSound;

public:	

	// �� ��ġ ���� ----------------------------------------------------------------------------

	int m_HP;		// ü��
	int m_MP;		// ������
	int m_SP;		// ���׹̳ʷ�
	int m_DF;		// ���� 

	int m_IncMP;
	int m_IncSP;

	// �� ��ġ ����� ���� ���� ----------------------------------------------------------------

	int m_aManaWaste[17];				// ���ݿ� ���� ���� �Ҹ� 	
	int m_nDFPenalty;					// ���潺 �г�Ƽ ( ���� ���߽� ���潺�� ���ϵǴ� ����) 
	int m_nSPWasteR;					// �޸���� ���׹̳� ���Һ� 
	int m_nSPWasteC;					// ���߽� ���׹̳� ���Һ� 
	int m_nConTime;						// ���� �ð�
	int m_nAbsTime;						// ��� �ð�
	BOOL m_bSPAbsorb;					// ���׹̳� ��� ���� 
	BOOL m_bMPAbsorb;					// ���� ��� ����
	BOOL m_bConcen;
	DWORD m_dwTime;						// �ӽ� �ð� ���� 

	// �� ��ġ ����� ���� �Լ� ----------------------------------------------------------------

	int DecideStatus(int nStatus, int nOld);		// ĳ���� ���¸� �� ��ġ�� ����ؼ� �Ǵ� 

	void SetSPWaste(int nPenalty);					// ���׹̳� �Ҹ� ����
	void SetManaWaste(int nWpMana);					// ���� �Ҹ� ���� 
	void SetDefense(int nDF);						// ���� ����

	void AcountHPWaste(int nPower);					// ü�� �Ҹ� ��� �Լ� 
	void AcountSPWaste();							// ���׹̳� �Ҹ� ��� �Լ� 
	void AcountManaWaste(int nStatus);				// ���� �Ҹ� ��� �Լ� 

	void AcountMPAbsorb();							// ��������� ��� �Լ� 
	void AcountSPAbsorb();							// ���׹̳� ����� ��� �Լ� 
	//------------------------------------------------------------------------------------------

	int GetHP() { return m_HP; }
	int GetMP() { return m_MP; }
	int GetSP() { return m_SP; }
	int GetIncMP() { return m_IncMP; }

	int m_nNowMotion, m_nNowFrame;	
};

#endif // !defined(AFX_MAINCHAR_H__A65AEE8A_ED09_4A1D_86A0_3433CE8A5670__INCLUDED_)