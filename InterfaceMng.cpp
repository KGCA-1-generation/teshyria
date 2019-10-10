// InterfaceMng.cpp: implementation of the CInterfaceMng class.
//
//////////////////////////////////////////////////////////////////////

#include <ddraw.h>
#include "Dx7Draw.h"
#include "InterfaceMng.h"
#include "ItfDefine.h"
#include "Image.h"
#include "DM6Mp3.h"
#include "DM6Show.h"
#include "IDdata.h"
#include "Animation.h"
#include "MainChar.h"


#define SAFETY_DELETE(p)	{ if(p != NULL) { delete (p); (p)=NULL; } }
#define SWAP(a,b)			{ (a) ^= (b) ^= (a) ^= (b); }
#define SCREEN_WIDTH		800    // 스크린 가로 해상도
#define SCREEN_HEIGHT		600    // 스크린 세로 해상도

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CInterfaceMng::CInterfaceMng()
{
	Init();
}

CInterfaceMng::~CInterfaceMng()
{
	SAFETY_DELETE(m_pImgBackGround);
	SAFETY_DELETE(m_pImgExtra1);
	SAFETY_DELETE(m_pImgExtra2);
	SAFETY_DELETE(m_pImgExtra3);
	SAFETY_DELETE(m_pImgExtra4);
	SAFETY_DELETE(m_pImgExtra5);
	SAFETY_DELETE(m_pImgExtra6);
	SAFETY_DELETE(m_pAniMystery);

	SAFETY_DELETE(m_pBgm_mng);

	SAFETY_DELETE(m_pDMShow);
}

//////////////   void Init(void)   //////////////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::Init(void)
{
	m_pImgBackGround = NULL;
	m_pImgExtra1	 = NULL;
	m_pImgExtra2	 = NULL;
	m_pImgExtra3	 = NULL;
	m_pImgExtra4	 = NULL;
	m_pImgExtra5	 = NULL;
	m_pImgExtra6	 = NULL;
	m_pAniMystery	 = NULL;

	m_ImgTime.LoadSpr("DATA\\Interface\\Spr\\Time.spr");
	m_ImgEscMenu.LoadSpr("DATA\\Interface\\Spr\\EscMenu.spr");
	m_ImgEscBtn.LoadSpr("DATA\\Interface\\Spr\\EscBtn.spr"); 
	m_ImgCursor.LoadSpr("DATA\\Interface\\Spr\\PreCursor.spr"); 
	m_ImgStatus.LoadSpr("DATA\\Interface\\Spr\\Status.spr");
	m_ImgButton.LoadSpr("DATA\\Interface\\Spr\\Button.spr");

	m_ImgStatusWin.LoadSpr("DATA\\Interface\\Spr\\StatusWin.spr");		// 선미가 추가한 코딩 5:

	m_pBgm_mng = new CMp3Manager(1);

	m_pDMShow = NULL;

	m_pScr = NULL;

	m_pBgm[0]  = "DATA\\Interface\\Bgm\\MainMenu.mp3";
	m_pBgm[1]  = "DATA\\Interface\\Bgm\\Endding.mp3";
	m_pBgm[2]  = "DATA\\Interface\\Bgm\\PreMission.mp3";
	m_pBgm[3]  = "DATA\\Interface\\Bgm\\Play_1_1.mp3";
	m_pBgm[4]  = "DATA\\Interface\\Bgm\\Play_1_2.mp3";
	m_pBgm[5]  = "DATA\\Interface\\Bgm\\Play_2_1.mp3";
	m_pBgm[6]  = "DATA\\Interface\\Bgm\\Play_2_2.mp3";
	m_pBgm[7]  = "DATA\\Interface\\Bgm\\Play_3_1.mp3";
	m_pBgm[8]  = "DATA\\Interface\\Bgm\\Play_3_2.mp3";
	m_pBgm[9]  = "DATA\\Interface\\Bgm\\Succeed.mp3";
	m_pBgm[10] = "DATA\\Interface\\Bgm\\Fail.mp3";
	
	m_nGameMode = OPENNING_MOVIE;

	m_bNowEvent = false;

	m_bMouseLB	 = false;
	m_bKeyESC    = false;
	m_bKeyReturn = false;
	m_bKeyNext	 = false;
	m_bKeyCancel = false;

	SetCursor(CURSOR_MENU_NORMAL);
	m_nAddCount = 0;
	m_nCursorCount = m_nCursor + m_nAddCount;	

	m_nStep = 0;
	m_nFadeRate = FADE_MAX;
	m_wFadeColor = 0;

	m_nPointBtn = -1;

	m_dwTime = m_dwNowTime = m_dwOpenTime = timeGetTime();

	m_nPreBgmVolum = m_nBgmVolum = 6;	
	m_nSoundVolum = m_nSoundVolum = 6;

	m_bChangeBgm = true;
	m_bChangeVolum = false;

	for(int i = 0; i<NAME_MAX; i++)
	{
		m_szInputName[i] = '\0';
		m_szDeleteName[i] = '\0';
	}
	m_nCountName = 0;
	for(int num = 0; num < KEY_KIND; num++)
		m_bKeyConsent[num] = true;

	m_ID.LoadIDdata("DATA\\Interface\\User\\User.data");

	m_nSelectID = m_ID.GetTotalNumID() - 1;

	m_bFinish = false;

	m_nSceneScrollY = SCENE_BOTTOM;

	m_nCountAniMys1 = m_nCountAniMys2 = m_nCountAniMys3 = m_nCountAniMys4 = 0;

	InitScroll();

	m_nPlayMin = 0;		
	m_nPlaySec = 0;		

	m_nScore = 0;
	m_nScoreTotal= 0;

	m_nRankSt = 0;
	m_nRankTotalSt = 0;

	m_nSameRank = 1;	
	m_nSameRankTotal = 1;

	for(int j=0; j<ID_MAX; j++)
	{
		m_Rank[j].nScore = 0;
		m_RankTotal[j].nScore = 0;
		if(j == m_nSelectID)
		{
			m_Rank[j].bPlayingNow = true;
			m_RankTotal[j].bPlayingNow = true;
		}
		else
		{
			m_Rank[j].bPlayingNow = false;
			m_RankTotal[j].bPlayingNow = false;
		}
		for(int k=0; k<NAME_MAX; k++)
		{
			m_Rank[j].szName[k] = '\0';		
			m_RankTotal[j].szName[k] = '\0';
		}
	}
}

//////////////   void InitPlay(void)   //////////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::InitPlay(void)
{
	SAFETY_DELETE(m_pImgBackGround);
	m_pImgBackGround = new CImage;

	SAFETY_DELETE(m_pImgExtra1);
	SAFETY_DELETE(m_pImgExtra2);
	SAFETY_DELETE(m_pImgExtra3);
	SAFETY_DELETE(m_pImgExtra4);
	SAFETY_DELETE(m_pImgExtra5);
	SAFETY_DELETE(m_pImgExtra6);
	SAFETY_DELETE(m_pAniMystery);

	m_bNowEvent = false;

	m_bMouseLB	 = false;
	m_bKeyESC    = false;
	m_bKeyReturn = false;
	m_bKeyNext	 = false;
	m_bKeyCancel = false;

	m_nAddCount = 0;
	m_nCursorCount = m_nCursor + m_nAddCount;	

	m_nStep = 0;
	m_nFadeRate = FADE_MAX;

	m_nPointBtn = -1;

	m_dwTime = m_dwNowTime = m_dwOpenTime = timeGetTime();

	m_bChangeVolum = false;

	m_nWeaponClass = WP_NONE;
	m_nArmorClass = AM_NONE;
	m_nHPotionNum = 0;			
	m_nMPotionNum = 0;			
	m_nSPotionNum = 0;	

	m_nWeight = 0;
	m_nWeightItem = 0;

	for(int i = 0; i<NAME_MAX; i++)
	{
		m_szInputName[i] = '\0';
		m_szDeleteName[i] = '\0';
	}
	m_nCountName = 0;
	for(int num = 0; num < KEY_KIND; num++)
		m_bKeyConsent[num] = true;

	m_bFinish = false;

	m_nSceneScrollY = SCENE_BOTTOM;

	m_nCountAniMys1 = m_nCountAniMys2 = m_nCountAniMys3 = m_nCountAniMys4 = 0;

	InitScroll();

	switch(m_nGameMode)
	{
	case PLAYING_1_1:
		m_nResult = 0;
		m_bLimitTime = true;
		m_nStartMin = m_nLimitMin = LIMIT_MIN_1;
		m_nStartSec = m_nLimitSec = LIMIT_SEC_1;
		break;
	case PLAYING_1_2:
		m_nResult = 0;
		m_bLimitTime = false;
		m_nAliveFriends = MAX_FRIEND_1;
		break;
	case PLAYING_2_1:
		m_nResult = 0;
		m_bLimitTime = true;
		m_nStartMin = m_nLimitMin = LIMIT_MIN_2;
		m_nStartSec = m_nLimitSec = LIMIT_SEC_2;
		break;
	case PLAYING_2_2:
		m_nResult = 0;
		m_bLimitTime = false;
		m_nAliveFriends = MAX_FRIEND_2;
		break;
	case PLAYING_3_1:
		m_nResult = 0;
		m_bLimitTime = false;
		m_nAliveFriends = MAX_FRIEND_3;
		break;
	case PLAYING_3_2:
		m_nResult = 0;
		m_bLimitTime = true;
		m_nStartMin = m_nLimitMin = LIMIT_MIN_3;
		m_nStartSec = m_nLimitSec = LIMIT_SEC_3;
		break;
//	default:
//		m_bLimitTime = false;
	}

	m_nPlayMin = 0;		
	m_nPlaySec = 0;		

	m_nScore = 0;
	m_nScoreTotal = 0;

	m_nRankSt = 0;
	m_nRankTotalSt = 0;

	m_nSameRank = 1;	
	m_nSameRankTotal = 1;

	for(int j=0; j<ID_MAX; j++)
	{
		m_Rank[j].nScore = 0;
		m_RankTotal[j].nScore = 0;
		if(j == m_nSelectID)
		{
			m_Rank[j].bPlayingNow = true;
			m_RankTotal[j].bPlayingNow = true;
		}
		else
		{
			m_Rank[j].bPlayingNow = false;
			m_RankTotal[j].bPlayingNow = false;
		}
		for(int k=0; k<NAME_MAX; k++)
		{
			m_Rank[j].szName[k] = '\0';		
			m_RankTotal[j].szName[k] = '\0';
		}
	}
}

//////////////   void SetCursorFrame(void)   ////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetCursorFrame(void)
{
	m_nCursorCount = m_nCursor + m_nAddCount;	
	
//	m_dwTime = timeGetTime() - m_dwNowTime;
//	if( m_dwTime > CURSOR_DELAY )  
	if( timeGetTime() - m_dwNowTime > CURSOR_DELAY )
	{
		m_nAddCount++;
		if(m_nAddCount >= CURS_FRAME)
			m_nAddCount = 0;
		m_dwNowTime = timeGetTime();
	}
}

//////////////   void SetKeyESC(bool bPush)   ///////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetKeyESC(bool bPush)       
{
	if(m_nGameMode % 10 == 5   &&   m_nGameMode >= PLAYING_1_1   &&   m_nGameMode <= PLAYING_3_2) 
	{		
		m_bKeyESC = bPush;	
		if(m_bKeyESC)
		{
			m_nPreBgmVolum = m_nBgmVolum;
			m_nPreSoundVolum = m_nSoundVolum;
		}
		else
		{
			m_nBgmVolum = m_nPreBgmVolum;
			m_nSoundVolum = m_nPreSoundVolum;
			InitScroll();
			m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
		}		
	}
}

//////////////    void PushChar(char name)    ////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::PushChar(char name, int num)
{
	m_szInputName[m_nCountName] = name; 
	m_nCountName++; 
	m_bKeyConsent[num] = false;
}
//////////////   void BackSpace(void)    /////////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::BackSpace(void)
{
	for(int i=m_nCountName; i<NAME_MAX; i++)
		m_szInputName[i - 1] = m_szInputName[i]; 
	m_nCountName --;

	m_bKeyConsent[36] = false;

}
/////////////    void SetKeyConsent(bool bPush, int num)    //////////////////////////////////////////////////////////////
void CInterfaceMng::SetKeyConsent(bool bPush, int num)
{
	m_bKeyConsent[num] = true;
}

