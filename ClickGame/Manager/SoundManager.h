#ifndef SOUNDMANAGER_H_
#define SOUNDMANAGER_H_

#include"../Library/Sound/Sound.h"
#include"../Library/Engine/Engine.h"
#include"LibraryManager.h"
#include<map>
#include<string>



class SoundManager
{
public:
	static SoundManager* Instance();

	void RegisterSound();

	void SoundBGM(std::string key_,int vol_ = -1000);
	void SoundSE(std::string key_, int vol_ = -1000);

	void ReleaseSound();
	

private:
	SoundManager();

private:
	static SoundManager* p_instance;

	std::string m_bgm;
	std::string m_bgm_file;

	std::map<std::string, std::string> sound_file;

	AudioPlayer* pAudio;

};


#endif