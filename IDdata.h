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
	char	szName[NAME_MAX];			// 유저 ID
	int		nScore[MISSION_NUM];		// 6개 각 미션 점수 들어감..(총합:각 번지 점수 합, 중간합: 현재 미션 번지 점수부터 0번지 점수 합) 
//	int		nRanking[MISSION_NUM];		// 6개 각 미션 랭킹
//	int		nRankUntill[MISSION_NUM];	// 첫번째 미션부터 현재 미션까지의 랭킹
	int		nSaveMission;				// 마지막 저장된 미션
}ID;


class CIDdata  
{
protected:
	ID*		m_pID;	
	int		m_nSelectID;				// 현재 ID번지
	int		m_nTotalNumID;				// 등록된 아이디 갯수
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
