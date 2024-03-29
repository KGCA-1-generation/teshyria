#ifndef _ITFDEFINE_H_
#define _ITFDEFINE_H_

///////////  게임 모드(CInterfaceMaster::m_nGameMode)  ////////////////////////////////////
#define	SHOW_LOGO				0
#define	OPENNING_MOVIE			1
#define	SHOW_TITLE				2
#define	SHOW_MAIN_MENU			3
#define START_ID_MENU			4
#define SHOW_OPTION_MENU		5
#define SHOW_CREATER			6
#define SHOW_BASIC_SCENE		7
#define SHOW_ENDDING			8
#define	EXIT_GAME				9


#define SHOW_1_STORY_OBJ		10
#define SHOW_1_1_MISSION_LOGO	11
#define SHOW_1_1_MISSION_OBJ	12
#define SELECT_ITEM_1_1			13
#define LOADING_1_1				14
#define PLAYING_1_1				15
#define FAIL_1_1				16
#define SUCCEED_1_1				17
#define SHOW_1_2_MISSION_LOGO	21
#define SHOW_1_2_MISSION_OBJ	22
#define SELECT_ITEM_1_2			23
#define LOADING_1_2				24
#define PLAYING_1_2				25
#define FAIL_1_2				26
#define SUCCEED_1_2				27

#define SHOW_2_STORY_OBJ		30
#define SHOW_2_1_MISSION_LOGO	31
#define SHOW_2_1_MISSION_OBJ	32
#define SELECT_ITEM_2_1			33
#define LOADING_2_1				34
#define PLAYING_2_1				35
#define FAIL_2_1				36
#define SUCCEED_2_1				37
#define SHOW_2_2_MISSION_LOGO	41
#define SHOW_2_2_MISSION_OBJ	42
#define SELECT_ITEM_2_2			43
#define LOADING_2_2				44
#define SHOW_2_2_MISSION_OBJ	42
#define PLAYING_2_2				45
#define FAIL_2_2				46
#define SUCCEED_2_2				47

#define SHOW_3_STORY_OBJ		50
#define SHOW_3_1_MISSION_LOGO	51
#define SHOW_3_1_MISSION_OBJ	52
#define SELECT_ITEM_3_1			53
#define LOADING_3_1				54
#define PLAYING_3_1				55
#define FAIL_3_1				56
#define SUCCEED_3_1				57
#define SHOW_3_2_MISSION_LOGO	61
#define SHOW_3_2_MISSION_OBJ	62
#define SELECT_ITEM_3_2			63
#define LOADING_3_2				64
#define PLAYING_3_2				65
#define FAIL_3_2				66
#define SUCCEED_3_2				67

#define SHOW_EVENT				100
/*
//------- 이벤트 게임 모드 --------
#define EVENT_1_1				100

#define EVENT_1_2_1				150
  #define EVENT_1_2_2				151
  #define EVENT_1_2_3				152
#define EVENT_1_2_2				200


#define EVENT_2_1_1				250
#define EVENT_2_1_2				300
#define EVENT_2_1_3				350

#define EVENT_2_2				400


#define EVENT_3_1				450

#define EVENT_3_2_1				500
#define EVENT_3_2_2				550
*/
///////////////////////////// 게임 모드에 따른 배경음 /////////////////////////////////////////////
#define BGM_MAINMENU			0
#define BGM_ENDDING				1
#define BGM_PRE_MISSION			2
#define BGM_1_1					3
#define BGM_1_2					4
#define BGM_2_1					5
#define BGM_2_2					6
#define BGM_3_1					7
#define BGM_3_2					8 
#define BGM_SUCCEED				9 
#define BGM_FAIL				10
		
#define BGM_EVT_1_1				11
#define BGM_EVT_1_2_1			12
#define BGM_EVT_1_2_2			13
#define BGM_EVT_2_1_1			14
#define BGM_EVT_2_1_2			15
#define BGM_EVT_2_1_3			16
#define BGM_EVT_2_2				17
#define BGM_EVT_3_1				18
#define BGM_EVT_3_2_1			19
#define BGM_EVT_3_2_2			20

