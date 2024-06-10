#include "SoundManager.h"
#include "../Application.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	for (auto& pair : sounds_)
	{
		//サウンドオブジェクトを解放する
		delete pair.second;
	}
	//マップをクリアする
	sounds_.clear();
}

SoundManager& SoundManager::GetInstance()
{
	//シングルトンインスタンスを生成する
	static SoundManager instance;
	//インスタンスを返す
	return instance;
}

void SoundManager::ReleaseInstance()
{
	//インスタンスは自動的に破棄される
}

void SoundManager::PlaySound(std::string filename, bool loop)
{
	//マップ内を検索する
	auto soundPair = sounds_.find(filename);
	// サウンドがマップ内にない場合
	if (soundPair == sounds_.end())
	{
		//新しいサウンドオブジェクトを生成する
		Sound* sound = new Sound();
		//サウンドを読み込む
		if (sound->Load(filename))
		{
			//サウンドを再生する
			sound->Play(loop);
			//マップに追加する
			sounds_[filename] = sound;
		}
		else
		{
			//読み込みに失敗した場合はオブジェクトを解放する
			delete sound;
		}
	}
	else
	{
		//すでに読み込まれているサウンドを再生する
		soundPair->second->Play(loop);
	}
}

void SoundManager::StopSound(std::string filename)
{
	//マップ内を検索する
	auto soundPair = sounds_.find(filename);
	// サウンドがマップ内にある場合
	if (soundPair != sounds_.end())
	{
		//サウンドを停止する
		soundPair->second->Stop();
	}
}

void SoundManager::SetVolume(int volume)
{
	for (auto& pair : sounds_)
	{
		//サウンドのボリュームを設定する
		pair.second->SetVolume(volume);
	}
}

