// CharMng.cpp: implementation of the CCharMng class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include "Animation.h"
#include "function.h"
#include "NeedStruct.h"
#include "MainChar.h"
#include "Enemy.h"
#include "ScoutMage.h"
#include "ScoutKnight.h"
#include "MstKnight.h"
#include "RangerKnight.h"
#include "RangerMage.h"
#include "OurMage.h"
#include "OurKnight.h"
#include "Animal.h"

#include "CharObj.h"
#include "PathFinder.h"
#include "Assist.h"

#include "CharMng.h"
#include "MapMng.h"

#define NULL   0
#define TRUE   1
#define FALSE  0

#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } }
#define SWAP(a,b)	{SORTY temp = a; a=b; b=temp; }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CCharMng::CCharMng()
{
	m_pStageInfo = NULL;
	m_pMapMng    = NULL;
	m_pTeshy     = NULL;
	m_pPath      = NULL;
	m_nArrowDir  = -1;
	m_nSelectedChar = -1;
	m_nObjectNum = 0;
	m_nTest = 0;
	for(int i=0; i<ANILIST; i++) m_pAniArray[i] = NULL;
	for(i=0; i<MAXNUM; i++) {
		m_pScMage[i]    = NULL;
		m_pScKnight[i]  = NULL;
		m_pRgKnight[i]  = NULL;
		m_pRgMage[i]    = NULL;
		m_pMKnight[i]   = NULL;
		m_pOurMage[i]   = NULL;
		m_pOurKnight[i] = NULL;
		m_pAnimal[i]    = NULL;
	}
	for(i=0; i<OBJCNT; i++) m_pObject[i] = NULL;
}

CCharMng::~CCharMng()
{
	for(int i=0; i<ANILIST; i++) SAFE_DELETE(m_pAniArray[i])
	SAFE_DELETE(m_pPath)
	SAFE_DELETE(m_pAssist)
	for(i=0; i<OBJCNT; i++) SAFE_DELETE(m_pObject[i])
	DeleteChar();
}

void CCharMng::LoadAnis()
{
	strcpy(m_strAniFile[0], "DATA\\teshy.ani");
	strcpy(m_strAniFile[1], "DATA\\ScMage.ani");
	strcpy(m_strAniFile[2], "DATA\\ScKnight.ani");
	strcpy(m_strAniFile[3], "DATA\\RgMage.ani");
	strcpy(m_strAniFile[4], "DATA\\RgKnight.ani");
	strcpy(m_strAniFile[5], "DATA\\MKnight.ani");
	strcpy(m_strAniFile[6], "DATA\\DvMage.ani");
	strcpy(m_strAniFile[7], "DATA\\DvKnight.ani");
	strcpy(m_strAniFile[8], "DATA\\DvMKnight.ani");
	strcpy(m_strAniFile[9], "DATA\\Mage.ani");
	strcpy(m_strAniFile[10], "DATA\\Knight.ani");
	strcpy(m_strAniFile[11], "DATA\\Rabbit.ani");
	strcpy(m_strAniFile[12], "DATA\\Wolf.ani");
	strcpy(m_strAniFile[13], "DATA\\Bear.ani");
	strcpy(m_strAniFile[14], "DATA\\Skeleton.ani");
	strcpy(m_strAniFile[15], "DATA\\Orc.ani");
	strcpy(m_strAniFile[16], "DATA\\Nol.ani");
	strcpy(m_strAniFile[17], "DATA\\Atten.ani");
	strcpy(m_strAniFile[18], "DATA\\TObject.ani");

	// �׽ø��� �׸� �ε�
	m_pAniArray[0] = new CAnimation;
	m_pAniArray[0]->LoadAni(m_strAniFile[0]);
	// �׿� ĳ���� �׸� �ε�
	for(int i=1; i<ANILIST; i++)
	{
		if(m_pStageInfo->aCharCnt[i-1] != 0 )
		{
			m_pAniArray[i] = new CAnimation;
			m_pAniArray[i]->LoadAni(m_strAniFile[i]);
		}
	}
	m_pAniArray[18] = new CAnimation;
	m_pAniArray[18]->LoadAni(m_strAniFile[18]);
}

