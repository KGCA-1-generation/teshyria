// Event.cpp: implementation of the CEvent class.
//
//////////////////////////////////////////////////////////////////////
#include <ddraw.h>
#include <windows.h>
//#include <stdlib.h>
//#include <time.h>
//#include <mmsystem.h>

#include "Dx7Draw.h"
#include "Image.h"
#include "Animation.h"
#include "Event.h"
#include "MainChar.h"
#include "TextData.h"
#include "Function.h"

#define		SCREEN_COPY		0
#define		FADE_IN			1
#define		FADE_OUT		2
#define		NEXT_FRAME		3
#define		TEXT_PASS		4
#define		DRAW_TEXT		5
#define		EXIT_EVENT		6

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

CEvent::CEvent()
{
	SetRect(&m_rtClient, 0, 0, 800, 600);

	m_pImage = NULL;
	memset(m_aEvtComplete, 0, 9);

	m_strTextFile[0][0] = "Data\\Text\\EventText1-1.TDT";
	m_strTextFile[0][1] = "Data\\Text\\EventText1-2.TDT";
	m_strTextFile[1][0] = "Data\\Text\\EventText2-1.TDT";
	m_strTextFile[1][1] = "";
	m_strTextFile[2][0] = "Data\\Text\\EventText3-1.TDT";
	m_strTextFile[2][1] = "";
	m_strTextFile[3][0] = "Data\\Text\\EventText4-1.TDT";
	m_strTextFile[3][1] = "Data\\Text\\EventText4-2.TDT";
	m_strTextFile[4][0] = "Data\\Text\\EventText5-1.TDT";
	m_strTextFile[4][1] = "";
	m_strTextFile[5][0] = "Data\\Text\\EventText6-1.TDT";
	m_strTextFile[5][1] = "";
	m_strTextFile[6][0] = "Data\\Text\\EventText7-1.TDT";
	m_strTextFile[6][1] = "";
	m_strTextFile[7][0] = "Data\\Text\\EventText8-1.TDT";
	m_strTextFile[7][1] = "";
	m_strTextFile[8][0] = "Data\\Text\\EventText9-1.TDT";
	m_strTextFile[8][1] = "";


	m_strImgFile[0] = "Data\\Event\\Event1.SPR";
	m_strImgFile[1] = "Data\\Event\\Event2.SPR";
	m_strImgFile[2] = "Data\\Event\\Event3.SPR";
	m_strImgFile[3] = "Data\\Event\\Event4.SPR";
	m_strImgFile[4] = "Data\\Event\\Event5.SPR";
	m_strImgFile[5] = "Data\\Event\\Event6.SPR";
	m_strImgFile[6] = "Data\\Event\\Event7.SPR";
	m_strImgFile[7] = "Data\\Event\\Event8.SPR";
	m_strImgFile[8] = "Data\\Event\\Event9.SPR";
	m_CharImg.LoadSpr("Data\\Event\\Event_Char.SPR");
	m_WinImg.LoadImg ("Data\\Event\\Event_Win.Img");
	m_NextImg.LoadAni("Data\\WinButton.Ani");
	InitEventInfo();
}

CEvent::~CEvent()
{
	SAFE_DELETE(m_pImage);
}

void CEvent::InitEventInfo()
{
	m_nEventtMode = SCREEN_COPY;
	m_nFade = 0;
	m_nFrame = -1;
	m_nTextIndex = -1;
	m_nTotalIndex = 100;
	m_nTotalFrame = 0;
	m_nBtnFrame = 0;
	m_bNext = false;
	m_dwTime = timeGetTime();
}


bool CEvent::EventLoop(int nEvtNum)
{
	m_nEventNum = nEvtNum;

	switch(m_nEventtMode)
	{
	case SCREEN_COPY:	ScreenCopy();		break;
	case FADE_OUT:		FadeEffect(true);	break;
	case FADE_IN:		FadeEffect(false);	break;
	case TEXT_PASS:		TextPass();			break;
	case DRAW_TEXT:		DrawTextData();		break;
	case NEXT_FRAME:	NextFrame();		break;
	case EXIT_EVENT:	return false;		break;
	}

	return true;
}

void CEvent::TextPass()
{
	if( m_nTextIndex < m_nTotalIndex-1 )
	{
		m_nTextIndex++; 
		m_pText->ReadTextData(m_strTextFile[m_nEventNum][m_nFrame], m_Text, m_nTextIndex);
		m_nTotalIndex = m_pText->GetTotalIndex();
	
		if(m_pText->m_loopstop == FALSE)
			m_pText->m_loopstop = TRUE;
		else m_pText->m_loopstop = FALSE;

		m_bNext = true;
		m_nEventtMode = DRAW_TEXT;
	}
	else m_nEventtMode = FADE_OUT;
}
 

