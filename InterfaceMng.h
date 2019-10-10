// InterfaceMng.h: interface for the CInterfaceMng class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_INTERFACEMNG_H__103DE301_1659_11D6_944A_0050DA898DDD__INCLUDED_)
#define AFX_INTERFACEMNG_H__103DE301_1659_11D6_944A_0050DA898DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Image.h"
#include "DM6Mp3.h"
#include "DM6Show.h"
#include "IDdata.h"
#include "Animation.h"


class CImage;
class CDx7Draw;
class CDM6Mp3;
class CMp3Manager;
class CDM6Show;
class CIDdata;
class CAnimation;

// 선미가 추가한 코딩 1
class CMainChar;


/* m_pImgBackGround : 
		MainMenu    IDMenu   ItemBack   OptionMenu   Creater   BasicScene
		MissionLogo   Story  Mission   Loading   VicResult

   Extra new 이미지 :
		MainBtn
		IDbtn   IDMark
		ItemCaption   Armor   Weapon   SmallWeapon   SmallArmor   UpDownBtn
		VolumBar
		BasicText
		StoryText   StoryTitle   StoryArea
		MissionText   MissionTitle   VicObject   FailObject
		FailResult   FailBack
		Mystery(CAnimation)
		
   정적 생성될 이미지 :
		Time  EscMenu  EscBtn  Cursor   Status   Button
*/
typedef struct Ranking
{
	bool	bPlayingNow;				// 현재 플레이 하고 있던 아이디 인가
	char	szName[NAME_MAX];			// 유저 ID
	int		nScore;						// 점수 들어감..
}Rank;



class CInterfaceMng  
{
//////////////////////////////////////////////////////////////////////////
// 멤버 변수들
//////////////////////////////////////////////////////////////////////////
protected:


	CMainChar* m_pTeshy;					// 선미가 추가한 코딩 2: 테시리아 포인터 
	CImage		m_ImgStatusWin;				// 선미가 추가한 코딩 4: 상태창 이미지

	CImage	    *m_pImgBackGround;			// 배경 이미지
	CImage	    *m_pImgExtra1;				// 각 화면에서 쓰일 이미지
	CImage	    *m_pImgExtra2;				// 각 화면에서 쓰일 이미지
	CImage	    *m_pImgExtra3;				// 각 화면에서 쓰일 이미지
	CImage	    *m_pImgExtra4;				// 각 화면에서 쓰일 이미지
	CImage	    *m_pImgExtra5;				// 각 화면에서 쓰일 이미지
	CImage	    *m_pImgExtra6;				// 각 화면에서 쓰일 이미지
	CAnimation  *m_pAniMystery;				// 아이템 장착창에서 쓸 물음표 애니메이션
	int			m_nCountAniMys1;			// 물음표 애니메이션 프레임 번호를 갖고 있는 변수
	int			m_nCountAniMys2;			// 물음표 애니메이션 프레임 번호를 갖고 있는 변수
	int			m_nCountAniMys3;			// 물음표 애니메이션 프레임 번호를 갖고 있는 변수
	int			m_nCountAniMys4;			// 물음표 애니메이션 프레임 번호를 갖고 있는 변수


	CImage		m_ImgTime;					// 본 게임화면에서 쓰일 제한 시간 이미지
	CImage		m_ImgEscMenu;				// 본 게임화면에서 ESC메뉴 화면의 배경 이미지
	CImage		m_ImgEscBtn;				// 본 게임화면에서 ESC메뉴 화면에서 배경음 및 사운드 볼륨 조절 버튼 이미지
	CImage		m_ImgCursor;				// 마우스 커서 이미지
	CImage		m_ImgStatus;				// 본 게임화면에서 상태 메뉴 이미지
	CImage		m_ImgButton;				// 전체적인 메뉴에서 주로 쓰이는 버튼 이미지

