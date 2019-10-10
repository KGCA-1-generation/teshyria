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

// ���̰� �߰��� �ڵ� 1
class CMainChar;


/* m_pImgBackGround : 
		MainMenu    IDMenu   ItemBack   OptionMenu   Creater   BasicScene
		MissionLogo   Story  Mission   Loading   VicResult

   Extra new �̹��� :
		MainBtn
		IDbtn   IDMark
		ItemCaption   Armor   Weapon   SmallWeapon   SmallArmor   UpDownBtn
		VolumBar
		BasicText
		StoryText   StoryTitle   StoryArea
		MissionText   MissionTitle   VicObject   FailObject
		FailResult   FailBack
		Mystery(CAnimation)
		
   ���� ������ �̹��� :
		Time  EscMenu  EscBtn  Cursor   Status   Button
*/
typedef struct Ranking
{
	bool	bPlayingNow;				// ���� �÷��� �ϰ� �ִ� ���̵� �ΰ�
	char	szName[NAME_MAX];			// ���� ID
	int		nScore;						// ���� ��..
}Rank;



class CInterfaceMng  
{
//////////////////////////////////////////////////////////////////////////
// ��� ������
//////////////////////////////////////////////////////////////////////////
protected:


	CMainChar* m_pTeshy;					// ���̰� �߰��� �ڵ� 2: �׽ø��� ������ 
	CImage		m_ImgStatusWin;				// ���̰� �߰��� �ڵ� 4: ����â �̹���

	CImage	    *m_pImgBackGround;			// ��� �̹���
	CImage	    *m_pImgExtra1;				// �� ȭ�鿡�� ���� �̹���
	CImage	    *m_pImgExtra2;				// �� ȭ�鿡�� ���� �̹���
	CImage	    *m_pImgExtra3;				// �� ȭ�鿡�� ���� �̹���
	CImage	    *m_pImgExtra4;				// �� ȭ�鿡�� ���� �̹���
	CImage	    *m_pImgExtra5;				// �� ȭ�鿡�� ���� �̹���
	CImage	    *m_pImgExtra6;				// �� ȭ�鿡�� ���� �̹���
	CAnimation  *m_pAniMystery;				// ������ ����â���� �� ����ǥ �ִϸ��̼�
	int			m_nCountAniMys1;			// ����ǥ �ִϸ��̼� ������ ��ȣ�� ���� �ִ� ����
	int			m_nCountAniMys2;			// ����ǥ �ִϸ��̼� ������ ��ȣ�� ���� �ִ� ����
	int			m_nCountAniMys3;			// ����ǥ �ִϸ��̼� ������ ��ȣ�� ���� �ִ� ����
	int			m_nCountAniMys4;			// ����ǥ �ִϸ��̼� ������ ��ȣ�� ���� �ִ� ����


	CImage		m_ImgTime;					// �� ����ȭ�鿡�� ���� ���� �ð� �̹���
	CImage		m_ImgEscMenu;				// �� ����ȭ�鿡�� ESC�޴� ȭ���� ��� �̹���
	CImage		m_ImgEscBtn;				// �� ����ȭ�鿡�� ESC�޴� ȭ�鿡�� ����� �� ���� ���� ���� ��ư �̹���
	CImage		m_ImgCursor;				// ���콺 Ŀ�� �̹���
	CImage		m_ImgStatus;				// �� ����ȭ�鿡�� ���� �޴� �̹���
	CImage		m_ImgButton;				// ��ü���� �޴����� �ַ� ���̴� ��ư �̹���

	CMp3Manager *m_pBgm_mng;				// MP3 ����� ����� �����ϴ� Ŭ���� ����
	char		*m_pBgm[11];				// �� ������� ��� �� �̸��� �����ϰ� �ִ� ���ڿ� �迭
	bool		m_bChangeBgm;				// m_pBgm_mng->PlayBGM("����� �����.mp3") �Լ��� ȣ���ؾ� �ϳ� ���ؾ� �ϴ°��� �˷��ִ� ����
	bool		m_bChangeVolum;				// ������� ũ�⸦ ���� ��ų �������� ���θ� ��Ÿ���� ����

	CDM6Show	*m_pDMShow;					// �����׹��� ���� ������ ������� ���� ���̷�Ʈ��� ��ü
	
