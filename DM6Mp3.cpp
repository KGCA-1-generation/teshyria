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
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : �ʿ��� �������̽����� COM ��ü�κ��� ���´�
//------------------------------------------------------------------------------
#define SAFE_RELEASE(p) { if (p) { (p)->Release (); (p) = NULL; } }

CDM6Mp3::CDM6Mp3()
{
	// COM ��ü�� �ʱ�ȭ��Ų��
	CoInitialize (NULL);

	// �׷��� ���� ��ü�� ��´�
	if (CoCreateInstance (CLSID_FilterGraph, NULL, CLSCTX_INPROC,
				IID_IGraphBuilder, reinterpret_cast <void **> (&graph_builder)) 
				!= S_OK)
	{
		init_flag = false;
		return;
	}

	// ��Ʈ�� �������̽��� ���´�
	if (graph_builder->QueryInterface (IID_IMediaControl, 
				reinterpret_cast <void **> (&media_control)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// Ž�� �������̽��� ���´�
	if (graph_builder->QueryInterface (IID_IMediaSeeking,
				reinterpret_cast <void **> (&media_seeking)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// �⺻ ����� �������̽��� ���´�
	if (graph_builder->QueryInterface (IID_IBasicAudio,
				reinterpret_cast <void **> (&basic_audio)) != S_OK)
	{
		init_flag = false;
		return;
	}

	// �̵�� �̺�Ʈ �������̽��� ���´�
	if (graph_builder->QueryInterface (IID_IMediaEvent,
				reinterpret_cast <void **> (&media_event)) != S_OK)
	{
		init_flag = false;
		return;
	}
	
	init_flag = true;
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : �������̽����� ������ �Ѵ�
//------------------------------------------------------------------------------
CDM6Mp3::~CDM6Mp3()
{
	// ��� ���͵��� delete�Ѵ�
	CleanGraph ();

	// �������̽����� ������ �Ѵ�
	SAFE_RELEASE (basic_audio);
	SAFE_RELEASE (media_seeking);
	SAFE_RELEASE (media_control);
	SAFE_RELEASE (graph_builder);

	// COM�� uninitialize�Ѵ�
	CoUninitialize ();
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : ��������
//  ��  �� : �׷����� �ִ� ��� ���͵��� �����Ѵ�
//------------------------------------------------------------------------------
bool CDM6Mp3 :: CleanGraph (void)
{
	IEnumFilters *pFilterEnum;
	IBaseFilter  **ppFilters;
	
	if (!init_flag)
		return false;

	// ���ָ� �����
	if (media_control)
		media_control->Stop ();
	
	// �׷��� ���� ��� ���͸� ��� ���� EnumFilters �����͸� ��´�
	if (graph_builder->EnumFilters (&pFilterEnum) != S_OK)
		return false;

	int iFiltCount = 0;
	int iPos = 0;

	// ������ ������ �ľ�
	while (S_OK == pFilterEnum->Skip (1))
		iFiltCount++;

	ppFilters = reinterpret_cast <IBaseFilter **> 
			(_alloca (sizeof (IBaseFilter *) * iFiltCount));
	
	pFilterEnum->Reset ();

	// ���Ϳ� ���� �����͸� ��� ���´�
	while (S_OK == pFilterEnum->Next (1, &(ppFilters[iPos++]), NULL));
	SAFE_RELEASE (pFilterEnum);

	// ��� ���͸� �׷������� �����Ѵ�
	for (iPos = 0; iPos < iFiltCount; iPos++)
	{
		graph_builder->RemoveFilter (ppFilters[iPos]);
		while (ppFilters[iPos]->Release () != 0);	// ref count �� ��ŭ �Ѵ�
	}

	return true;
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : �� ��ü�� �ʱ�ȭ�� ����� �Ǿ� ���� �ʴٸ�, �ʱ�ȭ�� �ٽ� �����Ѵ�
//------------------------------------------------------------------------------
void CDM6Mp3 :: InitClass (void)
{
	// ��� ���͵��� delete�Ѵ�
	CleanGraph ();

	// �������̽����� ������ �Ѵ�
	SAFE_RELEASE (basic_audio);
	SAFE_RELEASE (media_seeking);
	SAFE_RELEASE (media_control);
	SAFE_RELEASE (graph_builder);

	// COM�� uninitialize�Ѵ�
	CoUninitialize ();

	CDM6Mp3 ();
}


//------------------------------------------------------------------------------
//  ��  �� : ���� ������
//  ���ϰ� : �� ���͸� ������ �ִ����� ����
//  ��  �� : �־��� ���Ͱ� ���� �׷����� �����ϴ����� ���θ� �˾ƺ���
//------------------------------------------------------------------------------
bool CDM6Mp3 :: HasFilter (IBaseFilter *filter)
{
	bool found = false;

	IEnumFilters *pFilterEnum;
	IBaseFilter  **ppFilters;
	
	if (!init_flag)
		return false;

	// �׷��� ���� ��� ���͸� ��� ���� EnumFilters �����͸� ��´�
	if (graph_builder->EnumFilters (&pFilterEnum) != S_OK)
		return false;

	int iFiltCount = 0;
	int iPos = 0;

	// ������ ������ �ľ�
	while (S_OK == pFilterEnum->Skip (1))
		iFiltCount++;

	ppFilters = reinterpret_cast <IBaseFilter **> 
			(_alloca (sizeof (IBaseFilter *) * iFiltCount));
	
	pFilterEnum->Reset ();

	// ���Ϳ� ���� �����͸� ��� ���´�
	while (S_OK == pFilterEnum->Next (1, &(ppFilters[iPos++]), NULL));
	SAFE_RELEASE (pFilterEnum);

	// ��� ���͸� �˻��Ѵ�
	for (iPos = 0; iPos < iFiltCount; iPos++)
	{
		if (filter == ppFilters[iPos])
			found = true;
		while (ppFilters[iPos]->Release () != 0);	// ref count �� ��ŭ �Ѵ�
	}

	return found;
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : ����� EC_COMPLETE�� BGM�� ä�ο��� �������Ƿ�(��Ȯ�ϰԴ� ȣ��ǹ�
//           ��), �а����� ������ BGM ä���̶�� ���� �Ʒ����� ó���Ѵ�
//------------------------------------------------------------------------------
void CDM6Mp3 :: OnEvent (void)
{
	long lEventCode, lParam1, lParam2; 

	// TimeOut �� 10ms�� �صξ���. ��, ȣ�⸶�� 0ms �� ��ٸ���. ���� ���� ��ƾ
	// ������ �̿� ��ȭ�� �ʿ��ϸ�, �̿� ���� ���� �޼ҵ���� �ʿ��� ������ ��
	// �δ�.
	if (media_event->GetEvent (&lEventCode, &lParam1, &lParam2, 0) == S_OK) 
	{ 
		// ������ ������ ���, BGM�� ��ȯ �����Ѵ�
		if (lEventCode == EC_COMPLETE) 
		{ 
			Stop ();
			SetPosition (0);
			Run ();
		} 
	}
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : FILTER_STATE (enum ����)
//  ��  �� : ���� ä���� ���¸� �˷��ش�
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
//  ��  �� : �ִ� ä�� ����
//  ���ϰ� : ����
//  ��  �� : ������ ����ŭ�� ä�� ��ü�� �����Ѵ�
//------------------------------------------------------------------------------
CMp3Manager :: CMp3Manager (int channel_num)
{
	if ((channels = new CDM6Mp3 [channel_num]) == NULL || channel_num < MIN_CHANNELS)
	{
		init_flag = false;
		return;
	}

	this->channel_num = channel_num;
	cur_channel = 1;		// 0�� ä���� ��� ���� �������� ����ϱ� ������, ��
							// �� ����� ���� �� ���� ���� 1�� �����Ѵ�
	init_flag = true;
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : �Ҵ� �ڿ��� ��� �����Ѵ�
//------------------------------------------------------------------------------
CMp3Manager :: ~CMp3Manager ()
{
	// �ݵ�� ä�κ��� �����ؾ� �Ѵ�
	delete [] channels;	
}


//------------------------------------------------------------------------------
//  ��  �� : �ʿ��� ä���� ����
//  ���ϰ� : ����
//  ��  �� : �� ��ü�� �ʱ�ȭ�� ����� �Ǿ� ���� �ʴٸ�, �ʱ�ȭ�� �ٽ� �����Ѵ�
//------------------------------------------------------------------------------
void CMp3Manager :: InitClass (int channel_num)
{	
	if (channels != NULL)
	{
		delete [] channels;
		channels = NULL;
	}

	// �� �׷��� ���� �̷��� ��ø� ���־�� �Ѵ�
	CMp3Manager :: CMp3Manager (channel_num);		
}


//------------------------------------------------------------------------------
//  ��  �� : ������ ������ �н�, BGM �����ΰ�?
//  ���ϰ� : ��� ä��
//  ��  �� : �־��� �ε����� �ش��ϴ� ���͸� �����Ѵ� (BGM �̶��, 1�� ä�θ���,
//           �׷��� �ʴٸ�, cur_channel�� �ش��ϴ� ä���� ����Ѵ�
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


	// �����������, �Ϲ� ȿ�������� ���� (ä���� �ּ�ġ���,ȿ���� ���� �Ұ���)
	if (BGM_flag == true)
		channel = 0;
	else if (channel_num < 2)
		return ISERROR;
	else
		channel = cur_channel;

	// �׷��� ������ ���� �����͸� ���´�
	if ((graph_builder = channels[channel].GetGraphBuilder ()) == NULL)
		return ISERROR;

	// ������ ���� ���θ� �˻�
	if (_stat (path, &temp) != 0)
		return ISERROR;

	#ifndef UNICODE
		MultiByteToWideChar (CP_ACP, 0, path, -1, wFileName, MAX_PATH);
	#else
		lstrcpy (wFileName, path);
	#endif

	// ä���� ������Ų��
	if (channels[channel].Stop () != S_OK)
		return ISERROR;

	// �׷��� ���� �ٸ� ���������(�ٸ� ����) ����� -_-;;
	if (channels[channel].CleanGraph () == false)
		return ISERROR;

	// ���͸� �׷����� ���������ν� �ε��Ѵ�
	if (graph_builder->AddSourceFilter (wFileName, wFileName, 
			&temp_filter) != S_OK)
		return ISERROR;

	// ����� ������ ��� ���� ���´�
	if (temp_filter->FindPin (L"Output", &pPin) != S_OK)
		return ISERROR;

	// �������� ����
	if (graph_builder->Render (pPin) != S_OK)
	{
		SAFE_RELEASE (pPin);	// ���Ե� ���ʹ� ���� ��� �� �ڵ����� ���ŵȴ�
		return ISERROR;
	}
	
	SAFE_RELEASE (pPin);
	SAFE_RELEASE (temp_filter);

	// ��ġ �ʱ�ȭ �� ���� ����
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
//  ��  �� : ����
//  ���ϰ� : ����
//  ��  �� : �׷����� �Ͼ�� ���� �̺�Ʈ�� ó���Ѵ�. ����� EC_COMPLETE ��Ʈ�� 
//           ���� ���� �޽������� ó���Ѵ�. ����� ���Ͱ� ������� ���̶��, ó
//           ������ �ٽ� �����Ѵ� (LOOP)
//------------------------------------------------------------------------------
void CMp3Manager :: OnEvent (void)
{
	// ����� ó�� �̺�Ʈ ���ᰡ �ϳ��� �̷��� ª��. ������ ���� �ζ���ȭ����
	// �ʾҴ�.
	channels[0].OnEvent ();
}


//------------------------------------------------------------------------------
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ä���� �����Ѵ�
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
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ä���� �����
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
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ä���� ��� �����. Stop���� ���̴� �ٽ� Run�� ���� �� ��� ��
//           ġ�� �޶����ٴ� ���� �ִ�.(������ Stop ����� �ƿ� Run�� ���� �� ��
//           �� ��������� �ʴ´�. ��, ������ ���� �ٽ� ������ �־�� �Ѵ�.)
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
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ���ǿ� ä���� ������ ä���� ��� ���� ��Ų��
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
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ä���� �����
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
//  ��  �� : ����
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��� ä���� ��� �����. 
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
//  ��  �� : ���� �� (-10000 ~ 0)
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��ü ä�ο� ���� ���� �������� �����Ѵ�
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
//  ��  �� : ���� �� (-10000[����] ~ 10000[������])
//  ���ϰ� : HRESULT(���� ������ �ִ��� �����ϰ�)
//  ��  �� : ��ü ä�ο� ���� ���� �뷱����(�¿� ���� ����?)�� �����Ѵ�
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