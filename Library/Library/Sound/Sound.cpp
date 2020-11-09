#include "Sound.h"

#pragma comment(lib,"dxguid.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")

//===============================================
	//	オーディオリソース管理
	//===============================================

	//--------------------------------------------
	 //　初回呼び出し用コンストラクタ
DirectSound::DirectSound(HWND hwnd_) {


	DirectSoundCreate8(nullptr, &d_sound8_, nullptr);
	d_sound8_->SetCooperativeLevel(hwnd_, DSSCL_NORMAL);

	DSBUFFERDESC desc = {};
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRLVOLUME;
	desc.dwBufferBytes = 0;
	desc.dwReserved = 0;
	desc.lpwfxFormat = nullptr;
	desc.guid3DAlgorithm = GUID_NULL;

	d_sound8_->CreateSoundBuffer(&desc, &primary_buffer_, nullptr);
}
//-------------------------------------
//　データ読み込み
IDirectSoundBuffer8* DirectSound::LoadWaveFile(std::string file_name_) {


	HMMIO hmmio = nullptr;
	MMIOINFO mminfo = {};
	hmmio = mmioOpen((LPSTR)file_name_.c_str(), &mminfo, MMIO_READ);

	//ファイルのオープンに失敗した場合
	if (!hmmio) {
		MessageBox(0, "waveファイルのオープンに失敗", 0, MB_OK);
		return nullptr;
	}

	MMRESULT result;
	MMCKINFO rif_chunk;
	rif_chunk.fccType = mmioFOURCC('W', 'A', 'V', 'E');
	result = mmioDescend(hmmio, &rif_chunk, NULL, MMIO_FINDRIFF);

	//　リフチャンクの検索に失敗した場合
	if (result != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return nullptr;
	}

	MMCKINFO fmt_chunk;
	fmt_chunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
	result = mmioDescend(hmmio, &fmt_chunk, &rif_chunk, MMIO_FINDCHUNK);

	// フォーマットチャンクの検索に失敗した場合
	if (result != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return nullptr;
	}

	//音質やチャンネル数の情報を格納する
	WAVEFORMATEX format;
	DWORD fmsize = fmt_chunk.cksize;
	if (mmioRead(hmmio, (HPSTR)& format, fmsize) != fmsize) {
		mmioClose(hmmio, 0);
		return nullptr;
	}

	//データ検索の為一度戻る
	mmioAscend(hmmio, &fmt_chunk, 0);

	MMCKINFO data_chunk;
	data_chunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
	result = mmioDescend(hmmio, &data_chunk, &rif_chunk, MMIO_FINDCHUNK);
	// データチャンクの検索に失敗した場合
	if (result != MMSYSERR_NOERROR) {
		mmioClose(hmmio, 0);
		return nullptr;
	}

	// 波形データ
	DWORD data_size = data_chunk.cksize;
	BYTE* wav_data = new BYTE[data_chunk.cksize];
	if (mmioRead(hmmio, (HPSTR)wav_data, data_chunk.cksize) != data_chunk.cksize) {
		delete[] wav_data;
		return nullptr;
	}


	//正常読み込み
	mmioClose(hmmio, 0);

	// セカンダリバッファの設定
	DSBUFFERDESC desc = {};
	desc.dwSize = sizeof(DSBUFFERDESC);
	desc.dwFlags = DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS |
		DSBCAPS_CTRLPAN | DSBCAPS_CTRLFREQUENCY;
	desc.dwBufferBytes = data_size;   //メモリサイズ
	desc.dwReserved = 0;			  //予約域
	desc.lpwfxFormat = &format;     //WAVEフォーマット
	desc.guid3DAlgorithm = GUID_NULL; //3Dエフェクトを使用しない

	IDirectSoundBuffer* buf = 0;
	d_sound8_->CreateSoundBuffer(&desc, &buf, nullptr);

	//戻り値がIDirectSoundBufferの為、IDirectSoundBuffer8に戻す
	buf->QueryInterface(IID_IDirectSoundBuffer8, (void**)& sec_buffer_);
	buf->Release();


	// WAVデータ書き込み
	LPVOID write_data;
	DWORD len;
	sec_buffer_->Lock(
		0,		     	//ロック開始位置
		0,			    //メモリサイズ
		&write_data,	//ロック位置のポインタ
		&len,		    //大きさ
		NULL,	  	    //範囲を超えた場合のメモリ領域指定
		NULL,
		DSBLOCK_ENTIREBUFFER //指定の大きさだけロックする
	);

	memcpy(write_data, wav_data, len);
	sec_buffer_->Unlock(write_data, len, nullptr, 0);
	//　元音源を削除
	delete[] wav_data;

	if (sec_buffer_ == nullptr) {
		MessageBox(0, "waveファイルの読み込みに失敗", 0, MB_OK);
	}

	return sec_buffer_;
}

