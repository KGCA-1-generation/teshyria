//----------------------------------------------------------------------
// Title: 떼쉬리아.
//
// Desc: 게임은 게임인디 어쩌고 저쩌고... 
// Date: 2001년 11월 27일 부터
//
// 걍 한번 만들어 봤다.
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
#include "InterfaceMng.h"  // 인터페이스 추가
#include "MapMng.h"
#include "Event.h"

// 임시 인클루드. 나중에 지우는거 검토
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
int        nActiveApp = false; // BackGround인지 ForeGround인지

CDx7Draw   DxDraw;
CDx7Input  DxInput(MOUSE_USE, KEYBOARD_USE); // 마우스 와 키보드 사용 유무

CDx7Sound  DxSound;

CCharMng      Character;  // 캐릭터 관리 객체 생성
CMapMng       Map;        // 맵     관리 객체 생성
CInterfaceMng Interface;  // 인터페이스 추가 
CTextData     TextData;	  // 텍스트 클래스 객체 선언
StageInfo     Stage;
CEvent	      Event;
CImage        Test;

WORD* pScreen;				// BackBuffer의 WORD포인터형 변수
DWORD dwFrameTime;
int   nKeyInput, nDirection;// Input 상태(주캐릭에 필요한 사항)
int   nRealWidth;			// 실제 가로 폭
int	  nSpeed = 13;
int   nNowStage = 1;		// 스테이지 일련번호 1~6;
BOOL  bMiniMapShow = true, bMouseDown = true; 
int	  nEvtNum = 0;

point Teshy, Curs;   // 떼쉬 좌표

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
// 테스트를 위한 임시 전역변수 및 함수
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
	Map.m_pMap->SetScrInfo(&Teshy); // 맵생성시마다 호출

	Character.SetInfo(&Map, &Stage); 
	Event.SetEventInfo(&DxDraw, &TextData);
	Interface.SetMainChar(Character.m_pTeshy);	// 선미가 추가한 코딩 4: 테시리아 포인터 세팅 (인터페이스에 )

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
				InputProcess();      // Input 처리

				if(Interface.GetGameMode() % 10 == 5   &&   
				   Interface.GetGameMode() >= PLAYING_1_1   &&   
				   Interface.GetGameMode() <= PLAYING_3_2 ) 	// 본 게임 플레이 모드...  
				{
					if( !Interface.GetNowEvent() )
					{
						CreatureProcess();
						DrawProcess();			// Draw  처리
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
			else		// 프로그램이 BackGround면 일시 중지
				WaitMessage();
		}
		Interface.GetBgm_mng()->OnEvent();
	}
	return 0;
}

//----------------------------------------------------------------------
// 그려주는 곳
//----------------------------------------------------------------------
void DrawProcess()
{
	if(Interface.GetStep() == 4)
	{
		for(int i=0; i<=18; i+=2) // 페이드 처리
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
				// 캐릭 그려 주는 부분
				

				Map.DrawCliffMap(pScreen, false);
				Character.Draw(pScreen, &Curs);
				Map.DrawCliffMap(pScreen, true);
				if(bMiniMapShow) Map.m_pMap->DrawMinimap(pScreen, (int)Teshy.x, (int)Teshy.y);	

		//		Map.DrawFog(pScreen);
				Interface.DrawInterface(pScreen, nRealWidth, &DxDraw);
			//	Test.DrawSpr(0, 600-Test.GetHeight(0), 0, pScreen);
				// 프레임 속도 그려 주는 부분
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
// Event 시 Input 처리 (키보드만)
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
		if(keys[DIK_ESCAPE] & 0x80)  // 지금은 종료 처리
		{
			//for(int i=0; i<=18; i+=2) // 페이드 처리
			//	DxDraw.FadeEffect(i, 0);

			PostMessage(hMainWnd, WM_CLOSE, 0, 0);			
		}
		else { keybuffer[DIK_ESCAPE] = true; }
	}
}