//////////////   void SetGameMode(int nMode, CDx7Draw* DxDraw)   //////////////////////////////////////////////////////////
void CInterfaceMng::SetGameMode(int nMode, CDx7Draw* DxDraw)
{
	if( (nMode >= SHOW_MAIN_MENU   &&   nMode <= SHOW_CREATER  &&   m_nGameMode >= SHOW_MAIN_MENU   &&   m_nGameMode <= SHOW_CREATER)  ||
		(nMode >= SHOW_1_STORY_OBJ  &&  nMode <= SELECT_ITEM_1_1)   ||
		(nMode % 20 >= 2   &&    nMode % 20 <= 3   &&   nMode >= SHOW_1_2_MISSION_OBJ   &&  nMode <= SELECT_ITEM_3_2)   ||
		(nMode % 20 >= 11  &&    nMode % 20 <= 13  &&   nMode >= SHOW_2_1_MISSION_LOGO  &&  nMode <= SELECT_ITEM_3_1) )
		m_bChangeBgm = false;
	else
		m_bChangeBgm = true;

	m_nGameMode = nMode;
	InitPlay();

	// 메인메뉴 화면...
	if(nMode == SHOW_MAIN_MENU)
	{	
		SAFETY_DELETE(m_pDMShow);

		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\MainMenu.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\MainBtn.spr");
		if(m_bChangeBgm) 
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_MAINMENU] );
	}
	// 아이디 메뉴 화면...
	else if(nMode == START_ID_MENU)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\IDMenu.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\IDbtn.spr");
		m_pImgExtra2 = new CImage;
		m_pImgExtra2->LoadSpr("DATA\\Interface\\Spr\\IDMark.spr");
	}
	// 본 게임 플레이 화면...
	else if(nMode % 10 == 5   &&   nMode >= PLAYING_1_1   &&   nMode <= PLAYING_3_2) 
	{
		switch(nMode)
		{
		case PLAYING_1_1:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_1_1] );
			break;
		case PLAYING_1_2:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_1_2] );
			break;
		case PLAYING_2_1:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_2_1] );
			break;
		case PLAYING_2_2:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_2_2] );
			break;
		case PLAYING_3_1:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_3_1] );
			break;
		case PLAYING_3_2:
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_3_2] );
			break;
		}
	}
	// 오프닝 무비 화면...
	else if(nMode == OPENNING_MOVIE)
	{	
		m_pDMShow = new CDM6Show;
		m_pDMShow->PrepareMovie(DxDraw->GetDDrawPtr(), 
								DxDraw->GetPSurface(), 
								"DATA\\Interface\\Movie\\intro.avi");							 
		m_pDMShow->SetBlitPos(100, 100, 700, 500);
	}
	// 제작진 보여주기 화면...
	else if(nMode == SHOW_CREATER)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Creater.spr");
	}
	// 옵션 메뉴 화면...
	else if(nMode == SHOW_OPTION_MENU)
	{
		m_nPreBgmVolum = m_nBgmVolum;
		m_nPreSoundVolum = m_nSoundVolum;
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Option.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\VolumBar.spr");
	}
	// 기본 배경 설명 화면...
	else if(nMode == SHOW_BASIC_SCENE)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\BasicScene.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\BasicText.spr");
		if(m_bChangeBgm) 
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_PRE_MISSION] );
	}
	// 스토리 설명 및 목적 화면...
	else if(nMode >= SHOW_1_STORY_OBJ   &&   nMode <= SHOW_3_STORY_OBJ   &&   nMode % 10 == 0)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Story.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\StoryTitle.spr");
		m_pImgExtra2 = new CImage;
		m_pImgExtra2->LoadSpr("DATA\\Interface\\Spr\\StoryArea.spr");
		m_pImgExtra3 = new CImage;
		m_pImgExtra3->LoadSpr("DATA\\Interface\\Spr\\StoryText.spr");
		if(m_bChangeBgm) 
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_PRE_MISSION] );
	}
	// 각 미션 로고...
	else if(nMode >= SHOW_1_1_MISSION_LOGO   &&  nMode <= SHOW_3_2_MISSION_LOGO   &&   nMode % 10 == 1)
	{
		switch(nMode)
		{
		case SHOW_1_1_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo1_1.spr");
			break;
		case SHOW_1_2_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo1_2.spr");
			break;
		case SHOW_2_1_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo2_1.spr");
			break;
		case SHOW_2_2_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo2_2.spr");
			break;
		case SHOW_3_1_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo3_1.spr");
			break;
		case SHOW_3_2_MISSION_LOGO:
			m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Logo3_2.spr");
			break;
		}
		if(m_bChangeBgm) 
			m_pBgm_mng->PlayBGM( m_pBgm[BGM_PRE_MISSION] );
	}
	// 각 미션 목적 및 설명 화면...
	else if(nMode >= SHOW_1_1_MISSION_OBJ   &&   nMode <= SHOW_3_2_MISSION_OBJ   &&	  nMode % 10 == 2)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Mission.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\MissionTitle.spr");
		m_pImgExtra2 = new CImage;
		m_pImgExtra2->LoadSpr("DATA\\Interface\\Spr\\MissionText.spr");
		m_pImgExtra3 = new CImage;
		m_pImgExtra3->LoadSpr("DATA\\Interface\\Spr\\VicObject.spr");
		m_pImgExtra4 = new CImage;
		m_pImgExtra4->LoadSpr("DATA\\Interface\\Spr\\FailObject.spr");
	}
	// 아이템 선택 화면...
	else if(m_nGameMode >= SELECT_ITEM_1_1	&&	m_nGameMode <= SELECT_ITEM_3_2   &&   m_nGameMode % 10 == 3)
	{      
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\ItemBack.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\SmallWeapon.spr");
		m_pImgExtra2 = new CImage;
		m_pImgExtra2->LoadSpr("DATA\\Interface\\Spr\\SmallArmor.spr");
		m_pImgExtra3 = new CImage;
		m_pImgExtra3->LoadSpr("DATA\\Interface\\Spr\\ItemCaption.spr");
		m_pImgExtra4 = new CImage;
		m_pImgExtra4->LoadSpr("DATA\\Interface\\Spr\\UpDownBtn.spr");
		m_pImgExtra5 = new CImage;
		m_pImgExtra5->LoadSpr("DATA\\Interface\\Spr\\Weapon.spr");
		m_pImgExtra6 = new CImage;
		m_pImgExtra6->LoadSpr("DATA\\Interface\\Spr\\Armor.spr");
		m_pAniMystery = new CAnimation;
		m_pAniMystery->LoadAni("DATA\\Interface\\Ani\\Mystery.ani");
	}
	// 로딩 화면...
	else if(m_nGameMode >= LOADING_1_1	&&	m_nGameMode <= LOADING_3_2   &&   m_nGameMode % 10 == 4)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\Loading.spr");
	}
	// 미션 성공 결과 화면...
	else if(m_nGameMode % 10 == 7  &&  m_nGameMode >= SUCCEED_1_1  &&  m_nGameMode <= SUCCEED_3_2)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\VicResult.spr");
		m_pBgm_mng->PlayBGM( m_pBgm[BGM_SUCCEED] );

		if(m_bLimitTime)
			m_nScore = 100 + (m_nStartMin*60 + m_nStartSec) - (m_nPlayTimeMin*60 + m_nPlayTimeSec) - GetWasteHPotion()*3 - GetWasteMPotion()*3 - GetWasteSPotion()*2 ;
		else
			m_nScore = 100 + m_nAliveFriends*4 - m_nPlayTimeMin*60 - m_nPlayTimeSec - GetWasteHPotion()*3 - GetWasteMPotion()*3 - GetWasteSPotion()*2 ;
		
		int i, j, k;
		for(i = 0; i < m_ID.GetTotalNumID(); i++)
		{
			if(i == m_nSelectID)
			{
				m_Rank[i].bPlayingNow = true;
				m_RankTotal[i].bPlayingNow = true;

				m_Rank[i].nScore = m_nScore;
				for(j = 0; j <= m_nGameMode/10 - 1; j++)
					m_RankTotal[i].nScore += m_ID.GetIDScore(i)[j];
				m_RankTotal[i].nScore += m_nScore;

				m_nScoreTotal = m_RankTotal[i].nScore;
			}
			else
			{
				m_Rank[i].nScore = m_ID.GetIDScore(i)[m_nGameMode/10 - 1];
				for(j = 0; j <= m_nGameMode/10 - 1; j++)
					m_RankTotal[i].nScore += m_ID.GetIDScore(i)[j];
			}
			for(j = 0; j<NAME_MAX; j++)
			{
				m_Rank[i].szName[j] = m_ID.GetIDName(i)[j];
				m_RankTotal[i].szName[j] = m_ID.GetIDName(i)[j];
			}
		}
		for(i=0; i<m_ID.GetTotalNumID(); i++)  // 랭킹 결정
		{
			for(j=0; j<m_ID.GetTotalNumID()-i; j++)
			{
				if(m_Rank[j].nScore < m_Rank[j+1].nScore)
				{
					SWAP(m_Rank[j].nScore, m_Rank[j+1].nScore);
					SWAP(m_Rank[j].bPlayingNow, m_Rank[j+1].bPlayingNow);
					for(k=0; k<NAME_MAX; k++)
						SWAP(m_Rank[j].szName[k], m_Rank[j+1].szName[k]);
				}
				if(m_RankTotal[j].nScore < m_RankTotal[j+1].nScore)
				{
					SWAP(m_RankTotal[j].nScore, m_RankTotal[j+1].nScore);
					SWAP(m_RankTotal[j].bPlayingNow, m_RankTotal[j+1].bPlayingNow);
					for(k=0; k<NAME_MAX; k++)
						SWAP(m_RankTotal[j].szName[k], m_RankTotal[j+1].szName[k]); 
				}
			}
		}
	}
	// 미션 실패 결과 화면...
	else if(m_nGameMode % 10 == 6  &&  m_nGameMode >= FAIL_1_1  &&  m_nGameMode <= FAIL_3_2)
	{
		m_pImgBackGround->LoadSpr("DATA\\Interface\\Spr\\VicResult.spr");
		m_pImgExtra1 = new CImage;
		m_pImgExtra1->LoadSpr("DATA\\Interface\\Spr\\FailResultTitle.spr");
		m_pImgExtra2 = new CImage;
		m_pImgExtra2->LoadSpr("DATA\\Interface\\Spr\\FailBack.spr");
		m_pBgm_mng->PlayBGM( m_pBgm[BGM_FAIL] );
	}

	//----------------------------------------------------------------------------------------
	m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
}