	CMp3Manager *m_pBgm_mng;				// MP3 배경음 출력을 관리하는 클래스 변수
	char		*m_pBgm[11];				// 각 배경음의 경로 및 이름을 저장하고 있는 문자열 배열
	bool		m_bChangeBgm;				// m_pBgm_mng->PlayBGM("변경될 배경음.mp3") 함수를 호출해야 하나 안해야 하는가를 알려주는 변수
	bool		m_bChangeVolum;				// 배경음의 크기를 변경 시킬 것인지의 여부를 나타내는 변수

	CDM6Show	*m_pDMShow;					// 오프닝무비에 쓰일 동영상 출력으로 쓰일 다이렉트쇼우 객체
	
	unsigned long *m_pScr;					// 페이드인/아웃 할때 pScreen 의 주소를 갖고 있는 변수

	int			m_nGameMode;				// 각 화면을 나타내는 변수
	int			m_nReadyGameMode;			// 화면이 전환 되기전 페이드 아웃 될때 다음 화면의 번호를 저장하고 있는 변수

	bool		m_bNowEvent;				// 본 게임 화면에서 현재 이벤트 화면인지 아닌지를 갖고 잇는 변수

	int			m_nMousePosX;				// 마우스 가로 좌표
	int			m_nMousePosY;				// 마우스 세로 좌표

	int			m_bMouseLB;					// 마우스 클릭 여부 변수
	bool		m_bKeyESC;					// 본 게임 화면에서 ESC메뉴의 활성화 여부를 갖고 있는 변수
	bool		m_bKeyReturn;				// Enter키 눌림 여부 변수
	bool		m_bKeyNext;					// 마우스 클릭 및 Enter키 및 ESC키 및 Space키 를 눌렸는지 여부 변수
	bool		m_bKeyCancel;				// Esc키 눌림 여부 변수

	int			m_nCursor;					// 각 게임 화면과 마우스의 위치에 따른 마우스 커서 모양의 번호를 갖고 있는 변수
	int			m_nCursorCount;				// 마우스 커서의 애니메이션을 위해 스프라이트 이미지의 프레임 번호를 갖고 잇는 변수
	int			m_nAddCount;				// 마우스 커서의 애니메이션 프레임 번호의 값을 증가시켜주는 변수

	int			m_nStep;					// 각 게임 화면에서의 단계를 나타내는 변수(0:페이드인 / 1:실제화면 / 2:패이드 아웃    //  0:ESC메뉴 위로 스크롤 / 1:ESC메뉴 실제 화면 / 2:ESC메뉴 아래로 스크롤)
	int			m_nFadeRate;				// 페이드 인/아웃되는 양을 갖고 있는 변수
	unsigned short m_wFadeColor;			// 페이드 인/아웃되는 색깔 값을 갖고 있는 변수

	int			m_nPointBtn;				// 메뉴의 움직임을 주기위해 마우스가 어떤 버튼위에 있을때 그 지정된 값을 갖고 있는 변수 

	unsigned long m_dwTime;					// 디레이를 주기 위한 변수
	unsigned long m_dwNowTime;				// 디레이를 주기 위해 현재 시간을 갖고 있는 변수
	unsigned long m_dwOpenTime;				// 화면 전환이 이루어졌을 순간의 시간을 갖고 있는 변수

	int			m_nWeaponClass;				// 무기 종류의 번호를 갖고 잇는 변수
	int			m_nArmorClass;				// 방어구 종류의 버호를 갖고 있는 변수
	int			m_nHPotionNum;				// 힐링포션 갯수
	int			m_nMPotionNum;				// 마나 포션 갯수
	int			m_nSPotionNum;				// 스테미나 포션 갯수

	int			m_nHPotionStart;			// 처음 들고 시작하는 힐링포션 갯수
	int			m_nMPotionStart;			// 처음 들고 시작하는 마나포션 갯수
	int			m_nSPotionStart;			// 처음 들고 시작하는 스테미나포션 갯수
	int			m_nWeightStart;				// 본 게임 플레이 시작전에 케릭터 클래스에 무게를 넘겨주기 위한 변수
	int			m_nWeaponStart;				// 본 게임 플레이 시작전에 케릭터 클래스에 무기 종류를 넘겨주기 위한 변수
	int			m_nArmorStart;				// 본 게임 플레이 시작전에 케릭터 클래스에 무기 종류를 넘겨주기 위한 변수

