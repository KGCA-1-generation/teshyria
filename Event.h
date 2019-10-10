// Event.h: interface for the CEvent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EVENT_H__4CC26604_5B7C_4FBE_947C_F3684AD3C25F__INCLUDED_)
#define AFX_EVENT_H__4CC26604_5B7C_4FBE_947C_F3684AD3C25F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <ddraw.h>
//#include "Image.h"	// Added by ClassView
//#include "Image.h"	// Added by ClassView
class CDx7Draw;
class CMainChar;
class CImage;
class CTextData;
class CAnimation;

class CEvent  
{

protected:
	LPDIRECTDRAWSURFACE7	m_pSurface;		// DxDraw Suface
	CDx7Draw*	m_pDxDraw;					// DxDarw Pointer

	CTextData*	m_pText;			// Text출력을 위한 텍스트 클래스 객체 
	CImage*		m_pImage;			// 출력할 일러스트 이미지 
	CImage		m_CharImg;			// 출력할 대사창 이미지 
	CAnimation	m_NextImg;			// 화살표 이미지 

	char*	m_strTextFile[9][2];		// 이벤트별 텍스트 파일명
	char*	m_strImgFile[9];			// 이벤트별 일러스트 이미지 파일명 


	RECT	m_rtClient;				// 스크린 영역

	int		m_nEventtMode;			// 현재 수행해야할 함수의 일련 번호 

	int		m_nFade;				// Fade In/Out 단계 

	int		m_nTotalIndex;			// 현재 프래임의 텍스트 총수 
	int		m_nTextIndex;			// 출력될 텍스트의 인덱스 

	int		m_nTotalFrame;			// 현재 이벤트의 총 프레임수 
	int		m_nFrame;				// 출력할 일러스트 프레임 

	int		m_nEventNum;			// 현재 이벤트 번호 
	bool	m_aEvtComplete[9];		// 각 이벤트의 완수 여부 

	bool	m_bNext;				// 다음 텍스트로 넘어갔는지 여부 
	int		m_nBtnFrame;
	char	m_Text[256];			// 출력될 텍스트 버퍼 
	DWORD m_dwTime;					// 화살표를 깜빡이게할 타이머 


public:
	void DrawButton();
	CImage m_WinImg;
	void DrawDialogWin();

	bool EventLoop(int nEvtNum);	// 실제 이벤트 루프	
	void InitEventInfo();			// 이벤트 정보 초기화 함수 
	
	void NextFrame();				// 다음 프레임으로 넘어가는 함수 
	void TextPass();				// 다음 텍스트로 넘어가는 함수 

	void DrawIllust();				// 일러스트 출력함수 
	void DrawTextData();			// 텍스트 출력함수 
	void FadeEffect(bool bFade);	// FadeEffect함수 
	void ScreenCopy();				// 게임화면을 카피해두는 함수 

	bool IsEventComplete(int nEvtNum) { return m_aEvtComplete[nEvtNum]; }	// 해당 이벤트가 완수 되었는지 여부를 얻는 함수 

	void SetDxDraw(CDx7Draw* pDxDraw) { m_pDxDraw = pDxDraw; }			// DxDraw 포인터 세팅 
	void SetTextData(CTextData* pText) { m_pText = pText; }				// 텍스트 객체 포인터 세팅
	void SetEventInfo(CDx7Draw *pDxDraw, CTextData *pText)			// 외부변수들 얻어오는 함수  
	{	m_pDxDraw = pDxDraw;  m_pText = pText;}

	CEvent();
	virtual ~CEvent();
};

#endif // !defined(AFX_EVENT_H__4CC26604_5B7C_4FBE_947C_F3684AD3C25F__INCLUDED_)