	unsigned long *m_pScr;					// ���̵���/�ƿ� �Ҷ� pScreen �� �ּҸ� ���� �ִ� ����

	int			m_nGameMode;				// �� ȭ���� ��Ÿ���� ����
	int			m_nReadyGameMode;			// ȭ���� ��ȯ �Ǳ��� ���̵� �ƿ� �ɶ� ���� ȭ���� ��ȣ�� �����ϰ� �ִ� ����

	bool		m_bNowEvent;				// �� ���� ȭ�鿡�� ���� �̺�Ʈ ȭ������ �ƴ����� ���� �մ� ����

	int			m_nMousePosX;				// ���콺 ���� ��ǥ
	int			m_nMousePosY;				// ���콺 ���� ��ǥ

	int			m_bMouseLB;					// ���콺 Ŭ�� ���� ����
	bool		m_bKeyESC;					// �� ���� ȭ�鿡�� ESC�޴��� Ȱ��ȭ ���θ� ���� �ִ� ����
	bool		m_bKeyReturn;				// EnterŰ ���� ���� ����
	bool		m_bKeyNext;					// ���콺 Ŭ�� �� EnterŰ �� ESCŰ �� SpaceŰ �� ���ȴ��� ���� ����
	bool		m_bKeyCancel;				// EscŰ ���� ���� ����

	int			m_nCursor;					// �� ���� ȭ��� ���콺�� ��ġ�� ���� ���콺 Ŀ�� ����� ��ȣ�� ���� �ִ� ����
	int			m_nCursorCount;				// ���콺 Ŀ���� �ִϸ��̼��� ���� ��������Ʈ �̹����� ������ ��ȣ�� ���� �մ� ����
	int			m_nAddCount;				// ���콺 Ŀ���� �ִϸ��̼� ������ ��ȣ�� ���� ���������ִ� ����

	int			m_nStep;					// �� ���� ȭ�鿡���� �ܰ踦 ��Ÿ���� ����(0:���̵��� / 1:����ȭ�� / 2:���̵� �ƿ�    //  0:ESC�޴� ���� ��ũ�� / 1:ESC�޴� ���� ȭ�� / 2:ESC�޴� �Ʒ��� ��ũ��)
	int			m_nFadeRate;				// ���̵� ��/�ƿ��Ǵ� ���� ���� �ִ� ����
	unsigned short m_wFadeColor;			// ���̵� ��/�ƿ��Ǵ� ���� ���� ���� �ִ� ����

	int			m_nPointBtn;				// �޴��� �������� �ֱ����� ���콺�� � ��ư���� ������ �� ������ ���� ���� �ִ� ���� 

	unsigned long m_dwTime;					// ���̸� �ֱ� ���� ����
	unsigned long m_dwNowTime;				// ���̸� �ֱ� ���� ���� �ð��� ���� �ִ� ����
	unsigned long m_dwOpenTime;				// ȭ�� ��ȯ�� �̷������ ������ �ð��� ���� �ִ� ����

	int			m_nWeaponClass;				// ���� ������ ��ȣ�� ���� �մ� ����
	int			m_nArmorClass;				// �� ������ ��ȣ�� ���� �ִ� ����
	int			m_nHPotionNum;				// �������� ����
	int			m_nMPotionNum;				// ���� ���� ����
	int			m_nSPotionNum;				// ���׹̳� ���� ����

	int			m_nHPotionStart;			// ó�� ��� �����ϴ� �������� ����
	int			m_nMPotionStart;			// ó�� ��� �����ϴ� �������� ����
	int			m_nSPotionStart;			// ó�� ��� �����ϴ� ���׹̳����� ����
	int			m_nWeightStart;				// �� ���� �÷��� �������� �ɸ��� Ŭ������ ���Ը� �Ѱ��ֱ� ���� ����
	int			m_nWeaponStart;				// �� ���� �÷��� �������� �ɸ��� Ŭ������ ���� ������ �Ѱ��ֱ� ���� ����
	int			m_nArmorStart;				// �� ���� �÷��� �������� �ɸ��� Ŭ������ ���� ������ �Ѱ��ֱ� ���� ����

