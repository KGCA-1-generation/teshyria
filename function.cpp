#include <math.h>
#include "function.h"

WORD Convert16(COLORREF color)
{
	WORD r, g, b;

	r = (WORD)(GetRValue(color)>>3);
	g = (WORD)(GetGValue(color)>>2);
	b = (WORD)(GetBValue(color)>>3);

	return ((r<<11)|(g<<5)|b);
}

int GetDirection(int x, int y)
{
	double pi = 3.1415926535, rad, Angle;
	int nJump = 0;

	rad = (double)y / (double)x;

	if( y<0 ) nJump = 4;  // �Ʒ��κ�

	Angle = atan(rad) * 57.2958;  // (180 / pi) = 57.2958
	if( Angle > 0.0  && Angle < 22.5 ) return (0+nJump);
	if( Angle > 22.5 && Angle < 67.5 ) return (1+nJump);
	if( Angle > 67.5 && Angle < 90.0 ) return (2+nJump);
	if( Angle >-90.0 && Angle <-67.5 ) return (2+nJump);
	if( Angle >-67.5 && Angle <-22.5 ) return (3+nJump);
	if( Angle >-22.5 && Angle < 0.0  ) return (4+nJump)%8;

	if( x==0 && y>0 ) return 2;
	if( y==0 && x<0 ) return 4;
	if( x==0 && y<0 ) return 6;	
	return 0;
}

//--------------------------------------------------------------------
// �� �� ������ �Ÿ��� ���ϴ� �Լ�
//--------------------------------------------------------------------
int GetDistance(int x, int y)
{	
	return (int)sqrt((double)(x*x)+(double)(y*y));
}

//--------------------------------------------------------------------
// Release ��忡�� memcpy()���� �� ���� �Լ���!!!
// ������ ��� ����.......(�Ѥ�;;)
//--------------------------------------------------------------------
void memcpy2(unsigned char *dest, unsigned char *src, int size)
{ 
	int i, flag = 0;
	unsigned long *p1, *p2;
	
	unsigned long address = (unsigned long)src;
	if ( (address % 4) !=0 )  // 4����Ʈ ������ �Ǿ� ���� �ʴٸ�..
	{ 
		for (i=0; i<(int)(address%4); i++) // 4����Ʈ ������ ���� �κи� ����..
			*(dest++) = *(src++);
	}       // ���� src ���� 4����Ʈ ������ �� ����
	size -= (address%4);
	
	p1 = (unsigned long *)src;
	p2 = (unsigned long *)dest;
	
	if ((size % 4) != 0)  // 4����Ʈ ������ �ϰ� ���� ����Ʈ�� ����ٸ�..
	{ 
		for (i=0; i<((size>>2)-1); i++)
		{ 
			*p2 = *p1;
			p2++; 
			p1++;
		}
		src = (unsigned char *)p1;  dest = (unsigned char *)p2; // ���� ��⸸ ����
		for (i=0; i<(size%4); i++)
			*(dest++) = *(src++);
	}
	else
	{
		for (i=0; i<(size>>2); i++)
			*(p2++) = *(p1++);
	}
}