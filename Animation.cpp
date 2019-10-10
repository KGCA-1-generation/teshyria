// Animation.cpp: implementation of the CAnimation class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <string.h>
#include "Animation.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CAnimation::CAnimation()
{
	m_pCX        = NULL;
	m_pCY        = NULL;
	m_pMotFrmNum = NULL;
	m_pMotOffset = NULL;	
	m_pFrmData   = NULL;
	m_sMotID     = NULL;
	m_nRealFrame = 0;
}

CAnimation::~CAnimation()
{
	if(m_pCX != NULL)        { delete m_pCX; m_pCX = NULL; }
	if(m_pCY != NULL)        { delete m_pCY; m_pCY = NULL; }
	if(m_pMotFrmNum != NULL) { delete m_pMotFrmNum; m_pMotFrmNum = NULL; }
	if(m_pMotOffset != NULL) { delete m_pMotOffset; m_pMotOffset = NULL; }
	if(m_sMotID != NULL)     { delete[12] m_sMotID; m_sMotID = NULL; }
	if(m_pFrmData != NULL)   { delete m_pFrmData; m_pFrmData = NULL; }
}

bool CAnimation::LoadAni(char *fn)
{
	FILE* file;

	file = fopen(fn, "rb");
	if(file == NULL) return false;

	char sprname[16], SprFName[256] = {0, };
	fread(sprname, 16, 1, file);
	strcpy(SprFName, "DATA\\" );
	strcat(SprFName, sprname);
	if(!LoadSpr(SprFName)) return false;

	m_pCX = new int[m_nTotalFrame];
	if(m_pCX==NULL) return false;
	m_pCY = new int[m_nTotalFrame];
	if(m_pCY==NULL) return false;
	for(int i=0; i<m_nTotalFrame; i++)
	{
		fread(m_pCX+i, 4, 1, file);
		fread(m_pCY+i, 4, 1, file);
	}
	fread(&m_nMotCount, 4, 1, file);

	m_sMotID     = new char[m_nMotCount][12];
	if(m_sMotID==NULL) return false;
	m_pMotFrmNum = new int[m_nMotCount];
	if(m_pMotFrmNum==NULL) return false;
	m_pMotOffset = new int[m_nMotCount];
	if(m_pMotOffset==NULL) return false;

	for(i=0; i<m_nMotCount; i++)
	{
		fread(m_sMotID+i, 10, 1, file);
		fread(m_pMotFrmNum+i, 4, 1, file);
		fread(m_pMotOffset+i, 4, 1, file);
	}

	fread(&m_nTotalMotFrm, 4, 1, file);
	m_pFrmData = new int[m_nTotalMotFrm];
	if(m_pFrmData==NULL) return false;
	fread(m_pFrmData, 4, m_nTotalMotFrm, file);

	if(fclose(file)!=NULL) return false;
	return true;
}

//----------------------------------------------------------
// 캐릭의 애니를 그려줌
//----------------------------------------------------------
void CAnimation::DrawAni(int x, int y, int Motion, int Frame, WORD *Screen)
{
	//int a = m_pFrmData[m_pMotOffset[Motion]+Frame];
	m_nRealFrame = GetFrmData(Motion, Frame);

	DrawSpr(x-m_pCX[m_nRealFrame], y-m_pCY[m_nRealFrame], m_nRealFrame, Screen);
}

//----------------------------------------------------------
// 캐릭의 애니를 그려줌
//----------------------------------------------------------
void CAnimation::DrawAniOL(int x, int y, int Motion, int Frame, WORD *Screen)
{
	m_nRealFrame = GetFrmData(Motion, Frame);

	DrawSprOL(x-m_pCX[m_nRealFrame], y-m_pCY[m_nRealFrame], m_nRealFrame, Screen);
}


