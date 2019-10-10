// MapMng.cpp: implementation of the CMapMng class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <windows.h>
#include "MapMng.h"
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMapMng::CMapMng()
{
	m_pMap = NULL;
	m_pTileImg = NULL;
	m_pCliffImg = NULL;
	m_pObjImg = NULL;

	m_strStageFile[0] = "DATA\\Map\\S1M1.mis";
	m_strStageFile[1] = "DATA\\Map\\S1M2.mis";
	m_strStageFile[2] = "DATA\\Map\\S2M1.mis";
	m_strStageFile[3] = "DATA\\Map\\S2M2.mis";
	m_strStageFile[4] = "DATA\\Map\\S3M1.mis";
	m_strStageFile[5] = "DATA\\Map\\S3M2.mis";
}

CMapMng::~CMapMng()
{
//	SAFE_DELETE(m_pObjImg);
	SAFE_DELETE(m_pCliffImg);
	SAFE_DELETE(m_pTileImg);
	SAFE_DELETE(m_pMap);
}

bool CMapMng::LoadMapImg(BOOL b565Mode)
{
	FILE* file;

	file = fopen("Data\\Map\\TeshyriaMap.Img", "rb");
	if(file == NULL) return false;

	fread( &m_nTileW, 4, 1, file );
	fread( &m_nTileH, 4, 1, file );
	fread( &m_nAttrW, 4, 1, file );
	fread( &m_nAttrH, 4, 1, file );

	fread( &m_nTileTotal, 4, 1, file );
	fread( &m_nCliffTotal, 4, 1, file );
	fread( &m_nObjTotal, 4, 1, file );

	InitMapImg();

	// 타일 이미지 읽기 
	int size, i;
	size = m_nTileW*m_nTileH;	
	for( i=0; i<m_nTileTotal; i++)
	{	
		WORD* pImg = new WORD [size];
		fread( pImg, 2, size, file);
		m_pTileImg[i].InitImg(pImg, m_nTileW, m_nTileH, b565Mode);
		SAFE_DELETE(pImg);
	}

	// Cliff 이미지 읽기 
	int iSize;
	int* pOff = new int [m_nCliffTotal];
	fread( pOff, 4, m_nCliffTotal, file );
	fread( &iSize, 4, 1, file );

	for( i=0; i<m_nCliffTotal; i++ )
	{
		if( i < m_nCliffTotal-1 )	size = pOff[i+1] - pOff[i];
		else size = iSize - pOff[i];
		WORD* pImg = new WORD [size];
		fread( pImg, 2, size, file);	
		m_pCliffImg[i].InitCliffImg(pImg, m_nTileW, m_nTileH, size);
		SAFE_DELETE(pImg);
	}

	// 오브젝트 이미지 읽기 
	pOff = new int [m_nObjTotal];
	fread( pOff, 4, m_nObjTotal, file );
	
	int* W = new int[m_nObjTotal];
	int* H = new int[m_nObjTotal];

	for( i=0; i<m_nObjTotal; i++){
		fread( &W[i], 4, 1, file );
		fread( &H[i], 4, 1, file );
	}

	fread( &iSize, 4, 1, file );

	for( i=0; i<m_nObjTotal; i++ )
	{
		if( i < m_nObjTotal-1 ) size = pOff[i+1] - pOff[i];
		else size = iSize - pOff[i];

		WORD* pImg = new WORD [size];
		fread( pImg, 2, size, file);	
		m_pObjImg[i].InitCliffImg(pImg, W[i], H[i], size);
		SAFE_DELETE(pImg);
	}
	SAFE_DELETE(W);
	SAFE_DELETE(H);
	m_SightImg.LoadSpr("Data\\map\\Sight.Spr");
//	m_Black.LoadSpr("Data\\Black.Spr");
//	m_Fog.LoadImg("Data\\Fog.Img");

	return true;	
}

void CMapMng::InitMapImg()
{
	m_pTileImg = new CMapImg [m_nTileTotal];
	m_pCliffImg = new CMapImg [m_nCliffTotal];
	m_pObjImg =  new CMapImg [m_nObjTotal];
}

void CMapMng::LoadMap()
{
	m_nStory = (m_nStage-1)/2+1;

	SAFE_DELETE(m_pMap);
	m_pMap = new CMap;
	m_pMap->LoadMap(m_MapFile);
	m_pMap->SetTileSize(m_nTileW, m_nTileH, m_nAttrW, m_nAttrH);
	m_pMap->SetMapImg(m_pTileImg, m_pCliffImg, m_pObjImg);
}

void CMapMng::DrawTileMap(WORD* pScreen)
{
	m_pMap->DrawTileMap(pScreen);
}

void CMapMng::DrawCliffMap(WORD *pScreen, bool Alpha)
{
	m_pMap->DrawCliffMap(pScreen, Alpha);
}

