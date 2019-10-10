// TextData.cpp: implementation of the CTextData class.
//
//////////////////////////////////////////////////////////////////////

#include <windows.h>
#include <stdio.h>
#include "TextData.h"

#define MAX_HOR 80
#define MAX_VER 50
#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTextData::CTextData()
{
	m_loopstop = FALSE;
	m_nAttr    = 0;
}

CTextData::~CTextData()
{

}

void CTextData::ReadTextData(char* filename, char* szData, int index)
{
	FILE* fp;

	DWORD  dwTot;
	DWORD* dwLen;
	DWORD* dwAttr;
	BYTE buf[2400] = {0, }; //sourc
	int dsize = sizeof(DWORD);

	fp = fopen(filename, "rb");
	
	//Step1. 총 Index를 얻는다.
	fread(&dwTot,dsize,1,fp);

	//Step2. 각 Index별 문자열 길이를 얻는다.
	dwLen = new DWORD [dwTot];						//배열 마련	
	for(int i=0; i<(int)dwTot; i++) dwLen[i] = 0;	//배열 초기화
	
	for(int j=0; j<(int)dwTot; j++)
	{
		fseek(fp,dsize+j*dsize,SEEK_SET);
		fread(&dwLen[j],dsize,1,fp);
	}
	m_nTotalIndex = dwTot;

	//Step3. 각 Index별 내장된 속성값을 얻는다.
	dwAttr = new DWORD [dwTot];
	for(int h=0; h<(int)dwTot; h++) dwAttr[h] = 0;
	
	for(int g=0; g<(int)dwTot; g++)
	{
		fseek(fp,dsize+(int)dwTot*dsize+g*dsize,SEEK_SET);
		fread(&dwAttr[g],dsize,1,fp);
	}

	//Step5. 해당 Index의 내장된 속성값을 얻는다. 'attr' - Call By Reference
	int offset1 = dsize + (dwTot * dsize) * 2;   

	for(int k=0; k<index; k++)
	{
		offset1 += dsize;
	}	
	fseek(fp,offset1,SEEK_SET);	
	fread(&m_nAttr, dwAttr[index], 1, fp);
	m_nAttr = dwAttr[index];

	//Step4. 해당 Index의 실제 문자열 데이터를 읽어온다.
	//문자열을 담을 변수는 외부변수로 배열의 크기가 정해져 있어야하며, 
	//함수의 인자(char* szData)로서 참조되어야 한다.
	int offset2 = dsize + (dwTot * dsize) * 2;   

	for(int l=0; l<index; l++)
	{
		offset2 += dwLen[l];
	}	
	fseek(fp,offset2,SEEK_SET);	
	fread(buf, dwLen[index], 1, fp);
	memcpy(szData,buf,dwLen[index]);

	//Step5. 청소
	fclose(fp);
	//delete dwLen; dwLen = NULL;
	//delete dwAttr;
	SAFE_DELETE(dwLen)
	SAFE_DELETE(dwAttr)

}

void CTextData::LineSet(HDC hdc, int x, int y, char *szText, 
						COLORREF color, int font_size, LPCSTR font_type, BOOL hangul)
{
	int TotLen = strlen(szText);	
	char LineBuf[MAX_VER][MAX_HOR];
	int  Width[MAX_VER];
	int sx=0, nx=0, ny=0;

	for(int i=0; i<TotLen; i++)
	{	
		char* ptr;
		ptr = (char*)memccpy(LineBuf[ny]+nx, szText+sx, 0x0d, 1);
		
		if(ptr)
		{
			Width[ny] = nx;
			ny++;
			nx=0;
			sx += 2;
		}
		else if(szText[sx] == '\0')
		{
			Width[ny] = nx;
		} 
		else
		{
			nx++;
			sx++;
		}			
	}

	for(int j=0; j<ny; j++)
	{
		GDI_TextOut(hdc,x,y+(25*j),	LineBuf[j], Width[j], color, font_size, font_type, hangul);
	}
}