void CCharMng::SetInfo(CMapMng* pMap, StageInfo* Stage)
{
	m_pMapMng = pMap;
	m_pStageInfo = Stage;
	m_nTeamCnt = m_pStageInfo->nRGTeamCnt + m_pStageInfo->nSCTeamCnt;
	SettingCharAttr();
	
	LoadAnis();
	m_pPath = new CPathFinder;
	m_pPath->SetMap(m_pMapMng->m_pMap->m_pAttrMap, m_pMapMng->m_pMap->GetAttrMapWidth());

	CreateChar();
}
void CCharMng::CreateChar()
{
	int i, j, count=0;
	m_pTeshy = new CMainChar(m_pAniArray[0], this);
	m_pAssist = new CAssist(m_pAniArray[17]);

	for(i=0; i<ALLCharTYPE; i++) {
		switch(i) {
		case 0 : // ������ ������ ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pScMage[j] = new CScoutMage(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count]);
				m_pEnemy[count] = (CEnemy *)m_pScMage[j];
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}			
			break;
		case 1 : // ������ ��� ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pScKnight[j] = new CScoutKnight(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count]);
				m_pEnemy[count] = (CEnemy *)m_pScKnight[j];
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 2 : // ������ ������ ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pRgMage[j] = new CRangerMage(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count]);
				m_pEnemy[count] = (CEnemy *)m_pRgMage[j];
				if(m_pStageInfo->abLeader[count]) {
					m_pEnemy[count]->IamLeader();
					m_pTeam[m_pStageInfo->aTeamNum[count]].nLeaderNum = count;
				}
				else {
					m_pEnemy[count]->SetGap(m_pStageInfo->aPosInTeam[count]);
				}

				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 3:  // ������ ���
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pRgKnight[j] = new CRangerKnight(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count]);
				m_pEnemy[count] = (CEnemy *)m_pRgKnight[j];
				if(m_pStageInfo->abLeader[count]) {
					m_pEnemy[count]->IamLeader();
					m_pTeam[m_pStageInfo->aTeamNum[count]].nLeaderNum = count;
				}
				else {
					m_pEnemy[count]->SetGap(m_pStageInfo->aPosInTeam[count]);
				}
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 4 : // ������ ������ ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pMKnight[j] = new CMstKnight(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count]);
				m_pEnemy[count] = (CEnemy *)m_pMKnight[j];
				if(m_pStageInfo->abLeader[count]) {
					m_pEnemy[count]->IamLeader();
					m_pTeam[m_pStageInfo->aTeamNum[count]].nLeaderNum = count;
				}
				else {
					m_pEnemy[count]->SetGap(m_pStageInfo->aPosInTeam[count]);
				}
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 5 : // ���� ������ ����
			m_aUnitSum[i] = count;
			break;
		case 6 : // ���� ��� ����
			m_aUnitSum[i] = count;
			break;
		case 7 : // ���� ������ ����
			m_aUnitSum[i] = count;
			break;
		case 8 : // �Ʊ� ������ ������ ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pOurMage[j] = new COurMage(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count]);
				m_pEnemy[count] = (CEnemy *)m_pOurMage[j];
				count ++;
			}
			break;
		case 9 : // �Ʊ� ������ ��� ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pOurKnight[j] = new COurKnight(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count]);
				m_pEnemy[count] = (CEnemy *)m_pOurKnight[j];
				count ++;
			}
			break;
		case 10 : // �䳢 ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pAnimal[j] = new CAnimal(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count], i);
				m_pEnemy[count] = (CEnemy *)m_pAnimal[j];
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 11 : // ���� ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pAnimal[j] = new CAnimal(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count], i);
				m_pEnemy[count] = (CEnemy *)m_pAnimal[j];
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 12 : // ������ ����
			m_aUnitSum[i] = count;
			for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) {
				m_pAnimal[j] = new CAnimal(m_pAniArray[i+1], this,
											  m_pStageInfo->aXPosInMap[count], 
											  m_pStageInfo->aYPosInMap[count],
											  m_pStageInfo->aTeamNum[count], i);
				m_pEnemy[count] = (CEnemy *)m_pAnimal[j];
				m_pEnemy[count]->SetPathFinder(m_pPath);
				count ++;
			}
			break;
		case 13 : // ���̷��� ����
			m_aUnitSum[i] = count;
			break;
		case 14 : // ��ũ ����
			m_aUnitSum[i] = count;
			break;
		case 15 : // �� ����
			m_aUnitSum[i] = count;
			break;
		}
	}
	m_nLoadedCharCnt = count;
}

