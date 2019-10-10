// Map.cpp: implementation of the CMap class.
//
//////////////////////////////////////////////////////////////////////
#include <stdio.h>
#include <math.h>
#include <string.h>
//#include <windows.h>
#include "Map.h"
//#include "NeedStruct.h"
#include "function.h"
#include "MapImg.h"

//#define PI	3.14159265359

#define ABS(a)		(((a) < (0)) ? -(a) : (a))
#define SQR(a)		((a)*(a))
#define DIST(a,b)	(sqrt(((a)*(a)+(b)*(b))))
#define dHALF_MASK      0xf7de    // 565모드시 반투명 마스크
#define dHALF_MASK_4BYTE      0xf7def7de   // 565모드시 4바이트 반투명 마스크
#define RGB(r, g ,b)  ((DWORD) (((BYTE) (r) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (b)) << 16))) 
#define SAFE_DELETE(p)  { if(p) { delete (p); (p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMap::CMap()
{
	m_bEvent = false;

	m_nEnemyCnt = 0;
	m_nConEnemyCnt = 0;
	m_pTileMap  = NULL;
	m_pCliffMap = NULL;
	m_pObjMap   = NULL;
	m_pAttrMap  = NULL;

	m_pTileImg  = NULL;
	m_pObjImg   = NULL;
	m_pCliffImg = NULL;

	m_nXscl = 0;
	m_nYscl = 0;
	m_fXvec = 0.0f;
	m_fYvec = 0.0f;
	m_fXerror = 0.0f;
	m_fYerror = 0.0f;

	m_Col[0] = Convert16(RGB(255, 88, 237));
	m_Col[1] = Convert16(RGB(116, 0, 41));
	m_Col[2] = Convert16(RGB(156, 156, 156));
	m_Col[3] = Convert16(RGB(0, 150, 255));
	m_Col[4] = Convert16(RGB(46, 141, 0));
	m_Col[5] = Convert16(RGB(46, 141, 0));

	for(int i=0; i<UNITCNT; i++)
	{
		m_nEnemyPos[i].x = -1;
		m_nEnemyPos[i].y = -1;
		m_nConEnemyPos[i].x = -1;
		m_nConEnemyPos[i].y = -1;
	}
}

CMap::~CMap()
{
	SAFE_DELETE(m_pTileMap);
	SAFE_DELETE(m_pCliffMap);
	SAFE_DELETE(m_pObjMap);
	SAFE_DELETE(m_pAttrMap);
}

bool CMap::LoadMap(char *pFile)
{
	FILE* file;
	int size, i;

	file = fopen(pFile, "rb");
	if(file == NULL) return false;

	fread( &m_nMapW, 4, 1, file );
	fread( &m_nMapH, 4, 1, file );
	fread( &m_nAttrMapW, 4, 1, file);
	fread( &m_nAttrMapH, 4, 1, file );
	
	if( !InitMap() )  return false; 

	size = m_nMapW*m_nMapH; 
	fread( m_pTileMap, 4, size, file );		// 타일맵배열 읽기 
	fread( m_pCliffMap, 4, size, file );	// 절벽 맵배열 읽기 
	
	for(  i=0; i<m_nAttrMapH; i++ )			// 오브젝트 맵의 오브젝트 번호 읽기 
		for( int j=0; j<m_nAttrMapW; j++ )
		{
			fread( &m_pObjMap[i*m_nAttrMapW+j].nObj, 4, 1, file );
			
		}

	for(  i=0; i<m_nAttrMapH; i++ )			// 오브젝트 맵의 오브젝트 위치 읽기 
		for( int j=0; j<m_nAttrMapW; j++ )
		{
			fread( &m_pObjMap[i*m_nAttrMapW+j].nX, 4, 1, file );
			fread( &m_pObjMap[i*m_nAttrMapW+j].nY, 4, 1, file );
		}
	size = m_nAttrMapW*m_nAttrMapH; 
	fread( m_pAttrMap, 4, size, file );		// 속성맵 배열 읽기 

	return true;
}