void CTextData::GDI_TextOut(HDC hdc, int x, int y, char* strText, int length,
							COLORREF color, int font_size, LPCSTR font_type, BOOL hangul)
{
	HFONT font, oldfont;	
	if(hangul == FALSE)
		//FW_BOLD 굵은 글씨 출력때 5번째 인자 
		font = CreateFont(font_size,0,0,0,0,0,0,0,ANSI_CHARSET,0,0,0,0,font_type);
	else
		font = CreateFont(font_size,0,0,0,0,0,0,0,HANGUL_CHARSET,0,0,0,0,font_type);
	oldfont =(HFONT)SelectObject(hdc,font);
	SetTextColor(hdc,color);
	SetBkMode(hdc,TRANSPARENT);
	TextOut(hdc, x, y, strText, length);
	SelectObject(hdc,oldfont);
	DeleteObject(font);
}


void CTextData::TypeWritting(HDC hdc, int x, int y, char *szText, 
							 COLORREF color, int font_size, LPCSTR font_type, BOOL hangul)
{
	int TotLen = strlen(szText);	
	char LineBuf[MAX_VER][MAX_HOR];
	int  Width[MAX_VER];
	int sx=0, nx=0, ny=0;

	for(int i=0; i<TotLen; i++)
	{	
		char* ptr;
		ptr = (char*)memccpy(LineBuf[ny]+nx, szText+sx, 0x0d, 1);
		
		if(ptr)
		{
			Width[ny] = nx;
			ny++;
			nx=0;
			sx += 2;
		}
		else if(szText[sx] == '\0')
		{
			Width[ny] = nx;
		} 
		else
		{
			nx++;
			sx++;
		}			
	}
	
	static int h=1, v=0;

	if(m_loopstop == FALSE)
	{
		h++;
		if(LineBuf[v][0] == '\r')
		{
			h=1;
			v++;
		}

		GDI_TextOut(hdc, x, y+(v*20), LineBuf[v], h, color, font_size, font_type, hangul);

		if(h == Width[v]) 
		{	
			h=1;
			v++;	
		}	
		if(v>0)
		{
			for(int j=0; j<v; j++)
			{
				GDI_TextOut(hdc,x,y+(20*j),	LineBuf[j], Width[j], color, font_size, font_type, hangul);	
			}
		}	
		if(v == ny) 
		{
			m_loopstop = TRUE;
			h = 1;
			v = 0;
		}
	}
	else
	{
		for(int j=0; j<ny; j++)
		{
			GDI_TextOut(hdc,x,y+(20*j),	LineBuf[j], Width[j], color, font_size, font_type, hangul);
		}
	}
}

void CTextData::VScrollText(HDC hdc, int x, int y, int end_y, int offset, char *szText, 
							 COLORREF color, int font_size, LPCSTR font_type, BOOL hangul)
{
	int TotLen = strlen(szText);	
	char LineBuf[MAX_VER][MAX_HOR];
	int  Width[MAX_VER];
	int sx=0, nx=0, ny=0;

	for(int i=0; i<TotLen; i++)
	{	
		char* ptr;
		ptr = (char*)memccpy(LineBuf[ny]+nx, szText+sx, 0x0d, 1);
		
		if(ptr)
		{
			Width[ny] = nx;
			ny++;
			nx=0;
			sx += 2;
		}
		else if(szText[sx] == '\0')
		{
			Width[ny] = nx;
		} 
		else
		{
			nx++;
			sx++;
		}			
	}

	static int v = y;

	if(m_loopstop == FALSE)
	{
		for(int j=0; j<ny; j++)
		{
			GDI_TextOut(hdc,x,v+(20*j),	LineBuf[j], Width[j], color, font_size, font_type, hangul);
		}

		if		(y > end_y) v -= offset; 
		else if	(y < end_y) v += offset;

		if( (y>end_y && v<=end_y) || (y<end_y && v>=end_y) || (y == end_y) ) 
		{	
			m_loopstop = TRUE;
			v = y;
		}
	}
	else
	{
		for(int j=0; j<ny; j++)
		{
			GDI_TextOut(hdc,x,end_y+(20*j), LineBuf[j], Width[j], color, font_size, font_type, hangul);
		}
	}
}