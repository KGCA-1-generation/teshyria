#ifndef _NeedStruct_H_
#define _NeedStruct_H_
#define		BOOL	int

struct TeamInfo {  
	int  bLeader;   // 리더의 생사 여부
	int  bFind;     // 주인공을 발견 했는지 여부
	int  bAllDead;  // 팀 전원 사망 
	int  LeaderX;   // 리더의 x좌표
	int  LeaderY;   // 리더의 y좌표
	int  Distance;  // 리더와 주인공과의 거리
	int  nLeaderNum; // 리더의 캐릭 번호
	int* pWpX;
	int* pWpY;
	int	 nWPCnt;
	int	 nWpNum;	// 현재 웨이 포인트 번호 
	int  nDeadCnt;

	void clear() {
		nWpNum	 = 0;
		nDeadCnt  = 0;
		bLeader  = 1;  // 생존 여부
		bFind    = 0;  // 테시리아 발견 여부
		bAllDead = false;
		LeaderX  = 0;  // 리더 위치
		LeaderY  = 0;
		nLeaderNum = -1;
		Distance = 1000;  // 테시리아와의 거리
	};
}; // 팀 정보 구조체


struct rect {
	int x1;
	int y1;
	int x2;
	int y2;
};

struct SORTY {
	int m_nChar;
	int m_nNum;
	int m_nY, m_nX;
	int m_nScrX, m_nScrY;
	void clear() {
		m_nChar = 0;
		m_nNum  = 0;
		m_nScrX = 0;
		m_nScrY = 0;
		m_nX    = 0;
		m_nY    = 0;		
	};
};

struct point {
	int x;
	int y;
};

struct Object {
	int nObj;
	int nX;
	int nY;
};

struct StageInfo {
	int nSCTeamCnt;        // 정찰대 팀수
	int nRGTeamCnt;        // 순찰대 팀수
	int nOurCnt;           // 아군   팀수
	int nMonCnt;           // 몬스터 수
	int nEventCnt;         // 이벤트 수
	int aCharCnt[16];      // 종류별캐릭터 수(0~15) 
	int nCharTotal;        // 모든 캐릭터의 총수
	int aTeamNum[100];     // 캐릭터의 팀번호(배열은 캐릭터 총수만큼)
	int aPosInTeam[100];   // 팀에서의 위치
	int aXPosInMap[100];   // 맵에서의 x 위치
	int aYPosInMap[100];   // 맵에서의 y 위치
	BOOL abLeader[100];    // 리더인지 여부
	int aWayNum[100];	   
	int aWayCnt[100];
	int aWayX[50][50];
	int aWayY[50][50];
	int aEventNum[9];
	int nTeamCnt;
	rect artEvent[9];
};

#endif _NeedStruct_H_