bool CMap::InitMap()
{
	int size = m_nMapW*m_nMapH;
	m_pTileMap = new int [size];
	m_pCliffMap = new int [size];

	size = m_nAttrMapW*m_nAttrMapH;
	m_pObjMap = new Object [size];
	m_pAttrMap = new int /*UINT*/ [size];

	if( m_pTileMap == NULL ) return false;
	if( m_pCliffMap == NULL ) return false;
	if( m_pObjMap == NULL ) return false;
	if( m_pAttrMap == NULL ) return false;
	return true;
}

void CMap::DrawTileMap(unsigned short *pScreen)
{
	int x, y, iTile;
	m_nDrawXs = m_nXscl / m_nTileW;
	m_nDrawYs = m_nYscl / m_nTileH;
	int nXscr = m_nXscl % m_nTileW;
	int nYscr = m_nYscl % m_nTileH;
	
	for( int i=0; i<10; i++)
		for( int j=0; j<14; j++)
		{
			if( !(m_pAttrMap[((m_nDrawYs+i)*2*m_nAttrMapW)+((m_nDrawXs+j)*2)] &0x00000008) )
			{
				x = j*m_nTileW-nXscr;
				y = i*m_nTileH-nYscr;
				iTile = m_pTileMap[((m_nDrawYs+i)*m_nMapW)+(m_nDrawXs+j)];
				
				if( x+m_nTileW > 0 && x < 800 && y+m_nTileH > 0 && y < 547 )
				{	
					m_pTileImg[iTile].SetPointerScreen(pScreen);
					m_pTileImg[iTile].DrawTileImg(x, y);				
				}
			}
		}
	
}

void CMap::SetTileSize(int x, int y, int ax, int ay)
{
	m_nTileW = x; 
	m_nTileH = y; 
	m_nAttrW = ax;
	m_nAttrH = ay;
	m_nWidth = m_nMapW*m_nTileW;
	m_nHeight = m_nMapH*m_nTileH;
}

void CMap::SetScroll(point* Te, point* Cur, int nDir, int nSpeed)
{
	int nXdist, nYdist;	  // 캐릭터에서 마우스까지의 X, Y거리
	float fdist;		  // 캐릭터에서 마우스까지의 직선거리 

	m_nXscl = Te->x - 400;  m_nYscl = Te->y - 300;
	// 캐릭터에서 마우스까지의 XY거리 구하기 
	nXdist = Cur->x - 400;	
	nYdist = Cur->y - 300;

	// 캐릭터에서 마우스까지의 직선거리 구하기 
	fdist = (float)DIST( ABS(nXdist), ABS(nYdist) );

	// 단위 벡터 구하기 
	if(fdist)
	{
		m_fXvec = nXdist / fdist;  // cos
		m_fYvec = nYdist / fdist;  // sin
	}
	else
	{
		m_fXvec = 0;
		m_fYvec = 0;
	}
	
	float nX = nSpeed*1.0f, nY = nSpeed*1.0f;
	
	if( nDir%4 == 0 ) {	nX *= 1.2f;	nY *= 1.2f; }      // 좌우 방향
	else if( nDir%4 == 2 ) { nX *= 1.0f; nY *= 1.0f; } // 상하 방향
	
	m_nXmove = (int)(m_fXvec*nX);
	m_nYmove = (int)(m_fYvec*nY);

	m_fXerror += (m_fXvec*nX) - m_nXmove;
	m_fYerror += (m_fYvec*nY) - m_nYmove;

	// 오차 줄이기 
	if( m_fXerror/nX > 1 ) {	m_nXmove += 1;	m_fXerror = 0; }
	if( m_fYerror/nY > 1 ) {	m_nYmove += 1;	m_fYerror = 0; }
	if( m_fXerror/nX < -1) {	m_nXmove -= 1;	m_fXerror = 0; }
	if( m_fYerror/nY < -1) {	m_nYmove -= 1;	m_fYerror = 0; }

	int X = m_nXscl+m_nXmove;
	int Y = m_nYscl+m_nYmove;

	if( IsPassableP( (X+400), (m_nYscl+300)) )
//	if( !(m_pAttrMap[((X+400)/m_nAttrW) + ((m_nYscl+300)/m_nAttrH)* m_nAttrMapW] & 0x00000001) )
		if( X >= 0 && X+800 <= m_nWidth )
			m_nXscl = X;
	if( IsPassableP( (m_nXscl+400), (Y+300)) )
//	if( !(m_pAttrMap[((m_nXscl+400)/m_nAttrW) + ((Y+300)/m_nAttrH)* m_nAttrMapW] & 0x00000001) )
		if( Y >= 0 && Y+547 <= m_nHeight )
			m_nYscl = Y;

	Te->x = m_nXscl+400;
	Te->y = m_nYscl+300;
	
	m_bEvent = m_pAttrMap[(Te->x/m_nAttrW) + ((Te->y)/m_nAttrH)* m_nAttrMapW] >> 16;

}

