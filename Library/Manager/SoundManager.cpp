#include"../Definition/Definition.h"
#include"SoundManager.h"

SoundManager* SoundManager::p_instance = 0;

SoundManager* SoundManager::Instance()
{
	if (p_instance == 0)
	{
		p_instance = new SoundManager;
	}

	return p_instance;
}

void SoundManager::RegisterSound()
{
	pAudio->Load("BGM",sound_file["BGM"]);
	
}

void SoundManager::SoundBGM(std::string key_,int vol_)
{
	pAudio->Play(key_,vol_,true);
}

void SoundManager::SoundSE(std::string key_, int vol_)
{
	pAudio->Play(key_, vol_);
}

void SoundManager::ReleaseSound()
{
	for (auto v : sound_file)
	{
		pAudio->Release(v.second);
	}
}

SoundManager::SoundManager()
{
	pAudio = AudioPlayer::GetInstance(FindWindow(WINDOW_CLASS_NAME,nullptr));
	
	
	
	sound_file["BGM"] = "Res/Sound/bgm055_loop.wav";

}