//----------------------------------------------------------------------
// Input 처리 (마우스와 키보드만)
//----------------------------------------------------------------------
void InputProcess()
{
	HRESULT hr;
	
	nKeyInput = 0;

	// 마우스 처리
	if(MOUSE_USE)
	{
		DIMOUSESTATE MouseState;
		hr = DxInput.GetMousePtr()->GetDeviceState(sizeof(MouseState), &MouseState);
		if(hr==(DIERR_INPUTLOST | DIERR_NOTACQUIRED))
			DxInput.GetMousePtr()->Acquire();
		if(MouseState.rgbButtons[1] & 0x80) // 마우스 오른쪽 버튼 다운시(달리기)
			nKeyInput = 1;
		if(MouseState.rgbButtons[0] & 0x80) // 마우스 왼쪽 버튼 다운시(공격)
		{
			if(bMouseDown) {
				nKeyInput = 3;
				bMouseDown = FALSE;
			}
		} else { bMouseDown = TRUE; }

		Curs.x += (int)(MouseState.lX*1.7); // 1.7은 마우스 감도를 보정
		Curs.y += (int)(MouseState.lY*1.7);
		Curs.x = max(0, min(Curs.x, SCREEN_WIDTH-1));
		Curs.y = max(0, min(Curs.y, SCREEN_HEIGHT-1));

		int nXdir = Curs.x-(SCREEN_WIDTH>>1);
		int nYdir = (SCREEN_HEIGHT>>1)-Curs.y;
		if ((nXdir!=0) || (nYdir!=0)) {
			nDirection = GetDirection(nXdir, nYdir);
		}		
	}

	// 키보드 처리
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

		if(keys[DIK_Q] & 0x80) {  // 어검술
			if(keybuffer[DIK_Q]) {
				nKeyInput = 10;
				keybuffer[DIK_Q] = FALSE;
			}
		} else { keybuffer[DIK_Q] = TRUE; }
		if(keys[DIK_W] & 0x80) {  // 비도술
			if(keybuffer[DIK_W]) {
				nKeyInput = 11;
				keybuffer[DIK_W] = FALSE;
			}
		} else { keybuffer[DIK_W] = TRUE; }
		if(keys[DIK_A] & 0x80) {  // 섭혼술
			if(keybuffer[DIK_A]) {
				nKeyInput = 12;
				keybuffer[DIK_A] = FALSE;
			}
		} else { keybuffer[DIK_A] = TRUE; }
		if(keys[DIK_D] & 0x80) {  // 발키리
			if(keybuffer[DIK_D]) {
				nKeyInput = 13;
				keybuffer[DIK_D] = FALSE;
			}
		} else { keybuffer[DIK_D] = TRUE; }
		if(keys[DIK_S] & 0x80) {  // 드래곤 피어
			if(keybuffer[DIK_S]) {
				nKeyInput = 14;
				keybuffer[DIK_S] = FALSE;
			}
		} else { keybuffer[DIK_S] = TRUE; }
		if(keys[DIK_E] & 0x80) {  // 소드브레이커
			if(keybuffer[DIK_E]) {
				nKeyInput = 15;
				keybuffer[DIK_E] = FALSE;
			}
		} else { keybuffer[DIK_E] = TRUE; }
//---------------------------------------------------
// 임시 테스트
		if(keys[DIK_P] & 0x80) {  // 죽기 포즈
			if(keybuffer[DIK_P]) {
				nKeyInput = 16;
				keybuffer[DIK_P] = FALSE;
			}
		} else { keybuffer[DIK_P] = TRUE; }
		if(keys[DIK_O] & 0x80) {  // 미션 성공 포즈
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
		if(keys[DIK_LCONTROL] & 0x80) {  // 집중
			if(nKeyInput!=1 && nKeyInput!=2) nKeyInput = 5;
		}

///////////////////// 인터페이스 용 키 /////////////////////////////
		if(keys[DIK_RETURN] & 0x80)
		{
			if(keybuffer[DIK_RETURN])
			{
				nKeyInput = 100;
				keybuffer[DIK_RETURN] = FALSE;
			}
		} else { keybuffer[DIK_RETURN] = TRUE; }

		if(keys[DIK_ESCAPE] & 0x80)  // 지금은 종료 처리
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

		//--- 테스트용 키 ------------------( 나중에 지워야 함 )--------
		if(keys[DIK_F9] & 0x80)		  { Interface.SetGameMode(PLAYING_1_1); }
		if(keys[DIK_F12] & 0x80)	  { Interface.SetGameMode(EXIT_GAME); }
		if(Interface.GetGameMode() % 10 == 5 && Interface.GetGameMode() >= PLAYING_1_1   &&   Interface.GetGameMode() <= PLAYING_3_2)
		{
			if(keys[DIK_F1] & 0x80)   { Interface.SetResult(1); }  // 미션성공
			if(keys[DIK_F2] & 0x80)   { Interface.SetResult(2); }  // 미션실패
		}
	}
	Character.StatusIn(&nDirection, nKeyInput, &Teshy, &Curs);
	Interface.SetInputSts(Curs.x, Curs.y, nKeyInput);
}