bool CMap::IsPassable(int x, int y)
{
	if( !(m_pAttrMap[x + y*m_nAttrMapW] & 0x00000001) )
		return true;
	return false;
}

bool CMap::IsPassableP(int x, int y)
{
	if( !(m_pAttrMap[(x/m_nAttrW) + (y/m_nAttrH)*m_nAttrMapW] & 0x00000001) )
		return true;
	return false;
}

void CMap::DrawCliffMap(unsigned short *pScreen, bool Alpha)
{
	int nDrawXs = m_nXscl/m_nTileW;
	int nDrawYs = m_nYscl/m_nTileH;
	
	int nXscr = m_nXscl % m_nTileW;
	int nYscr = m_nYscl % m_nTileH;

	int x, y, ax, ay;
	
	if( Alpha )
	{
		for( int i=0; i<10; i++)
			for( int j=0; j<14; j++)
			{
				x = j*m_nTileW-nXscr;
				y = i*m_nTileH-nYscr;
			
				ax = (nDrawXs+j)*2;
				ay = (nDrawYs+i)*2;

				
				if( m_pAttrMap[ay*m_nAttrMapW+ax] & 0x00000004 )
				{
					int iTile = m_pCliffMap[((nDrawYs+i)*m_nMapW)+(nDrawXs+j)];
					
					if( iTile > -1 )
  					{
						if( x+m_nTileW > 0 && x <  800 && y+m_nTileH > 0 && y < 547 )
						{
							m_pCliffImg[iTile].SetPointerScreen(pScreen);
							m_pCliffImg[iTile].DrawCliffImg(x, y);
						}
					}
				}
			}
	} 
	else 
	{
		for( int i=0; i<10; i++)
			for( int j=0; j<14; j++)
			{
				x = j*m_nTileW-nXscr;
				y = i*m_nTileH-nYscr;
				ax = (nDrawXs+j)*2;
				ay = (nDrawYs+i)*2;

				if( !(m_pAttrMap[ay*m_nAttrMapW+ax] & 0x00000004) )
				{
					int iTile = m_pCliffMap[((nDrawYs+i)*m_nMapW)+(nDrawXs+j)];
					
					if( iTile > -1 )
  					{
						if( x+m_nTileW > 0 && x <  800 && y+m_nTileH > 0 && y < 547 )
						{
							m_pCliffImg[iTile].SetPointerScreen(pScreen);
							m_pCliffImg[iTile].DrawCliffImg(x, y);
						}
					}
				}
			}
	}

}

void CMap::DrawObjectMap(unsigned short *pScreen, int nObj, int nX, int nY)
{
	m_pObjImg[nObj].SetPointerScreen(pScreen);
	m_pObjImg[nObj].DrawCliffImg(nX, nY);
}
void CMap::DrawObjectMap(unsigned short *pScreen)
{
	int i, j, x, y, cx, cy, nObj;
	m_nDrawXs = m_nXscl/m_nAttrW-10;
	m_nDrawYs = m_nYscl/m_nAttrH-10;

	for( i=0; i<37; i++)
		for(  j=0; j<45; j++)
		{
			if( m_nDrawYs+i >=0 && m_nDrawYs+i < m_nAttrMapH && m_nDrawXs+j >=0 && m_nDrawXs+j < m_nAttrMapW )
			{
				nObj = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nObj;
				if( nObj > -1 && nObj >= 30 && nObj < 33 )
				{
					x = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nX-m_nXscl;
					y = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nY-m_nYscl;
					cx = m_pObjImg[nObj].GetWidth();
					cy = m_pObjImg[nObj].GetHeight();

					if( x-(cx/2) < 800 && (y-cy) < 547 && x+(cx/2) >0 &&  y > 0 )
					{
						m_pObjImg[nObj].SetPointerScreen(pScreen);
						m_pObjImg[nObj].DrawCliffImg(x-cx/2, y-cy);
					}					
				}
			}
		}
}