void CEvent::NextFrame()
{
	m_nFrame++;
	m_nTextIndex = -1;
	m_nEventtMode = FADE_IN;
	
	if( m_nFrame == m_nTotalFrame  ) m_nEventtMode = EXIT_EVENT;
}
void CEvent::DrawDialogWin()
{
	WORD* pScreen = m_pDxDraw->LockOff();
	WORD* pImg = m_WinImg.GetData();
	int	nw=m_WinImg.GetWidth(0);
	int sx = 47, sy = 43, i, j;
	if( m_pText->GetAttr() == 1 ) sx = 212;
	
	DWORD Scr, Back, Win;

	int t = sy*800;
	for( i=0; i<m_WinImg.GetHeight(0); i++ ) {
		for( j=0; j<nw; j++ )
		{
			if( (*(pImg+i*nw+j)) != Convert16 (RGB(255, 0, 255)) )
			{
				Scr = *(DWORD*)(pScreen+t+j+sx);
				Win = *(DWORD*)(pImg+i*nw+j);
				Back = ((Scr & 0xf7def7de) + (Win & 0xf7def7de) )>>1; //<<16dHALF_MASK)>>1)|(ScrdHALF_MASK>>1); //+ (Back & dHALF_MASK_4BYTE)>>1;
				*(DWORD*)(pScreen+t+j+sx) = Back;
			}
		}
		t += 800;
	}
	m_pDxDraw->UnlockOff();
}


void CEvent::DrawIllust()
{
	WORD* pScreen = m_pDxDraw->LockOff();
	m_pImage->DrawSpr(0, 0, m_nFrame, pScreen);
	m_CharImg.DrawSpr(39, 35, m_pText->GetAttr()-1, pScreen);
	m_pDxDraw->UnlockOff();
	DrawDialogWin();
	DrawButton();
}

void CEvent::DrawButton()
{
	WORD* pScreen = m_pDxDraw->LockOff();
	
	int nNextW = 550;
	if( m_pText->GetAttr() == 1 )	nNextW = 700;
	m_NextImg.DrawAni(nNextW, 150, 0, m_nBtnFrame, pScreen);

	DWORD time = timeGetTime() - m_dwTime;
	if( time > 1000/30 )	{
		m_nBtnFrame++; 
		if( m_nBtnFrame==m_NextImg.GetMotFrmCount(0) ) m_nBtnFrame = 0; 
		m_dwTime = timeGetTime();		
	}

	m_pDxDraw->UnlockOff();
}

void CEvent::ScreenCopy()
{
	m_pDxDraw->CreateOffSurface();
	m_pDxDraw->OffToBack();
	m_pDxDraw->BackToOff2();
	
	SAFE_DELETE(m_pImage);

	m_pImage = new CImage;
	m_pImage->LoadSpr(m_strImgFile[m_nEventNum]);
	m_nTotalFrame = m_pImage->GetTotalFrame();
	m_nEventtMode = FADE_OUT;
}

void CEvent::DrawTextData()
{
		HDC   hdc;
		DrawIllust();
		m_pSurface = m_pDxDraw->GetOSurface();
		m_pSurface->GetDC(&hdc);
		int nTextW = 70;
		if( m_pText->GetAttr() == 1 )	nTextW = 230;

	//	m_pText->TypeWritting(hdc, nTextW, 70, m_Text, RGB(0, 0, 0), 14, "µ¸¿òÃ¼", true);
	//	m_pText->LineSet(hdc, nTextW+1, 64, m_Text, RGB(100, 100, 100), 15, "µ¸¿òÃ¼", true);

		m_pText->LineSet(hdc, nTextW, 63, m_Text, RGB(0, 0, 0), 15, "µ¸¿òÃ¼", true);
		
		m_pSurface->ReleaseDC(hdc);
		m_pDxDraw->Flip();
}

void CEvent::FadeEffect(bool bFade)
{
	if( bFade )
	{
		if ( m_nFade == 100 ) { m_nEventtMode = NEXT_FRAME; return; }
		else m_nFade++;
	}
	else 
	{
		if( m_nFade == 0 )  { m_nEventtMode = TEXT_PASS; return; }
		else m_nFade--;
	}

	if( m_nFrame >= 0 && m_nFrame < m_nTotalFrame )
	{
		m_pSurface = m_pDxDraw->GetOSurface();
		m_pDxDraw->ClearSurface(m_pSurface, RGB(0, 0, 0));
		WORD* pScreen = m_pDxDraw->LockOff();
		m_pImage->FadeSpr(0, 0, m_nFrame, pScreen, m_nFade);
		m_pDxDraw->UnlockOff();
		m_pDxDraw->Flip();
	}
	else 
	{
		if( m_nFade > 30 ) 
		{
			m_pDxDraw->Off2ToBack();
			m_pDxDraw->GetPSurface()->Flip(0, 0);
			m_nFade = 100;
		}
		else m_pDxDraw->FadeEffect(m_nFade++, 0);
	}
}






