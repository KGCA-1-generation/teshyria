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

	CTextData*	m_pText;			// Text����� ���� �ؽ�Ʈ Ŭ���� ��ü 
	CImage*		m_pImage;			// ����� �Ϸ���Ʈ �̹��� 
	CImage		m_CharImg;			// ����� ���â �̹��� 
	CAnimation	m_NextImg;			// ȭ��ǥ �̹��� 

	char*	m_strTextFile[9][2];		// �̺�Ʈ�� �ؽ�Ʈ ���ϸ�
	char*	m_strImgFile[9];			// �̺�Ʈ�� �Ϸ���Ʈ �̹��� ���ϸ� 


	RECT	m_rtClient;				// ��ũ�� ����

	int		m_nEventtMode;			// ���� �����ؾ��� �Լ��� �Ϸ� ��ȣ 

	int		m_nFade;				// Fade In/Out �ܰ� 

	int		m_nTotalIndex;			// ���� �������� �ؽ�Ʈ �Ѽ� 
	int		m_nTextIndex;			// ��µ� �ؽ�Ʈ�� �ε��� 

	int		m_nTotalFrame;			// ���� �̺�Ʈ�� �� �����Ӽ� 
	int		m_nFrame;				// ����� �Ϸ���Ʈ ������ 

	int		m_nEventNum;			// ���� �̺�Ʈ ��ȣ 
	bool	m_aEvtComplete[9];		// �� �̺�Ʈ�� �ϼ� ���� 

	bool	m_bNext;				// ���� �ؽ�Ʈ�� �Ѿ���� ���� 
	int		m_nBtnFrame;
	char	m_Text[256];			// ��µ� �ؽ�Ʈ ���� 
	DWORD m_dwTime;					// ȭ��ǥ�� �����̰��� Ÿ�̸� 


public:
	void DrawButton();
	CImage m_WinImg;
	void DrawDialogWin();

	bool EventLoop(int nEvtNum);	// ���� �̺�Ʈ ����	
	void InitEventInfo();			// �̺�Ʈ ���� �ʱ�ȭ �Լ� 
	
	void NextFrame();				// ���� ���������� �Ѿ�� �Լ� 
	void TextPass();				// ���� �ؽ�Ʈ�� �Ѿ�� �Լ� 

	void DrawIllust();				// �Ϸ���Ʈ ����Լ� 
	void DrawTextData();			// �ؽ�Ʈ ����Լ� 
	void FadeEffect(bool bFade);	// FadeEffect�Լ� 
	void ScreenCopy();				// ����ȭ���� ī���صδ� �Լ� 

	bool IsEventComplete(int nEvtNum) { return m_aEvtComplete[nEvtNum]; }	// �ش� �̺�Ʈ�� �ϼ� �Ǿ����� ���θ� ��� �Լ� 

	void SetDxDraw(CDx7Draw* pDxDraw) { m_pDxDraw = pDxDraw; }			// DxDraw ������ ���� 
	void SetTextData(CTextData* pText) { m_pText = pText; }				// �ؽ�Ʈ ��ü ������ ����
	void SetEventInfo(CDx7Draw *pDxDraw, CTextData *pText)			// �ܺκ����� ������ �Լ�  
	{	m_pDxDraw = pDxDraw;  m_pText = pText;}

	CEvent();
	virtual ~CEvent();
};

#endif // !defined(AFX_EVENT_H__4CC26604_5B7C_4FBE_947C_F3684AD3C25F__INCLUDED_)