void CMap::DrawMinimap(unsigned short *pScreen, int tX, int tY)
{
	int X = (int)(tX/m_nAttrW)-28;
	int Y = (int)(tY/m_nAttrH)-28;
	int i, j,scr[4], k;
	int  t ;
	DWORD Scr, Back;

	t = 0;
	for( i=0; i<112; i++ ) {
			for( j=800-112; j<800; j++ )
		{
			Scr = *(DWORD*)(pScreen+t+j);
			Back = (Scr & 0xf7def7de)>>1; //<<16dHALF_MASK)>>1)|(ScrdHALF_MASK>>1); //+ (Back & dHALF_MASK_4BYTE)>>1;
			*(DWORD*)(pScreen+t+j) = Back;
		}
		t += 800;
	}

	for( i=0; i<56; i++ )
		for( j=0; j<56; j++ )
		{
			scr[0] = (i*2)*800+(j*2)+800-112;			
			scr[1] = scr[0]+1;			
			scr[2] = (i*2+1)*800+(j*2)+800-112;			
			scr[3] = scr[2]+1;

		//	for( k=0; k<4; k++ )
		//		pScreen[scr[k]] = 0x0000;//(pScreen[scr[k]]&dHALF_MASK)>>1;*/
		
			if( (Y+i) >= 0 && (Y+i) < m_nAttrMapH && (X+j) >= 0 && (X+j) < m_nAttrMapW )
			{
				int iCol = (m_pAttrMap[(Y+i)*m_nAttrMapW+(X+j)] & 0x0000ff00 )>>8;
				if( iCol>0 )
				{
					pScreen[scr[0]] = m_Col[iCol];
				}
				if( (m_pAttrMap[(Y+i)*m_nAttrMapW+(X+j)] & 0x00000010))	
					for( k=0; k<4; k++ )
						pScreen[scr[k]] = m_Col[5];

				if( (X+j) == (tX/m_nAttrW) && (Y+i) == (tY/m_nAttrH) )
					for(  k=0; k<4; k++ )
						pScreen[scr[k]] = m_Col[0];
			}
		}

}

// 화면상에 보이는 오브젝트 수 리턴
int CMap::SortObjectCount()
{
	int Count = 0, x, y, cx, cy;
	m_nDrawXs = m_nXscl/m_nAttrW-10;
	m_nDrawYs = m_nYscl/m_nAttrH-10;

	for( int i=0; i<37; i++)
		for( int j=0; j<45; j++)
		{
			if( m_nDrawYs+i >=0 && m_nDrawYs+i < m_nAttrMapH && m_nDrawXs+j >=0 && m_nDrawXs+j < m_nAttrMapW )
			{
				int iObj = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nObj;
				if( iObj > -1 )
					if( iObj < 30 || iObj > 32 )
					{
						x = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nX-m_nXscl;
						y = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nY-m_nYscl;
						cx = m_pObjImg[iObj].GetWidth();
						cy = m_pObjImg[iObj].GetHeight();
						if( x-(cx>>1) < 800 && (y-cy) < 548 && x+(cx>>1) > 0 &&  y>0 )
							Count++;
					}
			}
		}
	return Count;
}

