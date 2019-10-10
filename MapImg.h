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
	
	unsigned short* m_pScreen;			// 이미지가 출력될 스크린 포인터 
	unsigned short* m_pImg;				// 실제 이미지 정보 
	int m_nWidth;				// 이미지 가로 사이즈 
	int m_nHeight;				// 이미지 세로 사이즈 

public:
	void SetPointerScreen(WORD* pScreen) { m_pScreen = pScreen; }	// 스크린 포인터 세팅함수 
	int GetWidth() { return m_nWidth; }								// 이미지 가로 크기 얻는 함수 
	int GetHeight() { return m_nHeight; }							// 이미지 세로 크기 얻는 함수 

	void DrawCliffImg(int x, int y);						// 절벽과 오브젝트 이미지 출력함수 
	void DrawTileImg(int x, int y);							// 타일 이미지  출력함수 
	void InitCliffImg(unsigned short *pImg, int x, int y, int size);	// 절벽과 오브젝트 이미지 초기화 함수 
	void InitImg(unsigned short *pImg, int x, int y, BOOL b565Mode);					// 타일이미지 초기화 함수 

	CMapImg();
	virtual ~CMapImg();

};

#endif // !defined(AFX_MAPIMG_H__C0828103_08FB_11D6_B3AA_0050DA898E67__INCLUDED_)
