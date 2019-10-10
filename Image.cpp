// Image.cpp: implementation of the CImage class.
//
//////////////////////////////////////////////////////////////////////
#include <string.h>
#include <stdio.h>
#include "NeedStruct.h"
#include "function.h"
#include "Image.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define dHALF_MASK 0xf7de    // 565���� ������ ����ũ
#define RGB(r,g,b) ((unsigned long)(((unsigned char)(r)|((unsigned long)((unsigned char)(g))<<8))|(((unsigned long)(unsigned char)(b))<<16)))

#define SAFE_DELETE(p) { if(p) { delete (p);  (p)=NULL; } }

CImage::CImage()
{
	m_pData      = NULL;
	m_pFrmWidth  = NULL;
	m_pFrmHeight = NULL;
	m_pFrmOffset = NULL;
}

CImage::~CImage()
{
	Delete();
}

//-------------------------------------------------------------------
//  ���� �ε� �Լ�
//  return true(����), false(�ε� ����)
//-------------------------------------------------------------------
bool CImage::LoadImg(char *fname)
{
	FILE* file;

	file = fopen(fname, "rb");
	if(file == NULL) return false;

	m_CKey = Convert16(RGB(255, 0, 255));
	m_pFrmWidth  = new int[1];
	m_pFrmHeight = new int[1];
	m_pFrmOffset = new int[1];

	fread(m_pFrmWidth,  4, 1, file);
	fread(m_pFrmHeight, 4, 1, file);
	m_pFrmOffset[0] = 0;

	m_nImgSize = m_pFrmWidth[0] * m_pFrmHeight[0];

	m_pData = new WORD[m_nImgSize];

	fread(m_pData, 2, m_nImgSize, file);

	fclose(file);
	return true;
}

//-------------------------------------------------------------------
//  0�� ���� ���� �ε� �Լ�
//  return true(����), false(�ε� ����)
//-------------------------------------------------------------------
bool CImage::LoadSpr(char *fname)
{
	FILE* file;
	unsigned long tempCol;
	rect* rtTemp;

	file = fopen(fname, "rb");
	if(file == NULL) return false;

	fread( &tempCol, 4, 1, file );
	m_CKey = Convert16(tempCol);
	char temp[14];
	fread( temp, 14, 1, file);
	fread(&m_nTotalFrame, 4, 1, file);
	rtTemp       = new rect[m_nTotalFrame];
	if(rtTemp==NULL) return false;
	m_pFrmWidth  = new int[m_nTotalFrame];
	if(m_pFrmWidth==NULL) return false;
	m_pFrmHeight = new int[m_nTotalFrame];
	if(m_pFrmHeight==NULL) return false;
	m_pFrmOffset = new int[m_nTotalFrame];
	if(m_pFrmOffset==NULL) return false;

	fread(rtTemp, sizeof(rect), m_nTotalFrame, file); // �� ������ �Ŷ� ����.
	delete rtTemp; rtTemp = NULL;

	fread(m_pFrmWidth,  4, m_nTotalFrame, file);
	fread(m_pFrmHeight, 4, m_nTotalFrame, file);
	fread(m_pFrmOffset, 4, m_nTotalFrame, file);

	fread(&m_nImgSize, 4, 1, file);
	m_pData = new WORD[m_nImgSize];
	if(m_pData==NULL) return false;
	fread(m_pData, 2, m_nImgSize, file);

	if(fclose(file)!=NULL) return false;
	return true;
}

void CImage::DrawImg(int x, int y, int frame, unsigned short *Screen)
{
	int off=m_pFrmOffset[frame]>>1;

	for(int j=y; j<y+m_pFrmHeight[frame]; j++) {
		for(int i=x; i<x+m_pFrmWidth[frame]; i++) {
			if(i>=0 && i<800 && j>=0 && j<600) {
				if(m_pData[off] != m_CKey)
				{
					//Screen[j*lpitch+i] = ((Screen[j*lpitch+i]&dHALF_MASK)>>1) +
					//	                 ((m_pData[off]&dHALF_MASK)>>1);
					Screen[j*800/*lpitch*/+i] = m_pData[off];
				}
			}
			off++;
		}
	}
}