	int			m_nHPotionNow;				// 현재 소지하고 있는 힐링포션 갯수
	int			m_nMPotionNow;				// 현재 소지하고 있는 마나포션 갯수
	int			m_nSPotionNow;				// 현재 소지하고 있는 스테미나포션 갯수

	int			m_nWeight;					// 현재 소지한 아이템 총 무게
	int			m_nWeightItem;				// 현재 소지한 무기,방어구 무게
	

	int			m_nBgmVolum;				// 배경음 크기
	int			m_nSoundVolum;				// 사운드 크기
	int			m_nPreBgmVolum;				// 취소 버튼 클릭시를 대비한 원래 배경음 크기
	int			m_nPreSoundVolum;			// 취소 버튼 클릭시를 대비한 원래 사운드 크기

	CIDdata m_ID;							// CIDdata 클래스 변수	
	int		m_nSelectID;					// 현재 선택한 ID 번호..

	char	m_szInputName[NAME_MAX];		// 아이디 입력창에 보일 문자열
	int		m_nCountName;					// 위 문자열의 카운터
	bool	m_bKeyConsent[KEY_KIND];		// 위 문자(아이디)를 찍을때 1번 눌리면 1글자만 찍히게 하기위한 변수
	char	m_szDeleteName[NAME_MAX];		// 선택된 아이디의 문자열

	bool	m_bFinish;						// 단 한번만 무언가를 수행하고 난 후를 처리하기 위한 변수

	int		m_nSceneScrollY;				// 기본배경 화면에서 스크롤 되는 그림의 세로 좌표

	int		m_nScrollY;						// 본 게임 플레이 화면중 ESC메뉴가 위로 스크롤 되게 하는 좌표

	bool	m_bLimitTime;					// 제한 시간이 있는 미션일 경우 true 값으로 셋팅해서 아래 시간을 초기화해 줌...
	int		m_nLimitMin;					// 제한시간 (분 단위)
	int		m_nLimitSec;					// 제한시간 (초 단위)
	int		m_nStartMin;					// 플레이 시작시 제한 시간이 저장되는 변수(분 단위)
	int		m_nStartSec;					// 플레이 시작시 제한 시간이 저장되는 변수(초 단위)
	int		m_nPlayMin;						// 미션 플레이 시간(분 단위)
	int		m_nPlaySec;						// 미션 플레이 시간(초 단위)
	int		m_nPlayTimeMin;					// 미션 결과 화면에서 보여질 플레이 시간(분 단위)
	int		m_nPlayTimeSec;					// 미션 결과 화면에서 보여질 플레이 시간(초 단위)

	int		m_nAliveFriends;				// 살아남은 아군 수

	int		m_nResult;						// 미션 결과 ( 초기: 0,  성공: 1, 실패: 2 )

	int		m_nScore;						// 해당 미션 점수
	int		m_nScoreTotal;					// 이번 미션까지의 점수	

	Rank	m_Rank[ID_MAX];					// 이번 미션 점수의 순위를 저장하기 위한 구조체 변수
	Rank	m_RankTotal[ID_MAX];			// 이번 미션까지의 점수 순위를 저장하기 위한 구조체 변수					

	int		m_nRankSt;						// 랭킹 순위 스크롤창에서 맨위에 보여줄 아이디의 번호		
	int		m_nRankTotalSt;					// 이번 미션까지의 랭킹 순위 스크롤창에서 맨위에 보여줄 아이디의 번호			
	int		m_nSameRank;					// 이번 미션의 점수가 같은 아이디는 같은 랭킹으로 기록하기 위한 변수
	int		m_nSameRankTotal;				// 이번 미션까지의 점수의 합이 같은 아이디는 같은 랭킹으로 기록하기 위한 변수


///////////////////////////////////////////////////////////////////////////
// public 멤버 함수들
///////////////////////////////////////////////////////////////////////////
public:

