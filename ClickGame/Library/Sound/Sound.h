#ifndef DIRECTSOUND_H_
#define DIRECTSOUND_H_


#include <string>
#include <unordered_map>
#include <string>
#include <dsound.h>

class DirectSound {

public:


	DirectSound(HWND hwnd_ = nullptr);

	/*
	　
	 wavファイルの書き出しを行い、データを返す

	使用方法：
		DirectSound("hoge.wave");
	*/
	IDirectSoundBuffer8* LoadWaveFile(std::string file_name_);

	~DirectSound();

private:

	IDirectSound8* d_sound8_;			//サウンドインターフェース
	IDirectSoundBuffer* primary_buffer_;//プライマリーバッファ―
	IDirectSoundBuffer8* sec_buffer_;
};





//============================
// オーディオプレイヤークラス
//============================

class AudioPlayer {

public:

	static AudioPlayer* GetInstance(HWND hwnd_ = nullptr) {
		static AudioPlayer instance(hwnd_);
		return &instance;
	}

	/*
		ウェブファイル読み込み
	引数：
		alias サウンドを使用する際の別名
		file_name サウンドファイル名
	概要：
		指定されたサウンドファイルを読み込む
	*/
	bool Load(std::string alias_, std::string file_name_);

	/*
		再生処理
	引数：
		volumr  音量（０で最大　-10000で無音）
		is_loop 再生方法（true=ループ再生）

	概要：
	　　指定された音量で、第２引数でしていされた
	  　再生方法により再生する
	*/
	void Play(
		std::string alias_,
		int volume_ = -1000,
		bool is_loop_ = false);

	// 停止
	void Stop(std::string alias_);

	/*
		ボリューム設定
	引数：
		volume 音量（0で最大 -10000で無音）
	*/
	void SetVolume(std::string alias_, int volume_);

	//解放処理
	void Release(std::string alias_);

private:

	//コンストラクタ
	AudioPlayer(HWND hwnd_) {
		//再生先のウィンドウハンドルを取得
		sound_ = new DirectSound(hwnd_);
		ZeroMemory(&sound_data_, 0, sizeof(IDirectSoundBuffer8));
	}

	/*
		サウンドファイル検索
	引数：
		alias 読み込み時に設定したサウンドファイルの別名
	戻り値：
		成功　→　サウンドデータ
		失敗　→　null
	*/
	IDirectSoundBuffer8* Find(std::string alias_);

	~AudioPlayer();

private:


	DirectSound* sound_;
	std::unordered_map<std::string, IDirectSoundBuffer8*> sound_data_; //セカンダリバッファ
	WAVEFORMATEX format_;			  //データフォーマット構造体
	DWORD size_;					  //waveファイルのサイズ


};




#endif
