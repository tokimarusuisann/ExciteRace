#pragma once
#include <DxLib.h>
#include <string>

class Sound
{
public:

    // �R���X�g���N�^
    Sound();

    // �f�X�g���N�^
    ~Sound();

    // �T�E���h�t�@�C����ǂݍ���
    int Load(std::string filename);

    // �T�E���h���Đ�����
    void Play(bool loop = false);

    // �T�E���h�̍Đ����~����
    void Stop(void);

    // �{�����[����ݒ肷��
    void SetVolume(int volume);

private:

    int soundHandle_;

};