//////////////   void SetInputSts(int nX, int nY, int nInput)   ///////////////////////////////////////////////////////////
void CInterfaceMng::SetInputSts(int nX, int nY, int nInput)
{	
	m_nMousePosX = nX;
	m_nMousePosY = nY;

	m_bMouseLB	 =  (nInput == 3 ? true : false);
	
	m_bKeyCancel =  (nInput == 101 ? true : false);	

	if(m_nGameMode % 10 == 3   &&   m_nGameMode >= SELECT_ITEM_1_1	&&	m_nGameMode <= SELECT_ITEM_3_2   &&   m_nWeaponClass == WP_NONE   &&   m_nArmorClass == AM_NONE)
		m_bKeyReturn = false;
	else
		m_bKeyReturn = (nInput == 100 ? true : false); 

	if(m_bMouseLB  ||  m_bKeyCancel   ||  m_bKeyReturn  ||  (nInput == 7) )
		m_bKeyNext = true;
	else
		m_bKeyNext = false;

	m_nPointBtn = -1;

	SetCursorFrame();

	//---------[ 오프닝 동영상 ] 화면 처리 -------------------------------
	if(m_nGameMode == OPENNING_MOVIE)
	{	
		if(m_bKeyNext == true)	// 유저가 클릭 및 키를 눌리면 다음 화면으로 전환
		{
			SetGameMode(m_nGameMode + 2);
		}
	}
	//---------[ 메인메뉴 ] 화면에서의 처리 ------------------------------
	else if(m_nGameMode == SHOW_MAIN_MENU)						
	{
		if(m_nMousePosX >= BTN_START_L  &&
		   m_nMousePosX <= BTN_START_R  &&
		   m_nMousePosY >= BTN_START_T  &&
		   m_nMousePosY <= BTN_START_B)						// START 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = START_ID_MENU;
			}
		}
		else if(m_nMousePosX >= BTN_OPT_L  &&
				m_nMousePosX <= BTN_OPT_R  &&
				m_nMousePosY >= BTN_OPT_T  &&	
				m_nMousePosY <= BTN_OPT_B)					// 옵션 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_OPTION_MENU;
			}
		}
		else if(m_nMousePosX >= BTN_CREATER_L  &&
				m_nMousePosX <= BTN_CREATER_R  &&
				m_nMousePosY >= BTN_CREATER_T  &&
				m_nMousePosY <= BTN_CREATER_B)				// 제작자들 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 2;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_CREATER;
			}
		}
		else if(m_nMousePosX >= BTN_EXIT_L  &&
				m_nMousePosX <= BTN_EXIT_R  &&
				m_nMousePosY >= BTN_EXIT_T  &&	
				m_nMousePosY <= BTN_EXIT_B)					// 나가기 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 3;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = EXIT_GAME;
			}
		}
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyCancel)
		{
			m_nStep = 2;
			m_nReadyGameMode = EXIT_GAME;
		}
	}
	//--------[ 제작진 보여주기 ] 화면 처리-----------------------------------
	else if(m_nGameMode == SHOW_CREATER)
	{
		if(m_nMousePosX >= BTN_CANCEL_L  &&
		   m_nMousePosX <= BTN_CANCEL_R  &&
		   m_nMousePosY >= BTN_CANCEL_T  &&
		   m_nMousePosY <= BTN_CANCEL_B)						// [확인] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;
			}
		}
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyNext == true  &&  m_bMouseLB == false)
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;
		}
	}
	//---------[ 옵션 메뉴 ] 화면 처리 ---------------------------------------
	else if(m_nGameMode == SHOW_OPTION_MENU)
	{
		if(m_nMousePosX >= BTN_OK_L  &&
		   m_nMousePosX <= BTN_OK_R  &&
		   m_nMousePosY >= BTN_OK_T  &&
		   m_nMousePosY <= BTN_OK_B)							// [확인] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;
			}
		}
		else if(m_nMousePosX >= BTN_CANCEL_L  &&
			    m_nMousePosX <= BTN_CANCEL_R  &&
			    m_nMousePosY >= BTN_CANCEL_T  &&
			    m_nMousePosY <= BTN_CANCEL_B)					// [돌아가기] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				m_nBgmVolum = m_nPreBgmVolum;
				m_nSoundVolum = m_nPreSoundVolum;
				m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;
			}
		}
		else if(m_nMousePosY >= BGM_TOP  &&  m_nMousePosY <= BGM_TOP + VOLUM_HEIGHT)
		{
			 if(m_nMousePosX >= BTN_VOLUM_0	 &&
				m_nMousePosX <= BTN_VOLUM_0 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 0;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_1	 &&
					 m_nMousePosX <= BTN_VOLUM_1 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 1;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_2	 &&
					 m_nMousePosX <= BTN_VOLUM_2 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 2;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_3	 &&
					 m_nMousePosX <= BTN_VOLUM_3 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 3;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_4	 &&
					 m_nMousePosX <= BTN_VOLUM_4 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 4;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_5	 &&
					 m_nMousePosX <= BTN_VOLUM_5 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 5;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_6	 &&
					 m_nMousePosX <= BTN_VOLUM_6 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nBgmVolum = 6;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
				}
			 }
			 else
				SetCursor(CURSOR_MENU_NORMAL);
		}	
		else if(m_nMousePosY >= SOUND_TOP  &&  m_nMousePosY <= SOUND_TOP + VOLUM_HEIGHT)
		{
			 if(m_nMousePosX >= BTN_VOLUM_0	 &&
				m_nMousePosX <= BTN_VOLUM_0 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 0;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_1	 &&
					 m_nMousePosX <= BTN_VOLUM_1 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 1;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_2	 &&
					 m_nMousePosX <= BTN_VOLUM_2 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 2;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_3	 &&
					 m_nMousePosX <= BTN_VOLUM_3 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 3;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_4	 &&
					 m_nMousePosX <= BTN_VOLUM_4 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 4;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_5	 &&
					 m_nMousePosX <= BTN_VOLUM_5 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 5;
				}
			 }
			 else if(m_nMousePosX >= BTN_VOLUM_6	 &&
					 m_nMousePosX <= BTN_VOLUM_6 + VOLUM_WIDTH)
			 {
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					m_nSoundVolum = 6;
				}
			 }
			 else
				SetCursor(CURSOR_MENU_NORMAL);
		}		
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn)
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;
		}
		else if(m_bKeyCancel)
		{
			m_nBgmVolum = m_nPreBgmVolum;
			m_nSoundVolum = m_nPreSoundVolum;
			m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;
		}
	}
	//-----------[ ID 메뉴 ] 화면 처리 ---------------------------------
	else if(m_nGameMode == START_ID_MENU)
	{
		int num = ((m_nMousePosY - BTN_SELECT_T) / BTN_SELECT_HEIGHT % 12) + ((m_nMousePosX - BTN_SELECT_L) / BTN_SELECT_WIDTH * 12);

		if(m_nMousePosX >= BTN_OK_L  &&
		   m_nMousePosX <= BTN_OK_R  &&
		   m_nMousePosY >= BTN_OK_T  &&
		   m_nMousePosY <= BTN_OK_B  &&
		   m_nSelectID < ID_MAX)							// [OK] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = m_ID.GetIDSaveMission(m_nSelectID);
			}
		}
		else if(m_nMousePosX >= BTN_CANCEL_L  &&
			    m_nMousePosX <= BTN_CANCEL_R  &&
			    m_nMousePosY >= BTN_CANCEL_T  &&
			    m_nMousePosY <= BTN_CANCEL_B)				// [BACK] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;
			}
		}
		else if(m_nMousePosX >= BTN_CREATE_L  &&
			    m_nMousePosX <= BTN_CREATE_R  &&
			    m_nMousePosY >= BTN_CREATE_T  &&
			    m_nMousePosY <= BTN_CREATE_B  &&
				m_ID.GetTotalNumID() < ID_MAX &&
				m_nCountName > 0)							// [CREATE] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 2;
			if(m_bMouseLB)
			{
				m_ID.CreateID(m_szInputName);
				for(int i = 0; i<NAME_MAX; i++)
					m_szInputName[i] = '\0';
				m_nCountName = 0;
				m_ID.SaveIDdata("DATA\\Interface\\User\\User.data");
				m_nSelectID = m_ID.GetTotalNumID() - 1;
			}
		}
		else if(m_nMousePosX >= BTN_DELETE_L  &&
			    m_nMousePosX <= BTN_DELETE_R  &&
			    m_nMousePosY >= BTN_DELETE_T  &&
			    m_nMousePosY <= BTN_DELETE_B  &&
				m_ID.GetTotalNumID() > 0	  &&
				m_nSelectID < ID_MAX)						// [DELETE] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 3;
			if(m_bMouseLB)
			{
				m_ID.DeleteID(m_nSelectID);
				for(int i = 0; i<NAME_MAX; i++)
					m_szDeleteName[i] = '\0';
				if(m_nSelectID == m_ID.GetTotalNumID())
					m_nSelectID = m_ID.GetTotalNumID() - 1;
				m_ID.SaveIDdata("DATA\\Interface\\User\\User.data");
			}
		}
		else if(num >= 0   &&    num < ID_MAX   &&   m_ID.GetTotalNumID() > num)		// 등록된 아이디
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			if(m_bMouseLB)
			{
				m_nSelectID = num;
				for(int i = 0; i<NAME_MAX; i++)
					m_szDeleteName[i] = m_ID.GetIDName(m_nSelectID)[i];
			}
		}
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn && m_nSelectID < ID_MAX)
		{
			m_nStep = 2;
			m_nReadyGameMode = m_ID.GetIDSaveMission(m_nSelectID);
		}
		else if(m_bKeyCancel)
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;
		}
	}
	//-----------[ 기본 배경 설명 ] 화면 처리 --------------------------------
	else if(m_nGameMode == SHOW_BASIC_SCENE)				
	{
		if(m_nMousePosX >= BTN_CANCEL_L  &&
		   m_nMousePosX <= BTN_CANCEL_R  &&
		   m_nMousePosY >= BTN_CANCEL_T  &&
		   m_nMousePosY <= BTN_CANCEL_B)						// [확인] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_1_STORY_OBJ;
			}
		}
		else
		{
			if(m_bFinish)
				SetCursor(CURSOR_MENU_NORMAL);
			else
				SetCursor(CURSOR_WAIT);				
		}

		if(m_bKeyReturn)
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_1_STORY_OBJ;
		}
	}
	//--------[ 스토리/미션  설명 및 목적 ] 보이기 화면 처리 -------------------
	else if( (m_nGameMode % 10 == 0  ||  m_nGameMode % 10 == 2)  &&   m_nGameMode >= SHOW_1_STORY_OBJ    &&    m_nGameMode <= SHOW_3_2_MISSION_OBJ)
	{
		if(m_nMousePosX >= BTN_CANCEL_L  &&
		   m_nMousePosX <= BTN_CANCEL_R  &&
		   m_nMousePosY >= BTN_CANCEL_T  &&
		   m_nMousePosY <= BTN_CANCEL_B)						// [다음] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode + 1;
			}				
		}
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn)	// Enter 키
		{
			m_nStep = 2;
			m_nReadyGameMode = m_nGameMode + 1;
		}
	}	
	//---------[ 미션 로고 ] 화면 처리 -----------------------------------------
	else if( m_nGameMode % 10 == 1   &&	 m_nGameMode >= SHOW_1_1_MISSION_LOGO   &&  m_nGameMode <= SHOW_3_2_MISSION_LOGO)
	{		
		SetCursor(CURSOR_ABOVE_BUTTON);
//		m_dwNowTime = timeGetTime();
//		if( ( (m_dwNowTime - m_dwOpenTime >= DELAY_MODE)  &&  m_bFinish == false)  ||  m_bKeyNext == true)	// 디레이가 끝나거나(자동전환) 유저가 클릭 및 키를 눌리면 다음 화면으로 전환
		if( ( (timeGetTime() - m_dwTime >= DELAY_MODE)  &&  m_bFinish == false)  ||  m_bKeyNext == true)	// 디레이가 끝나거나(자동전환) 유저가 클릭 및 키를 눌리면 다음 화면으로 전환
		{
			m_nStep = 2;
			m_nReadyGameMode = m_nGameMode + 1;
			m_bFinish = true;
		}
	}
	//----------[ 아이템 선택 메뉴 ] 화면 처리 ---------------------------
	else if(m_nGameMode % 10 == 3   &&   m_nGameMode >= SELECT_ITEM_1_1	&&	m_nGameMode <= SELECT_ITEM_3_2)   
	{
		if(m_nMousePosX >= BTN_OK_L  &&
		   m_nMousePosX <= BTN_OK_R  &&
		   m_nMousePosY >= BTN_OK_T  &&
		   m_nMousePosY <= BTN_OK_B	 &&
		   m_nWeaponClass != WP_NONE &&
		   m_nArmorClass != AM_NONE)						// [다음] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 100;
			if(m_bMouseLB)
			{
				m_nHPotionStart = m_nHPotionNum;
				m_nMPotionStart = m_nMPotionNum;
				m_nSPotionStart = m_nSPotionNum;
				m_nHPotionNow   = m_nHPotionStart;	
				m_nMPotionNow   = m_nMPotionStart;	
				m_nSPotionNow   = m_nSPotionStart;
				m_nWeightStart  = m_nWeight;
				m_nWeaponStart  = m_nWeaponClass;	
				m_nArmorStart	= m_nArmorClass;

				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode + 1;
			}
		}
		else if(m_nMousePosX >= BTN_CANCEL_L  &&
			    m_nMousePosX <= BTN_CANCEL_R  &&
			    m_nMousePosY >= BTN_CANCEL_T  &&
			    m_nMousePosY <= BTN_CANCEL_B)				// [돌아가기] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 101;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode - 1;
			}
		}

		else if(m_nMousePosX >= BTN_WP_1_L	&&
			    m_nMousePosX <= BTN_WP_1_R	&&
			    m_nMousePosY >= BTN_WP_1_T	&&
			    m_nMousePosY <= BTN_WP_1_B)					// 무기중의 [레이피어] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_RAIPEAR);
			}
		}
		else if(m_nMousePosX >= BTN_WP_2_L	&&
			    m_nMousePosX <= BTN_WP_2_R	&&
			    m_nMousePosY >= BTN_WP_2_T	&&
			    m_nMousePosY <= BTN_WP_2_B)					// 무기중의 [에페] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_EPEE);
			}
		}
		else if(m_nMousePosX >= BTN_WP_3_L	&&
			    m_nMousePosX <= BTN_WP_3_R	&&
			    m_nMousePosY >= BTN_WP_3_T	&&
			    m_nMousePosY <= BTN_WP_3_B)					// 무기중의 [글라디우스] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 2;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_GLADIUS);
			}
		}
		else if(m_nMousePosX >= BTN_WP_4_L	&&
			    m_nMousePosX <= BTN_WP_4_R	&&
			    m_nMousePosY >= BTN_WP_4_T	&&
			    m_nMousePosY <= BTN_WP_4_B)					// 무기중의 [롱소드] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 3;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_LONG);
			}
		}
		else if(m_nMousePosX >= BTN_WP_5_L	&&
			    m_nMousePosX <= BTN_WP_5_R	&&
			    m_nMousePosY >= BTN_WP_5_T	&&
			    m_nMousePosY <= BTN_WP_5_B)					// 무기중의 [바스타드] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 4;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_BASTARD);
			}
		}
		else if(m_nMousePosX >= BTN_WP_6_L	&&
			    m_nMousePosX <= BTN_WP_6_R	&&
			    m_nMousePosY >= BTN_WP_6_T	&&
			    m_nMousePosY <= BTN_WP_6_B)					// 무기중의 [투핸드소드] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 5;
			if(m_bMouseLB)
			{
				InitItem(0);
				SetWeaponClass(WP_TWOHAND);
			}
		}
		else if(m_nMousePosX >= BTN_WP_7_L	&&
			    m_nMousePosX <= BTN_WP_7_R	&&
			    m_nMousePosY >= BTN_WP_7_T	&&
				m_nMousePosY <= BTN_WP_7_B)					// 무기중의 [드래곤본] 이미지 사각 영역..
		{
			m_nPointBtn = 6;
			if(m_nGameMode >= SELECT_ITEM_1_2)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					InitItem(0);
					SetWeaponClass(WP_DRAGON);
				}
			}
		}
		else if(m_nMousePosX >= BTN_WP_8_L	&&
			    m_nMousePosX <= BTN_WP_8_R	&&
			    m_nMousePosY >= BTN_WP_8_T	&&
			    m_nMousePosY <= BTN_WP_8_B)					// 무기중의 [세인트카이저] 이미지 사각 영역..
		{
			m_nPointBtn = 7;
			if(m_nGameMode >= SELECT_ITEM_3_2)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					InitItem(0);
					SetWeaponClass(WP_SAINT);
				}
			}
		}

		else if(m_nMousePosX >= BTN_AM_1_L	&&
			    m_nMousePosX <= BTN_AM_1_R	&&
			    m_nMousePosY >= BTN_AM_1_T	&&
			    m_nMousePosY <= BTN_AM_1_B)					// 방어구중의 [클로즈아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 8;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_CLOSE);
			}
		}
		else if(m_nMousePosX >= BTN_AM_2_L	&&
			    m_nMousePosX <= BTN_AM_2_R	&&
			    m_nMousePosY >= BTN_AM_2_T	&&
			    m_nMousePosY <= BTN_AM_2_B)					// 방어구중의 [하프아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 9;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_HALF);
			}
		}
		else if(m_nMousePosX >= BTN_AM_3_L	&&
			    m_nMousePosX <= BTN_AM_3_R	&&
			    m_nMousePosY >= BTN_AM_3_T	&&
			    m_nMousePosY <= BTN_AM_3_B)					// 방어구중의 [체인아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 10;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_CHAIN);
			}
		}
		else if(m_nMousePosX >= BTN_AM_4_L	&&
			    m_nMousePosX <= BTN_AM_4_R	&&
			    m_nMousePosY >= BTN_AM_4_T	&&
			    m_nMousePosY <= BTN_AM_4_B)					// 방어구중의 [스케일아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 11;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_SCALE);
			}
		}
		else if(m_nMousePosX >= BTN_AM_5_L	&&
			    m_nMousePosX <= BTN_AM_5_R	&&
			    m_nMousePosY >= BTN_AM_5_T	&&
			    m_nMousePosY <= BTN_AM_5_B)					// 방어구중의 [플레이트아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 12;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_PLATE);
			}
		}
		else if(m_nMousePosX >= BTN_AM_6_L	&&
			    m_nMousePosX <= BTN_AM_6_R	&&
			    m_nMousePosY >= BTN_AM_6_T	&&
			    m_nMousePosY <= BTN_AM_6_B)					// 방어구중의 [컴포지트아머] 이미지 사각 영역..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 13;
			if(m_bMouseLB)
			{
				InitItem(1);
				SetArmorClass(AM_COMPOSITE);
			}
		}
		else if(m_nMousePosX >= BTN_AM_7_L	&&
			    m_nMousePosX <= BTN_AM_7_R	&&
			    m_nMousePosY >= BTN_AM_7_T	&&
			    m_nMousePosY <= BTN_AM_7_B)					// 방어구중의 [드래곤스케일아머] 이미지 사각 영역..
		{
			m_nPointBtn = 14;
			if(m_nGameMode >= SELECT_ITEM_2_2)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				if(m_bMouseLB)
				{
					InitItem(1);
					SetArmorClass(AM_DRAGON);
				}
			}
		}
		else if(m_nMousePosX >= BTN_AM_8_L	&&
			    m_nMousePosX <= BTN_AM_8_R	&&
			    m_nMousePosY >= BTN_AM_8_T	&&
			    m_nMousePosY <= BTN_AM_8_B)					// 방어구중의 [세인트크리스탈아머] 이미지 사각 영역..
		{
			m_nPointBtn = 15;
			if(m_nGameMode >= SELECT_ITEM_2_1)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				
				if(m_bMouseLB)
				{
					InitItem(1);
					SetArmorClass(AM_SAINT);
				}
			}
		}				

		else if(m_nMousePosX >= BTN_HP_ADD_L	&&
			    m_nMousePosX <= BTN_HP_ADD_R	&&
			    m_nMousePosY >= BTN_HP_ADD_T	&&
			    m_nMousePosY <= BTN_HP_ADD_B	&&
				m_nWeight <= MAX_WEIGHT - WT_POTION)				// HP포션 증가 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 50;
			if(m_bMouseLB)
				SetHPotionNum(m_nHPotionNum + 1);
		}
		else if(m_nMousePosX >= BTN_HP_SUB_L	&&
			    m_nMousePosX <= BTN_HP_SUB_R	&&
			    m_nMousePosY >= BTN_HP_SUB_T	&&
			    m_nMousePosY <= BTN_HP_SUB_B	&&
				m_nWeight >= m_nWeightItem + WT_POTION   && 
				m_nHPotionNum > 0)				// HP포션 감소 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 51;
			if(m_bMouseLB)
				SetHPotionNum(m_nHPotionNum - 1);
		}
		else if(m_nMousePosX >= BTN_MP_ADD_L	&&
			    m_nMousePosX <= BTN_MP_ADD_R	&&
			    m_nMousePosY >= BTN_MP_ADD_T	&&
			    m_nMousePosY <= BTN_MP_ADD_B	&&
				m_nWeight <= MAX_WEIGHT - WT_POTION)				// MP포션 증가 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 52;
			if(m_bMouseLB)
				SetMPotionNum(m_nMPotionNum + 1);
		}
		else if(m_nMousePosX >= BTN_MP_SUB_L	&&
			    m_nMousePosX <= BTN_MP_SUB_R	&&
			    m_nMousePosY >= BTN_MP_SUB_T	&&
			    m_nMousePosY <= BTN_MP_SUB_B	&&
				m_nWeight >= m_nWeightItem + WT_POTION   && 
				m_nMPotionNum > 0 )				// MP포션 감소 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 53;
			if(m_bMouseLB)
				SetMPotionNum(m_nMPotionNum - 1);
		}
		else if(m_nMousePosX >= BTN_SP_ADD_L	&&
			    m_nMousePosX <= BTN_SP_ADD_R	&&
			    m_nMousePosY >= BTN_SP_ADD_T	&&
			    m_nMousePosY <= BTN_SP_ADD_B	&&
				m_nWeight <= MAX_WEIGHT - WT_POTION)				// SP포션 증가 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 54;
			if(m_bMouseLB)
				SetSPotionNum(m_nSPotionNum + 1);
		}
		else if(m_nMousePosX >= BTN_SP_SUB_L	&&
			    m_nMousePosX <= BTN_SP_SUB_R	&&
			    m_nMousePosY >= BTN_SP_SUB_T	&&
			    m_nMousePosY <= BTN_SP_SUB_B	&&
				m_nWeight >= m_nWeightItem + WT_POTION   && 
				m_nSPotionNum > 0)				// SP포션 감소 버튼..
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 55;
			if(m_bMouseLB)
				SetSPotionNum(m_nSPotionNum - 1);
		}

		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn	 &&	 m_nWeaponClass != WP_NONE   &&   m_nArmorClass != AM_NONE)
		{
			m_nStep = 2;
			m_nReadyGameMode = m_nGameMode + 1;
		}
		else if(m_bKeyCancel)
		{
			m_nStep = 2;
			m_nReadyGameMode = m_nGameMode - 1;
		}
	}
	//?????????????????[ 로딩 ] 화면 처리 ?????????????????????????????????????????????????????????
	//( 현재 테스트용으로 디레이를 줘서 화면 전개하게 한 것으로 , 나중에 본 게임 화면에서 필요한 리소스를 실제로 로딩하게 처리해야함)
	else if(m_nGameMode % 10 == 4  &&  m_nGameMode >= LOADING_1_1	&&	m_nGameMode <= LOADING_3_2)		  
	{
		SetCursor(CURSOR_WAIT);	// 로딩 애니메이션 마우스 커서로...
//		m_dwNowTime = timeGetTime();
//		if( (m_dwNowTime - m_dwOpenTime >= DELAY_MODE)  &&  m_bFinish == false)
		if( (timeGetTime() - m_dwTime >= DELAY_MODE)  &&  m_bFinish == false)
		{
			m_nStep = 2;
			m_nReadyGameMode = m_nGameMode + 1;
			m_bFinish = true;
		}
	}
	//---------------[ 본 게임 플레이 ] 화면 처리 ------------------------
	else if(m_nGameMode % 10 == 5   &&   m_nGameMode >= PLAYING_1_1   &&   m_nGameMode <= PLAYING_3_2) 
	{
		//ESC 키를 누르면 게임중 메뉴 가 뜸...(메인루프안에서 if(GetKeyESC())(DrawProccess();} ... else{EscMenu();} 써야 할것..)
		if(m_bKeyESC)						// 게임 중 메뉴 호출 화면...
		{
			m_bChangeVolum = true;
			if(m_nMousePosX >= BTN_YES_L  &&
			   m_nMousePosX <= BTN_YES_R  &&
			   m_nMousePosY >= BTN_YES_T  &&
			   m_nMousePosY <= BTN_YES_B)					// [확인] 버튼
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 0;
				if(m_bMouseLB)
				{
					m_bKeyESC = false;						// 본 게임 플레이 화면으로 돌아옴..
					InitScroll();
				}
			}
			else if(m_nMousePosX >= BTN_NO_L  &&
					m_nMousePosX <= BTN_NO_R  &&
					m_nMousePosY >= BTN_NO_T  &&
					m_nMousePosY <= BTN_NO_B)				// [취소] 버튼
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 1;
				if(m_bMouseLB)
				{
					m_nBgmVolum = m_nPreBgmVolum;
					m_nSoundVolum = m_nPreSoundVolum;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
					m_bKeyESC = false;						// 본 게임 플레이 화면으로 돌아옴..
					InitScroll();
				}
			}
			else if(m_nMousePosX >= BTN_MAIN_L  &&
					m_nMousePosX <= BTN_MAIN_R  &&
					m_nMousePosY >= BTN_MAIN_T  &&
					m_nMousePosY <= BTN_MAIN_B)				// [ 메인메뉴로 ]  버튼...
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 2;
				if(m_bMouseLB)
				{
					m_nBgmVolum = m_nPreBgmVolum;
					m_nSoundVolum = m_nPreSoundVolum;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
					m_nStep = 2;
					m_nFadeRate = FADE_MIN;
					m_nReadyGameMode = SHOW_MAIN_MENU;
				}
			}

			else if(m_nMousePosX >= VOLUM_BGM_UP_L  &&
					m_nMousePosX <= VOLUM_BGM_UP_R  &&
					m_nMousePosY >= VOLUM_BGM_UP_T  &&
					m_nMousePosY <= VOLUM_BGM_UP_B	&&
					m_nBgmVolum < 6)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 3;
				if(m_bMouseLB)
				{
					m_nBgmVolum ++;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
				}
			}
			else if(m_nMousePosX >= VOLUM_BGM_DOWN_L  &&
					m_nMousePosX <= VOLUM_BGM_DOWN_R  &&
					m_nMousePosY >= VOLUM_BGM_DOWN_T  &&
					m_nMousePosY <= VOLUM_BGM_DOWN_B  &&
					m_nBgmVolum > 0)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 4;
				if(m_bMouseLB)
				{
					m_nBgmVolum --;
					m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
				}
			}
			else if(m_nMousePosX >= VOLUM_SOUND_UP_L  &&
					m_nMousePosX <= VOLUM_SOUND_UP_R  &&
					m_nMousePosY >= VOLUM_SOUND_UP_T  &&
					m_nMousePosY <= VOLUM_SOUND_UP_B  &&
					m_nSoundVolum < 6)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 5;
				if(m_bMouseLB)
				{
					m_nSoundVolum ++;
					//SetChangeVolum(true);
				}
			}
			else if(m_nMousePosX >= VOLUM_SOUND_DOWN_L  &&
					m_nMousePosX <= VOLUM_SOUND_DOWN_R  &&
					m_nMousePosY >= VOLUM_SOUND_DOWN_T  &&
					m_nMousePosY <= VOLUM_SOUND_DOWN_B  &&
					m_nSoundVolum > 0)
			{
				SetCursor(CURSOR_ABOVE_BUTTON);
				m_nPointBtn = 6;
				if(m_bMouseLB)
				{
					m_nSoundVolum --;
					//SetChangeVolum(true);
				}
			}

			else
				SetCursor(CURSOR_MENU_NORMAL);	
		}
		else	// 본 게임 화면에서의 처리????????????????????????????????????????????
		{
			//.........???????????????????????????????????????????????
			//????????????????????????????????????????????????????????
			if(m_bChangeVolum)
			{
				m_pBgm_mng->SetVolume( 0, (6 - m_nBgmVolum) * (-500) );				// 배경음 볼륨 적용...
				m_bChangeVolum = false;
			}
//			InitScroll();
			SetCursor(CURSOR_MOVE_ABLE);	 // 게임 진행중, 이동가능한 필드위에서의 일반적인 마우스 커서

			if(m_nResult == 1  &&  m_bFinish == false) // 미션 성공
			{
				m_nStep = 4;
				m_nReadyGameMode = m_nGameMode + 2;
				m_bFinish = true;
				m_nPlayTimeMin = m_nPlayMin;
				m_nPlayTimeSec = m_nPlaySec;
			}
			else if(m_nResult == 2  &&  m_bFinish == false)  // 미션 실패
			{
				m_nStep = 4;
				m_nReadyGameMode = m_nGameMode + 1;
				m_bFinish = true;
			}				
		}//??????????????????????????????????????????????????????????????????????????????????????????????
	}

	//-------------------[ 미션 성공 결과 ] 화면 처리 ---------------------
	else if(m_nGameMode % 10 == 7  &&  m_nGameMode >= SUCCEED_1_1  &&  m_nGameMode <= SUCCEED_3_2)
	{
		if(m_nMousePosX >= BTN_OK_L  &&
		   m_nMousePosX <= BTN_OK_R  &&
		   m_nMousePosY >= BTN_OK_T  &&
		   m_nMousePosY <= BTN_OK_B)						// [다음] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				if(m_nGameMode == SUCCEED_3_2)
				{
					m_nStep = 2;
					m_nReadyGameMode = SHOW_ENDDING;		// 엔딩 화면으로...
				}
				else if(m_nGameMode % 20 == 17)	
				{
					m_nStep = 2;
					m_nReadyGameMode = m_nGameMode + 4;		// 다음 미션 로고 화면으로 됨...
				}
				else  // else if(m_nGameMode % 20 == 7)
				{
					m_nStep = 2;
					m_nReadyGameMode = m_nGameMode + 3;		// 다음 스토리 설정 화면으로 됨...
				}
				m_ID.SetIDSaveMission(m_nReadyGameMode, m_nSelectID);						// 자동 세이브
				m_ID.SetIDScore(m_nScore, m_nSelectID, (m_nGameMode - 7) / 10 - 1);			// 자동 세이브
				m_ID.SaveIDdata("DATA\\Interface\\User\\User.data");						// 자동 세이브
			}
		}
		else if(m_nMousePosX >= BTN_CANCEL_L  &&
			    m_nMousePosX <= BTN_CANCEL_R  &&
			    m_nMousePosY >= BTN_CANCEL_T  &&
			    m_nMousePosY <= BTN_CANCEL_B)				// [메인메뉴로] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;			// 메인메뉴 화면으로 됨...
			}
		}///////////////////////여기부터 수정중////////////////////////////