	int			m_nHPotionNow;				// ���� �����ϰ� �ִ� �������� ����
	int			m_nMPotionNow;				// ���� �����ϰ� �ִ� �������� ����
	int			m_nSPotionNow;				// ���� �����ϰ� �ִ� ���׹̳����� ����

	int			m_nWeight;					// ���� ������ ������ �� ����
	int			m_nWeightItem;				// ���� ������ ����,�� ����
	

	int			m_nBgmVolum;				// ����� ũ��
	int			m_nSoundVolum;				// ���� ũ��
	int			m_nPreBgmVolum;				// ��� ��ư Ŭ���ø� ����� ���� ����� ũ��
	int			m_nPreSoundVolum;			// ��� ��ư Ŭ���ø� ����� ���� ���� ũ��

	CIDdata m_ID;							// CIDdata Ŭ���� ����	
	int		m_nSelectID;					// ���� ������ ID ��ȣ..

	char	m_szInputName[NAME_MAX];		// ���̵� �Է�â�� ���� ���ڿ�
	int		m_nCountName;					// �� ���ڿ��� ī����
	bool	m_bKeyConsent[KEY_KIND];		// �� ����(���̵�)�� ������ 1�� ������ 1���ڸ� ������ �ϱ����� ����
	char	m_szDeleteName[NAME_MAX];		// ���õ� ���̵��� ���ڿ�

	bool	m_bFinish;						// �� �ѹ��� ���𰡸� �����ϰ� �� �ĸ� ó���ϱ� ���� ����

	int		m_nSceneScrollY;				// �⺻��� ȭ�鿡�� ��ũ�� �Ǵ� �׸��� ���� ��ǥ

	int		m_nScrollY;						// �� ���� �÷��� ȭ���� ESC�޴��� ���� ��ũ�� �ǰ� �ϴ� ��ǥ

	bool	m_bLimitTime;					// ���� �ð��� �ִ� �̼��� ��� true ������ �����ؼ� �Ʒ� �ð��� �ʱ�ȭ�� ��...
	int		m_nLimitMin;					// ���ѽð� (�� ����)
	int		m_nLimitSec;					// ���ѽð� (�� ����)
	int		m_nStartMin;					// �÷��� ���۽� ���� �ð��� ����Ǵ� ����(�� ����)
	int		m_nStartSec;					// �÷��� ���۽� ���� �ð��� ����Ǵ� ����(�� ����)
	int		m_nPlayMin;						// �̼� �÷��� �ð�(�� ����)
	int		m_nPlaySec;						// �̼� �÷��� �ð�(�� ����)
	int		m_nPlayTimeMin;					// �̼� ��� ȭ�鿡�� ������ �÷��� �ð�(�� ����)
	int		m_nPlayTimeSec;					// �̼� ��� ȭ�鿡�� ������ �÷��� �ð�(�� ����)

	int		m_nAliveFriends;				// ��Ƴ��� �Ʊ� ��

	int		m_nResult;						// �̼� ��� ( �ʱ�: 0,  ����: 1, ����: 2 )

	int		m_nScore;						// �ش� �̼� ����
	int		m_nScoreTotal;					// �̹� �̼Ǳ����� ����	

	Rank	m_Rank[ID_MAX];					// �̹� �̼� ������ ������ �����ϱ� ���� ����ü ����
	Rank	m_RankTotal[ID_MAX];			// �̹� �̼Ǳ����� ���� ������ �����ϱ� ���� ����ü ����					

	int		m_nRankSt;						// ��ŷ ���� ��ũ��â���� ������ ������ ���̵��� ��ȣ		
	int		m_nRankTotalSt;					// �̹� �̼Ǳ����� ��ŷ ���� ��ũ��â���� ������ ������ ���̵��� ��ȣ			
	int		m_nSameRank;					// �̹� �̼��� ������ ���� ���̵�� ���� ��ŷ���� ����ϱ� ���� ����
	int		m_nSameRankTotal;				// �̹� �̼Ǳ����� ������ ���� ���� ���̵�� ���� ��ŷ���� ����ϱ� ���� ����


///////////////////////////////////////////////////////////////////////////
// public ��� �Լ���
///////////////////////////////////////////////////////////////////////////
public:

	CInterfaceMng();
	virtual ~CInterfaceMng();

