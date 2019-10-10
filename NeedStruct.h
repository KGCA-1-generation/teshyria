#ifndef _NeedStruct_H_
#define _NeedStruct_H_
#define		BOOL	int

struct TeamInfo {  
	int  bLeader;   // ������ ���� ����
	int  bFind;     // ���ΰ��� �߰� �ߴ��� ����
	int  bAllDead;  // �� ���� ��� 
	int  LeaderX;   // ������ x��ǥ
	int  LeaderY;   // ������ y��ǥ
	int  Distance;  // ������ ���ΰ����� �Ÿ�
	int  nLeaderNum; // ������ ĳ�� ��ȣ
	int* pWpX;
	int* pWpY;
	int	 nWPCnt;
	int	 nWpNum;	// ���� ���� ����Ʈ ��ȣ 
	int  nDeadCnt;

	void clear() {
		nWpNum	 = 0;
		nDeadCnt  = 0;
		bLeader  = 1;  // ���� ����
		bFind    = 0;  // �׽ø��� �߰� ����
		bAllDead = false;
		LeaderX  = 0;  // ���� ��ġ
		LeaderY  = 0;
		nLeaderNum = -1;
		Distance = 1000;  // �׽ø��ƿ��� �Ÿ�
	};
}; // �� ���� ����ü


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
	int nSCTeamCnt;        // ������ ����
	int nRGTeamCnt;        // ������ ����
	int nOurCnt;           // �Ʊ�   ����
	int nMonCnt;           // ���� ��
	int nEventCnt;         // �̺�Ʈ ��
	int aCharCnt[16];      // ������ĳ���� ��(0~15) 
	int nCharTotal;        // ��� ĳ������ �Ѽ�
	int aTeamNum[100];     // ĳ������ ����ȣ(�迭�� ĳ���� �Ѽ���ŭ)
	int aPosInTeam[100];   // �������� ��ġ
	int aXPosInMap[100];   // �ʿ����� x ��ġ
	int aYPosInMap[100];   // �ʿ����� y ��ġ
	BOOL abLeader[100];    // �������� ����
	int aWayNum[100];	   
	int aWayCnt[100];
	int aWayX[50][50];
	int aWayY[50][50];
	int aEventNum[9];
	int nTeamCnt;
	rect artEvent[9];
};

#endif _NeedStruct_H_