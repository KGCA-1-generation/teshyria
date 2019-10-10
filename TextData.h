// TextData.h: interface for the CTextData class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTDATA_H__5EB51C21_F9FC_11D5_B3AA_0050DA898EA1__INCLUDED_)
#define AFX_TEXTDATA_H__5EB51C21_F9FC_11D5_B3AA_0050DA898EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct {
	int type;
	int index;
	int length;
	int total_index;
	int total_length;
}TEXTDATA;

class CTextData  
{
protected:

public:
	BOOL m_loopstop;
	int  m_nAttr;
	int  m_nTotalIndex;


public:
	int GetAttr()		{ return m_nAttr; }
	int GetTotalIndex() { return m_nTotalIndex; }
	void VScrollText(HDC hdc, int x, int y, int end_y, int offset, char *szText, 
		COLORREF color, int font_size, LPCSTR font_type, BOOL hangul);
	void TypeWritting(HDC hdc, int x, int y, char* szText, 
		COLORREF color, int font_size, LPCSTR font_type, BOOL hangul);
	void GDI_TextOut(HDC hdc, int x, int y, char* szText, int length,
		COLORREF color, int font_size, LPCSTR font_type, BOOL hangul);
	void LineSet(HDC hdc, int x, int y, char *szText, 
		COLORREF color, int font_size, LPCSTR font_type, BOOL hangul);
	void ReadTextData(char* filename, char* szData, int index);
	CTextData();
	virtual ~CTextData();

};

#endif // !defined(AFX_TEXTDATA_H__5EB51C21_F9FC_11D5_B3AA_0050DA898EA1__INCLUDED_)