bool CCharMng::CreateObj(int num, int x, int y, int tx, int ty)
{
	m_pObject[m_nObjectNum] = new CCharObj(m_pAniArray[18], this, num, x, y, tx, ty);
	if(m_pObject[m_nObjectNum]==NULL) return false;

	m_nObjectNum++;
	m_nObjectNum = m_nObjectNum % OBJCNT;

	return true;
}

bool CCharMng::DeleteObj(int num)
{	
	SAFE_DELETE(m_pObject[num])

	return true;
}

void CCharMng::DeleteChar()
{
	int i, j;

	SAFE_DELETE(m_pTeshy)
	
	for(i=0; i<ALLCharTYPE; i++) {
		for(j=0; j<m_pStageInfo->aCharCnt[i]; j++) 
			SAFE_DELETE(m_pEnemy[j+m_aUnitSum[i]])
	}
}
void CCharMng::Draw(unsigned short* Screen, point* Curs)
{
	int i, num;
	for(i=0; i<m_nLoadedCharCnt; i++)
	{
		if(m_pEnemy[i]->IsDead())
			m_pEnemy[i]->DrawChar(Screen, 0);
	}

	for(i=0; i<m_nCharCnt; i++)
	{
		switch(m_aSort[i].m_nChar)
		{
		case -1: // ������Ʈ�� �׸���
			m_pMapMng->DrawObjectMap(Screen, m_aSort[i].m_nNum, m_aSort[i].m_nScrX, m_aSort[i].m_nScrY);
			break;
		case 0:  // �׽ø��� �׸���
			m_pTeshy->DrawChar(Screen);
			break;
		default:
			num = m_aSort[i].m_nNum+m_aUnitSum[m_aSort[i].m_nChar-1];
			m_pEnemy[num]->DrawChar(Screen, (m_nSelectedChar==num));
			break;
		}
	}
	m_pAssist->DrawArrow(Screen, m_nArrowDir);

	// ĳ���Ϳ� ���� ������Ʈ �׸��� (�����ַο�, ��˼����)
	for(i=0; i<OBJCNT; i++)
	{
		if(m_pObject[i]!=NULL) m_pObject[i]->Draw(Screen, m_nTeX, m_nTeY);
	}
}

void CCharMng::StatusIn(int* dir, int key, point* Teshy, point* Curs)
{
	int i;
	MarkPos(m_nTeX, m_nTeY, 1, FALSE);
	m_nTeX = Teshy->x;
	m_nTeY = Teshy->y;
	MarkPos(m_nTeX, m_nTeY, 1, TRUE);
	m_pTeshy->SetStatus(dir, key);
	m_nSelectedChar=-1;
	
	// Ŀ�� ��ǥ�� �� ��ǥ�� ��ȯ
	m_nCurX = m_nTeX-400+Curs->x;
	m_nCurY = m_nTeY-300+Curs->y;
	// Ŀ���� ������ ĳ���� �ִ� �� üũ
	for(i=0; i<m_nLoadedCharCnt; i++)
	{
		if(!m_pEnemy[i]->IsDead() && m_pEnemy[i]->IsFrmRect(m_nCurX, m_nCurY))
		{
			m_nSelectedChar = i;
			break;
		}
	}

	if((m_nSelectedChar!=-1) && (m_pTeshy->WhatAttack()==1 || m_pTeshy->WhatAttack()==2)) {
		HitEnemy(m_nSelectedChar, (*dir), m_pTeshy->WhatAttack());
	}
	SetSortList();
	SortYPos();
}

BOOL CCharMng::CanScroll()
{
	if(m_pTeshy->IsLock()) return FALSE;

	return TRUE;
}

