// Dx7Show.h: interface for the CDM6Show class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DX7SHOW_H__24422D41_E277_11D5_944A_0050DA898DDD__INCLUDED_)
#define AFX_DX7SHOW_H__24422D41_E277_11D5_944A_0050DA898DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <mmstream.h>
#include <amstream.h>
#include <ddstream.h>

class CDM6Show  
{
	IMultiMediaStream		*m_pMMStream;              // 스트림관련
	IMediaStream			*m_pPrimaryVidStream;     
	IDirectDrawMediaStream	*m_pDDStream;     
	IDirectDrawSurface7		*m_pSurface;     
	IDirectDrawStreamSample *m_pSample;
	IDirectDrawSurface7     *m_pMovieSurface;

	char FileName[MAX_PATH];
	RECT DRect, SRect;  
	// SRect : Clip Size(Prepare에서 세팅), DRect : 찍을 위치 및 크기

	bool Open(IDirectDraw7* pDD, char* name, IMultiMediaStream** ptMMStream);

public :
	CDM6Show();
	virtual ~CDM6Show();

	void SetBlitPos(int t,int b,int l,int r); 
	void SetBlitPos(RECT dest); 
	void ShutDown();
	bool PrepareMovie(IDirectDraw7* pDD, IDirectDrawSurface7 *pDDSurface, char* FName);
	bool RenderStreamToSurface();
};

#endif // !defined(AFX_DX7SHOW_H__24422D41_E277_11D5_944A_0050DA898DDD__INCLUDED_)