///////////////////////////// 마우스 커서 모양 /////////////////////////////////////////////////////
#define	CURS_FRAME				4		// 각 커서 스프라이트 프레임 수
#define CURSOR_DELAY			500		// 커서 애니메이션 속도(1/1000 초)

#define CURSOR_MENU_NORMAL		0		// 인터페이스 메뉴 위의 커서 
#define CURSOR_ABOVE_BUTTON		(1 * CURS_FRAME)		// 인터페이스 메뉴의 버튼 위의 커서 
#define CURSOR_WAIT				(2 * CURS_FRAME)		// 유저에게 기다릴 것을 요청하는 커서 
#define CURSOR_MOVE_ABLE		(3 * CURS_FRAME)		// 게임 진행중, 이동가능한 필드위에서의 일반적인 커서 
#define CURSOR_MOVE_DISABLE		(4 * CURS_FRAME)		// 게임 진행중, 이동 불가능한 필드위에서의 커서 
#define CURSOR_ATTACK			(5 * CURS_FRAME)		// 게임 진행중, 적/몬스터/짐승 등의 공격 가능한 대상위에서의 커서
#define CURSOR_TALK				(6 * CURS_FRAME)		// 게임 진행중, 이벤트 등의 대화가 가능한 NPC 위에서의 커서
#define CURSOR_ABOVE_ITEM		(7 * CURS_FRAME)		// 게임 진행중, 아이템 위에서의 커서

///////////////////////////// 각 메뉴에서의 버튼 위치 좌표(공통)////// 임의의 테스트용 좌표임.../////
// OK / 다음
#define BTN_OK_L				557
#define BTN_OK_T				546
#define BTN_OK_R				660
#define BTN_OK_B				580
// 취소 / 돌아가기 / 메인메뉴로
#define BTN_CANCEL_L			677
#define BTN_CANCEL_T			546
#define BTN_CANCEL_R			780
#define BTN_CANCEL_B			580
// 저장
#define BTN_SAVE_L				437
#define BTN_SAVE_T				546
#define BTN_SAVE_R				540
#define BTN_SAVE_B				580

//-----게임중 Esc메뉴 버튼---------
// 메인메뉴로
#define BTN_MAIN_L				638
#define BTN_MAIN_T				506
#define BTN_MAIN_R				740
#define BTN_MAIN_B				540
// OK
#define BTN_YES_L				398
#define BTN_YES_T				506
#define BTN_YES_R				500
#define BTN_YES_B				540
//CANCEL
#define BTN_NO_L				518
#define BTN_NO_T				506
#define BTN_NO_R				620
#define BTN_NO_B				540

//볼륨조절 버튼
#define VOLUM_BGM_UP_L			592
#define VOLUM_BGM_UP_T			462
#define VOLUM_BGM_UP_R			610
#define VOLUM_BGM_UP_B			479

#define VOLUM_BGM_DOWN_L		592
#define VOLUM_BGM_DOWN_T		479
#define VOLUM_BGM_DOWN_R		610
#define VOLUM_BGM_DOWN_B		500

#define VOLUM_SOUND_UP_L		723
#define VOLUM_SOUND_UP_T		462
#define VOLUM_SOUND_UP_R		740
#define VOLUM_SOUND_UP_B		479

#define VOLUM_SOUND_DOWN_L		723
#define VOLUM_SOUND_DOWN_T		479
#define VOLUM_SOUND_DOWN_R		740
#define VOLUM_SOUND_DOWN_B		500

//-----옵션메뉴에서 볼륨 조절위해 클릭되는 부분 좌표-----
// 볼륨바 두께, 높이
#define VOLUM_WIDTH				90
#define VOLUM_HEIGHT			25
// 배경음,사운드 Top 좌표
#define BGM_TOP					470
#define SOUND_TOP				505
// 볼륨 X 좌표
#define BTN_VOLUM_0				130
#define BTN_VOLUM_1				220
#define BTN_VOLUM_2				310
#define BTN_VOLUM_3				400
#define BTN_VOLUM_4				490
#define BTN_VOLUM_5				580
#define BTN_VOLUM_6				670
//---------------------------------

