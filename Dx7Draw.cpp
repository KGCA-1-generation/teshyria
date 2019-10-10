// Dx7Draw.cpp: implementation of the CDx7Draw class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>
#include <ddraw.h>
#include "Dx7Draw.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDx7Draw::CDx7Draw()
{
	m_lpDD     = NULL;    
	m_lpDDPrim = NULL;
	m_lpDDBack = NULL;
	m_lpDDOff  = NULL;
	m_lpDDOff2 = NULL;
}
CDx7Draw::~CDx7Draw()
{
	ReleaseAll();
}

//----------------------------------------------------------------------
// DirectX 7.0 Init
//----------------------------------------------------------------------
HRESULT CDx7Draw::Init(HWND hwnd, int x, int y, int color, int mode)
{
	HRESULT hr;
	DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

	m_nWidth      = x;
	m_nHeight     = y;
	m_nScreenMode = mode;
	m_hWnd        = hwnd;
 
	if (FAILED(hr=DirectDrawCreateEx(NULL, (VOID**)&m_lpDD, IID_IDirectDraw7, NULL)))
	{
		return hr;
	}

	if (m_nScreenMode==1)      // 풀스크린 모드일때
	{  
		dwStyle &= WS_OVERLAPPEDWINDOW;
		dwStyle |= WS_POPUP ;
		SetWindowLong(hwnd, GWL_STYLE, dwStyle);

		if (FAILED(hr=m_lpDD->SetCooperativeLevel(hwnd, DDSCL_EXCLUSIVE | DDSCL_FULLSCREEN | DDSCL_ALLOWREBOOT)))
		{
			return hr;
		}

		if (FAILED(hr=m_lpDD->SetDisplayMode(x, y, color, 0, 0)))
		{
			return hr;
		}
		
		DDSURFACEDESC2 ddsd;
		// Create the primary surface with 1 back buffer
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS | DDSD_BACKBUFFERCOUNT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE | DDSCAPS_FLIP |
							  DDSCAPS_COMPLEX | 
							  DDSCAPS_VIDEOMEMORY;
							  //DDSCAPS_SYSTEMMEMORY; 
		ddsd.dwBackBufferCount = 1;

		if (FAILED(hr=m_lpDD->CreateSurface(&ddsd, &m_lpDDPrim, NULL)))
		{
			return hr;
		}
		
		DDSCAPS2 ddscaps;
		// Get a pointer to the back buffer
		ZeroMemory(&ddscaps, sizeof(ddscaps));
		ddscaps.dwCaps = DDSCAPS_BACKBUFFER;
		if (FAILED(hr=m_lpDDPrim->GetAttachedSurface(&ddscaps, &m_lpDDBack)))
		{
			return hr;
		}
		m_b565mode = Is565Mode();
		SetRect(&m_rtClient, 0, 0, x, y);

		// OffScreen Surface 생성
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags =  DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH ;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN ; //| DDSCAPS_SYSTEMMEMORY;
		ddsd.dwWidth  = x;
		ddsd.dwHeight = y;
		if (FAILED(hr=m_lpDD->CreateSurface(&ddsd, &m_lpDDOff, NULL)))
		{
			MessageBox(m_hWnd, "OffScreen Surface Create Failed!!!", "Error", MB_OK);
			return hr;
		}
		DDCOLORKEY ddck;
		ddck.dwColorSpaceLowValue = 0x00ff;
		ddck.dwColorSpaceHighValue = 0x00ff;
		m_lpDDOff->SetColorKey(DDCKEY_SRCBLT, &ddck);
		ClearSurface(m_lpDDOff, RGB(255, 0, 255));

	}
	else            // 윈도창 모드일때 
	{
		dwStyle &= ~WS_POPUP;
		dwStyle |= WS_OVERLAPPED | WS_MINIMIZEBOX | WS_CAPTION 
			       | WS_THICKFRAME | WS_SYSMENU;
		SetWindowLong(hwnd, GWL_STYLE, dwStyle);

		RECT rt, rtClient;
		SetRect(&rtClient, 0, 0, x, y);
		AdjustWindowRectEx( &rtClient, 
			                GetWindowLong(hwnd, GWL_STYLE),
						    NULL,
						    GetWindowLong(hwnd, GWL_EXSTYLE) );

		SetWindowPos(hwnd, NULL, 0, 0, 
					 rtClient.right-rtClient.left, 
					 rtClient.bottom-rtClient.top,
					 SWP_NOMOVE | SWP_SHOWWINDOW);
		
		GetClientRect(hwnd, &rt);

		ClientToScreen(hwnd, (POINT*)&rt.left);
		ClientToScreen(hwnd, (POINT*)&rt.right);

		// 창모드에 관한 협력 수준 설정
		if (FAILED(hr=m_lpDD->SetCooperativeLevel(hwnd, DDSCL_NORMAL) ))
		{
			MessageBox(hwnd, "협력 수준 설정 실패", "DirectDraw", MB_OK);
			return hr;
		}

		DDSURFACEDESC2 ddsd;
		// Primary Purface 생성
		ZeroMemory(&ddsd, sizeof(ddsd));
		ddsd.dwSize = sizeof(ddsd);
		ddsd.dwFlags = DDSD_CAPS;
		ddsd.ddsCaps.dwCaps = DDSCAPS_PRIMARYSURFACE;
		if (FAILED(hr=m_lpDD->CreateSurface(&ddsd, &m_lpDDPrim, NULL)))
		{
			MessageBox(hwnd, "PrimarySurface생성 실패", "DirectDraw", MB_OK);
			return hr;
		}

		// Back Buffer 생성
		ddsd.dwFlags = DDSD_CAPS | DDSD_WIDTH | DDSD_HEIGHT;
		ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN;
		ddsd.dwWidth  = x;
		ddsd.dwHeight = y;
		if (FAILED(hr=m_lpDD->CreateSurface(&ddsd, &m_lpDDBack, NULL)))
		{
			MessageBox(hwnd, "BackBuffer생성 실패", "DirectDraw", MB_OK);
			return hr;
		}

		// 클리퍼 생성
		LPDIRECTDRAWCLIPPER lpClipper;
		if (FAILED(hr=m_lpDD->CreateClipper(0, &lpClipper, NULL)))
		{
			MessageBox(hwnd, "클리핑 실패", "DirectDraw", MB_OK);
			return hr;
		}
		lpClipper->SetHWnd(0, hwnd);
		m_lpDDPrim->SetClipper(lpClipper);
		m_lpDDBack->SetClipper(lpClipper);
		lpClipper->Release();
	}
	ClearSurface(m_lpDDBack, RGB(0, 0, 0));
	return DD_OK;
}

