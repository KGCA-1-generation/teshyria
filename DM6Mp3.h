// DM6Mp3.h: interface for the CDM6Mp3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DM6MP3_H__7BA924E1_EA55_11D5_944A_0050DA898DDD__INCLUDED_)
#define AFX_DM6MP3_H__7BA924E1_EA55_11D5_944A_0050DA898DDD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <dshow.h>

#ifndef __DM6MP3_H_
#define __DM6MP3_H_

//------------------------------------------------------------------------------
//  Macros
//------------------------------------------------------------------------------
#define FC __fastcall
//#define SAFE_RELEASE(p) { if (p) { (p)->Release (); (p) = NULL; } }


///////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------------------------------
//  Name : CDM6Mp3
//  Desc : 사운드 필터 그래프 하나에 대한 관리를 행한다
//------------------------------------------------------------------------------
class CDM6Mp3  
{
public:
	enum { };
	//------------------------------------------------------------

protected:
	IGraphBuilder *graph_builder;
	IMediaControl *media_control;
	IMediaSeeking *media_seeking;
	IBasicAudio   *basic_audio;
	IMediaEvent   *media_event;

	bool init_flag;
	//------------------------------------------------------------
public:
	CDM6Mp3();
	~CDM6Mp3();

	void InitClass  (void);
	bool CleanGraph (void);
	bool HasFilter   (IBaseFilter *filter);

	FILTER_STATE GetState (void) const;

	void OnEvent (void);

	bool IsInitialized (void)				{return init_flag;}
	IGraphBuilder *GetGraphBuilder (void)	{return graph_builder;}

	HRESULT GetVolume (long *vol)  {return basic_audio->get_Volume (vol);}
	HRESULT GetBalance (long *bal) {return basic_audio->get_Balance (bal);}
	HRESULT SetVolume (const long vol)   
									{return basic_audio->put_Volume (vol);}
	HRESULT SetBalance (const long bal)  
									{return basic_audio->put_Balance (bal);}

	HRESULT Run   (void)  {return media_control->Run ();}
	HRESULT Stop  (void)  {return media_control->Stop ();}
	HRESULT Pause (void)  {return media_control->Pause ();}
	HRESULT SetPosition (LONGLONG pos)
							{return media_seeking->SetPositions (&pos, 
									AM_SEEKING_AbsolutePositioning, &pos, 
									AM_SEEKING_NoPositioning);}
	//------------------------------------------------------------
};

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

//------------------------------------------------------------------------------
//  Name : CMp3Manager
//  Desc : CDM6Mp3 를 관리한다
//------------------------------------------------------------------------------
class CMp3Manager
{
public:
	enum { DEF_CHANNELS = 5, MIN_CHANNELS = 1, ISERROR = -1 };
	//------------------------------------------------------------

protected:
	
	CDM6Mp3 * channels;

	bool init_flag;

	int cur_channel;
	int channel_num;
	//------------------------------------------------------------
public:
	CMp3Manager(int channel_num = DEF_CHANNELS);
	
	~CMp3Manager();

	void InitClass (int channel_num = DEF_CHANNELS);

	int FC PlaySound (const char *path, const bool BGM_flag = false);
	int FC PlayBGM   (const char *path)  {return PlaySound (path, true);}

	void OnEvent (void);

	HRESULT RunAll   (void);
	HRESULT StopAll  (void);
	HRESULT PauseAll (void);
	HRESULT RunSounds   (void);				// 0번을 제외한 채널만
	HRESULT StopSounds  (void);
	HRESULT PauseSounds (void);
	HRESULT RunBGM   (void)  {return channels[0].Run ();}
	HRESULT StopBGM  (void)  {return channels[0].Stop ();}
	HRESULT PauseBGM (void)  {return channels[0].Pause ();}
	HRESULT Run   (const int which)  {return channels[which].Run ();}
	HRESULT Stop  (const int which)  {return channels[which].Stop ();}
	HRESULT Pause (const int which)  {return channels[which].Pause ();}

	HRESULT SetEntVolume  (const long vol) const;
	HRESULT SetEntBalance (const long bal) const;

	HRESULT SetVolume (const int which, const long vol) const
							{return channels[which].SetVolume (vol);}
	HRESULT SetBalance (const int which, const long bal) const
							{return channels[which].SetBalance (bal);}

	FILTER_STATE GetState (const int which) const
									{return channels[which].GetState ();}
	long GetVolume (const int which) const
				{long vol; channels[which].GetVolume (&vol); return vol;}
	long GetBalance (const int which) const
				{long bal; channels[which].GetBalance (&bal); return bal;}

	bool IsInitialized (void)  {return init_flag;}
	CDM6Mp3 *GetChannel (const int which)  {return channels+which;}
	//------------------------------------------------------------
};

#undef FC
#endif

#endif // !defined(AFX_DM6MP3_H__7BA924E1_EA55_11D5_944A_0050DA898DDD__INCLUDED_)