void CImage::DrawSpr(int x, int y, int Frame, unsigned short *Screen)
{
	int bandnum, imgoffset, imgnum, off=m_pFrmOffset[Frame];
	int x1=0 , y1=0;
	int ylimit = y+m_pFrmHeight[Frame];

	if (ylimit > 600) ylimit = 600;
	if (x>0) x1=x;
	if (y>0) y1=y;
	if ((x+m_pFrmWidth[Frame])<=x1 || ylimit <= y1) return;
	
	// y�� ��� Ŭ���� ó��.
	if(y<0) {
		do {
			bandnum = (int)m_pData[off++];
			while(bandnum--) {
				off++;
				imgnum = (int)m_pData[off++];
				off += imgnum;
			}
			y++;
		} while(y);	
	}

	// y�� �ϴ� Ŭ���� ó��.	
	int ScrOffset = y * 800; //lpitch;	
	// x�� Ŭ���� ó�� ��� �κ�
	if( x<0 || x+m_pFrmWidth[Frame]>800 )
	{
		int beginX=0, endX;
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = x + (int)m_pData[off++];
				imgnum = (int)m_pData[off++];

				endX = imgnum;

				beginX = 0;
				if(imgoffset<0) beginX = -imgoffset;
				if(imgoffset+imgnum > 800) endX = 800-imgoffset;
				if(imgnum>beginX && endX>0)
					memcpy(Screen+ScrOffset+imgoffset+beginX,
					       m_pData+off+beginX,
						   (endX-beginX)<<1);
				off += imgnum;
			}
			y++; ScrOffset+=800; //lpitch;
		}
	}
	else // x�� Ŭ������ �ʿ���� �� ó��.
	{
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = (int)m_pData[off++];
				imgnum = (int)m_pData[off++];
				memcpy(Screen+ScrOffset+x+imgoffset, m_pData+off, (imgnum<<1));
				off += imgnum;
			}
			y++; ScrOffset+=800; //lpitch;
		}
	}
}
// �ƿ����α��� �׷��ִ� ��.
void CImage::DrawSprOL(int x, int y, int Frame, unsigned short *Screen)
{
	int bandnum, imgoffset, imgnum, off=m_pFrmOffset[Frame];
	int x1=0 , y1=0;
	int ylimit = y+m_pFrmHeight[Frame];

	if (ylimit > 600) ylimit = 600;
	if (x>0) x1=x;
	if (y>0) y1=y;
	if ((x+m_pFrmWidth[Frame])<=x1 || ylimit <= y1) return;
	
	// y�� ��� Ŭ���� ó��.
	if(y<0) {
		do {
			bandnum = (int)m_pData[off++];
			while(bandnum--) {
				off++;
				imgnum = (int)m_pData[off++];
				off += imgnum;
			}
			y++;
		} while(y);	
	}

	// y�� �ϴ� Ŭ���� ó��.	
	int ScrOffset = y * 800; //lpitch;	
	// x�� Ŭ���� ó�� ��� �κ�
	if( x<0 || x+m_pFrmWidth[Frame]>800 )
	{
		int beginX=0, endX;
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = x + (int)m_pData[off++];
				imgnum = (int)m_pData[off++];

				endX = imgnum;

				beginX = 0;
				if(imgoffset<0) beginX = -imgoffset;
				if(imgoffset+imgnum > 800) endX = 800-imgoffset;
				if(imgnum>beginX && endX>0)
					memcpy(Screen+ScrOffset+imgoffset+beginX,
					       m_pData+off+beginX,
						   (endX-beginX)<<1);
				off += imgnum;
			}
			y++; ScrOffset+=800; //lpitch;
		}
	}
	else // x�� Ŭ������ �ʿ���� �� ó��.
	{
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = (int)m_pData[off++];
				imgnum = (int)m_pData[off++];
				memcpy(Screen+ScrOffset+x+imgoffset, m_pData+off, (imgnum<<1));
				off += imgnum;
				*(Screen+ScrOffset+x+imgoffset -1)       = 0xffff;
				*(Screen+ScrOffset+x+imgoffset + imgnum) = 0xffff;
				//*(Screen+ScrOffset+x+imgoffset -1) = (*(m_pData+off)&dHALF_MASK)>>1;
				//*(Screen+ScrOffset+x+imgoffset + imgnum) = (*(m_pData+off+imgnum)&dHALF_MASK)>>1;
			}
			y++; ScrOffset+=800; //lpitch;
		}
	}
}

void CImage::Delete()
{
	SAFE_DELETE(m_pData)
	SAFE_DELETE(m_pFrmWidth)
	SAFE_DELETE(m_pFrmHeight)
	SAFE_DELETE(m_pFrmOffset)
}