//-----------------------------------
//　デストラクタ
DirectSound::~DirectSound() {

	d_sound8_->Release();
	//primary_buffer_->Release();
}



//==========================================
//  サウンドプレーヤー
//==========================================

bool AudioPlayer::Load(std::string alias_, std::string file_name_) {

	//まだ読み込んでいないサウンドデータである
	/*
		std::unordered_map.count(value）
		 valueをキーとした要素の検索を行う。
		 要素あり→１　要素なし→０
	*/
	if (sound_data_.count(alias_) == 0) {
		sound_data_.emplace(alias_, sound_->LoadWaveFile(file_name_));
		return true;
	}
	return false;
}

//------------------------------------
// 再生
void AudioPlayer::Play(
	std::string alias_,
	int volume_,
	bool is_loop_
) {

	IDirectSoundBuffer8* sound = Find(alias_);
	if (sound == nullptr) {
		return;//サウンドデータがなかった
	}

	if (is_loop_) {
		sound->SetVolume(volume_);
		sound->Play(0, 0, DSBPLAY_LOOPING);
	}
	else {
		sound->SetVolume(volume_);
		sound->Play(0, 0, 0);
		sound->SetCurrentPosition(0);
	}
}

//----------------------------------
// 停止
void AudioPlayer::Stop(std::string alias_) {

	IDirectSoundBuffer8* sound = Find(alias_);
	if (sound == nullptr) {
		return;//サウンドデータがなかった
	}
	sound->Stop();
}

//----------------------------------
// ボリューム調整
void AudioPlayer::SetVolume(std::string alias_, int volume_) {

	IDirectSoundBuffer8* sound = Find(alias_);
	if (sound == nullptr) {
		return;//サウンドデータがなかった
	}
	sound->SetVolume(volume_);
}

//-------------------------------
//解放処理
void AudioPlayer::Release(std::string alias_) {

	IDirectSoundBuffer8* sound = Find(alias_);
	if (sound == nullptr) {
		return;//サウンドデータがなかった
	}

	if (sound) {
		sound->Release();
		sound = nullptr;
		sound_data_.erase(alias_);//要素削除
	}
}

IDirectSoundBuffer8* AudioPlayer::Find(std::string alias_) {

	//サウンドデータをキーで検索
	auto data = sound_data_.find(alias_);
	//サウンドデータがみつかった場合
	if (data != sound_data_.end()) {
		return data->second;
	}
	return nullptr;//見つからなかったのでnullを渡す
}

AudioPlayer::~AudioPlayer() {

	if (sound_data_.empty()) {
		//要素の削除がすべて完了している場合はクリア
		sound_data_.clear();
	}
	//解放漏れがある場合
	for (auto data : sound_data_) {
		IDirectSoundBuffer8* sound = data.second;
		if (sound == nullptr) {
			continue;
		}
		sound->Release();
		//sound_data_.erase(data.first);
	}
	sound_data_.clear();

	if (sound_) {
		delete sound_;
	}
}