	CInterfaceMng();
	virtual ~CInterfaceMng();

	void	SetGameMode(int nMode, CDx7Draw* DxDraw = NULL);			// 화면 전환시키는 함수
	int		GetGameMode(void)			{ return m_nGameMode; }			// 현재 화면의 번호를 알켜주는 함수

	void	SetNowEvent(bool show)		{ m_bNowEvent = show; }			// 이벤트의 활성화/비활성화 시키는 함수
	bool	GetNowEvent(void)			{ return m_bNowEvent; }			// 현재 이벤트가 활성화 되어 있는 지의 값을 알켜주는 함수 

	CMp3Manager* GetBgm_mng(void)		{ return m_pBgm_mng; }			// 배경음 출력을 관리하는 객체를 알려주는 함수

	void	EtcGame(unsigned short* screen, int width, CDx7Draw* DxDraw);	// 메인 루프에서 조건에 따라 매번 호출되는 함수로 현재 게임 화면에 따른 그림들을 그리는 함수로 가게 해준다
	void	Loop_EscMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);    // 본 게임 플레이중 ESC메뉴 화면을 그려주는 함수
	void	DrawInterface(unsigned short* screen, int width, CDx7Draw* DxDraw);	  // 본 게임 플레이 중 인터페이스적인 것들 그려주는 함수(상태창(바,단도 갯수,스킬창 단추 그림), 제한시간, 마우스 포인터 등등) 

	void	SetInputSts(int nX, int nY, int nInput);// 메인 루프의 InputProcess() 끝에서 항상 호출되는 함수 

	void	SetKeyESC(bool bPush);										// ESC메뉴를 활성화/비활성화 시키는 함수
	bool	GetKeyESC(void)				{ return m_bKeyESC; }			// ESC메뉴의 활성화 여부를 알려주는 함수

	void	SetCursor(int type)			{ m_nCursor = type; }			// 커서의 모양을 바꿔주는 함수

	void	PushChar(char name, int num);								// 아이디 생성창에서 보여질 이름에 문자 추가하는 함수
	void	BackSpace(void);											// 아이디 생성창에서 백스페이스 처리 함수 
	void	SetKeyConsent(bool bPush, int num);							// 1번 누른 효과를 위해 쓰이는 함수
	bool	GetKeyConsent(int num)		{ return m_bKeyConsent[num]; }	// 1번 누른 효과를 위한 쓰이는 함수
	int		GetCountName(void)			{ return m_nCountName; }		// 아이디 입력창의 문자열의 카운터를 반환해주는 함수

	void	SetHPotionNum(int nNum);
	void	SetMPotionNum(int nNum);
	void	SetSPotionNum(int nNum);
//	int		GetHPotionNum(void)			{ return m_nHPotionNum; }
//	int		GetMPotionNum(void)			{ return m_nMPotionNum; }
//	int		GetSPotionNum(void)			{ return m_nSPotionNum; }

//	int		GetWeaponClass(void)		{ return m_nWeaponClass; }
//	int		GetArmorClass(void)			{ return m_nArmorClass; }

	void	SetStep(int nStep)			{ m_nStep = nStep; }
	int		GetStep(void)				{ return m_nStep; }

	void	SetResult(int nSucceed)		{ m_nResult = nSucceed; }

	//--------- 케릭터 클래스에 필요한 데이터를 넘겨주는 public 함수들 -------------
	int		GetHPotionStart(void)		{ return m_nHPotionStart; }
	int		GetMPotionStart(void)		{ return m_nMPotionStart; }
	int		GetSPotionStart(void)		{ return m_nSPotionStart; }
	int		GetWeightStart(void)		{ return m_nWeightStart; }
	int		GetWeaponClassStart(void)	{ return m_nWeaponClass; }
	int		GetArmorClassStart(void)	{ return m_nArmorClass; }
	//---------케릭터 클래스에 있는 데이터를 얻어오는 public 함수들 ----------------
	void	SetHPotionNow(int num)		{ m_nHPotionNow = num; }
	void	SetMPotionNow(int num)		{ m_nMPotionNow = num; }
	void	SetSPotionNow(int num)		{ m_nSPotionNow = num; }
	
	void	SetMainChar(CMainChar* pTeshy)	{ m_pTeshy = pTeshy; }					// 선미가 추가한 코딩 3: 테시리아 포인터 세팅