	void	SetGameMode(int nMode, CDx7Draw* DxDraw = NULL);			// ȭ�� ��ȯ��Ű�� �Լ�
	int		GetGameMode(void)			{ return m_nGameMode; }			// ���� ȭ���� ��ȣ�� �����ִ� �Լ�

	void	SetNowEvent(bool show)		{ m_bNowEvent = show; }			// �̺�Ʈ�� Ȱ��ȭ/��Ȱ��ȭ ��Ű�� �Լ�
	bool	GetNowEvent(void)			{ return m_bNowEvent; }			// ���� �̺�Ʈ�� Ȱ��ȭ �Ǿ� �ִ� ���� ���� �����ִ� �Լ� 

	CMp3Manager* GetBgm_mng(void)		{ return m_pBgm_mng; }			// ����� ����� �����ϴ� ��ü�� �˷��ִ� �Լ�

	void	EtcGame(unsigned short* screen, int width, CDx7Draw* DxDraw);	// ���� �������� ���ǿ� ���� �Ź� ȣ��Ǵ� �Լ��� ���� ���� ȭ�鿡 ���� �׸����� �׸��� �Լ��� ���� ���ش�
	void	Loop_EscMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);    // �� ���� �÷����� ESC�޴� ȭ���� �׷��ִ� �Լ�
	void	DrawInterface(unsigned short* screen, int width, CDx7Draw* DxDraw);	  // �� ���� �÷��� �� �������̽����� �͵� �׷��ִ� �Լ�(����â(��,�ܵ� ����,��ųâ ���� �׸�), ���ѽð�, ���콺 ������ ���) 

	void	SetInputSts(int nX, int nY, int nInput);// ���� ������ InputProcess() ������ �׻� ȣ��Ǵ� �Լ� 

	void	SetKeyESC(bool bPush);										// ESC�޴��� Ȱ��ȭ/��Ȱ��ȭ ��Ű�� �Լ�
	bool	GetKeyESC(void)				{ return m_bKeyESC; }			// ESC�޴��� Ȱ��ȭ ���θ� �˷��ִ� �Լ�

	void	SetCursor(int type)			{ m_nCursor = type; }			// Ŀ���� ����� �ٲ��ִ� �Լ�

	void	PushChar(char name, int num);								// ���̵� ����â���� ������ �̸��� ���� �߰��ϴ� �Լ�
	void	BackSpace(void);											// ���̵� ����â���� �齺���̽� ó�� �Լ� 
	void	SetKeyConsent(bool bPush, int num);							// 1�� ���� ȿ���� ���� ���̴� �Լ�
	bool	GetKeyConsent(int num)		{ return m_bKeyConsent[num]; }	// 1�� ���� ȿ���� ���� ���̴� �Լ�
	int		GetCountName(void)			{ return m_nCountName; }		// ���̵� �Է�â�� ���ڿ��� ī���͸� ��ȯ���ִ� �Լ�

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

	//--------- �ɸ��� Ŭ������ �ʿ��� �����͸� �Ѱ��ִ� public �Լ��� -------------
	int		GetHPotionStart(void)		{ return m_nHPotionStart; }
	int		GetMPotionStart(void)		{ return m_nMPotionStart; }
	int		GetSPotionStart(void)		{ return m_nSPotionStart; }
	int		GetWeightStart(void)		{ return m_nWeightStart; }
	int		GetWeaponClassStart(void)	{ return m_nWeaponClass; }
	int		GetArmorClassStart(void)	{ return m_nArmorClass; }
	//---------�ɸ��� Ŭ������ �ִ� �����͸� ������ public �Լ��� ----------------
	void	SetHPotionNow(int num)		{ m_nHPotionNow = num; }
	void	SetMPotionNow(int num)		{ m_nMPotionNow = num; }
	void	SetSPotionNow(int num)		{ m_nSPotionNow = num; }
	
	void	SetMainChar(CMainChar* pTeshy)	{ m_pTeshy = pTeshy; }					// ���̰� �߰��� �ڵ� 3: �׽ø��� ������ ����



