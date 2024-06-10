#pragma once
#include <map>
#include <string>
#include "Sound.h"

class SoundManager
{
public:

	//シングルトンインスタンスを取得する
	static SoundManager& GetInstance();

	//シングルトンインスタンスを解放する
	static void ReleaseInstance();

	//サウンドを再生する
	void PlaySound(std::string filename, bool loop = false);

	//サウンドを再生を止める
	void StopSound(std::string filename);

	//全てのサウンドのボリュームを設定する
	void SetVolume(int volume);

private:

	//コンストラクタ
	SoundManager();

	//デストラクタ
	~SoundManager();

	//メモリのオーバーヘッド削減のためポインタ(動的)
	//サウンドファイル名とそのサウンドオブジェのマップ
	std::map<std::string, Sound*> sounds_;

};