// 메인 메뉴 버튼..  (스타트, 옵션, 동영상 보기, 제작자들, 나가기)
#define BTN_START_L				462
#define BTN_START_T				179
#define BTN_START_R				752
#define BTN_START_B				234

#define BTN_OPT_L				462
#define BTN_OPT_T				268
#define BTN_OPT_R				752
#define BTN_OPT_B				323

#define BTN_CREATER_L			462
#define BTN_CREATER_T			358
#define BTN_CREATER_R			752
#define BTN_CREATER_B			413

//#define BTN_MOVIE_L				500
//#define BTN_MOVIE_T				110
//#define BTN_MOVIE_R				720
//#define BTN_MOVIE_B				280

#define BTN_EXIT_L				462
#define BTN_EXIT_T				508
#define BTN_EXIT_R				752
#define BTN_EXIT_B				563
// ID 등록 및 선택창에서
#define BTN_CREATE_L			680
#define BTN_CREATE_T			168
#define BTN_CREATE_R			770
#define BTN_CREATE_B			200

#define BTN_DELETE_L			680
#define BTN_DELETE_T			268
#define BTN_DELETE_R			770
#define BTN_DELETE_B			300


#define BTN_SELECT_L			35
#define BTN_SELECT_T			140

#define BTN_SELECT_WIDTH		175
#define BTN_SELECT_HEIGHT		34





//------아이템 장착 모드에서..--------------------------------------------------
//-------- 무기(WeaPon)
#define BTN_WP_1_L				420
#define BTN_WP_1_T				100
#define BTN_WP_1_R				500
#define BTN_WP_1_B				160

#define BTN_WP_2_L				510
#define BTN_WP_2_T				100
#define BTN_WP_2_R				590
#define BTN_WP_2_B				160

#define BTN_WP_3_L				600
#define BTN_WP_3_T				100
#define BTN_WP_3_R				680
#define BTN_WP_3_B				160

#define BTN_WP_4_L				690
#define BTN_WP_4_T				100
#define BTN_WP_4_R				770
#define BTN_WP_4_B				160

#define BTN_WP_5_L				420
#define BTN_WP_5_T				170
#define BTN_WP_5_R				500
#define BTN_WP_5_B				230

#define BTN_WP_6_L				510
#define BTN_WP_6_T				170
#define BTN_WP_6_R				590
#define BTN_WP_6_B				230

#define BTN_WP_7_L				600
#define BTN_WP_7_T				170
#define BTN_WP_7_R				680
#define BTN_WP_7_B				230

#define BTN_WP_8_L				690
#define BTN_WP_8_T				170
#define BTN_WP_8_R				770
#define BTN_WP_8_B				230

//--------- 방어구 (ArMor)
#define BTN_AM_1_L				420
#define BTN_AM_1_T				290
#define BTN_AM_1_R				500
#define BTN_AM_1_B				350

#define BTN_AM_2_L				510
#define BTN_AM_2_T				290
#define BTN_AM_2_R				590
#define BTN_AM_2_B				350

#define BTN_AM_3_L				600
#define BTN_AM_3_T				290
#define BTN_AM_3_R				680
#define BTN_AM_3_B				350

#define BTN_AM_4_L				690
#define BTN_AM_4_T				290
#define BTN_AM_4_R				770
#define BTN_AM_4_B				350

#define BTN_AM_5_L				420
#define BTN_AM_5_T				360
#define BTN_AM_5_R				500
#define BTN_AM_5_B				420

#define BTN_AM_6_L				510
#define BTN_AM_6_T				360
#define BTN_AM_6_R				590
#define BTN_AM_6_B				420

#define BTN_AM_7_L				600
#define BTN_AM_7_T				360
#define BTN_AM_7_R				680
#define BTN_AM_7_B				420

