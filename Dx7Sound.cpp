// Dx7Sound.cpp: implementation of the CDx7Sound class.
//
//////////////////////////////////////////////////////////////////////
#include <windows.h>   
#include <mmsystem.h>
#include <ddraw.h>     
#include <dsound.h>
#include "Dx7Sound.h"

//DEBUG
#include <stdio.h>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDx7Sound::CDx7Sound()
{
	m_bValid = FALSE;
	m_iVolume = 0;
	m_iSoundID = 0;
}

CDx7Sound::~CDx7Sound()
{
	ReleaseAll();
}

HRESULT CDx7Sound::Init(HWND hWnd)
{
	HRESULT hr;
	if(FAILED(hr=DirectSoundCreate(NULL, &m_lpDS, NULL))) return hr;
	if(FAILED(hr=m_lpDS->SetCooperativeLevel(hWnd,DSSCL_NORMAL))) return hr;
	
	memset(m_soundformat,0,sizeof(SOUNDFORMAT)*SOUNDS_TOTAL);
	for(int i=0; i<SOUNDS_TOTAL; i++)
	{
		if(m_soundformat[i].dsbuffer)
		{
			m_soundformat[i].dsbuffer->Release();
		}	
		memset(&m_soundformat[i],0,sizeof(SOUNDFORMAT));
		m_soundformat[i].state = SOUND_NULL;
	}

	return DS_OK;
}

void CDx7Sound::PlaySound(char *filename, int use, int loop)
{
	//인자 설명
	//char *filename 파일명
	//int use :  0 = 효과음,  1 = 배경음
	//int loop:  0 = 1회재생, 1 = 반복재생 

	if(use == 0) //효과음일때 루틴
	{
		m_iSoundID = m_iSoundID+1;
		if(m_iSoundID > SOUNDS_TOTAL) m_iSoundID = 1;		
		m_bValid = LoadWave(filename);

		if(m_bValid == TRUE)
		{	
			if(loop == 0)
			{	
				m_soundformat[m_iSoundID].dsbuffer->SetVolume(m_iVolume);			
				m_soundformat[m_iSoundID].dsbuffer->Play(0,0,0);	//Non-looping Mod	
			}	
			
			if(loop == 1) 
			{
				m_soundformat[m_iSoundID].dsbuffer->SetVolume(m_iVolume);	
				m_soundformat[m_iSoundID].dsbuffer->Play(0,0,1);	//Looping Mod		
			}
		}
	}

	if(use == 1) //배경음일때 루틴 
	{
		m_iSoundID = 0; 
		if(m_soundformat[m_iSoundID].dsbuffer)
		{
			m_soundformat[m_iSoundID].dsbuffer->Release();
		}
		memset(&m_soundformat[m_iSoundID],0,sizeof(SOUNDFORMAT));
		m_soundformat[m_iSoundID].state = SOUND_NULL;
		m_bValid = LoadWave(filename);
		if(m_bValid == TRUE)
		{
			if(loop == 0)
			{
				m_soundformat[m_iSoundID].dsbuffer->SetVolume(m_iVolume);			
				m_soundformat[m_iSoundID].dsbuffer->Play(0,0,0);	//Non-looping Mod
			}
			if(loop == 1) 
			{
				m_soundformat[m_iSoundID].dsbuffer->SetVolume(m_iVolume);	
				m_soundformat[m_iSoundID].dsbuffer->Play(0,0,1);	//Looping Mod		
			}
		}
	}
}

void CDx7Sound::Volume(int volume)
{
	if(volume > 0)		 volume = 0;
	if(volume < -10000)  volume = -10000;
	m_iVolume = volume;

	for(int i=0; i<SOUNDS_TOTAL; i++)
	{
		if(m_soundformat[i].dsbuffer)
			m_soundformat[i].dsbuffer->SetVolume(m_iVolume);	
	}	
}


void CDx7Sound::Stop()
{
	for(int i=0; i<SOUNDS_TOTAL; i++)
	{
		if(m_soundformat[i].dsbuffer)
			m_soundformat[i].dsbuffer->Stop();
	}
}