//----------------------------------------------------------------------
// 적캐릭터들의 패턴 및 상태및 애니 상태 결정
//----------------------------------------------------------------------
void CreatureProcess(void)
{
	// 마우스 스크롤 처리(왼쪽 버튼 다운시)
	if( (Character.CanScroll()) && ((nKeyInput==1) || (nKeyInput==2))) 
	{
		DWORD time;
		time = timeGetTime() - dwKeyTime;
		if( time > 4 )
		{
			Map.m_pMap->SetScroll(&Teshy, &Curs, nDirection, nSpeed);
			
			// 이벤트 발생영역을 체크하여 이벤트 영역이면 이벤트 모드로 전환 ------
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
// Fps 측정해서 뿌려주는 함수
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
// 테스트
//------------------------------------
	char str[100];
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 200, 70));
	
	wsprintf(str, "초당: %d fps", dwFrames);
	TextOut(hdc, 7, 5, str, strlen(str));
	wsprintf(str, "Mouse(%d, %d)", Curs.x, Curs.y);
	TextOut(hdc, 7, 21, str, strlen(str));
	wsprintf(str, "Teshy좌표 %d, %d", Teshy.x, Teshy.y);
	TextOut(hdc, 7, 37, str, strlen(str));
	
/*	//테시리아 관련 수치들 ---------------------------------------------------------
	wsprintf(str, "Teshy HP %d", Character.m_pTeshy->m_HP);
	TextOut(hdc, 7, 53, str, strlen(str));

	wsprintf(str, "Teshy MP %d", Character.m_pTeshy->m_MP);
	TextOut(hdc, 7, 69, str, strlen(str));

	wsprintf(str, "Teshy SP %d", Character.m_pTeshy->m_SP);
	TextOut(hdc, 7, 82, str, strlen(str));
	//------------------------------------------------------------------------------

	wsprintf(str, "캐릭번호 %d", Character.m_nSelectedChar);
	TextOut(hdc, 320, 5, str, strlen(str));

	for(int a=0; a<7; a++)
	{
		wsprintf(str, "%d캐릭 HP : %d", a, Character.m_pEnemy[a]->m_HP);
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
		
		wsprintf(str, "%2d, Move %3d,%3d, PosM %3d, %3d PosP %3d, %3d 나머지 %2d, %2d, 오차 %2d, %2d, 맵 %d", Character.m_nSelectedChar, 
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
	wsprintf(str, "공격 %d", Character.m_pTeshy->WhatAttack());
	TextOut(hdc, 160, 53, str, strlen(str));
/*	wsprintf(str, "통신 마법 경과 시간 %d, 죽은 기사수 %d", Character.m_pScMage[0]->m_nCommTime, Character.m_pScMage[0]->m_pTeam->nDeadCnt);
	TextOut(hdc, 200, 37, str, strlen(str));

	wsprintf(str, "통신 마법 경과 시간 %d, 죽은 기사수 %d", Character.m_pScMage[1]->m_nCommTime, Character.m_pScMage[1]->m_pTeam->nDeadCnt);
	TextOut(hdc, 500, 37, str, strlen(str));

	wsprintf(str, "KeyStatus %d", nKeyInput);
	TextOut(hdc, 160, 69, str, strlen(str));
	wsprintf(str, "AniLock %d", Character.m_pTeshy->IsLock());
	TextOut(hdc, 160, 85, str, strlen(str));

	wsprintf(str, "0번팀: 웨이갯수 %d, 현재 웨이: %d", Character.m_pRgMage[0]->m_pTeam->nWPCnt,Character.m_pRgMage[0]->m_pTeam->nWpNum );
	TextOut(hdc, 200, 30, str, strlen(str));

	for( int i=0; i<Stage.aWayCnt[0]-1; i++ )
	{
		MoveToEx(hdc, Stage.aWayX[0][i], Stage.aWayY[0][i], NULL);
		LineTo(hdc, Stage.aWayX[0][i+1], Stage.aWayY[0][i+1]);
	}

	wsprintf(str, "캐릭터 총 수 %d", Stage.nCharTotal);
	TextOut(hdc, 500, 181, str, strlen(str));
	wsprintf(str, "현재 로딩된 캐릭터 총 수 %d", Character.m_nLoadedCharCnt);
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

/*	맵 & 0x00000001 = 충돌체크 , 
	맵 & 0x00000002 = Object 유무 , 
	맵 & 0x00000004 = 반투명 유무 , 
	맵 & 0x00000008 = Blank유무 , 
	맵 & 0x00000010 = 적캐릭 타일, 
	>> 8 = 미니맵 유무  
	>> 16 = 이벤트 유무 */
		
	DxDraw.GetOSurface()->ReleaseDC(hdc);
}

void EtcGameProcess(void)
{
	switch(Interface.GetGameMode())
	{
	case EXIT_GAME:
		PostMessage(hMainWnd, WM_CLOSE, 0, 0);		// 프로그램 종료 처리
		break;
	default:
		Interface.EtcGame(pScreen, 800, &DxDraw);	// 본 게임 플레이 모드 외의 잡다한 게임 모드...
		break;
	}
}