/*		else if(m_nMousePosX >= BTN_SAVE_L  &&
			    m_nMousePosX <= BTN_SAVE_R  &&
			    m_nMousePosY >= BTN_SAVE_T  &&
			    m_nMousePosY <= BTN_SAVE_B	&&  
				m_bDelay == true)					// [ 저장 ]  버튼...(다음 미션 로고 화면으로 저장)
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 2;
			if(m_bMouseLB)
			{
				m_ID.SetIDSaveMission(m_nGameMode % 20 == 17 ? m_nGameMode + 4 : m_nGameMode + 3, m_nSelectID);
				m_ID.SetIDScore(m_nScore, m_nSelectID, (m_nGameMode - 7) / 10 - 1);
				m_ID.SaveIDdata("DATA\\Interface\\User\\User.data");
				m_bDelay = false;
			}
		}
*/		else if(m_nMousePosX >= BTN_RANK_UP_1_L  &&
			    m_nMousePosX <= BTN_RANK_UP_1_R  &&
			    m_nMousePosY >= BTN_RANK_UP_1_T  &&
			    m_nMousePosY <= BTN_RANK_UP_1_B)
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 10;
			if(m_bMouseLB  &&  m_nRankSt > 0)
				m_nRankSt --;				
		}
		else if(m_nMousePosX >= BTN_RANK_DOWN_1_L  &&
			    m_nMousePosX <= BTN_RANK_DOWN_1_R  &&
			    m_nMousePosY >= BTN_RANK_DOWN_1_T  &&
			    m_nMousePosY <= BTN_RANK_DOWN_1_B)
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 11;
			if(m_bMouseLB  &&  (m_nRankSt < m_ID.GetTotalNumID()-(1+RANK_LIMIT)) )
				m_nRankSt ++;				
		}
		else if(m_nMousePosX >= BTN_RANK_UP_2_L  &&
			    m_nMousePosX <= BTN_RANK_UP_2_R  &&
			    m_nMousePosY >= BTN_RANK_UP_2_T  &&
			    m_nMousePosY <= BTN_RANK_UP_2_B)
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 12;
			if(m_bMouseLB  &&  m_nRankTotalSt > 0)
				m_nRankTotalSt --;				
		}
		else if(m_nMousePosX >= BTN_RANK_DOWN_2_L  &&
			    m_nMousePosX <= BTN_RANK_DOWN_2_R  &&
			    m_nMousePosY >= BTN_RANK_DOWN_2_T  &&
			    m_nMousePosY <= BTN_RANK_DOWN_2_B)
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 13;
			if(m_bMouseLB  &&  (m_nRankTotalSt < m_ID.GetTotalNumID()-(1+RANK_LIMIT)) )
				m_nRankTotalSt ++;				
		}

		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn)									// Enter 키
		{
			if(m_nGameMode == SUCCEED_3_2)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_ENDDING;			// 엔딩 화면으로...
			}
			else if(m_nGameMode % 20 == 17)			
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode + 4;			// 다음 미션 로고 화면으로 됨...
			}
			else  // else if(m_nGameMode % 20 == 7)
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode + 3;			// 다음 스토리 설정 화면으로 됨...
			}
			m_ID.SetIDSaveMission(m_nReadyGameMode, m_nSelectID);
			m_ID.SetIDScore(m_nScore, m_nSelectID, (m_nGameMode - 7) / 10 - 1);
			m_ID.SaveIDdata("DATA\\Interface\\User\\User.data");
		}
		else if(m_bKeyCancel)								// ESC 키
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;				// 메인메뉴로...	
		}

	}

	//-------------------[ 미션 실패 결과 ] 화면 처리 ----------------------
	else if(m_nGameMode % 10 == 6  &&  m_nGameMode >= FAIL_1_1  &&  m_nGameMode <= FAIL_3_2)
	{
		if(m_nMousePosX >= BTN_OK_L  &&
		   m_nMousePosX <= BTN_OK_R  &&
		   m_nMousePosY >= BTN_OK_T  &&
		   m_nMousePosY <= BTN_OK_B)						// [재시도] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 0;
			if(m_bMouseLB)
			{
				if(m_nGameMode % 20 == 16)
				{
					m_nStep = 2;
					m_nReadyGameMode = m_nGameMode - 6;		// 이번 스토리 설정 화면으로 됨...
				}
				else  // else if(m_nGameMode % 20 == 7)
				{
					m_nStep = 2;
					m_nReadyGameMode = m_nGameMode - 5;		// 이번 미션 로고화면으로 됨...
				}
			}
		}
		else if(m_nMousePosX >= BTN_CANCEL_L  &&
			    m_nMousePosX <= BTN_CANCEL_R  &&
			    m_nMousePosY >= BTN_CANCEL_T  &&
			    m_nMousePosY <= BTN_CANCEL_B)				// [메인메뉴로] 버튼
		{
			SetCursor(CURSOR_ABOVE_BUTTON);
			m_nPointBtn = 1;
			if(m_bMouseLB)
			{
				m_nStep = 2;
				m_nReadyGameMode = SHOW_MAIN_MENU;			// 메인메뉴 화면으로 됨...
				//SetGameMode(SHOW_MAIN_MENU);			
			}
		}
		else
			SetCursor(CURSOR_MENU_NORMAL);

		if(m_bKeyReturn)									// Enter 키
		{
			if(m_nGameMode % 20 == 16)	
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode - 6;
				//SetGameMode(m_nGameMode - 6);		// 전 스토리 설정 화면으로 됨...
			}
			else  // else if(m_nGameMode % 20 == 7)
			{
				m_nStep = 2;
				m_nReadyGameMode = m_nGameMode - 5;
				//SetGameMode(m_nGameMode - 5);		// 전 미션 로고화면으로 됨...
			}
		}
		else if(m_bKeyCancel)									// ESC 키
		{
			m_nStep = 2;
			m_nReadyGameMode = SHOW_MAIN_MENU;
			//SetGameMode(SHOW_MAIN_MENU);			// 메인메뉴 화면으로 됨...	
		}
	}

	//---------( 테스트 중 뻥 날 것을 대비한 것으로 나중에 지워야 함 )--------
	//-----------[ 아직 구현 안된 화면 ] 처리 --------------------------------
	else
	{
		SetGameMode(EXIT_GAME);
	}

	//------------------------------------------------------------------------
	//--------[ 다음 게임 화면으로 전환 ]-------------------------------------
	if(m_nStep == 3)
	{
		m_nStep = 0;
		SetGameMode(m_nReadyGameMode);
	}
}