#define BTN_AM_8_L				690
#define BTN_AM_8_T				360
#define BTN_AM_8_R				770
#define BTN_AM_8_B				420

//-------- 포션(HP,MP,SP)
#define BTN_HP_ADD_L			502
#define BTN_HP_ADD_T			482
#define BTN_HP_ADD_R			518
#define BTN_HP_ADD_B			503

#define BTN_HP_SUB_L			502
#define BTN_HP_SUB_T			507
#define BTN_HP_SUB_R			518
#define BTN_HP_SUB_B			528

#define BTN_MP_ADD_L			627
#define BTN_MP_ADD_T			482
#define BTN_MP_ADD_R			643
#define BTN_MP_ADD_B			503

#define BTN_MP_SUB_L			627
#define BTN_MP_SUB_T			507
#define BTN_MP_SUB_R			643
#define BTN_MP_SUB_B			528

#define BTN_SP_ADD_L			752
#define BTN_SP_ADD_T			482
#define BTN_SP_ADD_R			768
#define BTN_SP_ADD_B			503

#define BTN_SP_SUB_L			752
#define BTN_SP_SUB_T			507
#define BTN_SP_SUB_R			768
#define BTN_SP_SUB_B			528

////////////////////// 케릭터 클래스와 함께 사용 할 define 매크로.. /////////////////////////
//----- 물음표 애니메이션 속도(1/1000초) --------
#define SPEED_ANI_MYSTERY		100
//------- 각 미션 시작시 얻는 초기 단도 갯수
#define MAX_KNIFE				60
#define MAX_POTION				60
//------- 테시리아 아이템 들 수 있는 최대 한도 무게
#define MAX_WEIGHT				80
//------- 주인공 케릭터 상태 최대 수치
#define MAX_HP					1000
#define MAX_MP					1000
#define MAX_SP					1000
//------- 무기 종류
#define WP_RAIPEAR				0
#define WP_EPEE					1
#define WP_GLADIUS				2
#define WP_LONG					3
#define WP_BASTARD				4
#define WP_TWOHAND				5
#define WP_DRAGON				6
#define WP_SAINT				7
#define WP_NONE					8
//------- 방어구 종류
#define AM_CLOSE				0
#define AM_HALF					1
#define AM_CHAIN				2
#define AM_SCALE				3
#define AM_PLATE				4
#define AM_COMPOSITE			5
#define AM_DRAGON				6
#define AM_SAINT				7
#define AM_NONE					8

//------- 무게
#define WT_WP_RAIPEAR			4
#define WT_WP_EPEE				4
#define WT_WP_GLADIUS			6
#define WT_WP_LONG				6
#define WT_WP_BASTARD			8
#define WT_WP_TWOHAND			10
#define WT_WP_DRAGON			4
#define WT_WP_SAINT				10
#define WT_WP_NONE				0

#define WT_AM_CLOSE				8
#define WT_AM_HALF				10
#define WT_AM_CHAIN				12
#define WT_AM_SCALE				14
#define WT_AM_PLATE				16
#define WT_AM_COMPOSITE			18
#define WT_AM_DRAGON			12
#define WT_AM_SAINT				20
#define WT_AM_NONE				0

#define WT_POTION				2


/////////////////// HP, MP, SP 게이지 길이 ///////////////////////////////////////////////////
#define LENGTH_HP				100
#define LENGTH_MP				100
#define LENGTH_SP				100

/////////////////// 자동 화면 전환 (로고,타이틀,각 미션 로고, 로딩, 엔딩) 시 딜레이 //////////
#define DELAY_MODE				2500	

/////////////////// 각 미션 제한 시간 ////////////////////////////////////////////////////////
#define LIMIT_MIN_1				12
#define LIMIT_SEC_1				0

#define LIMIT_MIN_2				15
#define LIMIT_SEC_2				30
			
#define LIMIT_MIN_3				20
#define LIMIT_SEC_3				15

/////////////////// 각 방어 미션 아군 수 /////////////////////////////////////////////////////
#define MAX_FRIEND_1			21
#define MAX_FRIEND_2			18
#define MAX_FRIEND_3			15