////////////////////////////////////////////////////////////////////////////
// protected ��� �Լ���
////////////////////////////////////////////////////////////////////////////
protected:
	void	Init(void);																// �����ڿ��� 1�� ȣ��Ǵ� �ʱ�ȭ �Լ�
	void	InitPlay(void);															// ȭ���� ��ȯ�Ǳ� ������ ȣ��Ǿ� �κ����� �ʱ�ȭ�� �����ִ� �Լ�

	void	Loop_Movie(CDx7Draw* DxDraw);											// �������� �����ִ� ȭ�� ó�� �Լ�
	void	Loop_MainMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);		// ���θ޴� ȭ���� �׷��ִ� �Լ�
	void	Loop_IDMenu(unsigned short* screen, int width, CDx7Draw* DxDraw);		// ID ���� ȭ���� �׷��ִ� �Լ�
	void	Loop_Creater(unsigned short* screen, int width, CDx7Draw* DxDraw);		// ������ �����ִ� ȭ���� �׷��ִ� �Լ�
	void	Loop_Option(unsigned short* screen, int width, CDx7Draw* DxDraw);		// �ɼ� ȭ���� �׷��ִ� �Լ�
	void	Loop_BasicScene(unsigned short* screen, int width, CDx7Draw* DxDraw);	// �⺻��� ���� ȭ���� �׷��ִ� �Լ�
	void	Loop_StoryObj(unsigned short* screen, int width, CDx7Draw* DxDraw);		// �� ���丮 ���� �� ���� ȭ���� �׷��ִ� �Լ�
	void	Loop_MissionLogo(unsigned short* screen, int width, CDx7Draw* DxDraw);	// �� �̼� �ΰ� ȭ���� �׷��ִ� �Լ�
	void	Loop_MissionObj(unsigned short* screen, int width, CDx7Draw* DxDraw);	// �� �̼� ���� �� ���� ȭ���� �׷��ִ� �Լ�
	void	Loop_ItemSelect(unsigned short* screen, int width, CDx7Draw* DxDraw);	// ������ ���� ȭ���� �׷��ִ� �Լ�
	void	Loop_Loading(unsigned short* screen, int width, CDx7Draw* DxDraw);		// �ε� ȭ���� �׷��ִ� �Լ�
	void	Loop_Victory(unsigned short* screen, int width, CDx7Draw* DxDraw);		// �̼� ���(����) ȭ���� �׷��ִ� �Լ�
	void	Loop_Fail(unsigned short* screen, int width, CDx7Draw* DxDraw);			// �̼� ���(����) ȭ���� �׷��ִ� �Լ�
	
	
	void	SetCursorFrame(void);													// Ŀ���� �ִϸ��̼� ó���� ���� �Լ�

	void	Fade(unsigned short srcColor, int rate, int width);						// ���̵� ��/�ƿ� �ǰ� �ϴ� �Լ�

	void	InitItem(bool bArmor);													// ������ �ʱ�ȭ..(���⸦ �ʱ�ȭ : bArmor = 0, ���� �ʱ�ȭ : bArmor = 1)
	void	SetWeaponClass(int nClass);												// ���� ����
	void	SetArmorClass(int nClass);												// �� ����

	void	InitScroll(void);														// ������ ESC�޴��� ���¸� �ʱ�ȭ�ϴ� �Լ�
	void	SetScroll(void);														// ������ ESC�޴��� ��ũ�ѵǰ� ��ǥ �����ϰ� �ϴ� �Լ�

	void	RunPlayTime();															// ���� �ð��� ���ҿ� �÷��� �ð��� ���� ��Ű�� �Լ�
	void	ShowLimitTime(unsigned short* screen, int width, CDx7Draw* DxDraw);		// ���� �ð��� �׸����� ����ϴ� �Լ�

	int		GetWasteHPotion(void)		{ return (m_nHPotionStart - m_nHPotionNow);}	// �Ҹ��� �������� ����
	int		GetWasteMPotion(void)		{ return (m_nMPotionStart - m_nMPotionNow);}	// �Ҹ��� �������� ����
	int		GetWasteSPotion(void)		{ return (m_nSPotionStart - m_nSPotionNow);}	// �Ҹ��� ���׹̳����� ����



};

#endif // !defined(AFX_INTERFACEMNG_H__103DE301_1659_11D6_944A_0050DA898DDD__INCLUDED_)
