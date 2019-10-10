// DM6Show.cpp: implementation of the CDM6Show class.
//
//////////////////////////////////////////////////////////////////////

#include "DM6Show.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDM6Show::CDM6Show()
{
	 m_pMMStream         = NULL;
	 m_pPrimaryVidStream = NULL;     
	 m_pDDStream         = NULL;     
	 m_pSurface          = NULL;     
	 m_pSample           = NULL;
	 m_pMovieSurface     = NULL;

	 CoInitialize(NULL);     // COM 라이브러리 초기화..
}

CDM6Show::~CDM6Show()
{
	ShutDown();				// 객체 소멸..
}

void CDM6Show::SetBlitPos(int l, int t, int r, int b)
{ 
	 DRect.top=t;
	 DRect.bottom=b;
	 DRect.left=l;
	 DRect.right=r; 
}

void CDM6Show::SetBlitPos(RECT dest)
{
	 DRect.top=dest.top;
	 DRect.bottom=dest.bottom;
	 DRect.left=dest.left;
	 DRect.right=dest.right; 
}

void CDM6Show::ShutDown()
{
	 if (m_pPrimaryVidStream) 
	 {
		 m_pPrimaryVidStream->Release();     
		 m_pPrimaryVidStream=NULL;
	 }
	 if (m_pDDStream) 
	 {
		 m_pDDStream->Release();
		 m_pDDStream=NULL;
	 }
	 if (m_pSample) 
	 {
		 m_pSample->Release();
		 m_pSample=NULL;
	 }
	 if (m_pSurface) 
	 {
		 m_pSurface->Release();   
		 m_pSurface=NULL;
	 }
	 if ((IMultiMediaStream *)m_pMMStream) 
	 {
		 (IMultiMediaStream *)m_pMMStream->Release();
		 m_pMMStream=NULL;

	 }
	 CoUninitialize();  
}

bool CDM6Show::Open(IDirectDraw7* pDD, char* name, IMultiMediaStream** ptMMStream)
{
	 IAMMultiMediaStream *pAMStream;     
	 WCHAR       wPath[MAX_PATH];     
 
	 if (FAILED(CoCreateInstance(CLSID_AMMultiMediaStream, NULL, CLSCTX_INPROC_SERVER,  IID_IAMMultiMediaStream, (void **)&pAMStream))) 
	 {
		  ShutDown();
		  return false;
	 }
	 if (FAILED(pAMStream->Initialize(STREAMTYPE_READ, 0, NULL)))
	 {
		  ShutDown();
		  return false;
	 }
	 if (FAILED(pAMStream->AddMediaStream(pDD, &MSPID_PrimaryVideo, 0, NULL))) 
	 {
		  ShutDown();
		  return false;     
	 }
	 if (FAILED(pAMStream->AddMediaStream(NULL, &MSPID_PrimaryAudio, AMMSF_ADDDEFAULTRENDERER, NULL)))
	 {
		  ShutDown();
		  return false;  
	 }

	 MultiByteToWideChar(CP_ACP, 0, name, -1, wPath, sizeof(wPath)/sizeof(wPath[0]));    
 
	 if (FAILED(pAMStream->OpenFile(wPath, 0))) 
	 {
		  ShutDown();
		  return false;      
	 }

	 *ptMMStream = pAMStream;     
	 pAMStream->AddRef();  
  
	 pAMStream->Release();     

	 return true;
}

bool CDM6Show::PrepareMovie(IDirectDraw7* pDD, IDirectDrawSurface7 *pDDSurface, char* FName)
{
	 if (!Open(pDD,FName,&m_pMMStream)) return false;

	 if (FAILED(m_pMMStream->GetMediaStream(MSPID_PrimaryVideo, &m_pPrimaryVidStream)))
	 {
		  ShutDown();
		  return false;
	 }     
	 if (FAILED(m_pPrimaryVidStream->QueryInterface(IID_IDirectDrawMediaStream, (void **)&m_pDDStream)))
	 {
		  ShutDown();      
		  return false;
	 }
	 if (FAILED(m_pDDStream->CreateSample(NULL, NULL, 0, &m_pSample)))
	 {
		  ShutDown();
		  return false;     
	 }
	 if (FAILED(m_pSample->GetSurface((IDirectDrawSurface**)&m_pSurface, &SRect)))
	 {
		  ShutDown();          
		  return false;
	 }
	 if (FAILED(m_pMMStream->SetState(STREAMSTATE_RUN)))
	 {
		  ShutDown();
		  return false;    
	 }

	 m_pMovieSurface=pDDSurface;
  
	 return true;

}

bool CDM6Show::RenderStreamToSurface()
{
     if (m_pSample->Update(0, NULL, NULL, 0) != S_OK) 
	 {  
		 ShutDown(); 
		 return false; 
	 } 
	 m_pMovieSurface->Blt(&DRect, m_pSurface, &SRect, DDBLT_WAIT, NULL); 
	 return true;
}