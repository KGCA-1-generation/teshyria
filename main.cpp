//----------------------------------------------------------------------
// Title: ��������.
//
// Desc: ������ �����ε� ��¼�� ��¼��... 
// Date: 2001�� 11�� 27�� ����
//
// �� �ѹ� ����� �ô�.
//----------------------------------------------------------------------
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#define NODEFAULTLIB

//----------------------------------------------------------------------
// Include files
//----------------------------------------------------------------------
#include <windows.h>
#include <mmsystem.h>
#include <dinput.h>
#include <ddraw.h>
#include <dsound.h>
#include <stdlib.h>
#include <time.h>

#include "main.h"
#include "Resource.h"
#include "function.h"
#include "NeedStruct.h"
#include "Dx7Draw.h"
#include "Dx7Input.h"
#include "Dx7Sound.h"
#include "Image.h"
#include "Animation.h"
#include "TextData.h"
#include "CharMng.h"
#include "InterfaceMng.h"  // �������̽� �߰�
#include "MapMng.h"
#include "Event.h"

// �ӽ� ��Ŭ���. ���߿� ����°� ����
#include "MainChar.h"
#include "MstKnight.h"
#include "ScoutMage.h"
//#include "ScoutKnight.h"
#include "RangerMage.h"
#include "Animal.h"

//----------------------------------------------------------------------
// Local definitions
//----------------------------------------------------------------------
#define NAME   "Thesyria"
#define TITLE  "Thesyria"

//----------------------------------------------------------------------
// Global Variables
//----------------------------------------------------------------------
HWND       hMainWnd;
HINSTANCE  hInst;
int        nActiveApp = false; // BackGround���� ForeGround����

CDx7Draw   DxDraw;
CDx7Input  DxInput(MOUSE_USE, KEYBOARD_USE); // ���콺 �� Ű���� ��� ����

CDx7Sound  DxSound;

CCharMng      Character;  // ĳ���� ���� ��ü ����
CMapMng       Map;        // ��     ���� ��ü ����
CInterfaceMng Interface;  // �������̽� �߰� 
CTextData     TextData;	  // �ؽ�Ʈ Ŭ���� ��ü ����
StageInfo     Stage;
CEvent	      Event;
CImage        Test;

WORD* pScreen;				// BackBuffer�� WORD�������� ����
DWORD dwFrameTime;
int   nKeyInput, nDirection;// Input ����(��ĳ���� �ʿ��� ����)
int   nRealWidth;			// ���� ���� ��
int	  nSpeed = 13;
int   nNowStage = 1;		// �������� �Ϸù�ȣ 1~6;
BOOL  bMiniMapShow = true, bMouseDown = true; 
int	  nEvtNum = 0;

point Teshy, Curs;   // ���� ��ǥ

//----------------------------------------------------------------------
// Function Definition
//----------------------------------------------------------------------
void DrawProcess     (void),
     InputProcess    (void),
	 CreatureProcess (void),
     DisplayFrameRate(void), 
	 EventInput		 (void),
	 EtcGameProcess	 (void);

//----------------------------------------------------------------------
// �׽�Ʈ�� ���� �ӽ� �������� �� �Լ�
//----------------------------------------------------------------------
char  keybuffer[256];
DWORD dwFrameTime2, dwKeyTime;

//----------------------------------------------------------------------
// Name: WindowProc()
// Desc: The Main Window Procedure
//----------------------------------------------------------------------
LRESULT CALLBACK
WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message) 
	{
	case WM_ACTIVATEAPP:
		nActiveApp = wParam;
		return 0L;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0L;
	case WM_MOVE:
		if (!SCREEN_MODE)
        {
			RECT rt;
			GetClientRect(hwnd, &rt);
			ClientToScreen(hwnd, (POINT*)&rt.left);
			ClientToScreen(hwnd, (POINT*)&rt.right);

			DxDraw.SetClientRect(&rt);
		}
		return 0L;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);
}

//----------------------------------------------------------------------
// Name: CreateInitWindow()
// Desc: Creates the Init window, which is the window 
//       which will receive user input.
//----------------------------------------------------------------------
bool 
CreateInitWindow(HINSTANCE hInstance)
{
    WNDCLASSEX wcex;

    wcex.cbSize        = sizeof (wcex);
	wcex.style         = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc   = (WNDPROC)WndProc;
	wcex.cbClsExtra    = 0L;
	wcex.cbWndExtra    = 0L;
	wcex.hInstance     = hInstance;
	wcex.hIcon         = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TESHYRIA));
	wcex.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH )GetStockObject(BLACK_BRUSH);
	wcex.lpszMenuName  = TEXT("");
	wcex.lpszClassName = NAME;
	wcex.hIconSm       = LoadIcon (NULL, IDI_APPLICATION);
    RegisterClassEx (&wcex);
    
    hMainWnd = CreateWindowEx(WS_EX_TOPMOST, NAME, TITLE, WS_POPUP,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          CW_USEDEFAULT, CW_USEDEFAULT,
                          NULL, NULL, hInstance, NULL);

    ShowWindow(hMainWnd, SW_SHOW);

    if (hMainWnd == NULL) return false;

    return true;
}

