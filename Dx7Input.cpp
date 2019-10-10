// Dx7Input.cpp: implementation of the CDx7Input class.
//
//////////////////////////////////////////////////////////////////////
#include <dinput.h>
#include "Dx7Input.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDx7Input::CDx7Input()
{
	m_lpDI         = NULL;
	m_lpdiKeyBoard = NULL;
	m_lpdiMouse    = NULL;
	m_bMouse       = false; 
	m_bKeyboard    = false;
}

CDx7Input::CDx7Input(bool bMouse, bool bKeyboard)
{
	m_lpDI         = NULL;
	m_lpdiKeyBoard = NULL;
	m_lpdiMouse    = NULL;
	m_bMouse       = bMouse; 
	m_bKeyboard    = bKeyboard;
}

CDx7Input::~CDx7Input()
{
	ReleaseAll();
}

HRESULT CDx7Input::Init(HANDLE hInst, HWND hwnd)
{
	HRESULT hr;

	if (FAILED(hr=DirectInputCreate((HINSTANCE)hInst, DIRECTINPUT_VERSION, 
									&m_lpDI, NULL)))
	{
		return hr;
	}

	if(m_bMouse)   // 마우스 사용시
	{
		if (FAILED(hr=m_lpDI->CreateDevice(GUID_SysMouse, &m_lpdiMouse, NULL)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiMouse->SetDataFormat(&c_dfDIMouse)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiMouse->SetCooperativeLevel(hwnd, 
			                       DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiMouse->Acquire()))
		{
			return hr;
		}
	}

	if(m_bKeyboard) // 키보드 사용시
	{
		if (FAILED(hr=m_lpDI->CreateDevice(GUID_SysKeyboard, &m_lpdiKeyBoard, NULL)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiKeyBoard->SetDataFormat(&c_dfDIKeyboard)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiKeyBoard->SetCooperativeLevel(hwnd, 
			                          DISCL_NONEXCLUSIVE | DISCL_BACKGROUND)))
		{
			return hr;
		}
		if (FAILED(hr=m_lpdiKeyBoard->Acquire()))
		{
			return hr;
		}
	}

	return DI_OK;
}

void CDx7Input::ReleaseAll()
{
	if( m_lpdiMouse != NULL )
	{
		m_lpdiMouse->Unacquire();
		m_lpdiMouse->Release();
		m_lpdiMouse = NULL ;
	}
//	SAFE_RELEASEDEVICE(m_lpdiMouse)
//	SAFE_RELEASEOBJECT(m_lpdiMouse)

	if( m_lpdiKeyBoard != NULL )
	{
		m_lpdiKeyBoard->Unacquire();
		m_lpdiKeyBoard->Release();
		m_lpdiKeyBoard = NULL ;
	}

	if( m_lpDI != NULL)
	{
		m_lpDI->Release();
		m_lpDI = NULL ;
	}
/**/
//	SAFE_RELEASEDEVICE(m_lpdiKeyBoard)
//	SAFE_RELEASEOBJECT(m_lpdiKeyBoard)

//	SAFE_RELEASEOBJECT(m_lpDI)
}

