#ifndef PATHFINDER_H
#define PATHFINDER

#define BOOL  int
struct NODE {     
	long f, h;
	int g, tmpg;
	int x, y;
   	NODE *Parent;
    NODE *Child[8];		
	NODE *NextNode;		
};

struct STACK {      
	NODE *NodePtr;
	STACK *NextStackPtr;
};

class CPathFinder 
{
private:
	NODE *OPEN;		
	NODE *CLOSED;
	NODE *PATH;		

	STACK *Stack;

	BOOL isPath;   

	int m_nMapW;     	


public:
	int* m_pAttrMap;
	void SetMap(int* pMap, int nW);
	BOOL SameTile(int x1, int y1, int x2, int y2);


	BOOL NewPath(int sx, int sy, int dx, int dy);  // Must be called and be true
	
	BOOL ReachedGoal(void); 
	void PathNextNode(void) { PATH=PATH->Parent; }
	int NodeGetX(void)      { return PATH->x; }
	int NodeGetY(void)      { return PATH->y; }
	int FreeTile(int x, int y); 
	CPathFinder();
    ~CPathFinder();

private:

	void FreeNodes(void);

	void FindPath(int sx, int sy, int dx, int dy);
	NODE *ReturnBestNode(void);
	void GenerateSuccessors(NODE *BestNode, int dx, int dy);
   	void GenerateSucc(NODE *BestNode,int x, int y, int dx, int dy);
	NODE *CheckOPEN(int x, int y);
	NODE *CheckCLOSED(int x, int y);
	void Insert(NODE *Successor);
	void PropagateDown(NODE *Old);
	void Push(NODE *Node); 
	NODE *Pop(void);


};

#endif // PATHFINDER_H