//////////////	 void InitScroll(void)    ///////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::InitScroll(void)
{
	m_nScrollY = SCREEN_HEIGHT;
	m_bFinish  = false;
}
//////////////	 void SetScroll(void)	  ///////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetScroll(void)
{
	m_nScrollY -= MAKE_SPEED;
	if(m_nScrollY < 0)
	{
		m_nScrollY = 0;
		m_bFinish = true;
	}
}

//////////////	 void InitItem(bool bArmor)    //////////////////////////////////////////////////////////////////////////
void CInterfaceMng::InitItem(bool bArmor)	// 아이템 초기화..				
{
	SetHPotionNum(0);
	SetMPotionNum(0);
	SetSPotionNum(0);

	if(bArmor)
		SetArmorClass(AM_NONE);
	else
		SetWeaponClass(WP_NONE);
}
//////////////	 void SetWeaponClass(int nClass)    /////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetWeaponClass(int nClass)	
{ 
	switch(m_nWeaponClass)
	{
	case WP_RAIPEAR:
		m_nWeight -= WT_WP_RAIPEAR;
		m_nWeightItem -= WT_WP_RAIPEAR;
		break;
	case WP_EPEE:
		m_nWeight -= WT_WP_EPEE;
		m_nWeightItem -= WT_WP_EPEE;
		break;
	case WP_GLADIUS:
		m_nWeight -= WT_WP_GLADIUS;
		m_nWeightItem -= WT_WP_GLADIUS;
		break;
	case WP_LONG:
		m_nWeight -= WT_WP_LONG;
		m_nWeightItem -= WT_WP_LONG;
		break;
	case WP_BASTARD:
		m_nWeight -= WT_WP_BASTARD;
		m_nWeightItem -= WT_WP_BASTARD;
		break;
	case WP_TWOHAND:	
		m_nWeight -= WT_WP_TWOHAND;
		m_nWeightItem -= WT_WP_TWOHAND;
		break;
	case WP_DRAGON:	
		m_nWeight -= WT_WP_DRAGON;
		m_nWeightItem -= WT_WP_DRAGON;
		break;
	case WP_SAINT:
		m_nWeight -= WT_WP_SAINT;
		m_nWeightItem -= WT_WP_SAINT;
		break;
	case WP_NONE:
		m_nWeight -= WT_WP_NONE;
		m_nWeightItem -= WT_WP_NONE;
		break;
	}

	m_nWeaponClass = nClass; 

	switch(m_nWeaponClass)
	{
	case WP_RAIPEAR:
		m_nWeight += WT_WP_RAIPEAR;
		m_nWeightItem += WT_WP_RAIPEAR;
		break;
	case WP_EPEE:
		m_nWeight += WT_WP_EPEE;
		m_nWeightItem += WT_WP_EPEE;
		break;
	case WP_GLADIUS:
		m_nWeight += WT_WP_GLADIUS;
		m_nWeightItem += WT_WP_GLADIUS;
		break;
	case WP_LONG:
		m_nWeight += WT_WP_LONG;
		m_nWeightItem += WT_WP_LONG;
		break;
	case WP_BASTARD:
		m_nWeight += WT_WP_BASTARD;
		m_nWeightItem += WT_WP_BASTARD;
		break;
	case WP_TWOHAND:	
		m_nWeight += WT_WP_TWOHAND;
		m_nWeightItem += WT_WP_TWOHAND;
		break;
	case WP_DRAGON:	
		m_nWeight += WT_WP_DRAGON;
		m_nWeightItem += WT_WP_DRAGON;
		break;
	case WP_SAINT:
		m_nWeight += WT_WP_SAINT;
		m_nWeightItem += WT_WP_SAINT;
		break;
	case WP_NONE:
		m_nWeight += WT_WP_NONE;
		m_nWeightItem += WT_WP_NONE;
		break;
	}
}
//////////////	 void SetArmorClass(int nClass)    //////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetArmorClass(int nClass)	
{
	switch(m_nArmorClass)
	{
	case AM_CLOSE:
		m_nWeight -= WT_AM_CLOSE;
		m_nWeightItem -= WT_AM_CLOSE;
		break;
	case AM_HALF:
		m_nWeight -= WT_AM_HALF;
		m_nWeightItem -= WT_AM_HALF;
		break;
	case AM_CHAIN:	
		m_nWeight -= WT_AM_CHAIN;
		m_nWeightItem -= WT_AM_CHAIN;
		break;
	case AM_SCALE:
		m_nWeight -= WT_AM_SCALE;
		m_nWeightItem -= WT_AM_SCALE;
		break;
	case AM_PLATE:
		m_nWeight -= WT_AM_PLATE;
		m_nWeightItem -= WT_AM_PLATE;
		break;
	case AM_COMPOSITE:
		m_nWeight -= WT_AM_COMPOSITE;
		m_nWeightItem -= WT_AM_COMPOSITE;
		break;
	case AM_DRAGON:
		m_nWeight -= WT_AM_DRAGON;
		m_nWeightItem -= WT_AM_DRAGON;
		break;
	case AM_SAINT:
		m_nWeight -= WT_AM_SAINT;
		m_nWeightItem -= WT_AM_SAINT;
		break;
	case AM_NONE:
		m_nWeight -= WT_AM_NONE;
		m_nWeightItem -= WT_AM_NONE;
		break;
	}

	m_nArmorClass = nClass; 

	switch(m_nArmorClass)
	{
	case AM_CLOSE:
		m_nWeight += WT_AM_CLOSE;
		m_nWeightItem += WT_AM_CLOSE;
		break;
	case AM_HALF:
		m_nWeight += WT_AM_HALF;
		m_nWeightItem += WT_AM_HALF;
		break;
	case AM_CHAIN:	
		m_nWeight += WT_AM_CHAIN;
		m_nWeightItem += WT_AM_CHAIN;
		break;
	case AM_SCALE:
		m_nWeight += WT_AM_SCALE;
		m_nWeightItem += WT_AM_SCALE;
		break;
	case AM_PLATE:
		m_nWeight += WT_AM_PLATE;
		m_nWeightItem += WT_AM_PLATE;
		break;
	case AM_COMPOSITE:
		m_nWeight += WT_AM_COMPOSITE;
		m_nWeightItem += WT_AM_COMPOSITE;
		break;
	case AM_DRAGON:
		m_nWeight += WT_AM_DRAGON;
		m_nWeightItem += WT_AM_DRAGON;
		break;
	case AM_SAINT:
		m_nWeight += WT_AM_SAINT;
		m_nWeightItem += WT_AM_SAINT;
		break;
	case AM_NONE:
		m_nWeight += WT_AM_NONE;
		m_nWeightItem += WT_AM_NONE;
		break;
	}
}
////////////   void SetHPotionNum(int nNum)    //////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetHPotionNum(int nNum)
{
	m_nWeight -= (m_nHPotionNum * WT_POTION);
	m_nHPotionNum = nNum; 
	m_nWeight += (m_nHPotionNum * WT_POTION); 
}
////////////   void SetMPotionNum(int nNum)    //////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetMPotionNum(int nNum)		
{
	m_nWeight -= (m_nMPotionNum * WT_POTION);
	m_nMPotionNum = nNum; 
	m_nWeight += (m_nMPotionNum * WT_POTION); 
}
////////////   void SetSPotionNum(int nNum)    //////////////////////////////////////////////////////////////////////////
void CInterfaceMng::SetSPotionNum(int nNum)		
{ 
	m_nWeight -= (m_nSPotionNum * WT_POTION);
	m_nSPotionNum = nNum; 
	m_nWeight += (m_nSPotionNum * WT_POTION); 
}

//////////////   void EtcGame(unsigned short* screen, int width, CDx7Draw* DxDraw)   //////////////////////////////////////
void CInterfaceMng::EtcGame(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	if(m_nGameMode == SHOW_MAIN_MENU)
		Loop_MainMenu(screen, width, DxDraw);
	else if(m_nGameMode == START_ID_MENU)
		Loop_IDMenu(screen, width, DxDraw);
	else if(m_nGameMode == OPENNING_MOVIE)
		Loop_Movie(DxDraw);
	else if(m_nGameMode == SHOW_CREATER)
		Loop_Creater(screen, width, DxDraw);
	else if(m_nGameMode == SHOW_OPTION_MENU)
		Loop_Option(screen, width, DxDraw);
	else if(m_nGameMode == SHOW_BASIC_SCENE)
		Loop_BasicScene(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 0   &&   m_nGameMode >= SHOW_1_STORY_OBJ   &&   m_nGameMode <= SHOW_3_STORY_OBJ)
		Loop_StoryObj(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 1   &&   m_nGameMode >= SHOW_1_1_MISSION_LOGO   &&  m_nGameMode <= SHOW_3_2_MISSION_LOGO)
		Loop_MissionLogo(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 2   &&   m_nGameMode >= SHOW_1_1_MISSION_OBJ   &&   m_nGameMode <= SHOW_3_2_MISSION_OBJ)
		Loop_MissionObj(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 3   &&   m_nGameMode >= SELECT_ITEM_1_1 	&&	m_nGameMode <= SELECT_ITEM_3_2)
		Loop_ItemSelect(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 4  &&  m_nGameMode >= LOADING_1_1	&&	m_nGameMode <= LOADING_3_2)	
		Loop_Loading(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 7  &&  m_nGameMode >= SUCCEED_1_1  &&  m_nGameMode <= SUCCEED_3_2)
		Loop_Victory(screen, width, DxDraw);
	else if(m_nGameMode % 10 == 6  &&  m_nGameMode >= FAIL_1_1  &&  m_nGameMode <= FAIL_3_2)
		Loop_Fail(screen, width, DxDraw);
}

