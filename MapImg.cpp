// MapImg.cpp: implementation of the CMapImg class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include "function.h"
#include "MapImg.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMapImg::CMapImg()
{
	m_pImg = NULL;
}

CMapImg::~CMapImg()
{
	m_pScreen = NULL;
	if( m_pImg != NULL ){delete m_pImg; m_pImg = NULL;}
}

void CMapImg::InitImg(unsigned short *pImg, int x, int y, BOOL b565Mode)
{
	m_nWidth  = x;
	m_nHeight = y;
	int size = x*y;
	m_pImg = new unsigned short [size];

	if( b565Mode )
	{	
		memcpy(m_pImg, pImg, size<<1);
	}
	else 
	{
		int i, j, n;
		unsigned short r, g, b;
		for( i=0; i<m_nHeight; i++ )
			for( j=0; j<m_nWidth; j++ )
			{
				n = i*m_nHeight+j;

				r = ((pImg[n]&0xf800)>>11);
				g = ((pImg[n]&0x07e0)>>6);							
				b = pImg[n]&0x001f;

				m_pImg[n] = ( (r<<10)| (g<<5)| b );
				//m_pImg[n] = ( (r<<11) | (g<<6) | b ); 테스트 
			}
	}
}

void CMapImg::InitCliffImg(unsigned short *pImg, int x, int y, int size)
{
	m_nWidth  = x;
	m_nHeight = y;

	m_pImg = new unsigned short [size];
	memcpy(m_pImg, pImg, size<<1);
}

void CMapImg::DrawTileImg(int x, int y)
{
	int i, off = 0;
	int cx = m_nWidth, cy = m_nHeight;
	int tx = 0, ty = 0;	
		
	if( x < 0 )	{ tx = -x; x = 0; cx = m_nWidth - tx; }
  	if( x+cx > 800 ) { cx = m_nWidth - (x+cx-800); }
  	if( y < 0 )	{ ty = -y; y = 0; cy = m_nHeight - ty; }
  	if( y+cy > 547 ) { cy = m_nHeight - (y+cy-547); }

	off = ty*m_nWidth+tx;
	int ScrOffset = y * 800;
	
	for(i=0; i<cy; i++)
	{
		memcpy(m_pScreen+ScrOffset+x, m_pImg+off, cx<<1);
		off += m_nWidth;
		ScrOffset += 800;
	}
}

void CMapImg::DrawCliffImg(int x, int y)
{
	int bandnum, imgoffset, imgnum, off=0;

	int ylimit = y+m_nHeight;

	// y의 상단 클리핑 처리.
	if(y<0) {
		do {
			bandnum = (int)m_pImg[off++];
			while(bandnum--) {
				off++;
				imgnum = (int)m_pImg[off++];
				off += imgnum;
			}
			y++;
		} while(y);	
	}
	// y의 하단 클리핑 처리.	
	if (ylimit > 547) ylimit = 547;

	if (ylimit <= y) return;
	//int lpitch = 800;
	int ScrOffset = y * 800;	
	// x의 클리핑 처리 출력 부분
	if( x<0 || x+m_nWidth>800 )
	{
		int beginX=0, endX;
		while(y<ylimit)
		{
			bandnum = (int)m_pImg[off++];
			while(bandnum--)
			{
				imgoffset = x + (int)m_pImg[off++];
				imgnum = (int)m_pImg[off++];

				endX = imgnum;

				beginX = 0;
				if(imgoffset<0) beginX = -imgoffset;
				if(imgoffset+imgnum > 800) endX = 800-imgoffset;
				if(imgnum>beginX && endX>0)
					memcpy(m_pScreen+ScrOffset+imgoffset+beginX,
					       m_pImg+off+beginX,
						   (endX-beginX)<<1);
				off += imgnum;
			}
			y++; ScrOffset+=800;
		}
	}
	else // x의 클리핑이 필요없을 때 처리.
	{
		while(y<ylimit)
		{
			bandnum = (int)m_pImg[off++];
			while(bandnum--)
			{
				imgoffset = (int)m_pImg[off++];
				imgnum = (int)m_pImg[off++];
				memcpy(m_pScreen+ScrOffset+x+imgoffset, m_pImg+off, (imgnum<<1));
				off += imgnum;
			}
			y++; ScrOffset+=800;
		}
	}
}