//----------------------------------------------------------------------
// Name: WinMain()
// Desc: Initialization, message loop
//----------------------------------------------------------------------
int PASCAL
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
        LPSTR lpCmdLine, int nCmdShow)
{
	MSG  msg;

    if (CreateInitWindow(hInstance)==false) {
		MessageBox(hMainWnd, "Create Window Error!!!", "Error", MB_OK);
		return 0;
	}
	UpdateWindow(hMainWnd);
	srand( (unsigned)time( NULL ) );

	// DirectX Draw init
	HRESULT hr;
	hr = DxDraw.Init(hMainWnd, 
		             SCREEN_WIDTH, SCREEN_HEIGHT, 
					 SCREEN_BPP, SCREEN_MODE);
	if(hr != DD_OK) return 0;
	nRealWidth = DxDraw.GetRealWidth();
	
	// DirectX Input init
	hr = DxInput.Init(hInstance, hMainWnd);
	if(hr != DI_OK) return 0;
	DxSound.Init(hMainWnd);

	Curs.x = 400; Curs.y = 300;
	hInst = hInstance;
	ShowCursor(false);

	Map.LoadStage(nNowStage, &Stage, &Teshy, DxDraw.Is565Mode());
	Map.m_pMap->SetScrInfo(&Teshy); // �ʻ����ø��� ȣ��

	Character.SetInfo(&Map, &Stage); 
	Event.SetEventInfo(&DxDraw, &TextData);
	Interface.SetMainChar(Character.m_pTeshy);	// ���̰� �߰��� �ڵ� 4: �׽ø��� ������ ���� (�������̽��� )

	memset(keybuffer, 1, 256);

//	Test.LoadSpr("DATA\\Test.spr");

//	Interface.SetGameMode(OPENNING_MOVIE, &DxDraw);
	Interface.SetGameMode(PLAYING_1_1, &DxDraw);
	dwFrameTime2 = dwFrameTime = timeGetTime();
	while (TRUE) {
		if (PeekMessage (&msg, NULL, 0, 0, PM_NOREMOVE) ) {
			if ( !GetMessage (&msg, NULL, 0, 0) ) 
				return msg.wParam;
			TranslateMessage (&msg) ;
			DispatchMessage (&msg) ;
		} 
		else
		{
			if(nActiveApp) 
			{
				InputProcess();      // Input ó��

				if(Interface.GetGameMode() % 10 == 5   &&   
				   Interface.GetGameMode() >= PLAYING_1_1   &&   
				   Interface.GetGameMode() <= PLAYING_3_2 ) 	// �� ���� �÷��� ���...  
				{
					if( !Interface.GetNowEvent() )
					{
						CreatureProcess();
						DrawProcess();			// Draw  ó��
					}
					else
					{
						if( !Event.IsEventComplete(nEvtNum-1) )
						{
							if(!Event.EventLoop(nEvtNum-1))
								Interface.SetNowEvent(false);
							EventInput();
						}
						else
							Interface.SetNowEvent(false);
					}
				}
				else 
				{
					EtcGameProcess();
				}
			}
			else		// ���α׷��� BackGround�� �Ͻ� ����
				WaitMessage();
		}
		Interface.GetBgm_mng()->OnEvent();
	}
	return 0;
}

