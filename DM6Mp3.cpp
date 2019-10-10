// DM6Mp3.cpp: implementation of the CDM6Mp3 class.
//
//////////////////////////////////////////////////////////////////////

#include <malloc.h>
#include <sys/stat.h>
#include "DM6Mp3.h"

//******************************************************************************
//  Methods of CDM6Mp3 class
//******************************************************************************
//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 필요한 인터페이스들을 COM 객체로부터 얻어온다
//------------------------------------------------------------------------------
#define SAFE_RELEASE(p) { if (p) { (p)->Release (); (p) = NULL; } }

CDM6Mp3::CDM6Mp3()
{
	// COM 객체를 초기화시킨다
	CoInitialize (NULL);

	// 그래프 필터 객체를 얻는다
	if (CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
				IID_IGraphBuilder, reinterpret_cast <void **> (&graph_builder)) 
				!= S_OK)
	{
		init_flag = false;
		return;
	}

	// 컨트롤 인터페이스를 얻어온다
	if (graph_builder->QueryInterface (IID_IMediaControl, 
				reinterpret_cast <void **> (&media_control)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// 탐색 인터페이스를 얻어온다
	if (graph_builder->QueryInterface (IID_IMediaSeeking,
				reinterpret_cast <void **> (&media_seeking)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// 기본 오디오 인터페이스를 얻어온다
	if (graph_builder->QueryInterface (IID_IBasicAudio,
				reinterpret_cast <void **> (&basic_audio)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// 미디어 이벤트 인터페이스를 얻어온다
	if (graph_builder->QueryInterface (IID_IMediaEvent,
				reinterpret_cast <void **> (&media_event)) != S_OK)
	{
		init_flag = false;
		return;
	}
	
	init_flag = true;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 인터페이스들을 릴리즈 한다
//------------------------------------------------------------------------------
CDM6Mp3::~CDM6Mp3()
{
	// 모든 필터들을 delete한다
	CleanGraph ();

	// 인터페이스들을 릴리즈 한다
	SAFE_RELEASE (basic_audio);
	SAFE_RELEASE (media_seeking);
	SAFE_RELEASE (media_control);
	SAFE_RELEASE (graph_builder);

	// COM을 uninitialize한다
	CoUninitialize ();
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 성공여부
//  설  명 : 그래프에 있는 모든 필터들을 제거한다
//------------------------------------------------------------------------------
bool CDM6Mp3 :: CleanGraph (void)
{
	IEnumFilters *pFilterEnum;
	IBaseFilter  **ppFilters;
	
	if (!init_flag)
		return false;

	// 연주를 멈춘다
	if (media_control)
		media_control->Stop ();
	
	// 그래프 내의 모든 필터를 얻기 위해 EnumFilters 포인터를 얻는다
	if (graph_builder->EnumFilters (&pFilterEnum) != S_OK)
		return false;

	int iFiltCount = 0;
	int iPos = 0;

	// 필터의 갯수를 파악
	while (S_OK == pFilterEnum->Skip (1))
		iFiltCount++;

	ppFilters = reinterpret_cast <IBaseFilter **> 
			(_alloca (sizeof (IBaseFilter *) * iFiltCount));
	
	pFilterEnum->Reset ();

	// 필터에 대한 포인터를 모두 얻어온다
	while (S_OK == pFilterEnum->Next (1, &(ppFilters[iPos++]), NULL));
	SAFE_RELEASE (pFilterEnum);

	// 모든 필터를 그래프에서 제거한다
	for (iPos = 0; iPos < iFiltCount; iPos++)
	{
		graph_builder->RemoveFilter (ppFilters[iPos]);
		while (ppFilters[iPos]->Release () != 0);	// ref count 수 만큼 한다
	}

	return true;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 이 객체가 초기화가 제대로 되어 있지 않다면, 초기화를 다시 수행한다
//------------------------------------------------------------------------------
void CDM6Mp3 :: InitClass (void)
{
	// 모든 필터들을 delete한다
	CleanGraph ();

	// 인터페이스들을 릴리즈 한다
	SAFE_RELEASE (basic_audio);
	SAFE_RELEASE (media_seeking);
	SAFE_RELEASE (media_control);
	SAFE_RELEASE (graph_builder);

	// COM을 uninitialize한다
	CoUninitialize ();

	CDM6Mp3 ();
}


//------------------------------------------------------------------------------
//  인  자 : 필터 포인터
//  리턴값 : 이 필터를 가지고 있는지의 여부
//  설  명 : 주어진 필터가 현재 그래프에 존재하는지의 여부를 알아본다
//------------------------------------------------------------------------------
bool CDM6Mp3 :: HasFilter (IBaseFilter *filter)
{
	bool found = false;

	IEnumFilters *pFilterEnum;
	IBaseFilter  **ppFilters;
	
	if (!init_flag)
		return false;

	// 그래프 내의 모든 필터를 얻기 위해 EnumFilters 포인터를 얻는다
	if (graph_builder->EnumFilters (&pFilterEnum) != S_OK)
		return false;

	int iFiltCount = 0;
	int iPos = 0;

	// 필터의 갯수를 파악
	while (S_OK == pFilterEnum->Skip (1))
		iFiltCount++;

	ppFilters = reinterpret_cast <IBaseFilter **> 
			(_alloca (sizeof (IBaseFilter *) * iFiltCount));
	
	pFilterEnum->Reset ();

	// 필터에 대한 포인터를 모두 얻어온다
	while (S_OK == pFilterEnum->Next (1, &(ppFilters[iPos++]), NULL));
	SAFE_RELEASE (pFilterEnum);

	// 모든 필터를 검색한다
	for (iPos = 0; iPos < iFiltCount; iPos++)
	{
		if (filter == ppFilters[iPos])
			found = true;
		while (ppFilters[iPos]->Release () != 0);	// ref count 수 만큼 한다
	}

	return found;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 현재는 EC_COMPLETE가 BGM용 채널에만 전해지므로(정확하게는 호출되므
//           로), 분간없이 무조건 BGM 채널이라는 전제 아래에서 처리한다
//------------------------------------------------------------------------------
void CDM6Mp3 :: OnEvent (void)
{
	long lEventCode, lParam1, lParam2; 

	// TimeOut 을 10ms로 해두었다. 즉, 호출마다 0ms 씩 기다린다. 실제 게임 루틴
	// 에서는 이에 변화가 필요하며, 이에 대한 관련 메소드들이 필요할 것으로 보
	// 인다.
	if (media_event->GetEvent (&lEventCode, &lParam1, &lParam2, 0) == S_OK) 
	{ 
		// 끝까지 도달한 경우, BGM은 순환 연주한다
		if (lEventCode == EC_COMPLETE) 
		{ 
			Stop ();
			SetPosition (0);
			Run ();
		} 
	}
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : FILTER_STATE (enum 형임)
//  설  명 : 현재 채널의 상태를 알려준다
//------------------------------------------------------------------------------
FILTER_STATE CDM6Mp3 :: GetState (void) const
{
	FILTER_STATE state;

	media_control->GetState (0, reinterpret_cast <long *> (&state));
	return state;
}


//******************************************************************************
//  Methods of CMp3Manager class
//******************************************************************************
//------------------------------------------------------------------------------
//  인  자 : 최대 채널 갯수
//  리턴값 : 없음
//  설  명 : 정해진 수만큼의 채널 객체를 생성한다
//------------------------------------------------------------------------------
CMp3Manager :: CMp3Manager (int channel_num)
{
	if ((channels = new CDM6Mp3 [channel_num]) == NULL || channel_num < MIN_CHANNELS)
	{
		init_flag = false;
		return;
	}

	this->channel_num = channel_num;
	cur_channel = 1;		// 0번 채널은 배경 음악 전용으로 사용하기 때문에, 일
							// 반 출력을 위해 이 변수 값은 1로 셋팅한다
	init_flag = true;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 할당 자원을 모두 해제한다
//------------------------------------------------------------------------------
CMp3Manager :: ~CMp3Manager ()
{
	// 반드시 채널부터 해제해야 한다
	delete [] channels;	
}


//------------------------------------------------------------------------------
//  인  자 : 필요한 채널의 갯수
//  리턴값 : 없음
//  설  명 : 이 객체가 초기화가 제대로 되어 있지 않다면, 초기화를 다시 수행한다
//------------------------------------------------------------------------------
void CMp3Manager :: InitClass (int channel_num)
{	
	if (channels != NULL)
	{
		delete [] channels;
		channels = NULL;
	}

	// 왜 그런지 몰라도 이렇게 명시를 해주어야 한다
	CMp3Manager :: CMp3Manager (channel_num);		
}


//------------------------------------------------------------------------------
//  인  자 : 연주할 파일의 패스, BGM 연주인가?
//  리턴값 : 사용 채널
//  설  명 : 주어진 인덱스에 해당하는 필터를 연주한다 (BGM 이라면, 1번 채널만을,
//           그렇지 않다면, cur_channel에 해당하는 채널을 사용한다
//------------------------------------------------------------------------------
int CMp3Manager :: PlaySound (const char *path, bool BGM_flag)
{
	if (!init_flag)
		return ISERROR;

	int channel;
	IPin *pPin;
	IGraphBuilder *graph_builder;
	IBaseFilter   *temp_filter;
	WCHAR wFileName[MAX_PATH];
	struct _stat temp;


	// 배경음악인지, 일반 효과음인지 구분 (채널이 최소치라면,효과음 연주 불가능)
	if (BGM_flag == true)
		channel = 0;
	else if (channel_num < 2)
		return ISERROR;
	else
		channel = cur_channel;

	// 그래프 빌더에 대한 포인터를 얻어온다
	if ((graph_builder = channels[channel].GetGraphBuilder ()) == NULL)
		return ISERROR;

	// 파일의 존재 여부를 검사
	if (_stat (path, &temp) != 0)
		return ISERROR;

	#ifndef UNICODE
		MultiByteToWideChar (CP_ACP, 0, path, -1, wFileName, MAX_PATH);
	#else
		lstrcpy (wFileName, path);
	#endif

	// 채널을 정지시킨다
	if (channels[channel].Stop () != S_OK)
		return ISERROR;

	// 그래프 내의 다른 쓰레기들을(다른 필터) 지운다 -_-;;
	if (channels[channel].CleanGraph () == false)
		return ISERROR;

	// 필터를 그래프에 포함함으로써 로드한다
	if (graph_builder->AddSourceFilter (wFileName, wFileName, 
			&temp_filter) != S_OK)
		return ISERROR;

	// 사용할 사운드의 출력 핀을 얻어온다
	if (temp_filter->FindPin (L"Output", &pPin) != S_OK)
		return ISERROR;

	// 렌더링을 수행
	if (graph_builder->Render (pPin) != S_OK)
	{
		SAFE_RELEASE (pPin);	// 포함된 필터는 다음 출력 때 자동으로 제거된다
		return ISERROR;
	}
	
	SAFE_RELEASE (pPin);
	SAFE_RELEASE (temp_filter);

	// 위치 초기화 및 연주 시작
	channels[channel].SetPosition (0);
	channels[channel].Run ();

	if (BGM_flag == true)
		return 0;

	int play_channel = cur_channel;

	if (cur_channel+1 == channel_num)
		cur_channel = 1;
	else
		++cur_channel;

	return play_channel;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : 없음
//  설  명 : 그래프에 일어나는 각종 이벤트를 처리한다. 현재는 EC_COMPLETE 스트림 
//           연주 종료 메시지만을 처리한다. 종료된 필터가 배경음악 용이라면, 처
//           음부터 다시 연주한다 (LOOP)
//------------------------------------------------------------------------------
void CMp3Manager :: OnEvent (void)
{
	// 현재는 처리 이벤트 종료가 하나라서 이렇게 짧다. 나중을 위해 인라인화하지
	// 않았다.
	channels[0].OnEvent ();
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 모든 채널을 연주한다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: RunAll (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 0; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Run ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 모든 채널을 멈춘다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: StopAll (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 0; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Stop ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 모든 채널을 잠시 멈춘다. Stop과의 차이는 다시 Run을 했을 때 재생 위
//           치가 달라진다는 점이 있다.(필터의 Stop 기능은 아예 Run을 했을 때 다
//           시 재생되지도 않는다. 즉, 필터의 핀을 다시 연결해 주어야 한다.)
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: PauseAll (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 0; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Pause ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 배경 음악용 채널을 제외한 채널을 모두 연주 시킨다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: RunSounds (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 1; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Run ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 모든 채널을 멈춘다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: StopSounds (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 1; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Stop ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 없음
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 모든 채널을 잠시 멈춘다. 
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: PauseSounds (void)
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 1; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].Pause ()) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 볼륨 값 (-10000 ~ 0)
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 전체 채널에 대해 같은 볼륨값을 적용한다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: SetEntVolume (const long vol) const
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 0; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].SetVolume (vol)) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//  인  자 : 볼륨 값 (-10000[왼쪽] ~ 10000[오른쪽])
//  리턴값 : HRESULT(에러 추출을 최대한 가능하게)
//  설  명 : 전체 채널에 대해 같은 밸런스값(좌우 음향 편중?)을 적용한다
//------------------------------------------------------------------------------
HRESULT CMp3Manager :: SetEntBalance (const long bal) const
{
	HRESULT hr = S_OK, temp_hr;

	for (int i = 0; i < channel_num; i++)
	{
		if ((temp_hr = channels[i].SetBalance (bal)) != S_OK)
			hr = temp_hr;
	}
	return hr;
}


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------