//////////////   void Loop_Movie(void)   //////////////////////////////////////////////////////////////////////////////////
void CInterfaceMng::Loop_Movie(CDx7Draw* DxDraw)
{
	if( !m_pDMShow->RenderStreamToSurface() ) {
		SetGameMode(SHOW_MAIN_MENU);
		return;
	}
}

//////////////   void Loop_MainMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)   ////////////////////////////////
void CInterfaceMng::Loop_MainMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_pImgExtra1->DrawSpr(BTN_START_L, BTN_START_T, 0, screen);
			break;
		case 1:
			m_pImgExtra1->DrawSpr(BTN_OPT_L, BTN_OPT_T, 1, screen);
			break;
		case 2:
			m_pImgExtra1->DrawSpr(BTN_CREATER_L, BTN_CREATER_T, 2, screen);
			break;
		case 3:
			m_pImgExtra1->DrawSpr(BTN_EXIT_L, BTN_EXIT_T, 3, screen);
			break;
		}
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////   void Loop_IDMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)   //////////////////////////////////
void CInterfaceMng::Loop_IDMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;

	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_OK_L, BTN_OK_T, 3, screen);
			break;
		case 1:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 1, screen);
			break;
		case 2:
			m_pImgExtra1->DrawSpr(BTN_CREATE_L, BTN_CREATE_T, 0, screen);
			break;
		case 3:
			m_pImgExtra1->DrawSpr(BTN_DELETE_L, BTN_DELETE_T, 1, screen);
			break;
		}
		int j;
		for(j=0; j<m_ID.GetTotalNumID(); j++)
		{
			if(m_nSelectID == j)
				m_pImgExtra2->DrawSpr(BTN_SELECT_L + (j/12 * BTN_SELECT_WIDTH ) + 6, BTN_SELECT_T + (j%12 * BTN_SELECT_HEIGHT) + 3, 0, screen);
			else
				m_pImgExtra2->DrawSpr(BTN_SELECT_L + (j/12 * BTN_SELECT_WIDTH ) + 10, BTN_SELECT_T + (j%12 * BTN_SELECT_HEIGHT) + 7, 1, screen);
		}
		DxDraw->UnlockOff();

		HDC		hdc;
		char    str[NAME_MAX];
		DxDraw->GetOSurface()->GetDC(&hdc);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(128, 255, 159));
		if(m_nCountName)
		{	
			wsprintf(str, "%s", m_szInputName);
			TextOut(hdc, 425, 176, str, strlen(str));		
		}
		SetTextColor(hdc, RGB(0, 241, 61));
		if(m_nSelectID < ID_MAX)
		{
			wsprintf(str, "%s", m_ID.GetIDName(m_nSelectID));
			TextOut(hdc, 425, 276, str, strlen(str));
		}
		for(j=0; j<m_ID.GetTotalNumID(); j++)
		{
			wsprintf(str, "%s", m_ID.GetIDName(j) );
			if(j == m_nSelectID)
				SetTextColor(hdc, RGB(0, 255, 64));
			else
				SetTextColor(hdc, RGB(200, 255, 0));
			TextOut(hdc, (BTN_SELECT_L + (j/12 * BTN_SELECT_WIDTH )) + 30, (BTN_SELECT_T + (j%12 * BTN_SELECT_HEIGHT)) + 6, str, strlen(str));
		}
		DxDraw->GetOSurface()->ReleaseDC(hdc);

		screen = DxDraw->LockOff();
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;

	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////   void Loop_Creater(unsigned short* screen, int width, CDx7Draw* DxDraw)   /////////////////////////////////
