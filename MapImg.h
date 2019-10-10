// MapImg.h: interface for the CMapImg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAPIMG_H__C0828103_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
#define AFX_MAPIMG_H__C0828103_08FB_11D6_B3AA_0050DA898E67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <windows.h>


class CMapImg  
{
protected:
	
	unsigned short* m_pScreen;			// �̹����� ��µ� ��ũ�� ������ 
	unsigned short* m_pImg;				// ���� �̹��� ���� 
	int m_nWidth;				// �̹��� ���� ������ 
	int m_nHeight;				// �̹��� ���� ������ 

public:
	void SetPointerScreen(WORD* pScreen) { m_pScreen = pScreen; }	// ��ũ�� ������ �����Լ� 
	int GetWidth() { return m_nWidth; }								// �̹��� ���� ũ�� ��� �Լ� 
	int GetHeight() { return m_nHeight; }							// �̹��� ���� ũ�� ��� �Լ� 

	void DrawCliffImg(int x, int y);						// ������ ������Ʈ �̹��� ����Լ� 
	void DrawTileImg(int x, int y);							// Ÿ�� �̹���  ����Լ� 
	void InitCliffImg(unsigned short *pImg, int x, int y, int size);	// ������ ������Ʈ �̹��� �ʱ�ȭ �Լ� 
	void InitImg(unsigned short *pImg, int x, int y, BOOL b565Mode);					// Ÿ���̹��� �ʱ�ȭ �Լ� 

	CMapImg();
	virtual ~CMapImg();

};

#endif // !defined(AFX_MAPIMG_H__C0828103_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
