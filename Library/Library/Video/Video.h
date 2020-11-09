#ifndef VIDEO_H_
#define VIDEO_H_

#include<Windows.h>
#include<dshow.h>
#include<string>

class Video
{
public:
	Video();
	HRESULT InitCom();
	HRESULT MediaControl();
	HRESULT MediaEvent();
	HRESULT Load(std::string filename_);
	void Play(int time_);

private:
	HRESULT h_res_;
	IGraphBuilder* graph_;
	IMediaControl* control_;
	IMediaEvent* event_;
};

#endif // !VIDEO_H_

