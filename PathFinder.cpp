#include "function.h"
#include "PathFinder.h"
#include <malloc.h>
#define SAFE_DELETE(a)	if(a) { delete a; a = NULL; }
////////////////////////////////////////////////////////////////////////////////
//                           Constructor Destructor                           //
////////////////////////////////////////////////////////////////////////////////

CPathFinder::CPathFinder()
{
   Stack = ( STACK* )calloc(1,sizeof( STACK ));
//	Stack = new STACK;
	isPath = FALSE;
	OPEN = NULL;
	CLOSED = NULL;
	PATH = NULL;
	m_pAttrMap = NULL;   
}

////////////////////////////////////////////////////////////////////////////////

CPathFinder::~CPathFinder()
{
	FreeNodes();
	free(Stack);
//	SAFE_DELETE( Stack );
}

////////////////////////////////////////////////////////////////////////////////
//                             Public Member Functions                        //
////////////////////////////////////////////////////////////////////////////////
void CPathFinder::SetMap(int *pMap, int nW)
{
	m_pAttrMap = pMap;
	m_nMapW = nW;
}

BOOL CPathFinder::NewPath(int sx,int sy, int dx,int dy)
{
	if ( FreeTile(dx,dy)&&FreeTile(sx,sy) && !SameTile(sx, sy, dx, dy) )
	{
		FreeNodes(); 
   		FindPath(sx,sy,dx,dy);
   		return (isPath=TRUE);
	}
	else
  		return (isPath=FALSE);
}

////////////////////////////////////////////////////////////////////////////////

