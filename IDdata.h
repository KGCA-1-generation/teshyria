// IDdata.h: interface for the CIDdata class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IDDATA_H__14CDB2C1_069F_11D6_944A_0050DA898DDD__INCLUDED_)
#define AFX_IDDATA_H__14CDB2C1_069F_11D6_944A_0050DA898DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ItfDefine.h"


typedef struct IDinfo
{
//	bool	bRegist;
	char	szName[NAME_MAX];			// ���� ID
	int		nScore[MISSION_NUM];		// 6�� �� �̼� ���� ��..(����:�� ���� ���� ��, �߰���: ���� �̼� ���� �������� 0���� ���� ��) 
//	int		nRanking[MISSION_NUM];		// 6�� �� �̼� ��ŷ
//	int		nRankUntill[MISSION_NUM];	// ù��° �̼Ǻ��� ���� �̼Ǳ����� ��ŷ
	int		nSaveMission;				// ������ ����� �̼�
}ID;


class CIDdata  
{
protected:
	ID*		m_pID;	
	int		m_nSelectID;				// ���� ID����
	int		m_nTotalNumID;				// ��ϵ� ���̵� ����
//	int		m_nCounterID;

public:
	CIDdata();
	virtual ~CIDdata();

	void	Init(void);

	bool	LoadIDdata(char* fname);
	bool	SaveIDdata(char* fname);

	void	DeleteID(int count);
	void	CreateID(char* name);
//	void	SetIDName(char* name, int count);

	char*	GetIDName(int count);
	int*	GetIDScore(int count);
	int		GetIDSaveMission(int count);

	void	SetIDScore(int score, int count, int mission);
	void	SetIDSaveMission(int mission, int count);

	int		GetTotalNumID(void)		{ return m_nTotalNumID; }
};

#endif // !defined(AFX_IDDATA_H__14CDB2C1_069F_11D6_944A_0050DA898DDD__INCLUDED_)
