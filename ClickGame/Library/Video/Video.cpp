#include"Video.h"

Video::Video()
{
	InitCom();
	MediaControl();
	MediaEvent();
}

HRESULT Video::InitCom()
{
	h_res_ = CoInitialize(NULL);
	if (FAILED(h_res_))
	{
		return false;
	}
	h_res_ = CoCreateInstance(
		CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IGraphBuilder,
		(void**)&graph_
	);

	if (FAILED(h_res_))
		return h_res_;
}

HRESULT Video::MediaControl()
{
	h_res_ = graph_->QueryInterface(IID_IMediaControl, (void**)&control_);
	if (FAILED(h_res_))
		return h_res_;
}

HRESULT Video::MediaEvent()
{
	h_res_ = graph_->QueryInterface(IID_IMediaEvent, (void**)&event_);
	if (FAILED(h_res_))
		return h_res_;
}

HRESULT Video::Load(std::string filename_)
{
	h_res_ = graph_->RenderFile(L"Res/Movie/【PS3】-El-Shaddai-エルシャダイ-　_ルシフェルver_-高画質版.avi", NULL);
	if (FAILED(h_res_))
		return h_res_;
}

void Video::Play(int time_)
{
	if (SUCCEEDED(h_res_))
	{
		h_res_ = control_->Run();
		if (SUCCEEDED(h_res_))
		{
			long pEvCode;
			h_res_ = event_->WaitForCompletion(INFINITE, &pEvCode);
		}
	}

	if (control_ != NULL)
	{
		control_->Stop();

		control_->Release();
	}
	if (event_ != NULL)
	{
		event_->Release();
	}
	if (graph_ != NULL)
	{
		graph_->Release();
	}
	

	CoUninitialize();
}