//----------------------------------------------------------------------
// �׷��ִ� ��
//----------------------------------------------------------------------
void DrawProcess()
{
	if(Interface.GetStep() == 4)
	{
		for(int i=0; i<=18; i+=2) // ���̵� ó��
			DxDraw.FadeEffect(i, 0);
		Interface.SetStep(3);
	}
	else
	{
		if( ! Interface.GetKeyESC() )
		{
			DWORD time2;

			time2 = timeGetTime() - dwFrameTime2;
			if( time2 > 1000/1000 )  {
				DxDraw.ClearSurface(DxDraw.GetOSurface(), RGB(255, 0, 255));
				pScreen = DxDraw.LockOff();
				Map.DrawTileMap(pScreen);				
				// ĳ�� �׷� �ִ� �κ�
				

				Map.DrawCliffMap(pScreen, false);
				Character.Draw(pScreen, &Curs);
				Map.DrawCliffMap(pScreen, true);
				if(bMiniMapShow) Map.m_pMap->DrawMinimap(pScreen, (int)Teshy.x, (int)Teshy.y);	

		//		Map.DrawFog(pScreen);
				Interface.DrawInterface(pScreen, nRealWidth, &DxDraw);
			//	Test.DrawSpr(0, 600-Test.GetHeight(0), 0, pScreen);
				// ������ �ӵ� �׷� �ִ� �κ�
				DxDraw.UnlockOff();
				DisplayFrameRate();

				dwFrameTime2 = timeGetTime();
				DxDraw.Flip();
			}	
		}
		else
		{
			Interface.Loop_EscMenu(pScreen, nRealWidth, &DxDraw);
		}
	}
}
//----------------------------------------------------------------------
// Event �� Input ó�� (Ű���常)
//---------------------------------------------------------------------
void EventInput()
{
	if(KEYBOARD_USE)
	{
		char keys[256];
		
		HRESULT hr = DxInput.GetKeyBDPtr()->GetDeviceState(256, keys);
		if(hr==(DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			DxInput.GetKeyBDPtr()->Acquire();

		if(keys[DIK_SPACE] & 0x80) { 
			if(keybuffer[DIK_SPACE]) {
				Event.TextPass(); 
				keybuffer[DIK_SPACE] = false;
			}
		} else { keybuffer[DIK_SPACE] = true; }
		if(keys[DIK_ESCAPE] & 0x80)  // ������ ���� ó��
		{
			//for(int i=0; i<=18; i+=2) // ���̵� ó��
			//	DxDraw.FadeEffect(i, 0);

			PostMessage(hMainWnd, WM_CLOSE, 0, 0);			
		}
		else { keybuffer[DIK_ESCAPE] = true; }
	}
}

//----------------------------------------------------------------------
// Input ó�� (���콺�� Ű���常)
//----------------------------------------------------------------------
void InputProcess()
{
	HRESULT hr;
	
	nKeyInput = 0;

	// ���콺 ó��
	if(MOUSE_USE)
	{
		DIMOUSESTATE MouseState;
		hr = DxInput.GetMousePtr()->GetDeviceState(sizeof(MouseState), &MouseState);
		if(hr==(DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			DxInput.GetMousePtr()->Acquire();
		if(MouseState.rgbButtons[1] & 0x80) // ���콺 ������ ��ư �ٿ��(�޸���)
			nKeyInput = 1;
		if(MouseState.rgbButtons[0] & 0x80) // ���콺 ���� ��ư �ٿ��(����)
		{
			if(bMouseDown) {
				nKeyInput = 3;
				bMouseDown = FALSE;
			}
		} else { bMouseDown = TRUE; }

		Curs.x += (int)(MouseState.lX*1.7); // 1.7�� ���콺 ������ ����
		Curs.y += (int)(MouseState.lY*1.7);
		Curs.x = max(0, min(Curs.x, SCREEN_WIDTH-1));
		Curs.y = max(0, min(Curs.y, SCREEN_HEIGHT-1));

		int nXdir = Curs.x-(SCREEN_WIDTH>>1);
		int nYdir = (SCREEN_HEIGHT>>1)-Curs.y;
		if ((nXdir!=0) || (nYdir!=0)) {
			nDirection = GetDirection(nXdir, nYdir);
		}		
	}

	// Ű���� ó��
	if(KEYBOARD_USE)
	{
		char keys[256];
		hr = DxInput.GetKeyBDPtr()->GetDeviceState(256, keys);
		if(hr==(DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			DxInput.GetKeyBDPtr()->Acquire();
	
		if(keys[DIK_TAB] & 0x80) { 
			if(keybuffer[DIK_TAB]) {
				bMiniMapShow ^= 1; 
				//DxSound.PlaySound("SndData\\c_bang.WAV",0,0);
				keybuffer[DIK_TAB] = FALSE;
			}
		} else { keybuffer[DIK_TAB] = TRUE; }

		if(keys[DIK_Q] & 0x80) {  // ��˼�
			if(keybuffer[DIK_Q]) {
				nKeyInput = 10;
				keybuffer[DIK_Q] = FALSE;
			}
		} else { keybuffer[DIK_Q] = TRUE; }
		if(keys[DIK_W] & 0x80) {  // �񵵼�
			if(keybuffer[DIK_W]) {
				nKeyInput = 11;
				keybuffer[DIK_W] = FALSE;
			}
		} else { keybuffer[DIK_W] = TRUE; }
		if(keys[DIK_A] & 0x80) {  // ��ȥ��
			if(keybuffer[DIK_A]) {
				nKeyInput = 12;
				keybuffer[DIK_A] = FALSE;
			}
		} else { keybuffer[DIK_A] = TRUE; }
		if(keys[DIK_D] & 0x80) {  // ��Ű��
			if(keybuffer[DIK_D]) {
				nKeyInput = 13;
				keybuffer[DIK_D] = FALSE;
			}
		} else { keybuffer[DIK_D] = TRUE; }
		if(keys[DIK_S] & 0x80) {  // �巡�� �Ǿ�
			if(keybuffer[DIK_S]) {
				nKeyInput = 14;
				keybuffer[DIK_S] = FALSE;
			}
		} else { keybuffer[DIK_S] = TRUE; }
		if(keys[DIK_E] & 0x80) {  // �ҵ�극��Ŀ
			if(keybuffer[DIK_E]) {
				nKeyInput = 15;
				keybuffer[DIK_E] = FALSE;
			}
		} else { keybuffer[DIK_E] = TRUE; }
//---------------------------------------------------
// �ӽ� �׽�Ʈ
		if(keys[DIK_P] & 0x80) {  // �ױ� ����
			if(keybuffer[DIK_P]) {
				nKeyInput = 16;
				keybuffer[DIK_P] = FALSE;
			}
		} else { keybuffer[DIK_P] = TRUE; }
		if(keys[DIK_O] & 0x80) {  // �̼� ���� ����
			if(keybuffer[DIK_O]) {
				nKeyInput = 17;
				keybuffer[DIK_O] = FALSE;
			}
		} else { keybuffer[DIK_O] = TRUE; }
//---------------------------------------------------

		if(keys[DIK_1] & 0x80) { nKeyInput = 18; }
		if(keys[DIK_2] & 0x80) { nKeyInput = 19; }
		if(keys[DIK_3] & 0x80) { nKeyInput = 20; }

		if(keys[DIK_SPACE] & 0x80) {
			if(keybuffer[DIK_SPACE]) {
				nKeyInput = 7;
				keybuffer[DIK_SPACE] = FALSE;
			}
		} else { keybuffer[DIK_SPACE] = TRUE; }

		if(keys[DIK_LSHIFT] & 0x80) {			
			if(nKeyInput==1 || nKeyInput==3) nKeyInput++;
			else nKeyInput = 6;
		}
		if(keys[DIK_LCONTROL] & 0x80) {  // ����
			if(nKeyInput!=1 && nKeyInput!=2) nKeyInput = 5;
		}

///////////////////// �������̽� �� Ű /////////////////////////////
		if(keys[DIK_RETURN] & 0x80)
		{
			if(keybuffer[DIK_RETURN])
			{
				nKeyInput = 100;
				keybuffer[DIK_RETURN] = FALSE;
			}
		} else { keybuffer[DIK_RETURN] = TRUE; }

		if(keys[DIK_ESCAPE] & 0x80)  // ������ ���� ó��
		{
			if(keybuffer[DIK_ESCAPE])
			{
				Interface.SetKeyESC( !Interface.GetKeyESC() ); 
				nKeyInput = 101;
				keybuffer[DIK_ESCAPE] = FALSE;
			}
		} else { keybuffer[DIK_ESCAPE] = TRUE; }

		if(Interface.GetGameMode() == START_ID_MENU)
		{
			int nShift = 0;
			if(keys[DIK_LSHIFT] & 0x80) { nShift = 32; }

			if(keys[DIK_A] & 0x80) {if(Interface.GetKeyConsent(0) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('a'-nShift, 0); }  else Interface.SetKeyConsent(true, 0);
			if(keys[DIK_B] & 0x80) {if(Interface.GetKeyConsent(1) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('b'-nShift, 1); }  else Interface.SetKeyConsent(true, 1);
			if(keys[DIK_C] & 0x80) {if(Interface.GetKeyConsent(2) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('c'-nShift, 2); }  else Interface.SetKeyConsent(true, 2);
			if(keys[DIK_D] & 0x80) {if(Interface.GetKeyConsent(3) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('d'-nShift, 3); }  else Interface.SetKeyConsent(true, 3);
			if(keys[DIK_E] & 0x80) {if(Interface.GetKeyConsent(4) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('e'-nShift, 4); }  else Interface.SetKeyConsent(true, 4);
			if(keys[DIK_F] & 0x80) {if(Interface.GetKeyConsent(5) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('f'-nShift, 5); }  else Interface.SetKeyConsent(true, 5);
			if(keys[DIK_G] & 0x80) {if(Interface.GetKeyConsent(6) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('g'-nShift, 6); }  else Interface.SetKeyConsent(true, 6);
			if(keys[DIK_H] & 0x80) {if(Interface.GetKeyConsent(7) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('h'-nShift, 7); }  else Interface.SetKeyConsent(true, 7);
			if(keys[DIK_I] & 0x80) {if(Interface.GetKeyConsent(8) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('i'-nShift, 8); }  else Interface.SetKeyConsent(true, 8);
			if(keys[DIK_J] & 0x80) {if(Interface.GetKeyConsent(9) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('j'-nShift, 9); }  else Interface.SetKeyConsent(true, 9);
			if(keys[DIK_K] & 0x80) {if(Interface.GetKeyConsent(10) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('k'-nShift, 10); }  else Interface.SetKeyConsent(true, 10);
			if(keys[DIK_L] & 0x80) {if(Interface.GetKeyConsent(11) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('l'-nShift, 11); }  else Interface.SetKeyConsent(true, 11);
			if(keys[DIK_M] & 0x80) {if(Interface.GetKeyConsent(12) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('m'-nShift, 12); }  else Interface.SetKeyConsent(true, 12);
			if(keys[DIK_N] & 0x80) {if(Interface.GetKeyConsent(13) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('n'-nShift, 13); }  else Interface.SetKeyConsent(true, 13);
			if(keys[DIK_O] & 0x80) {if(Interface.GetKeyConsent(14) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('o'-nShift, 14); }  else Interface.SetKeyConsent(true, 14);
			if(keys[DIK_P] & 0x80) {if(Interface.GetKeyConsent(15) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('p'-nShift, 15); }  else Interface.SetKeyConsent(true, 15);
			if(keys[DIK_Q] & 0x80) {if(Interface.GetKeyConsent(16) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('q'-nShift, 16); }  else Interface.SetKeyConsent(true, 16);
			if(keys[DIK_R] & 0x80) {if(Interface.GetKeyConsent(17) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('r'-nShift, 17); }  else Interface.SetKeyConsent(true, 17);
			if(keys[DIK_S] & 0x80) {if(Interface.GetKeyConsent(18) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('s'-nShift, 18); }  else Interface.SetKeyConsent(true, 18);
			if(keys[DIK_T] & 0x80) {if(Interface.GetKeyConsent(19) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('t'-nShift, 19); }  else Interface.SetKeyConsent(true, 19);
			if(keys[DIK_U] & 0x80) {if(Interface.GetKeyConsent(20) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('u'-nShift, 20); }  else Interface.SetKeyConsent(true, 20);
			if(keys[DIK_V] & 0x80) {if(Interface.GetKeyConsent(21) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('v'-nShift, 21); }  else Interface.SetKeyConsent(true, 21);
			if(keys[DIK_W] & 0x80) {if(Interface.GetKeyConsent(22) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('w'-nShift, 22); }  else Interface.SetKeyConsent(true, 22);
			if(keys[DIK_X] & 0x80) {if(Interface.GetKeyConsent(23) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('x'-nShift, 23); }  else Interface.SetKeyConsent(true, 23);
			if(keys[DIK_Y] & 0x80) {if(Interface.GetKeyConsent(24) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('y'-nShift, 24); }  else Interface.SetKeyConsent(true, 24);
			if(keys[DIK_Z] & 0x80) {if(Interface.GetKeyConsent(25) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('z'-nShift, 25); }  else Interface.SetKeyConsent(true, 25);
			if(keys[DIK_0] & 0x80) {if(Interface.GetKeyConsent(26) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('0', 26); }  else Interface.SetKeyConsent(true, 26);
			if(keys[DIK_1] & 0x80) {if(Interface.GetKeyConsent(27) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('1', 27); }  else Interface.SetKeyConsent(true, 27);
			if(keys[DIK_2] & 0x80) {if(Interface.GetKeyConsent(28) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('2', 28); }  else Interface.SetKeyConsent(true, 28);
			if(keys[DIK_3] & 0x80) {if(Interface.GetKeyConsent(29) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('3', 29); }  else Interface.SetKeyConsent(true, 29);
			if(keys[DIK_4] & 0x80) {if(Interface.GetKeyConsent(30) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('4', 30); }  else Interface.SetKeyConsent(true, 30);
			if(keys[DIK_5] & 0x80) {if(Interface.GetKeyConsent(31) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('5', 31); }  else Interface.SetKeyConsent(true, 31);
			if(keys[DIK_6] & 0x80) {if(Interface.GetKeyConsent(32) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('6', 32); }  else Interface.SetKeyConsent(true, 32);
			if(keys[DIK_7] & 0x80) {if(Interface.GetKeyConsent(33) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('7', 33); }  else Interface.SetKeyConsent(true, 33);
			if(keys[DIK_8] & 0x80) {if(Interface.GetKeyConsent(34) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('8', 34); }  else Interface.SetKeyConsent(true, 34);
			if(keys[DIK_9] & 0x80) {if(Interface.GetKeyConsent(35) == true  && Interface.GetCountName() < NAME_MAX - 1) Interface.PushChar('9', 35); }  else Interface.SetKeyConsent(true, 35);


			if(keys[DIK_BACK] & 0x80)
			{
				if(Interface.GetKeyConsent(36)==true  && Interface.GetCountName() > 0)
					Interface.BackSpace();
			}
			else Interface.SetKeyConsent(true, 36);

		}

		//--- �׽�Ʈ�� Ű ------------------( ���߿� ������ �� )--------
		if(keys[DIK_F9] & 0x80)		  { Interface.SetGameMode(PLAYING_1_1); }
		if(keys[DIK_F12] & 0x80)	  { Interface.SetGameMode(EXIT_GAME); }
		if(Interface.GetGameMode() % 10 == 5 && Interface.GetGameMode() >= PLAYING_1_1   &&   Interface.GetGameMode() <= PLAYING_3_2)
		{
			if(keys[DIK_F1] & 0x80)   { Interface.SetResult(1); }  // �̼Ǽ���
			if(keys[DIK_F2] & 0x80)   { Interface.SetResult(2); }  // �̼ǽ���
		}
	}
	Character.StatusIn(&nDirection, nKeyInput, &Teshy, &Curs);
	Interface.SetInputSts(Curs.x, Curs.y, nKeyInput);
}

//----------------------------------------------------------------------
// ��ĳ���͵��� ���� �� ���¹� �ִ� ���� ����
//----------------------------------------------------------------------
void CreatureProcess(void)
{
	// ���콺 ��ũ�� ó��(���� ��ư �ٿ��)
	if( (Character.CanScroll()) && ((nKeyInput==1) || (nKeyInput==2))) 
	{
		DWORD time;
		time = timeGetTime() - dwKeyTime;
		if( time > 4 )
		{
			Map.m_pMap->SetScroll(&Teshy, &Curs, nDirection, nSpeed);
			
			// �̺�Ʈ �߻������� üũ�Ͽ� �̺�Ʈ �����̸� �̺�Ʈ ���� ��ȯ ------
			nEvtNum = Map.m_pMap->IsEvent(); 
			//if( nEvtNum ) nGameMode = EVENT_MODE;
			if( nEvtNum ) Interface.SetNowEvent(true);
			//---------------------------------------------------------------------
			dwKeyTime = timeGetTime();
		}
	}
	Character.LocationIn(&Teshy);
}

//----------------------------------------------------------------------
// Fps �����ؼ� �ѷ��ִ� �Լ�
//----------------------------------------------------------------------
void DisplayFrameRate()
{
	HDC   hdc;
    DWORD time2;
	static DWORD dwFrames; 
	static DWORD dwFramesLast;
	static int iFrameCount;
    
    iFrameCount++;
    time2 = timeGetTime() - dwFrameTime;
    if( time2 > 1000 )
    {
        dwFrames = (iFrameCount*1000)/time2;
        dwFrameTime = timeGetTime();
        iFrameCount = 0;
    }
    if( dwFrames == 0 ) return;
    if (dwFrames != dwFramesLast)
    {
        dwFramesLast = dwFrames;
    }

	DxDraw.GetOSurface()->GetDC(&hdc);

//------------------------------------
// �׽�Ʈ
//------------------------------------
	char str[100];
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 200, 70));
	
	wsprintf(str, "�ʴ�: %d fps", dwFrames);
	TextOut(hdc, 7, 5, str, strlen(str));
	wsprintf(str, "Mouse(%d, %d)", Curs.x, Curs.y);
	TextOut(hdc, 7, 21, str, strlen(str));
	wsprintf(str, "Teshy��ǥ %d, %d", Teshy.x, Teshy.y);
	TextOut(hdc, 7, 37, str, strlen(str));
	
/*	//�׽ø��� ���� ��ġ�� ---------------------------------------------------------
	wsprintf(str, "Teshy HP %d", Character.m_pTeshy->m_HP);
	TextOut(hdc, 7, 53, str, strlen(str));

	wsprintf(str, "Teshy MP %d", Character.m_pTeshy->m_MP);
	TextOut(hdc, 7, 69, str, strlen(str));

	wsprintf(str, "Teshy SP %d", Character.m_pTeshy->m_SP);
	TextOut(hdc, 7, 82, str, strlen(str));
	//------------------------------------------------------------------------------

	wsprintf(str, "ĳ����ȣ %d", Character.m_nSelectedChar);
	TextOut(hdc, 320, 5, str, strlen(str));

	for(int a=0; a<7; a++)
	{
		wsprintf(str, "%dĳ�� HP : %d", a, Character.m_pEnemy[a]->m_HP);
		TextOut(hdc, 550, 5+a*16, str, strlen(str));
	}



/*	if( Character.m_nSelectedChar > -1 )
	{
		int b=0;
		int x = Character.m_pEnemy[Character.m_nSelectedChar]->m_nMoveX;
		int y = Character.m_pEnemy[Character.m_nSelectedChar]->m_nMoveY;
		if( Character.GetMapAttrN(x,y)&0x00000010 ) 
			b = 1;
		else b=0;
		
		wsprintf(str, "%2d, Move %3d,%3d, PosM %3d, %3d PosP %3d, %3d ������ %2d, %2d, ���� %2d, %2d, �� %d", Character.m_nSelectedChar, 
		Character.m_pEnemy[Character.m_nSelectedChar]->m_nMoveX, 
		Character.m_pEnemy[Character.m_nSelectedChar]->m_nMoveY,
		Character.m_pEnemy[Character.m_nSelectedChar]->GetXPos()/32, 
		Character.m_pEnemy[Character.m_nSelectedChar]->GetYPos()/32,
		Character.m_pEnemy[Character.m_nSelectedChar]->GetXPos(),
		Character.m_pEnemy[Character.m_nSelectedChar]->GetYPos(),
		Character.m_pEnemy[Character.m_nSelectedChar]->GetXPos()%32,
		Character.m_pEnemy[Character.m_nSelectedChar]->GetYPos()%32,
		Character.m_pEnemy[Character.m_nSelectedChar]->m_nTarX-Character.m_pEnemy[Character.m_nSelectedChar]->GetXPos(),
		Character.m_pEnemy[Character.m_nSelectedChar]->m_nTarY-Character.m_pEnemy[Character.m_nSelectedChar]->GetYPos(),
		b
		);
		TextOut(hdc, 200, 20, str, strlen(str));
	}	
	wsprintf(str, "���� %d", Character.m_pTeshy->WhatAttack());
	TextOut(hdc, 160, 53, str, strlen(str));
/*	wsprintf(str, "��� ���� ��� �ð� %d, ���� ���� %d", Character.m_pScMage[0]->m_nCommTime, Character.m_pScMage[0]->m_pTeam->nDeadCnt);
	TextOut(hdc, 200, 37, str, strlen(str));

	wsprintf(str, "��� ���� ��� �ð� %d, ���� ���� %d", Character.m_pScMage[1]->m_nCommTime, Character.m_pScMage[1]->m_pTeam->nDeadCnt);
	TextOut(hdc, 500, 37, str, strlen(str));

	wsprintf(str, "KeyStatus %d", nKeyInput);
	TextOut(hdc, 160, 69, str, strlen(str));
	wsprintf(str, "AniLock %d", Character.m_pTeshy->IsLock());
	TextOut(hdc, 160, 85, str, strlen(str));

	wsprintf(str, "0����: ���̰��� %d, ���� ����: %d", Character.m_pRgMage[0]->m_pTeam->nWPCnt,Character.m_pRgMage[0]->m_pTeam->nWpNum );
	TextOut(hdc, 200, 30, str, strlen(str));

	for( int i=0; i<Stage.aWayCnt[0]-1; i++ )
	{
		MoveToEx(hdc, Stage.aWayX[0][i], Stage.aWayY[0][i], NULL);
		LineTo(hdc, Stage.aWayX[0][i+1], Stage.aWayY[0][i+1]);
	}

	wsprintf(str, "ĳ���� �� �� %d", Stage.nCharTotal);
	TextOut(hdc, 500, 181, str, strlen(str));
	wsprintf(str, "���� �ε��� ĳ���� �� �� %d", Character.m_nLoadedCharCnt);
	TextOut(hdc, 500, 197, str, strlen(str));

*/	

/*	for(int t=0; t<Stage.nCharTotal; t++ )
	{
		wsprintf(str, "[%d][%d]:%d", Stage.aTeamNum[t], Stage.aPosInTeam[t], t);
						
		TextOut(hdc, Stage.aXPosInMap[t]-Map.m_pMap->GetMapXPos(), Stage.aYPosInMap[t]-Map.m_pMap->GetMapYPos(), str, strlen(str));
	}
	HPEN Now, Old;
	Now = (HPEN)GetStockObject(BLACK_PEN);
	Old = (HPEN)SelectObject(hdc, Now);

  	int  k=0;
	int nXScl = Map.m_pMap->GetMapXPos();
	int nYScl = Map.m_pMap->GetMapYPos();

	wsprintf(str, "%d",  Character.m_pEnemy[23]->GetTeamNum() );
	TextOut(hdc, 200, 200, str, strlen(str));

	wsprintf(str, "%d",  Character.m_pAnimal[0]->m_pTeam->nWPCnt );
	TextOut(hdc, 200, 220, str, strlen(str));

	for(int i=0; i<1; i++)
	{	
		if( Character.m_pAnimal[0]->m_pTeam->nWPCnt > 0 )
		{
			for( int j=0; j<Character.m_pAnimal[0]->m_pTeam->nWPCnt-1; j++ )
			{
				MoveToEx(hdc, Character.m_pAnimal[0]->m_pTeam->pWpX[j]-nXScl, 
							Character.m_pAnimal[0]->m_pTeam->pWpY[j]-nYScl, NULL);
				LineTo(hdc, Character.m_pAnimal[0]->m_pTeam->pWpX[j+1]-nXScl, 
							Character.m_pAnimal[0]->m_pTeam->pWpY[j+1]-nYScl);
				
			}
		}
	}
	SelectObject(hdc, Old);
/*	for(i=0; i<Stage.nTeamCnt; i++)
	{	
		if( Stage.aWayCnt[i] > 0 )
		for( int j=0; j<Stage.aWayCnt[i]; j++ )
		{
			wsprintf(str, "%d, %d",  Stage.aWayX[i][j], Stage.aWayY[i][j] );
			TextOut(hdc, Stage.aWayX[i][j]-Map.m_pMap->GetMapXPos(), Stage.aWayY[i][j]-Map.m_pMap->GetMapYPos(), str, strlen(str));
		
			if( j<Stage.aWayCnt[i]-1)
			{
				MoveToEx(hdc, Stage.aWayX[i][j]-Map.m_pMap->GetMapXPos(), Stage.aWayY[i][j]-Map.m_pMap->GetMapYPos(), NULL);
				LineTo(hdc, Stage.aWayX[i][j+1]-Map.m_pMap->GetMapXPos(), Stage.aWayY[i][j+1]-Map.m_pMap->GetMapYPos());
			}
		}
	}

	for( int i=0; i<Stage.nEventCnt; i++ )
	{
		rect rt = Stage.artEvent[i];
		if( !Event.IsEventComplete(i) )
		{
			wsprintf(str, "%d ={ %d, %d, %d, %d}", Stage.aEventNum[i], rt.x1*64, rt.y1*64, rt.x2*64, rt.y2*64);
			TextOut(hdc, 0, i*20+100, str, strlen(str));
		}
	}
*/
	int nXScl = Map.m_pMap->GetMapXPos();
	int nYScl = Map.m_pMap->GetMapYPos();
	int nAttrMapW = Map.m_pMap->GetAttrMapWidth();
	int nAttrMapH = Map.m_pMap->GetAttrMapHeight();
	int nDrawXs = (int)(nXScl/32);
	int nDrawYs = (int)(nYScl/32);
	int nXScr = nXScl%32;
	int nYScr = nYScl%32;

	for( int i=0; i<19; i++)
		for( int j=0; j<26; j++)
		{
			if( nDrawYs+i >=0 && nDrawYs+i < nAttrMapH && nDrawXs+j >=0 && nDrawXs+j < nAttrMapW )
			{				
				if( (Map.m_pMap->m_pAttrMap[(nDrawYs+i)*nAttrMapW+(nDrawXs+j)] & 0x00000010))
				{
					wsprintf(str, "A");
					TextOut(hdc, j*32-nXScr+5, i*32-nYScr+5, str, strlen(str));
				}
				if( (Map.m_pMap->m_pAttrMap[(nDrawYs+i)*nAttrMapW+(nDrawXs+j)] & 0x00000020))
				{
					wsprintf(str, "T");
					TextOut(hdc, j*32-nXScr+5, i*32-nYScr+5, str, strlen(str));
				}
				if( (Map.m_pMap->m_pAttrMap[(nDrawYs+i)*nAttrMapW+(nDrawXs+j)] >> 16) )
				{
					wsprintf(str, "%d", Map.m_pMap->m_pAttrMap[(nDrawYs+i)*nAttrMapW+(nDrawXs+j)]>>16);
					TextOut(hdc, j*32-nXScr+5, i*32-nYScr+5, str, strlen(str));
				}
			}
		}

/*	�� & 0x00000001 = �浹üũ , 
	�� & 0x00000002 = Object ���� , 
	�� & 0x00000004 = ������ ���� , 
	�� & 0x00000008 = Blank���� , 
	�� & 0x00000010 = ��ĳ�� Ÿ��, 
	>> 8 = �̴ϸ� ����  
	>> 16 = �̺�Ʈ ���� */
		
	DxDraw.GetOSurface()->ReleaseDC(hdc);
}

void EtcGameProcess(void)
{
	switch(Interface.GetGameMode())
	{
	case EXIT_GAME:
		PostMessage(hMainWnd, WM_CLOSE, 0, 0);		// ���α׷� ���� ó��
		break;
	default:
		Interface.EtcGame(pScreen, 800, &DxDraw);	// �� ���� �÷��� ��� ���� ����� ���� ���...
		break;
	}
}