// 화면상에 보이는 오브젝트 수 리턴
int CMap::SetObjectSortList(SORTY *pSort, int i)
{
	int Count = i, j, x, y, cx, cy, iObj;
	m_nDrawXs = m_nXscl/m_nAttrW-10;
	m_nDrawYs = m_nYscl/m_nAttrH-10;

	for( i=0; i<37; i++)
		for(  j=0; j<45; j++)
		{
			if( m_nDrawYs+i >=0 && m_nDrawYs+i < m_nAttrMapH && m_nDrawXs+j >=0 && m_nDrawXs+j < m_nAttrMapW )
			{
				iObj = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nObj;
				if( iObj > -1 )
					if( iObj < 30 || iObj > 32 )
					{
						x = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nX-m_nXscl;
						y = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nY-m_nYscl;
						cx = m_pObjImg[iObj].GetWidth();
						cy = m_pObjImg[iObj].GetHeight();
						if( x-(cx/2) < 800 && (y-cy) < 547 && x+(cx/2) >0 &&  y > 0 )
						{
							pSort[++Count].m_nChar = -1;
							pSort[Count].m_nNum = iObj;
							pSort[Count].m_nY = m_pObjMap[((m_nDrawYs+i)*m_nAttrMapW)+(m_nDrawXs+j)].nY;
							pSort[Count].m_nScrX = x-(cx/2);
							pSort[Count].m_nScrY = y-cy ;
						}		
					}
				
			}
		}
	return Count+1;
}

void CMap::SetMapImg(CMapImg *pTileImg, CMapImg *pCliffImg, CMapImg *pObjImg)
{
	m_pTileImg = pTileImg;
	m_pCliffImg = pCliffImg;
	m_pObjImg = pObjImg;
}

bool CMap::SetConEnemyPos(SORTY *pSort, int nCount)
{
	m_nEnemyCnt = nCount+1;

	if( m_pAttrMap[m_nEnemyPos[0].y*m_nAttrMapW+m_nEnemyPos[0].x] & 0x00000020 )
		m_pAttrMap[m_nEnemyPos[0].y*m_nAttrMapW+m_nEnemyPos[0].x] ^= 0x00000020;
	m_nEnemyPos[0].x = pSort[0].m_nX/m_nAttrW;
	m_nEnemyPos[0].y = pSort[0].m_nY/m_nAttrH;
	m_pAttrMap[m_nEnemyPos[0].y*m_nAttrMapW+m_nEnemyPos[0].x] |= 0x00000020;

	for( int i=1; i<m_nEnemyCnt; i++ )
	{
		if( m_pAttrMap[m_nEnemyPos[i].y*m_nAttrMapW+m_nEnemyPos[i].x] & 0x00000010 )
			m_pAttrMap[m_nEnemyPos[i].y*m_nAttrMapW+m_nEnemyPos[i].x] ^= 0x00000010;
		m_nEnemyPos[i].x = pSort[i].m_nX/m_nAttrW;
		m_nEnemyPos[i].y = pSort[i].m_nY/m_nAttrH;
		m_pAttrMap[m_nEnemyPos[i].y*m_nAttrMapW+m_nEnemyPos[i].x] |= 0x00000010;
	}

	int n = 0;
	for( i=1; i<m_nEnemyCnt; i++ )
	{
		if( m_nEnemyPos[i].x >= m_nMDrawXs && m_nEnemyPos[i].y >= m_nMDrawYs && 
			m_nEnemyPos[i].x < m_nMDrawXs + 56 && m_nEnemyPos[i].y < m_nMDrawYs+56 )
		{
			m_nConEnemyPos[n].x = m_nEnemyPos[i].x;
			m_nConEnemyPos[n++].y = m_nEnemyPos[i].y;
		}
	}
	m_nConEnemyCnt = n;
	return false;
/**/
}
/*
bool CMap::SetEnemyPos()
{
/*	int nCount = 0;
	for( int i=1; i<m_nEnemyCnt; i++ )
	{
		if( m_nEnemyPos[i].x >= m_nMDrawXs && m_nEnemyPos[i].y >= m_nMDrawYs && 
			m_nEnemyPos[i].x < m_nMDrawXs + 56 && m_nEnemyPos[i].y < m_nMDrawYs+56 )
		{
			m_nConEnemyPos[nCount].x = m_nEnemyPos[i].x;
			m_nConEnemyPos[nCount++].y = m_nEnemyPos[i].y;
		}
	}
	m_nConEnemyCnt = nCount;
	return false;
	return false;
}
*/
void CMap::SetEventArea(rect rtEvent, int nEvent)
{
	int i, j;

	for( i=rtEvent.y1*2; i<rtEvent.y2*2; i++ )
		for( j=rtEvent.x1*2; j<rtEvent.x2*2; j++ )
		{
			m_pAttrMap[i*m_nAttrMapW+j] |= nEvent<<16;
		}
}