BOOL CPathFinder::ReachedGoal(void) 
{                                      
	if ( !isPath ) return TRUE;  
	if ( PATH->Parent != NULL )  return FALSE;             
	else	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
BOOL CPathFinder::SameTile(int x1, int y1, int x2, int y2)
{
	if( x1 == x2 && y1 == y2 ) return true;
	else return false;
}

////////////////////////////////////////////////////////////////////////////////

int CPathFinder::FreeTile(int x, int y)
{     
	if( (m_pAttrMap[x+y*m_nMapW] & 0x00000001) || (m_pAttrMap[x+y*m_nMapW] & 0x00000010))
		return FALSE;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//						      Private Member Functions                        //
////////////////////////////////////////////////////////////////////////////////

void CPathFinder::FreeNodes(void)
{
  	NODE *Node, *OldNode;

	if ( OPEN != NULL )
	{
   		Node = OPEN->NextNode;
   		while ( Node != NULL )
		{
      		OldNode = Node;
      		Node = Node->NextNode;
			free(OldNode);
			//SAFE_DELETE(OldNode);
		}
	}

	if ( CLOSED != NULL )
	{
   		Node = CLOSED->NextNode;
   		while ( Node != NULL )
		{
			OldNode = Node;
			Node = Node->NextNode;
			free(OldNode);
			//SAFE_DELETE(OldNode);
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//                               A* Algorithm                                 //
////////////////////////////////////////////////////////////////////////////////

void CPathFinder::FindPath(int sx, int sy, int dx, int dy)
{
	NODE *Node, *BestNode;

	// OPEN = new NODE;
	//CLOSED = new NODE; 
	//Node = new NODE;

	Node=( NODE* )calloc(1,sizeof( NODE ));
	OPEN=( NODE* )calloc(1,sizeof( NODE ));
	CLOSED=( NODE* )calloc(1,sizeof( NODE ));
	Node->g = 0;
	Node->h = (dx-sx)*(dx-sx) + (dy-sy)*(dy-sy);  
	Node->f = Node->g+Node->h;

	Node->x = dx;
	Node->y = dy;

	OPEN->NextNode=Node;        
	for (int i=0; i<200; i++)
	{
		BestNode=ReturnBestNode();
		if( SameTile(BestNode->x, BestNode->y, sx, sy) )
	   	break;
		GenerateSuccessors(BestNode,sx,sy);
	}
	PATH = BestNode;
}

////////////////////////////////////////////////////////////////////////////////

NODE* CPathFinder::ReturnBestNode(void)
{
   NODE *tmp;

   if ( OPEN->NextNode == NULL )
   {
		return NULL;
   }

   tmp = OPEN->NextNode;   
   OPEN->NextNode = tmp->NextNode;    

   tmp->NextNode = CLOSED->NextNode;
   CLOSED->NextNode = tmp;

   return(tmp);
}

////////////////////////////////////////////////////////////////////////////////

void CPathFinder::GenerateSuccessors(NODE *BestNode, int dx, int dy)
{
   int x, y;

	if ( FreeTile(x=BestNode->x-1, y=BestNode->y-1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x, y=BestNode->y-1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x+1, y=BestNode->y-1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x+1, y=BestNode->y) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x+1, y=BestNode->y+1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		  
	if ( FreeTile(x=BestNode->x, y=BestNode->y+1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x-1, y=BestNode->y+1) )
		GenerateSucc(BestNode,x,y,dx,dy);
		   
	if ( FreeTile(x=BestNode->x-1, y=BestNode->y) )
		GenerateSucc(BestNode,x,y,dx,dy);
}

////////////////////////////////////////////////////////////////////////////////

void CPathFinder::GenerateSucc(NODE *BestNode,int x, int y, int dx, int dy)
{
	int g, c = 0;
	NODE *Old, *Successor;

	g = BestNode->g+1;	    

	if ( (Old=CheckOPEN(x, y)) != NULL ) 
	{
		for( c = 0; c < 8; c++)	if( BestNode->Child[c] == NULL ) break;
		BestNode->Child[c] = Old;

		if ( g < Old->g )  
		{
			Old->Parent = BestNode;
	   		Old->g = g;
	   		Old->f = g + Old->h;
		}
	}
	else if ( (Old=CheckCLOSED(x, y)) != NULL ) 
	{
        for( c = 0; c< 8; c++)  if ( BestNode->Child[c] == NULL ) break;
		BestNode->Child[c] = Old;

		if ( g < Old->g ) 
		{
	   	    Old->Parent = BestNode;
	   	    Old->g = g;
	   	    Old->f = g + Old->h;
	   	    PropagateDown(Old);  
		}
	}
	else
	{
		Successor = ( NODE* )calloc(1,sizeof( NODE ));
	   //Successor = new NODE;
		Successor->Parent = BestNode;
		Successor->g = g;
		Successor->h = (x-dx)*(x-dx) + (y-dy)*(y-dy);  
		Successor->f = g+Successor->h;     
		Successor->x = x;                 
		Successor->y = y;
		Insert(Successor);     
		for( c =0; c < 8; c++) if ( BestNode->Child[c] == NULL ) break;
		BestNode->Child[c] = Successor;
   }
}

////////////////////////////////////////////////////////////////////////////////

NODE* CPathFinder::CheckOPEN(int x, int y)
{
	NODE *tmp;

	tmp = OPEN->NextNode;
	while ( tmp != NULL )
	{
		if( SameTile(tmp->x, tmp->y, x, y) )	return (tmp);
		else	tmp = tmp->NextNode;
	}
	return(NULL);
}

////////////////////////////////////////////////////////////////////////////////

NODE* CPathFinder::CheckCLOSED(int x, int y)
{
	NODE *tmp;
	
	tmp = CLOSED->NextNode;

	while ( tmp != NULL )
	{
		if ( SameTile(tmp->x, tmp->y, x, y) )  	return(tmp);
		else   	tmp = tmp->NextNode;
	}
	return(NULL);
}

////////////////////////////////////////////////////////////////////////////////

void CPathFinder::Insert(NODE *Successor)
{
	NODE *tmp1, *tmp2;
	int f;

	if ( OPEN->NextNode == NULL )
	{
		OPEN->NextNode = Successor;
		return;
	}

    f = Successor->f;
	tmp1 = OPEN;
	tmp2 = OPEN->NextNode;
	while ( (tmp2 != NULL) && (tmp2->f < f) )
	{
   		tmp1 = tmp2;
		tmp2 = tmp2->NextNode;
	}

	Successor->NextNode = tmp2;
	tmp1->NextNode = Successor;
}

////////////////////////////////////////////////////////////////////////////////

void CPathFinder::PropagateDown(NODE *Old)
{
	int c, g;
	NODE *Child, *Father;

	g = Old->g;            
	for ( c = 0; c < 8; c++)
	{
   	    if ( (Child=Old->Child[c]) == NULL )   break;
		if ( g+1 < Child->g)
		{
	   		Child->g = g+1;
	   		Child->f = Child->g + Child->h;
	  	    Child->Parent = Old;           
			Push(Child);                 
		}     
   }

	while ( Stack->NextStackPtr != NULL )
	{
		Father = Pop();
		for (c = 0; c<8; c++)
		{
	   		if ( (Child=Father->Child[c]) == NULL )  break;
			if ( Father->g+1 < Child->g ) 
	  		{                          
	      		Child->g = Father->g+1;
	    		Child->f = Child->g+Child->h;
	    		Child->Parent = Father;
	    		Push(Child);
			}
		}
	}
}


////////////////////////////////////////////////////////////////////////////////
//                              STACK FUNCTIONS                               //
////////////////////////////////////////////////////////////////////////////////

void CPathFinder::Push(NODE *Node)
{
	STACK *tmp;

	tmp =( STACK* )calloc(1,sizeof( STACK ));
	//tmp = new STACK;
	tmp->NodePtr = Node;
	tmp->NextStackPtr = Stack->NextStackPtr;
	Stack->NextStackPtr = tmp;
}

////////////////////////////////////////////////////////////////////////////////

NODE* CPathFinder::Pop(void)
{
	NODE *tmp;
	STACK *tmpSTK;

	tmpSTK = Stack->NextStackPtr;
	tmp = tmpSTK->NodePtr;

	Stack->NextStackPtr = tmpSTK->NextStackPtr;
	 free(tmpSTK);
	//SAFE_DELETE(tmpSTK);
	return(tmp);
}