void CCharMng::LocationIn(point* Teshy)
{
	int i;
	BOOL Is = FALSE;

	for(i=0; i<m_pStageInfo->nRGTeamCnt; i++)
	{
		if(m_pEnemy[m_pTeam[i+m_pStageInfo->nSCTeamCnt].nLeaderNum]->IsDead()
			&& !m_pTeam[i+m_pStageInfo->nSCTeamCnt].bAllDead )
		{		
			ResetTeamInfo(i+m_pStageInfo->nSCTeamCnt); 
		}
	}

	// ��� ĳ������ ���� ������ �Ѱ��ִ� ��
	for(i=0; i<m_nLoadedCharCnt; i++)
		m_pEnemy[i]->SetStatus(Teshy, &m_pTeam[m_pEnemy[i]->GetTeamNum()]);

	// ������ �߽߰� ȭ��ǥ �ߴ� �� ó��.
	for(i=0; i<m_pStageInfo->nSCTeamCnt; i++)
	{
		if(m_pTeam[i].bFind && m_pTeam[i].bLeader && m_pTeam[i].Distance>300) {
			m_nArrowDir = GetDirection((m_pTeam[i].LeaderX-m_nTeX), (m_nTeY-m_pTeam[i].LeaderY));
			Is = TRUE;
		}
	}
	if(!Is) m_nArrowDir = -1;

	// ���ư��� ������Ʈ ó��.
	for(i=0; i<OBJCNT; i++) {
		if(m_pObject[i]) {
			if(m_pObject[i]->m_bEnd) {
				if((m_pObject[i]->GetType()==3) || (m_pObject[i]->GetType()==7))
				{
					m_pTeshy->AniContinue();
				}
				SAFE_DELETE(m_pObject[i])
				m_nTest = 0;
			}
		}		
	}
}

int CCharMng::GetDistance()
{
	return m_pTeshy->WhatAttack();
}

void CCharMng::SettingCharAttr()
{	
	int i;
	for(i=0; i<m_pStageInfo->nTeamCnt; i++) {
		m_pTeam[i].clear();
		m_pTeam[i].pWpX = m_pStageInfo->aWayX[i]; 
		m_pTeam[i].pWpY = m_pStageInfo->aWayY[i]; 
		m_pTeam[i].nWPCnt = m_pStageInfo->aWayCnt[i];
	}
	for(i=0; i<SORTCHAR; i++) m_aSort[i].clear();
}

void CCharMng::BeatenTeshy(int npower)
{
	m_pTeshy->AmIBeaten(npower);
}

//////////////////////////////////////////////////
///		Y���� �Լ� 
//////////////////////////////////////////////////
void CCharMng::SortYPos()
{
	for( int j=m_nCharCnt-1; j>0; j-- )
		for( int i=0; i<j; i++)
			if( m_aSort[i].m_nY > m_aSort[i+1].m_nY )
				SWAP(m_aSort[i], m_aSort[i+1]);
}

//////////////////////////////////////////////////
///		Y������ ��ü���� ���� �����Լ� 
//////////////////////////////////////////////////
void CCharMng::SetSortList()
{
	int i, j, Count=0, CharCnt=0; 
	//nObj = m_pMapMng->m_pMap->SortObjectCount();	// ȭ����� ������Ʈ ���� 

	// �׽ø��� ���� 
	m_aSort[Count].m_nChar = 0; 
	m_aSort[Count].m_nNum  = 0;
	m_aSort[Count].m_nX = m_nTeX;
	m_aSort[Count].m_nY = m_nTeY;

	for(i=0; i<13; i++) {
		for(j=0; j<m_pStageInfo->aCharCnt[i]; j++)
		{
			if(!m_pEnemy[CharCnt]->IsDead())
			{
				m_aSort[++Count].m_nChar = i+1;
				m_aSort[Count].m_nNum = j;
				m_aSort[Count].m_nY = m_pEnemy[Count-1]->GetYPos();
				m_aSort[Count].m_nX = m_pEnemy[Count-1]->GetXPos();
			}
			CharCnt++;
		}
	}

	if(m_pTeshy->m_bConcent)
		m_pTeshy->m_bConcent = m_pMapMng->m_pMap->SetConEnemyPos(m_aSort, Count);
	
	//m_pMapMng->m_pMap->SetEnemyPos(m_aSort, Count);
	// ������Ʈ ���� 
	m_nCharCnt = m_pMapMng->m_pMap->SetObjectSortList(m_aSort, Count);			
}