BOOL CDx7Sound::LoadWave(char *filename)
{
	HMMIO 			hwav;    
	MMCKINFO		parent, child;   
	WAVEFORMATEX    wf;   

	UCHAR *byBuffer,       
		  *pAudio1 = NULL, 
		  *pAudio2 = NULL; 

	DWORD AudioLen1=0,  
		  AudioLen2=0;  
		
	if(m_soundformat[m_iSoundID].state == SOUND_NULL)
	{
		 m_bValid = TRUE;
	}  

	if(m_bValid == FALSE) return FALSE;

	parent.ckid 	    = (FOURCC)0;
	parent.cksize 	    = 0;
	parent.fccType	    = (FOURCC)0;
	parent.dwDataOffset = 0;
	parent.dwFlags		= 0;

	child = parent;

	if((hwav = mmioOpen(filename, NULL, MMIO_READ | MMIO_ALLOCBUF))==NULL) return FALSE;

	parent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	if(mmioDescend(hwav, &parent, NULL, MMIO_FINDRIFF))
	{
		mmioClose(hwav, 0);
		return FALSE;	
	} 
	child.ckid = mmioFOURCC('f', 'm', 't', ' ');
	if(mmioDescend(hwav, &child, &parent, 0))
	{
		mmioClose(hwav, 0);
		return FALSE;	
	} 
	if(mmioRead(hwav, (char*)&wf, sizeof(wf)) != sizeof(wf))
	{
		mmioClose(hwav, 0);
		return FALSE;
	} 
	if(wf.wFormatTag != WAVE_FORMAT_PCM)
	{
		mmioClose(hwav, 0);
		return FALSE;
	} 
	if(mmioAscend(hwav, &child, 0))
	{
		mmioClose(hwav, 0);
		return FALSE;	
	}   
	child.ckid = mmioFOURCC('d', 'a', 't', 'a');
	if(mmioDescend(hwav, &child, &parent, MMIO_FINDCHUNK))
	{
		mmioClose(hwav, 0);
		return FALSE;	
	} 

	byBuffer = (UCHAR*)malloc(child.cksize);

	mmioRead(hwav, (char*)byBuffer, child.cksize);

	mmioClose(hwav, 0);

	memset(&m_waveformat, 0, sizeof(WAVEFORMATEX));

	m_waveformat.wFormatTag	     = WAVE_FORMAT_PCM;
	m_waveformat.nChannels	     = 1;               
	m_waveformat.nSamplesPerSec  = 18900;           
	m_waveformat.nBlockAlign	 = 1;                
	m_waveformat.nAvgBytesPerSec = m_waveformat.nSamplesPerSec * m_waveformat.nBlockAlign;
	m_waveformat.wBitsPerSample  = 8;
	m_waveformat.cbSize		     = 0;

//	OutputDebugString("aaa");
	m_DSBdesc.dwSize		= sizeof(DSBUFFERDESC);
	m_DSBdesc.dwFlags		= DSBCAPS_CTRLVOLUME | DSBCAPS_STATIC | DSBCAPS_LOCSOFTWARE;
	m_DSBdesc.dwBufferBytes	= child.cksize;
	m_DSBdesc.lpwfxFormat	= &m_waveformat;

	if (FAILED(m_lpDS->CreateSoundBuffer(&m_DSBdesc,&m_soundformat[m_iSoundID].dsbuffer,NULL)))
	{
	   free(byBuffer);
	   return FALSE;
	}

	m_soundformat[m_iSoundID].rate  = wf.nSamplesPerSec;
	m_soundformat[m_iSoundID].size  = child.cksize;
	m_soundformat[m_iSoundID].state = SOUND_LOADED;

	if (FAILED(m_soundformat[m_iSoundID].dsbuffer->Lock(0,					 
											child.cksize,			
											(void **)&pAudio1, 
											&AudioLen1,
											(void **)&pAudio2, 
											&AudioLen2,
											DSBLOCK_FROMWRITECURSOR)))
									 return FALSE;

	memcpy(pAudio1, byBuffer, AudioLen1);
	memcpy(pAudio2, (byBuffer + AudioLen1), AudioLen2);

	if (FAILED(m_soundformat[m_iSoundID].dsbuffer->Unlock(pAudio1, 
											  AudioLen1, 
											  pAudio2, 
											  AudioLen2)))
 									 return FALSE;

	free(byBuffer);

	return TRUE;
}

void CDx7Sound::ReleaseAll()
{	
	SAFE_RELEASE(m_lpDS);
}







