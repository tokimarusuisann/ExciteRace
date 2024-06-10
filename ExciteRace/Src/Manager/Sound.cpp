#include "Sound.h"

Sound::Sound()
{
	soundHandle_ = -1;
}

Sound::~Sound()
{
	//サウンドの再生を停止し、リソースを解放
	Stop();
}

int Sound::Load(std::string filename)
{
	//DxLibでサウンドを読み込み、ハンドルを取得
	soundHandle_ = LoadSoundMem(filename.c_str());
	// 読み込みに成功したかどうかを返す
	if (soundHandle_ == -1)
	{
		return -1;
	}
	return soundHandle_;
}

void Sound::Play(bool loop)
{
	if (soundHandle_ != -1)
	{
		if (loop)
		{
			// ループ再生する
			PlaySoundMem(soundHandle_, DX_PLAYTYPE_LOOP);
		}
		else
		{
			// 1回再生する
			PlaySoundMem(soundHandle_, DX_PLAYTYPE_BACK);
		}
	}
}

void Sound::Stop()
{
	// サウンドの再生を停止する
	StopSoundMem(soundHandle_);
}

void Sound::SetVolume(int volume)
{
	//ボリュームを設定する
	ChangeVolumeSoundMem(255 * volume / 100, soundHandle_);
}