////////////////////////////////////////////////////////////////////////////
// protected 멤버 함수들
////////////////////////////////////////////////////////////////////////////
protected:
	void	Init(void);																// 생성자에서 1번 호출되는 초기화 함수
	void	InitPlay(void);															// 화면이 전환되기 직전에 호출되어 부분적인 초기화를 시켜주는 함수

	void	Loop_Movie(CDx7Draw* DxDraw);											// 동영상을 보여주는 화면 처리 함수
	void	Loop_MainMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 메인메뉴 화면을 그려주는 함수
	void	Loop_IDMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);		// ID 선택 화면을 그려주는 함수
	void	Loop_Creater(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 제작진 보여주는 화면을 그려주는 함수
	void	Loop_Option(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 옵션 화면을 그려주는 함수
	void	Loop_BasicScene(unsigned short* screen, int width, CDx7Draw* DxDraw);	// 기본배경 설명 화면을 그려주는 함수
	void	Loop_StoryObj(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 각 스토리 설명 및 목적 화면을 그려주는 함수
	void	Loop_MissionLogo(unsigned short* screen, int width, CDx7Draw* DxDraw);	// 각 미션 로고 화면을 그려주는 함수
	void	Loop_MissionObj(unsigned short* screen, int width, CDx7Draw* DxDraw);	// 각 미션 설명 및 목적 화면을 그려주는 함수
	void	Loop_ItemSelect(unsigned short* screen, int width, CDx7Draw* DxDraw);	// 아이템 선택 화면을 그려주는 함수
	void	Loop_Loading(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 로딩 화면을 그려주는 함수
	void	Loop_Victory(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 미션 결과(성공) 화면을 그려주는 함수
	void	Loop_Fail(unsigned short* screen, int width, CDx7Draw* DxDraw);			// 미션 결과(실패) 화면을 그려주는 함수
	
	
	void	SetCursorFrame(void);													// 커서의 애니메이션 처리를 위한 함수

	void	Fade(unsigned short srcColor, int rate, int width);						// 페이드 인/아웃 되게 하는 함수

	void	InitItem(bool bArmor);													// 아이템 초기화..(무기를 초기화 : bArmor = 0, 방어구를 초기화 : bArmor = 1)
	void	SetWeaponClass(int nClass);												// 무기 선택
	void	SetArmorClass(int nClass);												// 방어구 선택

	void	InitScroll(void);														// 게임중 ESC메뉴의 상태를 초기화하는 함수
	void	SetScroll(void);														// 게임중 ESC메뉴가 스크롤되게 좌표 변경하게 하는 함수

	void	RunPlayTime();															// 제한 시간의 감소와 플레이 시간의 증가 시키는 함수
	void	ShowLimitTime(unsigned short* screen, int width, CDx7Draw* DxDraw);		// 제한 시간을 그림으로 출력하는 함수

	int		GetWasteHPotion(void)		{ return (m_nHPotionStart - m_nHPotionNow);}	// 소모한 힐링포션 갯수
	int		GetWasteMPotion(void)		{ return (m_nMPotionStart - m_nMPotionNow);}	// 소모한 마나포션 갯수
	int		GetWasteSPotion(void)		{ return (m_nSPotionStart - m_nSPotionNow);}	// 소모한 스테미나포션 갯수



};

#endif // !defined(AFX_INTERFACEMNG_H__103DE301_1659_11D6_944A_0050DA898DDD__INCLUDED_)
