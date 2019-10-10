// Dx7Draw.h: interface for the CDx7Draw class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DX7DRAW_H__957E9055_7234_4D88_B70A_77779968F6C0__INCLUDED_)
#define AFX_DX7DRAW_H__957E9055_7234_4D88_B70A_77779968F6C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

#define FC _fastcall

class CDx7Draw  
{
protected:	
	LPDIRECTDRAW7        m_lpDD;     
	LPDIRECTDRAWSURFACE7 m_lpDDPrim;     // Primary Surface
	LPDIRECTDRAWSURFACE7 m_lpDDBack;     // Back    Surface
	LPDIRECTDRAWSURFACE7 m_lpDDOff;      // OffScreen Surface
	LPDIRECTDRAWSURFACE7 m_lpDDOff2;     // OffScreen BackBuffer
	HWND                 m_hWnd;
	RECT                 m_rtClient;
	BOOL                 m_b565mode;     // 565모드이냐 555모드이냐
	int                  m_nRealWidth;   // lPitch/2 (16bit 이므로)
	int                  m_nHeight;      // 해상도 세로 폭
	int                  m_nWidth;       // 해상도 가로 폭
	int                  m_nScreenMode;  // 0:윈도창 모드,  1:풀모드

	// protected 함수들

public:
	LPDIRECTDRAW7        GetDDrawPtr()  { return m_lpDD; }
	LPDIRECTDRAWSURFACE7 GetPSurface()  { return m_lpDDPrim; }
	LPDIRECTDRAWSURFACE7 GetOSurface()  { return m_lpDDOff; }
	LPDIRECTDRAWSURFACE7 GetO2Surface() { return m_lpDDOff2; }
	LPDIRECTDRAWSURFACE7 GetBSurface();
	int                  GetRealWidth() { return m_nRealWidth; }
	WORD*                Lock(void);
	WORD*                LockOff(void);
	WORD*                LockOff2(void);
	HRESULT              Unlock(void);
	HRESULT              UnlockOff(void);
	HRESULT              UnlockOff2(void);
	HRESULT              Init(HWND hwnd, int x, int y, int color, int mode);
	void                 FadeEffect(int AlphaDepth, WORD color);
	void                 SetClientRect(RECT* rt);
	void                 ClearSurface(LPDIRECTDRAWSURFACE7 Surface, DWORD dwColor);
	void                 CreateOffSurface();
	void                 OffToBack();
	void                 BackToOff2();
	void                 Off2ToBack();
	void FC              Flip();
	void                 ReleaseAll();
	BOOL                 Is565Mode();

	CDx7Draw();
	virtual ~CDx7Draw();


};

inline
void CDx7Draw::SetClientRect(RECT *rt)
{
	SetRect(&m_rtClient, rt->left, rt->top, rt->right, rt->bottom);
}

inline
HRESULT CDx7Draw::Unlock()
{
	//if(m_nScreenMode) return m_lpDDOff->Unlock(NULL);
	return m_lpDDBack->Unlock(NULL);
}

inline
HRESULT CDx7Draw::UnlockOff()
{
	//if(m_nScreenMode) return m_lpDDOff->Unlock(NULL);
	return m_lpDDOff->Unlock(NULL);
}

inline
HRESULT CDx7Draw::UnlockOff2()
{
	//if(m_nScreenMode) return m_lpDDOff->Unlock(NULL);
	return m_lpDDOff2->Unlock(NULL);
}

inline
LPDIRECTDRAWSURFACE7 CDx7Draw::GetBSurface()
{
	//if(m_nScreenMode) return m_lpDDOff;
	return m_lpDDBack;
}

#undef FC
#endif // !defined(AFX_DX7DRAW_H__957E9055_7234_4D88_B70A_77779968F6C0__INCLUDED_)
