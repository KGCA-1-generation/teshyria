#ifndef _ITFDEFINE_H_
#define _ITFDEFINE_H_

///////////  ���� ���(CInterfaceMaster::m_nGameMode)  ////////////////////////////////////
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
//------- �̺�Ʈ ���� ��� --------
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
///////////////////////////// ���� ��忡 ���� ����� /////////////////////////////////////////////
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

///////////////////////////// ���콺 Ŀ�� ��� /////////////////////////////////////////////////////
#define	CURS_FRAME				4		// �� Ŀ�� ��������Ʈ ������ ��
#define CURSOR_DELAY			500		// Ŀ�� �ִϸ��̼� �ӵ�(1/1000 ��)

#define CURSOR_MENU_NORMAL		0		// �������̽� �޴� ���� Ŀ�� 
#define CURSOR_ABOVE_BUTTON		(1 * CURS_FRAME)		// �������̽� �޴��� ��ư ���� Ŀ�� 
#define CURSOR_WAIT				(2 * CURS_FRAME)		// �������� ��ٸ� ���� ��û�ϴ� Ŀ�� 
#define CURSOR_MOVE_ABLE		(3 * CURS_FRAME)		// ���� ������, �̵������� �ʵ��������� �Ϲ����� Ŀ�� 
#define CURSOR_MOVE_DISABLE		(4 * CURS_FRAME)		// ���� ������, �̵� �Ұ����� �ʵ��������� Ŀ�� 
#define CURSOR_ATTACK			(5 * CURS_FRAME)		// ���� ������, ��/����/���� ���� ���� ������ ����������� Ŀ��
#define CURSOR_TALK				(6 * CURS_FRAME)		// ���� ������, �̺�Ʈ ���� ��ȭ�� ������ NPC �������� Ŀ��
#define CURSOR_ABOVE_ITEM		(7 * CURS_FRAME)		// ���� ������, ������ �������� Ŀ��

///////////////////////////// �� �޴������� ��ư ��ġ ��ǥ(����)////// ������ �׽�Ʈ�� ��ǥ��.../////
// OK / ����
#define BTN_OK_L				557
#define BTN_OK_T				546
#define BTN_OK_R				660
#define BTN_OK_B				580
// ��� / ���ư��� / ���θ޴���
#define BTN_CANCEL_L			677
#define BTN_CANCEL_T			546
#define BTN_CANCEL_R			780
#define BTN_CANCEL_B			580
// ����
#define BTN_SAVE_L				437
#define BTN_SAVE_T				546
#define BTN_SAVE_R				540
#define BTN_SAVE_B				580

//-----������ Esc�޴� ��ư---------
// ���θ޴���
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

//�������� ��ư
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

//-----�ɼǸ޴����� ���� �������� Ŭ���Ǵ� �κ� ��ǥ-----
// ������ �β�, ����
#define VOLUM_WIDTH				90
#define VOLUM_HEIGHT			25
// �����,���� Top ��ǥ
#define BGM_TOP					470
#define SOUND_TOP				505
// ���� X ��ǥ
#define BTN_VOLUM_0				130
#define BTN_VOLUM_1				220
#define BTN_VOLUM_2				310
#define BTN_VOLUM_3				400
#define BTN_VOLUM_4				490
#define BTN_VOLUM_5				580
#define BTN_VOLUM_6				670
//---------------------------------

// ���� �޴� ��ư..  (��ŸƮ, �ɼ�, ������ ����, �����ڵ�, ������)
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
// ID ��� �� ����â����
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





//------������ ���� ��忡��..--------------------------------------------------
//-------- ����(WeaPon)
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

//--------- �� (ArMor)
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

//-------- ����(HP,MP,SP)
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

////////////////////// �ɸ��� Ŭ������ �Բ� ��� �� define ��ũ��.. /////////////////////////
//----- ����ǥ �ִϸ��̼� �ӵ�(1/1000��) --------
#define SPEED_ANI_MYSTERY		100
//------- �� �̼� ���۽� ��� �ʱ� �ܵ� ����
#define MAX_KNIFE				60
#define MAX_POTION				60
//------- �׽ø��� ������ �� �� �ִ� �ִ� �ѵ� ����
#define MAX_WEIGHT				80
//------- ���ΰ� �ɸ��� ���� �ִ� ��ġ
#define MAX_HP					1000
#define MAX_MP					1000
#define MAX_SP					1000
//------- ���� ����
#define WP_RAIPEAR				0
#define WP_EPEE					1
#define WP_GLADIUS				2
#define WP_LONG					3
#define WP_BASTARD				4
#define WP_TWOHAND				5
#define WP_DRAGON				6
#define WP_SAINT				7
#define WP_NONE					8
//------- �� ����
#define AM_CLOSE				0
#define AM_HALF					1
#define AM_CHAIN				2
#define AM_SCALE				3
#define AM_PLATE				4
#define AM_COMPOSITE			5
#define AM_DRAGON				6
#define AM_SAINT				7
#define AM_NONE					8

