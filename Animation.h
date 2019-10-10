// Animation.h: interface for the CAnimation class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ANIMATION_H__82074EBD_A673_4DAE_9638_8309CC9374DF__INCLUDED_)
#define AFX_ANIMATION_H__82074EBD_A673_4DAE_9638_8309CC9374DF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"

class CAnimation : public CImage  
{
public:
	bool LoadAni(char* fn);
	void DrawAni(int x, int y, int Motion, int Frame, WORD* Screen);
	void DrawAniOL(int x, int y, int Motion, int Frame, WORD* Screen);
	int GetMotCount() { return m_nMotCount; }
	int GetMotFrmCount(int mot) { return m_pMotFrmNum[mot]; }
	int GetFrmData(int mot, int frm) { return m_pFrmData[m_pMotOffset[mot]+frm]; }
	CAnimation();
	~CAnimation();
	
	// 변수들
public:
	int  m_nRealFrame;
	char (*m_sMotID)[12];
	int* m_pCX;
	int* m_pCY;

protected:	
	int  m_nMotCount;
	int  m_nTotalMotFrm;	
	int* m_pMotOffset;
	int* m_pMotFrmNum;      // 모션당 프레임수
	int* m_pFrmData;        // 모든 모션에서 쓰이는 프레임 데이타 포인터
};

#endif // !defined(AFX_ANIMATION_H__82074EBD_A673_4DAE_9638_8309CC9374DF__INCLUDED_)
