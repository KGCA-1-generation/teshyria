// Dx7Input.h: interface for the CDx7Input class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DX7INPUT_H__9F71EAA9_A119_4D2A_B8FF_0B803BFDF8CD__INCLUDED_)
#define AFX_DX7INPUT_H__9F71EAA9_A119_4D2A_B8FF_0B803BFDF8CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAFE_RELEASEOBJECT(p) { if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_RELEASEDEVICE(p) { if(p) { (p)->Unacquire(); (p)->Release(); (p)=NULL; } }

class CDx7Input
{
public:
	LPDIRECTINPUTDEVICE GetMousePtr() { return m_lpdiMouse; }
	LPDIRECTINPUTDEVICE GetKeyBDPtr() { return m_lpdiKeyBoard; }
	void ReleaseAll();
	HRESULT Init(HANDLE hInst, HWND hwnd);

	CDx7Input();
	CDx7Input(bool bMouse, bool bKeyboard); 
	virtual ~CDx7Input();

protected:
	LPDIRECTINPUT		m_lpDI;
	LPDIRECTINPUTDEVICE	m_lpdiKeyBoard;		// 키보드 
	LPDIRECTINPUTDEVICE	m_lpdiMouse;		// 마우스
	bool m_bKeyboard;
	bool m_bMouse;
};

#endif // !defined(AFX_DX7INPUT_H__9F71EAA9_A119_4D2A_B8FF_0B803BFDF8CD__INCLUDED_)