//------- ����
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


/////////////////// HP, MP, SP ������ ���� ///////////////////////////////////////////////////
#define LENGTH_HP				100
#define LENGTH_MP				100
#define LENGTH_SP				100

/////////////////// �ڵ� ȭ�� ��ȯ (�ΰ�,Ÿ��Ʋ,�� �̼� �ΰ�, �ε�, ����) �� ������ //////////
#define DELAY_MODE				2500	

/////////////////// �� �̼� ���� �ð� ////////////////////////////////////////////////////////
#define LIMIT_MIN_1				12
#define LIMIT_SEC_1				0

#define LIMIT_MIN_2				15
#define LIMIT_SEC_2				30
			
#define LIMIT_MIN_3				20
#define LIMIT_SEC_3				15

/////////////////// �� ��� �̼� �Ʊ� �� /////////////////////////////////////////////////////
#define MAX_FRIEND_1			21
#define MAX_FRIEND_2			18
#define MAX_FRIEND_3			15

/////////////////// â �����(��ũ��) �ӵ� ///////////////////////////////////////////////////
#define MAKE_SPEED				24

/////////////////// ���̵� �ƿ�/�� ��ũ��  ///////////////////////////////////////////////////
#define dHALF_MASK				0x7bde
#define FADE_RATE				6
#define FADE_MAX				29
#define FADE_MIN				4

#define FADE_LOAD_RATE			3

/////////////////// ���̵� ���� ���� /////////////////////////////////////////////////////////
#define NAME_MAX				16
/////////////////// ���̵� ���� ���� /////////////////////////////////////////////////////////
#define ID_MAX					24
/////////////////// �̼� �� ���� /////////////////////////////////////////////////////////////
#define MISSION_NUM				6

/////////////////// ���̵� �Է½� ���� Ű(������, ���� ���ո� ����, �齺���̽� ����) /////////
#define KEY_KIND				37

/////////////////// �⺻��� ���丮 ���� ��ũ�ѵǴ� ��ǥ /////////////////////////////////////
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

/////////////////// ������ ����â������ �� ������â�� ��ǥ�� ������ ����ǥ ��ǥ ///////////////
#define MYSTERY_X				40
#define MYSTERY_Y				43
/////////////////// ������ ����ȭ�鿡�� �������� �׷��� ǳ���� ��ǥ ///////////////////////////
#define CAPTION_HP_X			420			
#define CAPTION_MP_X			545
#define CAPTION_SP_X			670
#define CAPTION_POTION_Y		482

/////////////////// ���������� �ʱ� �Ʊ��� ////////////////////////////////////////////////////
#define FRIENDS_1				18
#define FRIENDS_2				15
#define FRIENDS_3				12

/////////////////// �̼� ���� ȭ�鿡 �� ���� Ÿ��Ʋ �׸� ��ǥ /////////////////////////////////
#define FAIL_RESULT_L			42
#define FAIL_RESULT_T			82

/////////////////// ��ŷ ��ư ��ǥ �� ��ŷ �����ֱ� �Ѱ� //////////////////////////////////////
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

/////////////////// ������ ����â�� ��ų ��ư ��ǥ ////////////////////////////////////////////
#define SKILL_TOP				574

//#define SKILL_UGUM_L			451
//#define SKILL_BIDO_L			508
//#define SKILL_SUPHON_L		568
#define SKILL_SWORD_L			625
#define SKILL_VALKYRE_L			685
#define SKILL_DRAGON_L			742

/////////////////// �̼� ���� ȭ�鿡 �� ���� Ÿ��Ʋ/��� �׸� ��ǥ ////////////////////////////
#define FAIL_RESULT_L			42
#define FAIL_RESULT_T			82

#define FAIL_BACK_L				42
#define FAIL_BACK_T				182



#endif