// Image.h: interface for the CImage class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGE_H__29EBADC8_CE04_11D5_9447_0050DA898F90__INCLUDED_)
#define AFX_IMAGE_H__29EBADC8_CE04_11D5_9447_0050DA898F90__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define BOOL int
#define WORD unsigned short
#define FC _fastcall

class CImage  
{
protected:
public:
	WORD* m_pData;
	WORD m_CKey;
	int  m_nImgSize;	
	int* m_pFrmOffset;
	int* m_pFrmHeight;
	int* m_pFrmWidth;
	int  m_nTotalFrame;

public:
	BOOL FadeSpr(int x, int y, int Frame, unsigned short *Screen, int nFade);
	// ÇÔ¼ö
	void DrawImg(int x, int y, int Frame, WORD *Screen);
	void DrawSpr(int x, int y, int Frame, WORD *Screen);
	void DrawSprOL(int x, int y, int Frame, WORD *Screen );
	bool LoadImg(char *fname);
	bool LoadSpr(char *fname);
	int	 GetTotalFrame() { return m_nTotalFrame; }
	int	 GetWidth(int Frame)	{ return m_pFrmWidth[Frame]; }
	int	 GetHeight(int Frame)	{ return m_pFrmHeight[Frame]; }
	void DrawPiece(int x, int y, int ClipTop, int ClipBottom, int Frame, WORD *Screen, int lpitch);
	WORD* GetData()		{ return m_pData; }
	void Delete();
	
	CImage();
	~CImage();
};

#undef FC
#endif // !defined(AFX_IMAGE_H__29EBADC8_CE04_11D5_9447_0050DA898F90__INCLUDED_)