int CCharMng::GetMapAttrP(int Xp, int Yp)
{
	return m_pMapMng->m_pMap->m_pAttrMap[(Yp/32)*m_pMapMng->m_pMap->GetAttrMapWidth()+(Xp/32)];
}

int CCharMng::GetMapAttrN(int Xn, int Yn)
{
	return m_pMapMng->m_pMap->m_pAttrMap[Yn*m_pMapMng->m_pMap->GetAttrMapWidth()+Xn];
}

//-----------------------------------------------------------
//  type : 1 �׽ø��� , 0 : �ٸ� �ֵ�.
//-----------------------------------------------------------
void CCharMng::MarkPos(int x, int y, int type, BOOL bOn)
{
	int w;
	if( x%32 < 16 ) w = -1; else w = 1;
	x = x>>5;
	y = y>>5;
	int check = 0x00000010;
	int mapwidth = m_pMapMng->m_pMap->GetAttrMapWidth();
	if(type==1) check = (check<<1); // �׽ø����� ���

	if(bOn) // �ѱ� ���
	{
		m_pMapMng->m_pMap->m_pAttrMap[y*mapwidth+x] |= check;
	//	m_pMapMng->m_pMap->m_pAttrMap[y*mapwidth+x+w] |= check;
	}
	else
	{
		m_pMapMng->m_pMap->m_pAttrMap[y*mapwidth+x] &= (~check);
	//	m_pMapMng->m_pMap->m_pAttrMap[y*mapwidth+x+w] &= (~check);
	}
}

void CCharMng::ResetTeamInfo(int num)
{
	int i=0, temp = -1;
	m_pEnemy[m_pTeam[num].nLeaderNum]->IamNotLeader();
	m_pStageInfo->abLeader[m_pTeam[num].nLeaderNum] = false;
	// �ϴ� ���������� �������� -1�� �Ѵ�.
	for(i=0; i<m_nLoadedCharCnt; i++)
	{
		if(m_pStageInfo->aTeamNum[i]==num)
		{
			m_pStageInfo->aPosInTeam[i]--;
			if(temp<m_pStageInfo->aPosInTeam[i]) 
				temp = m_pStageInfo->aPosInTeam[i];
		}
	}
	if(temp == -1) { 
		m_pTeam[num].bAllDead = true;
		return;
	}
	// �״��� �������� ������ ���� ������ �����ְ�. GapX�� ����.
	for(i=0; i<m_nLoadedCharCnt; i++)
	{
		if(m_pStageInfo->aTeamNum[i]==num)
		{
			if(m_pStageInfo->aPosInTeam[i]==0) {
				m_pTeam[num].nLeaderNum = i;
				m_pEnemy[i]->IamLeader();
				m_pStageInfo->abLeader[i] = true;
			}
			m_pEnemy[i]->SetGap(m_pStageInfo->aPosInTeam[i]);
		}
	}		
}

//-----------------------------------------------------------
// �׽ø��ư� �� ������Ʈ�� �� ĳ���̶� �浹 üũ
// return : ��ĳ���� ��ȣ,  -1�� �ƹ��͵� ���� ���� ��.
//-----------------------------------------------------------
int CCharMng::TObjectAndEnemy(rect* rt)
{
	for(int i=0; i</*ALLCharTYPE*/7; i++) 
	{
		for(int j=m_aUnitSum[i]; j<m_aUnitSum[i+1]; j++)
		{
			if(m_pEnemy[j]->IsFrmRect(rt)) return j;
		}
	}
	return -1;
}

void CCharMng::HitEnemy(int SChar, int dir, int AtckType)
{
	m_pEnemy[SChar]->AmIBeaten(&dir, AtckType);
	m_pTeshy->AttackNormalize();
}