BOOL CImage::FadeSpr(int x, int y, int Frame, unsigned short *Screen, int nFade)
{
	int rate = 100;
	rect rtClip;
	rtClip.x1 = nFade*(400/rate);
	rtClip.x2 = 800 - rtClip.x1;
	rtClip.y1 = nFade*((300)/rate);
	rtClip.y2 = 600 - rtClip.y1;
	
	if( nFade == rate ) return false;

	int bandnum, imgoffset, imgnum, off=m_pFrmOffset[Frame];
	int x1=0 , y1=0;
	int ylimit = y+m_pFrmHeight[Frame];

	if (ylimit > rtClip.y2) ylimit = rtClip.y2;
	if (x>0) x1=x;
	if (y>0) y1=y;
//	if ((x+m_pFrmWidth[Frame])<=x1 || ylimit <= y1) return false;
	
	// y�� ��� Ŭ���� ó��.
	if(y<rtClip.y1) {
		do {
			bandnum = (int)m_pData[off++];
			while(bandnum--) {
				off++;
				imgnum = (int)m_pData[off++];
				off += imgnum;
			}
			y++;
		} while(y<rtClip.y1);	
	}

	// y�� �ϴ� Ŭ���� ó��.	
	int ScrOffset = y * 800;	
	// x�� Ŭ���� ó�� ��� �κ�
	if( x<rtClip.x1 || x+m_pFrmWidth[Frame]>rtClip.x2 )
	{
		int beginX=rtClip.x1, endX;
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = x + (int)m_pData[off++];
				imgnum = (int)m_pData[off++];

				endX = imgnum;

				beginX = rtClip.x1;
				if(imgoffset<rtClip.x1) beginX = -imgoffset + rtClip.x1;
				if(imgoffset+imgnum > rtClip.x2) endX = rtClip.x2-imgoffset;
				if(imgnum>beginX && endX>rtClip.x1)
					memcpy(Screen+ScrOffset+imgoffset+beginX,
					       m_pData+off+beginX,
						   (endX-beginX)<<1);
				off += imgnum;
			}
			y++; ScrOffset+=800;
		}
	}
	
	return true;
}

//**************************** �߰�(����) *************************************
void CImage::DrawPiece(int x, int y, int ClipTop, int ClipBottom, int Frame, unsigned short *Screen, int lpitch)
{
	int bandnum, imgoffset, imgnum, off=m_pFrmOffset[Frame];
	int x1=0 , y1=0;
	int ylimit = y+m_pFrmHeight[Frame];

	if (ylimit > ClipBottom) ylimit = ClipBottom;
	if (x>0) x1=x;
	if (y>ClipTop) y1=ClipTop;
	if ((x+m_pFrmWidth[Frame])<=x1 || ylimit <= y1) return;
	
	// y�� ��� Ŭ���� ó��.
	if(y<ClipTop) {
		//y -= ClipTop;
		do {
			bandnum = (int)m_pData[off++];
			while(bandnum--) {
				off++;
				imgnum = (int)m_pData[off++];
				off += imgnum;
			}
			y++;
		} while(y!=ClipTop);	
	}

	// y�� �ϴ� Ŭ���� ó��.	
	// ���� 600�̳� ĳ���� ����â �κ��� ©�� �ش�.
	int ScrOffset = y * lpitch;	
	// x�� Ŭ���� ó�� ��� �κ�
	if( x<0 || x+m_pFrmWidth[Frame]>800 )
	{
		int beginX=0, endX;
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = x + (int)m_pData[off++];
				imgnum = (int)m_pData[off++];

				endX = imgnum;

				beginX = 0;
				if(imgoffset<0) beginX = -imgoffset;
				if(imgoffset+imgnum > 800) endX = 800-imgoffset;
				if(imgnum>beginX && endX>0)
					memcpy(Screen+ScrOffset+imgoffset+beginX,
					       m_pData+off+beginX,
						   (endX-beginX)<<1);
				off += imgnum;
			}
			y++; ScrOffset+=lpitch;
		}
	}
	else // x�� Ŭ������ �ʿ���� �� ó��.
	{
		while(y<ylimit)
		{
			bandnum = (int)m_pData[off++];
			while(bandnum--)
			{
				imgoffset = (int)m_pData[off++];
				imgnum = (int)m_pData[off++];
				memcpy(Screen+ScrOffset+x+imgoffset, m_pData+off, (imgnum<<1));
				off += imgnum;
			}
			y++; ScrOffset+=lpitch;
		}
	}
}