void CDx7Draw::ReleaseAll()
{
	SAFE_RELEASE(m_lpDDPrim)
	SAFE_RELEASE(m_lpDDOff)
	SAFE_RELEASE(m_lpDDOff2)
	SAFE_RELEASE(m_lpDD)
}

WORD* CDx7Draw::Lock(void)
{
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof(ddsd);
/*
	if(m_nScreenMode)
	{
		hr = m_lpDDOff->Lock(NULL, &ddsd, 
							 DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
							 NULL);
	}
	else
	{
*/
		hr = m_lpDDBack->Lock(NULL, &ddsd, 
							  DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
							  NULL);
		if ( hr != DD_OK )
		{
			m_lpDDPrim->Restore();
			m_lpDDBack->Restore();

			m_lpDDBack->Lock(NULL, &ddsd, 
							 DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
							 NULL);
		}
//	}
    return (WORD *)ddsd.lpSurface;
}

void CDx7Draw::Flip()
{
	if(m_nScreenMode) {  // 풀스크린일때
		HRESULT hr;
		//m_lpDDBack->Blt(NULL, m_lpDDOff, NULL, DDBLT_WAIT, NULL);
		OffToBack();
		hr = m_lpDDPrim->Flip( NULL, 0 ); 
		if( hr == DDERR_SURFACELOST ) {
			m_lpDDPrim->Restore();
			m_lpDDBack->Restore();
		} 
	} 
	else {               // 윈도 창 모드일때
		m_lpDDPrim->Blt(&m_rtClient, m_lpDDBack, NULL, DDBLT_WAIT, NULL);
	}
}

void CDx7Draw::ClearSurface(LPDIRECTDRAWSURFACE7 Surface, DWORD dwColor)
{
	DDBLTFX ddbltfx;
	ddbltfx.dwSize = sizeof(ddbltfx);
	ddbltfx.dwFillColor = dwColor;

	Surface->Blt(NULL, NULL, NULL, DDBLT_COLORFILL | DDBLT_WAIT, &ddbltfx);
}

