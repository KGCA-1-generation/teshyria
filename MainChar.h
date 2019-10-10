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

	// 임시 함수
	int  GetStatus() { return m_nCharStatus; }

	CMainChar(CAnimation* p, CCharMng* cp);
	virtual ~CMainChar();

public:      // 테쉬리아 스태치 구조체
	BOOL m_bConcent;  // 집중시 미니맵출력을 위해 추가한 변수
	char m_sMotID[12];
	//int  m_X, m_Y;
	CAnimation* m_pAni;
	CCharMng*   m_pCharMng;


protected:
	void BidoEtc();
	void SetAniData(int lock, int cnt, char* str);
	void InputProcess();
	BOOL IsEndFrm(int AniStatus);

	int  m_nX, m_nY; // 가변치
	int  m_nFrmCntLimit, m_nFrmCntNow;
	BOOL m_bAniLock, m_bSitdown, m_bAniStop;
	int  m_nInputStatus, m_nCharStatus, m_nDirection;
	int  m_nAttack;
	//CDx7Sound m_CSound;

public:	

	// 각 수치 변수 ----------------------------------------------------------------------------

	int m_HP;		// 체력
	int m_MP;		// 마나량
	int m_SP;		// 스테미너량
	int m_DF;		// 방어력 

	int m_IncMP;
	int m_IncSP;

	// 각 수치 계산을 위한 변수 ----------------------------------------------------------------

	int m_aManaWaste[17];				// 공격에 따른 마나 소모량 	
	int m_nDFPenalty;					// 디펜스 패널티 ( 마나 집중시 디펜스가 저하되는 비율) 
	int m_nSPWasteR;					// 달리기시 스테미너 감소분 
	int m_nSPWasteC;					// 집중시 스테미너 감소분 
	int m_nConTime;						// 집중 시간
	int m_nAbsTime;						// 흡수 시간
	BOOL m_bSPAbsorb;					// 스테미너 흡수 여부 
	BOOL m_bMPAbsorb;					// 마나 흡수 여부
	BOOL m_bConcen;
	DWORD m_dwTime;						// 임시 시간 변수 

	// 각 수치 계산을 위한 함수 ----------------------------------------------------------------

	int DecideStatus(int nStatus, int nOld);		// 캐릭의 상태를 각 수치를 고려해서 판단 

	void SetSPWaste(int nPenalty);					// 스테미너 소모량 설정
	void SetManaWaste(int nWpMana);					// 마나 소모량 설정 
	void SetDefense(int nDF);						// 방어력 설정

	void AcountHPWaste(int nPower);					// 체력 소모량 계산 함수 
	void AcountSPWaste();							// 스테미너 소모량 계산 함수 
	void AcountManaWaste(int nStatus);				// 마나 소모량 계산 함수 

	void AcountMPAbsorb();							// 마나흡수량 계산 함수 
	void AcountSPAbsorb();							// 스테미너 흡수량 계산 함수 
	//------------------------------------------------------------------------------------------

	int GetHP() { return m_HP; }
	int GetMP() { return m_MP; }
	int GetSP() { return m_SP; }
	int GetIncMP() { return m_IncMP; }

	int m_nNowMotion, m_nNowFrame;	
};

#endif // !defined(AFX_MAINCHAR_H__A65AEE8A_ED09_4A1D_86A0_3433CE8A5670__INCLUDED_)