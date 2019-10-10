#ifndef _function_H_
#define _function_H_

#define BYTE            unsigned char
#define GetRValue(rgb)  ((BYTE)(rgb))
#define GetGValue(rgb)  ((BYTE)(((WORD)(rgb)) >> 8))
#define GetBValue(rgb)  ((BYTE)((rgb)>>16))
#define WORD            unsigned short
#define DWORD           unsigned long
#define COLORREF        int
#define TRUE            1
#define FALSE           0
#define NULL            0
#define BOOL            int
//#define RGB(r, g ,b)  ((DWORD) (((BYTE) (r) | ((WORD) (g) << 8)) | (((DWORD) (BYTE) (b)) << 16)))

WORD Convert16    (COLORREF color);
int  GetDirection (int x, int y),
     GetDistance  (int x, int y);

#endif _function_H_