void CDx7Draw::FadeEffect(int AlphaDepth, WORD color)
{
	//DWORD* Scr = (DWORD *)LockOff2();
	DWORD* Scr = (DWORD *)Lock();

	DWORD high, low;
	DWORD dwColor = ((color<<16) | color);
	int   destDepth = 32 - AlphaDepth;

	high   = dwColor & 0xf81f07e0;
	high >>= 5;
	high  *= destDepth;
	high  &= 0xf81f07e0;

	low    = dwColor & 0x07e0f81f;
	low   *= destDepth;
	low  >>= 5;	
	low   &= 0x07e0f81f;

	dwColor = high | low;

	for(int i=0; i<(800*m_nHeight)>>1; i++)
	{
		high   = Scr[i] & 0xf81f07e0;
		high >>= 5;
		high  *= destDepth;
		high  &= 0xf81f07e0;

		low    = Scr[i] & 0x07e0f81f;
		low   *= destDepth;
		low  >>= 5;		
		low   &= 0x07e0f81f;

		Scr[i] = ( high | low ) + dwColor;
	}
/*
	UnlockOff2();
	m_lpDDPrim->BltFast(0, 0, m_lpDDOff2, &m_rtClient, 
           		        DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
*/
	Unlock();
	m_lpDDPrim->Flip( NULL, 0 );
	//Flip();
}

BOOL CDx7Draw::Is565Mode()
{
	DDSURFACEDESC2 ddsd;
	HDC hdc;

	m_lpDDBack->GetDC(&hdc);
	SetPixel(hdc, 0, 0, RGB(255, 255, 255));
	m_lpDDBack->ReleaseDC(hdc);
	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	m_lpDDBack->Lock(NULL, &ddsd, 
		             DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
					 NULL);
	m_nRealWidth = ddsd.lPitch >> 1;
	WORD *pScreen = (WORD *)ddsd.lpSurface;
	m_lpDDBack->Unlock(NULL);

	if(pScreen[0] == 0xFFFF) return TRUE; // 565모드일시 TRUE
	return FALSE;                         // 555모드일시 FALSE
}


WORD* CDx7Draw::LockOff(void)
{
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof(ddsd);

	hr = m_lpDDOff->Lock(NULL, &ddsd, 
						 DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
						 NULL);
    return (WORD *)ddsd.lpSurface;
}

WORD* CDx7Draw::LockOff2(void)
{
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	memset( &ddsd, 0, sizeof(ddsd) );
    ddsd.dwSize = sizeof(ddsd);

	hr = m_lpDDOff2->Lock(NULL, &ddsd, 
						 DDLOCK_WAIT | DDLOCK_SURFACEMEMORYPTR, 
						 NULL);
    return (WORD *)ddsd.lpSurface;
}

void CDx7Draw::CreateOffSurface()
{
	// OffScreen Surface 생성
	DDSURFACEDESC2 ddsd;
	HRESULT        hr;

	ZeroMemory(&ddsd, sizeof(ddsd));
	ddsd.dwSize = sizeof(ddsd);
	ddsd.dwFlags =  DDSD_CAPS | DDSD_HEIGHT | DDSD_WIDTH ;
	ddsd.ddsCaps.dwCaps = DDSCAPS_OFFSCREENPLAIN ; //| DDSCAPS_SYSTEMMEMORY;
	ddsd.dwWidth  = m_nWidth;
	ddsd.dwHeight = m_nHeight;
	SAFE_RELEASE(m_lpDDOff2)
	if (FAILED(hr=m_lpDD->CreateSurface(&ddsd, &m_lpDDOff2, NULL)))
	{
		MessageBox(m_hWnd, "OffScreen2 Surface Create Failed!!!", "Error", MB_OK);
	}
}

void CDx7Draw::OffToBack()
{
	m_lpDDBack->BltFast(0, 0, m_lpDDOff, &m_rtClient, 
		                DDBLTFAST_WAIT | DDBLTFAST_SRCCOLORKEY );
}

void CDx7Draw::BackToOff2()
{
	m_lpDDOff2->BltFast(0, 0, m_lpDDBack, &m_rtClient, DDBLTFAST_WAIT);
}

void CDx7Draw::Off2ToBack()
{
	m_lpDDBack->BltFast(0, 0, m_lpDDOff2, &m_rtClient, DDBLTFAST_WAIT);
	//SAFE_RELEASE(m_lpDDOff2)
}