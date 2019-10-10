// Dx7Sound.h: interface for the CDx7Sound class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DX7SOUND_H__08DEDB21_E47F_11D5_B3AA_0050DA898EA1__INCLUDED_)
#define AFX_DX7SOUND_H__08DEDB21_E47F_11D5_B3AA_0050DA898EA1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
 
#define SOUND_NULL      0 
#define SOUND_LOADED    1
#define SOUND_PLAYING   2
#define SOUND_STOPPED   3

#define SOUNDS_TOTAL    8

#define SAFE_DELETE(p)  { if(p) { delete (p);     (p)=NULL; } }
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }

typedef struct {
	LPDIRECTSOUNDBUFFER dsbuffer;  
	int state;                     
	int rate;                      
	int size;                      
} SOUNDFORMAT;

class CDx7Sound  
{
protected:
	int				m_iSoundID;
	BOOL			m_bValid;	
	LPDIRECTSOUND	m_lpDS;       
	DSBUFFERDESC	m_DSBdesc;       
	WAVEFORMATEX	m_waveformat; 
	SOUNDFORMAT		m_soundformat[SOUNDS_TOTAL];
	int				m_iVolume;

public:			
	void Stop();
	CDx7Sound();
	void PlaySound(char *filename,int use,int loop);
	void Volume(int volume);
	BOOL LoadWave(char* filename);
	HRESULT Init(HWND hWnd);
	void ReleaseAll();	
	virtual ~CDx7Sound();
};

#endif // !defined(AFX_DX7SOUND_H__08DEDB21_E47F_11D5_B3AA_0050DA898EA1__INCLUDED_)
