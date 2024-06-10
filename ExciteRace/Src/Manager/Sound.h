#pragma once
#include <DxLib.h>
#include <string>

class Sound
{
public:

    // コンストラクタ
    Sound();

    // デストラクタ
    ~Sound();

    // サウンドファイルを読み込む
    int Load(std::string filename);

    // サウンドを再生する
    void Play(bool loop = false);

    // サウンドの再生を停止する
    void Stop(void);

    // ボリュームを設定する
    void SetVolume(int volume);

private:

    int soundHandle_;

};