void CMapMng::DrawObjectMap(WORD *pScreen, int nObj, int nX, int nY)
{
	m_pMap->DrawObjectMap(pScreen, nObj, nX, nY);
}
/*
void CMapMng::DrawFog(WORD *pScreen)
{
//	m_Black.DrawFog(0, 0, 0, pScreen, 800);
//	m_Fog.DrawImg(0, 0, 0, pScreen, 800);
}*/


bool CMapMng::LoadStage(int nStage, StageInfo* pStg, point* Teshy, BOOL b565Mode)
{

	FILE* file;
	int Count, i, j;
	m_nStage = nStage;

	file = fopen(m_strStageFile[nStage-1], "rb");
	if(file == NULL) return false;	
	char temp[30];
	fread( &temp, 1, 30, file );
	strcpy(m_MapFile, "Data\\Map\\");
	strcat(m_MapFile, temp);
	fread( &temp, 1, 30, file );

	fread( &Teshy->x, 4, 1, file );
	fread( &Teshy->y, 4, 1, file );

	int x, y;
	fread( &x, 4, 1, file );		// 적 도주지점
	fread( &y, 4, 1, file );		

	fread( &pStg->nSCTeamCnt, 4, 1, file);
	fread( &pStg->nRGTeamCnt, 4, 1, file);
	fread( &pStg->nOurCnt, 4, 1, file);
	fread( &pStg->nMonCnt, 4, 1, file);
	fread( &pStg->nEventCnt, 4, 1, file);

	pStg->nTeamCnt = pStg->nSCTeamCnt+pStg->nRGTeamCnt+pStg->nOurCnt+pStg->nMonCnt;

	int ntemp;
	for( i=0; i<pStg->nSCTeamCnt; i++ )
		fread( &ntemp, 4, 1, file);
	for( i=0; i<pStg->nRGTeamCnt; i++ )
		fread( &ntemp, 4, 1, file);
	
	fread( &pStg->nCharTotal, 4, 1, file);

	int Num=0;
	for( i=0; i<16; i++ )
	{
		fread( &Count, 4, 1, file );
		pStg->aCharCnt[i] = Count;
		for( j=0; j<Count; j++ )
		{
			fread( &pStg->aTeamNum[Num], 4, 1, file);
			fread( &pStg->aPosInTeam[Num], 4, 1, file);
			pStg->abLeader[Num] = false;
			if(pStg->aPosInTeam[Num]==0) pStg->abLeader[Num] = true;
			fread( &pStg->aXPosInMap[Num], 4, 1, file);
			fread( &pStg->aYPosInMap[Num], 4, 1, file);
			fread( &ntemp, 4, 1, file);
			Num++;
		}
	}

	for( i=0; i<pStg->nEventCnt; i++ )
	{
		fread( &pStg->aEventNum[i], 4, 1, file );
		fread( &pStg->artEvent[i].x1, 4, 1, file );
		fread( &pStg->artEvent[i].y1, 4, 1, file );
		fread( &pStg->artEvent[i].x2, 4, 1, file );
		fread( &pStg->artEvent[i].y2, 4, 1, file );
	}

	for( i=0; i<pStg->nTeamCnt; i++ )
	{
		fread( &pStg->aWayCnt[i], 4, 1, file );

		for( j=0; j<pStg->aWayCnt[i]; j++ )
		{
			fread( &pStg->aWayX[i][j], 4, 1, file );
			fread( &pStg->aWayY[i][j], 4, 1, file );
		}
	}
	
	LoadMapImg(b565Mode);
	LoadMap();
	for( i=0; i<pStg->nEventCnt; i++ )
	{
		m_pMap->SetEventArea(pStg->artEvent[i], pStg->aEventNum[i]+1);
	}
	return true;
}

int CMapMng::GetMapAttrP(int Xp, int Yp)
{
	return m_pMap->m_pAttrMap[(Yp/32)*m_pMap->GetAttrMapWidth()+(Xp/32)];
}

int CMapMng::GetMapAttrN(int Xn, int Yn)
{
	return m_pMap->m_pAttrMap[Yn*m_pMap->GetAttrMapWidth()+Xn];
}

void CMapMng::DrawSightMask(WORD *pScreen)
{
	m_SightImg.DrawSpr(0, 0, 0, pScreen);
		//DWORD Scr, Back;
/*	DWORD* Scr, Back;
	int t = 0, i, j;
	for( i=0; i<600; i++ ) {
			for( j=0; j<nWidth; j+=2 )
		{
			Scr = (DWORD*)(pScreen+t+j);
			(*(Scr+t+j)) = ((*(Scr+t+j)) & 0xf7def7de)>>1; //<<16dHALF_MASK)>>1)|(ScrdHALF_MASK>>1); //+ (Back & dHALF_MASK_4BYTE)>>1;
		//	*(DWORD*)(pScreen+t+j) = Back;
		}
		t += nWidth;
	}*/
}