void CInterfaceMng::Loop_Creater(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 0, screen);
			break;
		}
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////   void Loop_Option(unsigned short* screen, int width, CDx7Draw* DxDraw)   //////////////////////////////////
void CInterfaceMng::Loop_Option(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr( BTN_VOLUM_0 + m_nBgmVolum * VOLUM_WIDTH + 1, BGM_TOP + 1, 0, screen);
			m_pImgExtra1->DrawSpr(BTN_VOLUM_0 + m_nSoundVolum * VOLUM_WIDTH + 1, SOUND_TOP + 1, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_OK_L, BTN_OK_T, 0, screen);
			break;
		case 1:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 1, screen);
			break;
		}
		m_pImgExtra1->DrawSpr( BTN_VOLUM_0 + m_nBgmVolum * VOLUM_WIDTH + 1, BGM_TOP + 1, 0, screen);
		m_pImgExtra1->DrawSpr(BTN_VOLUM_0 + m_nSoundVolum * VOLUM_WIDTH + 1, SOUND_TOP + 1, 0, screen);
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr( BTN_VOLUM_0 + m_nBgmVolum * VOLUM_WIDTH + 1, BGM_TOP + 1, 0, screen);
			m_pImgExtra1->DrawSpr(BTN_VOLUM_0 + m_nSoundVolum * VOLUM_WIDTH + 1, SOUND_TOP + 1, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////	 void Loop_BasicScene(unsigned short* screen, int width, CDx7Draw* DxDraw)    /////////////////////////////
void CInterfaceMng::Loop_BasicScene(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 0, screen);
			break;
		}
		if(m_nSceneScrollY + m_pImgExtra1->GetHeight(0) >= SCENE_TOP)
		{
			m_nSceneScrollY --;
			m_pImgExtra1->DrawPiece( (SCENE_WIDTH - m_pImgExtra1->GetWidth(0) )/2 + SCENE_LEFT, m_nSceneScrollY, 70, 530, 0, screen, width);
		}
		else
			m_bFinish = true;
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////////////    void Loop_StoryObj(unsigned short* screen, int width, CDx7Draw* DxDraw)    /////////////////////
void CInterfaceMng::Loop_StoryObj(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(127, 20, m_nGameMode / 20, screen);
			m_pImgExtra3->DrawSpr(STORY_TEXT_L + (STORY_TEXT_W - m_pImgExtra3->GetWidth(m_nGameMode/20)) / 2, STORY_TEXT_T + (STORY_TEXT_H - m_pImgExtra3->GetHeight(m_nGameMode/20)) / 2, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		m_pImgExtra1->DrawSpr(127, 20, m_nGameMode / 20, screen);
		m_pImgExtra3->DrawSpr(STORY_TEXT_L + (STORY_TEXT_W - m_pImgExtra3->GetWidth(m_nGameMode/20)) / 2, STORY_TEXT_T + (STORY_TEXT_H - m_pImgExtra3->GetHeight(m_nGameMode/20)) / 2, 0, screen);
		switch(m_nGameMode)
		{
		case SHOW_1_STORY_OBJ:
			m_pImgExtra2->DrawSpr(220, 90, 0, screen);
			break;
		case SHOW_2_STORY_OBJ:
			m_pImgExtra2->DrawSpr(40, 250, 1, screen);
			break;
		case SHOW_3_STORY_OBJ:
			m_pImgExtra2->DrawSpr(230, 320, 2, screen);
			break;
		}
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 0, screen);
			break;
		}
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(127, 20, m_nGameMode / 20, screen);
			m_pImgExtra3->DrawSpr(STORY_TEXT_L + (STORY_TEXT_W - m_pImgExtra3->GetWidth(m_nGameMode/20)) / 2, STORY_TEXT_T + (STORY_TEXT_H - m_pImgExtra3->GetHeight(m_nGameMode/20)) / 2, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////////    void Loop_MissionLogo(unsigned short* screen, int width, CDx7Draw* DxDraw)    /////////////////////
void CInterfaceMng::Loop_MissionLogo(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

///////////////////    void Loop_MissionObj(unsigned short* screen, int width, CDx7Draw* DxDraw)    /////////////////////
void CInterfaceMng::Loop_MissionObj(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(38, 70, m_nGameMode / 10 - 1, screen);
			m_pImgExtra2->DrawSpr(MISSION_TEXT_L + (MISSION_TEXT_W - m_pImgExtra2->GetWidth(m_nGameMode / 10 - 1))/2, MISSION_TEXT_T + (MISSION_TEXT_H - m_pImgExtra2->GetHeight(m_nGameMode / 10 - 1))/2, m_nGameMode / 10 - 1, screen);
			m_pImgExtra3->DrawSpr(430, 120, m_nGameMode / 10 - 1, screen);
			m_pImgExtra4->DrawSpr(430, 330, m_nGameMode / 10 - 1, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		m_pImgExtra1->DrawSpr(38, 70, m_nGameMode / 10 - 1, screen);
		m_pImgExtra2->DrawSpr(MISSION_TEXT_L + (MISSION_TEXT_W - m_pImgExtra2->GetWidth(m_nGameMode / 10 - 1))/2, MISSION_TEXT_T + (MISSION_TEXT_H - m_pImgExtra2->GetHeight(m_nGameMode / 10 - 1))/2, m_nGameMode / 10 - 1, screen);
		m_pImgExtra3->DrawSpr(430, 120, m_nGameMode / 10 - 1, screen);
		m_pImgExtra4->DrawSpr(430, 330, m_nGameMode / 10 - 1, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 0, screen);
			break;
		}
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(38, 70, m_nGameMode / 10 - 1, screen);
			m_pImgExtra2->DrawSpr(MISSION_TEXT_L + (MISSION_TEXT_W - m_pImgExtra2->GetWidth(m_nGameMode / 10 - 1))/2, MISSION_TEXT_T + (MISSION_TEXT_H - m_pImgExtra2->GetHeight(m_nGameMode / 10 - 1))/2, m_nGameMode / 10 - 1, screen);
			m_pImgExtra3->DrawSpr(430, 120, m_nGameMode / 10 - 1, screen);
			m_pImgExtra4->DrawSpr(430, 330, m_nGameMode / 10 - 1, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////    void Loop_ItemSelect(unsigned short* screen, int width, CDx7Draw* DxDraw)    ////////////////////////
void CInterfaceMng::Loop_ItemSelect(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면  
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		if(m_nWeaponClass != WP_NONE)
			m_pImgExtra5->DrawSpr( (125 - m_pImgExtra5->GetWidth(m_nWeaponClass) )/2 + 30, (220 - m_pImgExtra5->GetHeight(m_nWeaponClass) )/2 + 70, m_nWeaponClass, screen);		
		if(m_nArmorClass != AM_NONE)
		{
			m_pImgExtra6->DrawSpr( (70 - m_pImgExtra6->GetWidth(m_nArmorClass*4 + 3) )/2 + 315 , (70 - m_pImgExtra6->GetHeight(m_nArmorClass*4 + 3) )/2 + 70  , m_nArmorClass*4 + 3, screen);
			m_pImgExtra6->DrawSpr( (100 - m_pImgExtra6->GetWidth(m_nArmorClass*4 + 1) )/2 + 290, (130 - m_pImgExtra6->GetHeight(m_nArmorClass*4 + 1) )/2 + 160, m_nArmorClass*4 + 1, screen);
			m_pImgExtra6->DrawSpr( (140 - m_pImgExtra6->GetWidth(m_nArmorClass*4) )/2 + 30     , (230 - m_pImgExtra6->GetHeight(m_nArmorClass*4) )/2 + 320    , m_nArmorClass*4	 , screen);
			m_pImgExtra6->DrawSpr( (100 - m_pImgExtra6->GetWidth(m_nArmorClass*4 + 2) )/2 + 290, (120 - m_pImgExtra6->GetHeight(m_nArmorClass*4 + 2) )/2 + 430, m_nArmorClass*4 + 2, screen);
		}
		int i;
		for(i=0; i<8; i++)
		{			
			int xW, yW, xA, yA;

			if( (m_nGameMode < SELECT_ITEM_1_2   &&  i>=6)  ||
				(m_nGameMode < SELECT_ITEM_3_2   &&  i>=7) )
			{
				xW = BTN_WP_1_L + ((i%4) * 90);
				yW = BTN_WP_1_T + ((i/4) * 70);
				m_pAniMystery->DrawAni(xW + MYSTERY_X, yW + MYSTERY_Y, 0, i==6 ? m_nCountAniMys1 : m_nCountAniMys2, screen); 
			}
			else
			{
				xW = (80 - m_pImgExtra1->GetWidth(i))/2 + BTN_WP_1_L + ((i%4) * 90);
				yW = (60 - m_pImgExtra1->GetHeight(i))/2 + BTN_WP_1_T + ((i/4) * 70);
				m_pImgExtra1->DrawSpr(xW, yW, i, screen); 
			}

			if( (m_nGameMode < SELECT_ITEM_2_1   &&  i>=6)  ||
				(m_nGameMode < SELECT_ITEM_2_2   &&  i==6) )
			{
				xA = BTN_AM_1_L + ((i%4) * 90);
				yA = BTN_AM_1_T + ((i/4) * 70) + 2;
				m_pAniMystery->DrawAni(xA + MYSTERY_X, yA + MYSTERY_X, 0, i==6 ? m_nCountAniMys3 : m_nCountAniMys4, screen); 
			}
			else
			{
				xA = (80 - m_pImgExtra2->GetWidth(i))/2 + BTN_AM_1_L + ((i%4) * 90);
				yA = (60 - m_pImgExtra2->GetHeight(i))/2 + BTN_AM_1_T + ((i/4) * 70);
				m_pImgExtra2->DrawSpr(xA, yA, i, screen);
			}
		}

		unsigned long Time;
		switch(m_nPointBtn)
		{
		case 0:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(8))/2 + BTN_WP_1_L, (60 - m_pImgExtra1->GetHeight(8))/2 + BTN_WP_1_T, 8, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(0) + 5, nCursorY - m_pImgExtra3->GetHeight(0), 0,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_1_L - m_pImgExtra3->GetWidth(0) + 3, BTN_WP_1_T - m_pImgExtra3->GetHeight(0), 0,  screen); 
			break;
		case 1:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(9))/2 + BTN_WP_2_L, (60 - m_pImgExtra1->GetHeight(9))/2 + BTN_WP_2_T, 9, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(1) + 5, nCursorY - m_pImgExtra3->GetHeight(1), 1,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_2_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_2_T - m_pImgExtra3->GetHeight(1), 1,  screen); 
			break;
		case 2:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(10))/2 + BTN_WP_3_L, (60 - m_pImgExtra1->GetHeight(10))/2 + BTN_WP_3_T, 10, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(2) + 5, nCursorY - m_pImgExtra3->GetHeight(2), 2,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_3_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_3_T - m_pImgExtra3->GetHeight(1), 2,  screen); 
			break;
		case 3:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(11))/2 + BTN_WP_4_L, (60 - m_pImgExtra1->GetHeight(11))/2 + BTN_WP_4_T, 11, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(3) + 5, nCursorY - m_pImgExtra3->GetHeight(3), 3,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_4_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_4_T - m_pImgExtra3->GetHeight(1), 3,  screen); 
			break;
		case 4:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(12))/2 + BTN_WP_5_L, (60 - m_pImgExtra1->GetHeight(12))/2 + BTN_WP_5_T, 12, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(4) + 5, nCursorY - m_pImgExtra3->GetHeight(4), 4,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_5_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_5_T - m_pImgExtra3->GetHeight(1), 4,  screen); 
			break;
		case 5:
			m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(13))/2 + BTN_WP_6_L, (60 - m_pImgExtra1->GetHeight(13))/2 + BTN_WP_6_T, 13, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(5) + 5, nCursorY - m_pImgExtra3->GetHeight(5), 5,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_WP_6_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_6_T - m_pImgExtra3->GetHeight(1), 5,  screen); 
			break;
		case 6:
			if(m_nGameMode < SELECT_ITEM_1_2)
			{
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(19) + 5, nCursorY - m_pImgExtra3->GetHeight(19), 19,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_WP_7_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_7_T - m_pImgExtra3->GetHeight(1), 19,  screen); 
				Time = timeGetTime() - m_dwOpenTime;
				if( Time > SPEED_ANI_MYSTERY )  
				{
//					Mystery.DrawAni(BTN_WP_7_L + MYSTERY_X, BTN_WP_7_T + MYSTERY_Y, 0,  MysCount1, screen, width);
					m_nCountAniMys1++;
					if(m_nCountAniMys1 > 6)
						m_nCountAniMys1 = 0;
					m_dwOpenTime = timeGetTime();
				}
			}
			else
			{
				m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(14))/2 + BTN_WP_7_L, (60 - m_pImgExtra1->GetHeight(14))/2 + BTN_WP_7_T, 14, screen); 
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(6) + 5, nCursorY - m_pImgExtra3->GetHeight(6), 6,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_WP_7_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_7_T - m_pImgExtra3->GetHeight(1), 6,  screen); 
			}
			break;
		case 7:
			if(m_nGameMode < SELECT_ITEM_3_2)
			{
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(19) + 5, nCursorY - m_pImgExtra3->GetHeight(19), 19,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_WP_8_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_8_T - m_pImgExtra3->GetHeight(1), 19,  screen); 
				Time = timeGetTime() - m_dwOpenTime;
				if( Time > SPEED_ANI_MYSTERY )  
				{
//					Mystery.DrawAni(BTN_WP_8_L + MYSTERY_X, BTN_WP_8_T + MYSTERY_Y, 0,  MysCount2, screen, width);
					m_nCountAniMys2++;
					if(m_nCountAniMys2 > 6)
						m_nCountAniMys2 = 0;
					m_dwOpenTime = timeGetTime();
				}
			}
			else
			{
				m_pImgExtra1->DrawSpr((80 - m_pImgExtra1->GetWidth(15))/2 + BTN_WP_8_L, (60 - m_pImgExtra1->GetHeight(15))/2 + BTN_WP_8_T, 15, screen); 
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(7) + 5, nCursorY - m_pImgExtra3->GetHeight(7), 7,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_WP_8_L - m_pImgExtra3->GetWidth(1) + 3, BTN_WP_8_T - m_pImgExtra3->GetHeight(1), 7,  screen); 
			}
			break;
		case 8:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(8))/2 + BTN_AM_1_L, (60 - m_pImgExtra2->GetHeight(8))/2 + BTN_AM_1_T, 8,  screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(8) + 5, nCursorY - m_pImgExtra3->GetHeight(8), 8,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_1_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_1_T - m_pImgExtra3->GetHeight(1), 8,  screen); 
			break;
		case 9:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(9))/2 + BTN_AM_2_L, (60 - m_pImgExtra2->GetHeight(9))/2 + BTN_AM_2_T, 9,  screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(9) + 5, nCursorY - m_pImgExtra3->GetHeight(9), 9,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_2_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_2_T - m_pImgExtra3->GetHeight(1), 9,  screen); 
			break;
		case 10:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(10))/2 + BTN_AM_3_L, (60 - m_pImgExtra2->GetHeight(10))/2 + BTN_AM_3_T, 10,  screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(10) + 5, nCursorY - m_pImgExtra3->GetHeight(10), 10,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_3_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_3_T - m_pImgExtra3->GetHeight(1), 10,  screen); 
			break;
		case 11:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(11))/2 + BTN_AM_4_L, (60 - m_pImgExtra2->GetHeight(11))/2 + BTN_AM_4_T, 11, screen); 
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(11) + 5, nCursorY - m_pImgExtra3->GetHeight(11), 11,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_4_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_4_T - m_pImgExtra3->GetHeight(1), 11,  screen); 
			break;
		case 12:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(12))/2 + BTN_AM_5_L, (60 - m_pImgExtra2->GetHeight(12))/2 + BTN_AM_5_T, 12, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(12) + 5, nCursorY - m_pImgExtra3->GetHeight(12), 12,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_5_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_5_T - m_pImgExtra3->GetHeight(1), 12,  screen); 
			break;
		case 13:
			m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(13))/2 + BTN_AM_6_L, (60 - m_pImgExtra2->GetHeight(13))/2 + BTN_AM_6_T, 13, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(13) + 5, nCursorY - m_pImgExtra3->GetHeight(13), 13,  screen, width); 
			m_pImgExtra3->DrawSpr(BTN_AM_6_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_6_T - m_pImgExtra3->GetHeight(1), 13,  screen); 
			break;
		case 14:
			if(m_nGameMode < SELECT_ITEM_2_2)
			{
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(19) + 5, nCursorY - m_pImgExtra3->GetHeight(19), 19,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_AM_7_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_7_T - m_pImgExtra3->GetHeight(1), 19,  screen); 
				Time = timeGetTime() - m_dwOpenTime;
				if( Time > SPEED_ANI_MYSTERY )  
				{
//					Mystery.DrawAni(BTN_AM_7_L + MYSTERY_X, BTN_AM_7_T + MYSTERY_Y, 0,  MysCount3, screen, width);
					m_nCountAniMys3++;
					if(m_nCountAniMys3 > 6)
						m_nCountAniMys3 = 0;
					m_dwOpenTime = timeGetTime();
				}
			}
			else
			{
				m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(14))/2 + BTN_AM_7_L, (60 - m_pImgExtra2->GetHeight(14))/2 + BTN_AM_7_T, 14, screen); 
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(14) + 5, nCursorY - m_pImgExtra3->GetHeight(14), 14,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_AM_7_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_7_T - m_pImgExtra3->GetHeight(1), 14,  screen); 
			}
			break;
		case 15:
			if(m_nGameMode < SELECT_ITEM_2_1)
			{
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(19) + 5, nCursorY - m_pImgExtra3->GetHeight(19), 19,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_AM_8_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_8_T - m_pImgExtra3->GetHeight(1), 19,  screen); 
				Time = timeGetTime() - m_dwOpenTime;
				if( Time > SPEED_ANI_MYSTERY )  
				{
//					Mystery.DrawAni(BTN_AM_8_L + MYSTERY_X, BTN_AM_8_T + MYSTERY_Y, 0,  MysCount4, screen, width);
					m_nCountAniMys4++;
					if(m_nCountAniMys4 > 6)
						m_nCountAniMys4 = 0;
					m_dwOpenTime = timeGetTime();
				}
			}
			else
			{
				m_pImgExtra2->DrawSpr((80 - m_pImgExtra2->GetWidth(15))/2 + BTN_AM_8_L, (60 - m_pImgExtra2->GetHeight(15))/2 + BTN_AM_8_T, 15, screen); 
				//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(15) + 5, nCursorY - m_pImgExtra3->GetHeight(15), 15,  screen, width); 
				m_pImgExtra3->DrawSpr(BTN_AM_8_L - m_pImgExtra3->GetWidth(1) + 3, BTN_AM_8_T - m_pImgExtra3->GetHeight(1), 15,  screen); 
			}
			break;

		case 50:
			m_pImgExtra4->DrawSpr( BTN_HP_ADD_L, BTN_HP_ADD_T, 0, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(16) + 5, nCursorY - m_pImgExtra3->GetHeight(16), 16,  screen, width); 
			m_pImgExtra3->DrawSpr(CAPTION_HP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 16,  screen); 
			break;
		case 51:
			m_pImgExtra4->DrawSpr( BTN_HP_SUB_L, BTN_HP_SUB_T, 1, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(16) + 5, nCursorY - m_pImgExtra3->GetHeight(16), 16,  screen, width);
			m_pImgExtra3->DrawSpr(CAPTION_HP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 16,  screen); 
			break;
		case 52:
			m_pImgExtra4->DrawSpr( BTN_MP_ADD_L, BTN_MP_ADD_T, 0, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(17) + 5, nCursorY - m_pImgExtra3->GetHeight(17), 17,  screen, width);
			m_pImgExtra3->DrawSpr(CAPTION_MP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 17,  screen); 
			break;
		case 53:
			m_pImgExtra4->DrawSpr( BTN_MP_SUB_L, BTN_MP_SUB_T, 1, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(17) + 5, nCursorY - m_pImgExtra3->GetHeight(17), 17,  screen, width);
			m_pImgExtra3->DrawSpr(CAPTION_MP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 17,  screen); 
			break;
		case 54:
			m_pImgExtra4->DrawSpr( BTN_SP_ADD_L, BTN_SP_ADD_T, 0, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(18) + 5, nCursorY - m_pImgExtra3->GetHeight(18), 18,  screen, width);
			m_pImgExtra3->DrawSpr(CAPTION_SP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 18,  screen); 
			break;
		case 55:
			m_pImgExtra4->DrawSpr( BTN_SP_SUB_L, BTN_SP_SUB_T, 1, screen);
			//m_pImgExtra3->DrawSpr(nCursorX - m_pImgExtra3->GetWidth(18) + 5, nCursorY - m_pImgExtra3->GetHeight(18), 18,  screen, width);
			m_pImgExtra3->DrawSpr(CAPTION_SP_X - m_pImgExtra3->GetWidth(1) + 3, CAPTION_POTION_Y - m_pImgExtra3->GetHeight(1), 18,  screen); 
			break;

		case 100:
			m_ImgButton.DrawSpr(BTN_OK_L, BTN_OK_T, 0, screen);
			break;
		case 101:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 1, screen);
			break;
		}
		if(m_nPointBtn != 6)
			m_nCountAniMys1 = 0;
		if(m_nPointBtn != 7)
			m_nCountAniMys2 = 0;
		if(m_nPointBtn != 14)
			m_nCountAniMys3 = 0;
		if(m_nPointBtn != 15)
			m_nCountAniMys4 = 0;

		DxDraw->UnlockOff();
		
		HDC hdc;
		char str[100];
		DxDraw->GetOSurface()->GetDC(&hdc);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(255, 255, 255));
		wsprintf(str, "무게  :   %2d / %2d", m_nWeight, MAX_WEIGHT);
		TextOut(hdc, 440, 555, str, strlen(str));
		wsprintf(str, "%2d", m_nHPotionNum);
		TextOut(hdc, 480, 500, str, strlen(str));
		wsprintf(str, "%2d", m_nMPotionNum);
		TextOut(hdc, 605, 500, str, strlen(str));
		wsprintf(str, "%2d", m_nSPotionNum);
		TextOut(hdc, 730, 500, str, strlen(str));
		DxDraw->GetOSurface()->ReleaseDC(hdc);		

		screen = DxDraw->LockOff();
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////	  void Loop_Loading(unsigned short* screen, int width, CDx7Draw* DxDraw)     /////////////////////////////
void CInterfaceMng::Loop_Loading(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_LOAD_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

//////////////	  void Loop_EscMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)    //////////////////////////////
void CInterfaceMng::Loop_EscMenu(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	if( m_bFinish )
	{	
		if(m_nStep == 2)
		{
			if(m_nFadeRate < FADE_MAX)
			{
				m_ImgEscMenu.DrawSpr(0, 0, 0, screen);
				Fade(m_wFadeColor, m_nFadeRate, width);
				m_nFadeRate += FADE_LOAD_RATE;
			}
			else
			{
				m_nStep = 3;
				m_nFadeRate = FADE_MAX;
			}
		}
		else
		{
			m_ImgEscMenu.DrawSpr(0, 0, 0, screen);
			switch(m_nPointBtn)
			{
			case 0:
				m_ImgButton.DrawSpr(BTN_YES_L, BTN_YES_T, 0, screen);
				break;
			case 1:
				m_ImgButton.DrawSpr(BTN_NO_L, BTN_NO_T, 1, screen);
				break;
			case 2:
				m_ImgButton.DrawSpr(BTN_MAIN_L, BTN_MAIN_T, 4, screen);
				break;
			case 3:
				m_ImgEscBtn.DrawSpr( VOLUM_BGM_UP_L, VOLUM_BGM_UP_T, 0, screen);
				break;
			case 4:
				m_ImgEscBtn.DrawSpr( VOLUM_BGM_DOWN_L, VOLUM_BGM_DOWN_T, 1, screen);
				break;
			case 5:
				m_ImgEscBtn.DrawSpr( VOLUM_SOUND_UP_L, VOLUM_SOUND_UP_T, 0, screen);
				break;
			case 6:
				m_ImgEscBtn.DrawSpr( VOLUM_SOUND_DOWN_L, VOLUM_SOUND_DOWN_T, 1, screen);
				break;
			}
			DxDraw->UnlockOff();

			HDC hdc;
			char str[100];
			DxDraw->GetOSurface()->GetDC(&hdc);
			SetBkMode(hdc, TRANSPARENT);
			SetTextColor(hdc, RGB(0, 0, 0));
			wsprintf(str, "%d", m_nBgmVolum);
			TextOut(hdc, 565, 472, str, strlen(str));
			wsprintf(str, "%d", m_nSoundVolum);
			TextOut(hdc, 695, 472, str, strlen(str));
			DxDraw->GetOSurface()->ReleaseDC(hdc);

			screen = DxDraw->LockOff();
			m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		}
	}
	else
	{
		SetScroll();
		m_ImgEscMenu.DrawSpr(0, m_nScrollY, 0, screen);
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

////////////////    void Loop_Result(unsigned short* screen, int width, CDx7Draw* DxDraw)    ////////////////////////////
void CInterfaceMng::Loop_Victory(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	int x=80, x2= 420, y=425, height=22;
	int i, yy, yyy;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_OK_L, BTN_OK_T, 0, screen);
			break;
		case 1:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 1, screen);
			break;
		}
		DxDraw->UnlockOff();

		HDC hdc;
		char str[100];
		DxDraw->GetOSurface()->GetDC(&hdc);
		SetBkMode(hdc, TRANSPARENT);
		SetTextColor(hdc, RGB(154, 53, 255));
		wsprintf(str, "소요시간-> %2d : %2d", m_nPlayTimeMin, m_nPlayTimeSec);
		TextOut(hdc, 320, 198, str, strlen(str));
		if(m_nGameMode == SUCCEED_1_1  ||  m_nGameMode == SUCCEED_2_1  ||  m_nGameMode == SUCCEED_3_2)
		{
			wsprintf(str, "제한시간-> %2d : %2d", m_nStartMin, m_nStartSec);
			TextOut(hdc, 470, 198, str, strlen(str));
			wsprintf(str, "+ %d 점", (m_nStartMin*60 + m_nStartSec) - (m_nPlayTimeMin*60 + m_nPlayTimeSec));
			TextOut(hdc, 640, 198, str, strlen(str));
			wsprintf(str, "%d 점", m_nScore );
			TextOut(hdc, 420, 338, str, strlen(str));
		}
		else
		{
			wsprintf(str, "- %d 점", m_nPlayTimeMin * 60 + m_nPlayTimeSec);
			TextOut(hdc, 640, 198, str, strlen(str));
			wsprintf(str, "%d 명", m_nAliveFriends);
			TextOut(hdc, 450, 298, str, strlen(str));
			wsprintf(str, "+ %d 점", m_nAliveFriends * 4);
			TextOut(hdc, 650, 298, str, strlen(str));
			wsprintf(str, "%d 점", m_nScore );
			TextOut(hdc, 420, 338, str, strlen(str));
		}
		wsprintf(str, "%d 개", GetWasteHPotion());
		TextOut(hdc, 360, 258, str, strlen(str));
		wsprintf(str, "%d 개", GetWasteMPotion());
		TextOut(hdc, 450, 258, str, strlen(str));
		wsprintf(str, "%d 개", GetWasteSPotion());
		TextOut(hdc, 550, 258, str, strlen(str));
		wsprintf(str, "- %d 점", GetWasteHPotion()*3 + GetWasteMPotion()*3 + GetWasteSPotion()*2 );
		TextOut(hdc, 640, 258, str, strlen(str));
//		int x=80, x2= 420, y=425, height=22;
//		int i, yy, yyy;
		for(i=0, yy=0, yyy=0; i<m_ID.GetTotalNumID(); i++)
		{
			if(m_Rank[i].bPlayingNow)
				SetTextColor(hdc, RGB(255, 84, 2));
			else if(m_Rank[i].nScore == 0)
				SetTextColor(hdc, RGB(167, 167, 167));
			else
				SetTextColor(hdc, RGB(101, 101, 50));
			if(i >= m_nRankSt  &&  i <= m_nRankSt + RANK_LIMIT)
			{
				if(m_Rank[i].nScore == m_Rank[i-1].nScore   &&   i > 0)
					wsprintf(str, "%d등  :  %s -> %d ", m_nSameRank, m_Rank[i].szName, m_Rank[i].nScore);
				else
				{
					wsprintf(str, "%d등  :  %s -> %d ", i+1, m_Rank[i].szName, m_Rank[i].nScore);
					m_nSameRank = i+1;
				}
				TextOut(hdc, x, y + (height*yy), str, strlen(str));
				yy++;
			}
			//----
			if(m_RankTotal[i].bPlayingNow)
				SetTextColor(hdc, RGB(255, 84, 2));
			else if(m_RankTotal[i].nScore == 0)
				SetTextColor(hdc, RGB(167, 167, 167));
			else
				SetTextColor(hdc, RGB(101, 101, 50));
			if(i >= m_nRankTotalSt   &&   i <= m_nRankTotalSt + RANK_LIMIT)
			{
				if(m_RankTotal[i].nScore == m_RankTotal[i-1].nScore  &&  i > 0)
					wsprintf(str, "%d등  :  %s -> %d ", m_nSameRankTotal, m_RankTotal[i].szName, m_RankTotal[i].nScore);
				else
				{
					wsprintf(str, "%d등  :  %s -> %d ", i+1, m_RankTotal[i].szName, m_RankTotal[i].nScore);
					m_nSameRankTotal = i+1;
				}
				TextOut(hdc, x2, y + (height*yyy), str, strlen(str));
				yyy++;
			}
		}
		DxDraw->GetOSurface()->ReleaseDC(hdc);

		screen = DxDraw->LockOff();
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}
////////////////    void Loop_Fail(unsigned short* screen, int width, CDx7Draw* DxDraw)    //////////////////////////////
void CInterfaceMng::Loop_Fail(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	screen = DxDraw->LockOff();
	m_pScr = (unsigned long *)screen;

	switch(m_nStep)
	{
	case 0:		// 페이드인
		if(m_nFadeRate > FADE_MIN)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(FAIL_RESULT_L, FAIL_RESULT_T, 0, screen);
			m_pImgExtra2->DrawSpr(FAIL_BACK_L, FAIL_BACK_T, 0, screen);
			Fade( m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate -= FADE_RATE;
		}
		else
		{
			m_nStep = 1;			
			m_nFadeRate = FADE_MIN;
		}
		break;
	case 1:		// 본 화면
		m_pImgBackGround->DrawSpr(0, 0, 0, screen);
		m_pImgExtra1->DrawSpr(FAIL_RESULT_L, FAIL_RESULT_T, 0, screen);
		m_pImgExtra2->DrawSpr(FAIL_BACK_L, FAIL_BACK_T, 0, screen);
		switch(m_nPointBtn)
		{
		case 0:
			m_ImgButton.DrawSpr(BTN_OK_L, BTN_OK_T, 0, screen);
			break;
		case 1:
			m_ImgButton.DrawSpr(BTN_CANCEL_L, BTN_CANCEL_T, 1, screen);
			break;
		}
		m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
		break;
	case 2:		// 페이드 아웃
		if(m_nFadeRate < FADE_MAX)
		{
			m_pImgBackGround->DrawSpr(0, 0, 0, screen);
			m_pImgExtra1->DrawSpr(FAIL_RESULT_L, FAIL_RESULT_T, 0, screen);
			m_pImgExtra2->DrawSpr(FAIL_BACK_L, FAIL_BACK_T, 0, screen);
			Fade(m_wFadeColor, m_nFadeRate, width);
			m_nFadeRate += FADE_RATE;
		}
		else
		{
			m_nStep = 3;
			m_nFadeRate = FADE_MAX;
		}
		break;
	}

	DxDraw->UnlockOff();
	DxDraw->Flip();
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////    void DrawInterface(unsigned short* screen, int width, CDx7Draw* DxDraw)    //////////////////////////
void CInterfaceMng::DrawInterface(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	RunPlayTime();
	if(m_bLimitTime)
		ShowLimitTime(screen, width, DxDraw);
/*	m_ImgStatus.DrawSpr(0, SCREEN_HEIGHT - m_ImgStatus.GetHeight(0), 0, screen);
	switch(m_nGameMode)
	{
	case PLAYING_1_1:
		m_ImgStatus.DrawSpr(SKILL_SWORD_L, SKILL_TOP, 1, screen);
		m_ImgStatus.DrawSpr(SKILL_VALKYRE_L, SKILL_TOP, 1, screen);
		m_ImgStatus.DrawSpr(SKILL_DRAGON_L, SKILL_TOP, 1, screen);
		break;
	case PLAYING_1_2:
	case PLAYING_2_1:
		m_ImgStatus.DrawSpr(SKILL_SWORD_L, SKILL_TOP, 1, screen);
		m_ImgStatus.DrawSpr(SKILL_DRAGON_L, SKILL_TOP, 1, screen);
		break;
	case PLAYING_2_2:
		m_ImgStatus.DrawSpr(SKILL_SWORD_L, SKILL_TOP, 1, screen);
		break;
	}*/
	int HP = (int)(m_pTeshy->GetHP()*0.08);
	int MP = (int)(m_pTeshy->GetMP()*0.08);
	int SP = (int)(m_pTeshy->GetSP()*0.08);
	int IncMP = (int)(m_pTeshy->GetIncMP()*0.08);

	m_ImgStatusWin.DrawSpr(0, SCREEN_HEIGHT - m_ImgStatusWin.GetHeight(0), 0, screen);  // 상태창 바탕
	m_ImgStatusWin.DrawPiece(380, 505, 505+80-HP, 800, 1, screen, 800);					// HP
	m_ImgStatusWin.DrawPiece(57,  516, 505+80-SP, 800, 2, screen, 800);					// SP
//	m_ImgStatusWin.DrawPiece(52, 502, 502+80-IncSP, 502+80-SP, 3, screen, 800);			// IncSP
	m_ImgStatusWin.DrawPiece(112, 516, 505+80-IncMP, 505+80-MP, 5, screen, 800);		// IncMP
	m_ImgStatusWin.DrawPiece(112, 516, 505+80-MP, 800, 4, screen, 800);					// MP				
	
	m_ImgStatusWin.DrawPiece(380,  543, 500, 800, 6, screen, 800);		// HP 받침대 
	m_ImgStatusWin.DrawPiece(52, 502, 500, 800, 7, screen, 800);		// SP 받침대 
	m_ImgStatusWin.DrawPiece(107, 502, 500, 800, 8, screen, 800);		// MP 빋침대 


	DxDraw->UnlockOff();

	HDC hdc;
//	char str[100];
	DxDraw->GetOSurface()->GetDC(&hdc);
	SetBkMode(hdc, TRANSPARENT);
	SetTextColor(hdc, RGB(0, 0, 0));
	//------------------( 여기는 이 함수의 인자를 더 추가하여 물약 갯수,단도 갯수, 혹시나 HP,MP,SP 등을 표시하기 위해 쓴다)--------------
	DxDraw->GetOSurface()->ReleaseDC(hdc);

	screen = DxDraw->LockOff();
	m_ImgCursor.DrawSpr(m_nMousePosX, m_nMousePosY, m_nCursorCount, screen);
}
////////////////    void RunPlayTime(unsigned short* screen, int width, CDx7Draw* DxDraw)    ////////////////////////////
void CInterfaceMng::RunPlayTime()
{
//	m_dwNowTime = timeGetTime();
//	if(m_dwNowTime - m_dwOpenTime >= 1000)
	if(timeGetTime() - m_dwOpenTime >= 1000)
	{
		m_nPlaySec++;
		if(m_nPlaySec >= 60)
		{
			m_nPlayMin++;
			m_nPlaySec = 0;
		}
		if(m_bLimitTime)
		{
			m_nLimitSec --;	
			if(m_nLimitSec < 0)
			{
				m_nLimitMin --;
				m_nLimitSec = 59;
				if(m_nLimitMin < 0)
					m_nLimitMin = m_nLimitSec = 0;
			}		
		}
		m_dwOpenTime = timeGetTime();
	}	
}
//////////////////    void ShowLimitTime(unsigned short* screen, int width, CDx7Draw* DxDraw)    ////////////////////////
void CInterfaceMng::ShowLimitTime(unsigned short* screen, int width, CDx7Draw* DxDraw)
{
	int x = 350, y = 70, widthImg = 0, space = 3;

	m_ImgTime.DrawSpr(x, y, m_nLimitMin/10, screen);
	widthImg += m_ImgTime.GetWidth(m_nLimitMin/10) + space;

	m_ImgTime.DrawSpr(x+widthImg, y, m_nLimitMin%10, screen);
	widthImg += m_ImgTime.GetWidth(m_nLimitMin%10) + space;

	m_ImgTime.DrawSpr(x+widthImg, y+5, 10, screen);
	widthImg += m_ImgTime.GetWidth(10)+space;

	m_ImgTime.DrawSpr(x+widthImg, y, m_nLimitSec/10, screen);
	widthImg += m_ImgTime.GetWidth(m_nLimitSec/10) + space;

	m_ImgTime.DrawSpr(x+widthImg, y, m_nLimitSec%10, screen);
}


//////////////   void Fade(unsigned short srcColor, int rate, int width)   ////////////////////////////////////////////////
void CInterfaceMng::Fade(unsigned short srcColor, int rate, int width)
{  
	unsigned long high, low;
	unsigned long dwColor = ((srcColor<<16) | srcColor);
	int   destDepth = 32 - rate;

	high   = dwColor & 0xf81f07e0;
	high >>= 5;
	high  *= destDepth;
	high  &= 0xf81f07e0;

	low    = dwColor & 0x07e0f81f;
	low   *= destDepth;
	low  >>= 5;
	low   &= 0x07e0f81f;

	dwColor = high | low;

	for(int i=0; i<(width*SCREEN_HEIGHT)>>1; i++)
	{
		high   = m_pScr[i] & 0xf81f07e0;
		high >>= 5;
		high  *= destDepth;
		high  &= 0xf81f07e0;

		low    = m_pScr[i] & 0x07e0f81f;
		low   *= destDepth;
		low  >>= 5;		
		low   &= 0x07e0f81f;

		m_pScr[i] = ( high | low) + dwColor;
	}
}

