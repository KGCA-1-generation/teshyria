// IDdata.cpp: implementation of the CIDdata class.
//
//////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include "IDdata.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIDdata::CIDdata()
{
	Init();
}

CIDdata::~CIDdata()
{

}

void CIDdata::Init()
{
	m_pID = new ID[ID_MAX];
	
	for(int i=0; i<ID_MAX; i++)
	{
//		m_pID[i].bRegist = false;

		for(int j=0; j<NAME_MAX; j++)
			m_pID[i].szName[j] = '\0';

		for(int num=0; num<MISSION_NUM; num++)
		{
			m_pID[i].nScore[num] = 0;
//			m_pID[i].nRanking[num] = 0;
//			m_pID[i].nRankUntill[num] = 0;
		}
		m_pID[i].nSaveMission = SHOW_BASIC_SCENE;
	}
	
//	m_nCounterID = 0;

	m_nSelectID = m_nTotalNumID = 0;
}

void CIDdata::DeleteID(int count)
{
//	m_pID[count].bRegist = false;

	for(int i=0; i<NAME_MAX; i++)
		m_pID[count].szName[i] = '\0';

	for(int num=0; num<MISSION_NUM; num++)
	{
		m_pID[count].nScore[num] = 0;
//		m_pID[count].nRanking[num] = 0;
//		m_pID[count].nRankUntill[num] = 0;
	}
	m_pID[count].nSaveMission = SHOW_BASIC_SCENE;


	for(int m=count; m<m_nTotalNumID; m++)
	{
		for(int n=0; n<NAME_MAX; n++)
		{
			m_pID[m].szName[n] = m_pID[m+1].szName[n];
		
			m_pID[m+1].szName[i] = '\0';
		}
		
		for(int nm=0; nm<MISSION_NUM; nm++)
		{
			m_pID[m].nScore[nm] = m_pID[m+1].nScore[nm];
//			m_pID[count].nRanking[nm] = m_pID[count+1].nRanking[nm];
//			m_pID[count].nRankUntill[nm] = m_pID[count+1].nRankUntill[nm];

			m_pID[m+1].nScore[num] = 0;
//			m_pID[count+1].nRanking[num] = 0;
//			m_pID[count+1].nRankUntill[num] = 0;
		}
		m_pID[m].nSaveMission = m_pID[m+1].nSaveMission;

		m_pID[m+1].nSaveMission = 0;
	}

	m_nTotalNumID --;
	m_nSelectID = m_nTotalNumID - 1;
}

void CIDdata::CreateID(char* name)
{
	for(int i=0; i<NAME_MAX; i++)
		m_pID[m_nTotalNumID].szName[i] = name[i];

	for(int num=0; num<MISSION_NUM; num++)
	{
		m_pID[m_nTotalNumID].nScore[num] = 0;
//		m_pID[m_nTotalNumID].nRanking[num] = 0;
//		m_pID[m_nTotalNumID].nRankUntill[num] = 0;
	}
	m_pID[m_nTotalNumID].nSaveMission = SHOW_BASIC_SCENE;

	m_nTotalNumID ++;
	m_nSelectID = m_nTotalNumID - 1;
}

bool CIDdata::LoadIDdata(char* fname)
{
	FILE* file;
	
	if( ( file = fopen(fname, "rb") ) == NULL)
		return false;
	
	fread(& m_nTotalNumID, sizeof(int), 1, file);
	m_nSelectID = m_nTotalNumID - 1;

	for(int i=0; i<m_nTotalNumID; i++)
	{
//		fread(& m_pID[i].bRegist, sizeof(bool), 1, file);
		
		for(int j=0; j<NAME_MAX; j++)
			fread(& m_pID[i].szName[j], sizeof(char), 1, file);
		
		for(int num=0; num<MISSION_NUM; num++)
		{
			fread(& m_pID[i].nScore[num], sizeof(int), 1, file);
//			fread(& m_pID[i].nRanking[num], sizeof(int), 1, file);
//			fread(& m_pID[i].nRankUntill[num], sizeof(int), 1, file);
		}

		fread(& m_pID[i].nSaveMission, sizeof(int), 1, file);
	}	

	fclose(file);

	return true;
}

bool CIDdata::SaveIDdata(char* fname)
{
	FILE* file;
	
	if( ( file = fopen(fname, "wb") ) == NULL)
		return false;

	fwrite(& m_nTotalNumID, sizeof(int), 1, file);

	for(int i=0; i<m_nTotalNumID; i++)
	{
//		fwrite(& m_pID[i].bRegist, sizeof(bool), 1, file);

		for(int j=0; j<NAME_MAX; j++)
			fwrite(& m_pID[i].szName[j], sizeof(char), 1, file);
		
		for(int num=0; num<MISSION_NUM; num++)
		{
			fwrite(& m_pID[i].nScore[num], sizeof(int), 1, file);
//			fwrite(& m_pID[i].nRanking[num], sizeof(int), 1, file);
//			fwrite(& m_pID[i].nRankUntill[num], sizeof(int), 1, file);
		}

		fwrite(& m_pID[i].nSaveMission, sizeof(int), 1, file);
	}		

	fclose(file);

	return true;
}


char* CIDdata::GetIDName(int count)
{
	return m_pID[count].szName;
}

int* CIDdata::GetIDScore(int count)
{
	return m_pID[count].nScore;
}

int CIDdata::GetIDSaveMission(int count)
{
	return m_pID[count].nSaveMission;
}

void CIDdata::SetIDScore(int score, int count, int mission)
{
	m_pID[count].nScore[mission] = score;
}

void CIDdata::SetIDSaveMission(int mission, int count)
{
	m_pID[count].nSaveMission = mission;
}