/////////////////// 창 만드는(스크롤) 속도 ///////////////////////////////////////////////////
#define MAKE_SPEED				24

/////////////////// 페이드 아웃/인 매크로  ///////////////////////////////////////////////////
#define dHALF_MASK				0x7bde
#define FADE_RATE				6
#define FADE_MAX				29
#define FADE_MIN				4

#define FADE_LOAD_RATE			3

/////////////////// 아이디 길이 제한 /////////////////////////////////////////////////////////
#define NAME_MAX				16
/////////////////// 아이디 갯수 제한 /////////////////////////////////////////////////////////
#define ID_MAX					24
/////////////////// 미션 총 갯수 /////////////////////////////////////////////////////////////
#define MISSION_NUM				6

/////////////////// 아이디 입력시 쓰일 키(영문자, 숫자 조합만 가능, 백스페이스 쓰임) /////////
#define KEY_KIND				37

/////////////////// 기본배경 스토리 설명 스크롤되는 좌표 /////////////////////////////////////
#define SCENE_TOP				70
#define SCENE_LEFT				30
#define SCENE_WIDTH				740
#define SCENE_BOTTOM			530

#define MISSION_TEXT_L			40
#define MISSION_TEXT_T			127
#define MISSION_TEXT_W			340
#define MISSION_TEXT_H			430

#define STORY_TEXT_L			437
#define STORY_TEXT_T			132
#define STORY_TEXT_W			326
#define STORY_TEXT_H			372

/////////////////// 아이템 선택창에서의 각 아이템창의 좌표에 더해질 물음표 좌표 ///////////////
#define MYSTERY_X				40
#define MYSTERY_Y				43
/////////////////// 아이템 선택화면에서 포션위에 그려질 풍선말 좌표 ///////////////////////////
#define CAPTION_HP_X			420			
#define CAPTION_MP_X			545
#define CAPTION_SP_X			670
#define CAPTION_POTION_Y		482

/////////////////// 스테이지별 초기 아군수 ////////////////////////////////////////////////////
#define FRIENDS_1				18
#define FRIENDS_2				15
#define FRIENDS_3				12

/////////////////// 미션 실패 화면에 쓸 실패 타이틀 그림 좌표 /////////////////////////////////
#define FAIL_RESULT_L			42
#define FAIL_RESULT_T			82

/////////////////// 랭킹 버튼 좌표 및 랭킹 보여주기 한계 //////////////////////////////////////
#define BTN_RANK_UP_1_L			368
#define BTN_RANK_UP_1_R			386
#define BTN_RANK_UP_1_T			423
#define BTN_RANK_UP_1_B			443

#define BTN_RANK_DOWN_1_L		368
#define BTN_RANK_DOWN_1_R		386
#define BTN_RANK_DOWN_1_T		466
#define BTN_RANK_DOWN_1_B		486

#define BTN_RANK_UP_2_L			708
#define BTN_RANK_UP_2_R			726
#define BTN_RANK_UP_2_T			423
#define BTN_RANK_UP_2_B			443

#define BTN_RANK_DOWN_2_L		708
#define BTN_RANK_DOWN_2_R		726
#define BTN_RANK_DOWN_2_T		466
#define BTN_RANK_DOWN_2_B		486

#define RANK_LIMIT				2

/////////////////// 게임중 상태창의 스킬 버튼 좌표 ////////////////////////////////////////////
#define SKILL_TOP				574

//#define SKILL_UGUM_L			451
//#define SKILL_BIDO_L			508
//#define SKILL_SUPHON_L		568
#define SKILL_SWORD_L			625
#define SKILL_VALKYRE_L			685
#define SKILL_DRAGON_L			742

/////////////////// 미션 실패 화면에 쓸 실패 타이틀/배경 그림 좌표 ////////////////////////////
#define FAIL_RESULT_L			42
#define FAIL_RESULT_T			82

#define FAIL_BACK_L				42
#define FAIL_BACK_T				182



#endif