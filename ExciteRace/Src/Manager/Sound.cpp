#include "Sound.h"

Sound::Sound()
{
	soundHandle_ = -1;
}

Sound::~Sound()
{
	//�T�E���h�̍Đ����~���A���\�[�X�����
	Stop();
}

int Sound::Load(std::string filename)
{
	//DxLib�ŃT�E���h��ǂݍ��݁A�n���h�����擾
	soundHandle_ = LoadSoundMem(filename.c_str());
	// �ǂݍ��݂ɐ����������ǂ�����Ԃ�
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
			// ���[�v�Đ�����
			PlaySoundMem(soundHandle_, DX_PLAYTYPE_LOOP);
		}
		else
		{
			// 1��Đ�����
			PlaySoundMem(soundHandle_, DX_PLAYTYPE_BACK);
		}
	}
}

void Sound::Stop()
{
	// �T�E���h�̍Đ����~����
	StopSoundMem(soundHandle_);
}

void Sound::SetVolume(int volume)
{
	//�{�����[����ݒ肷��
	ChangeVolumeSoundMem(255 * volume / 100, soundHandle_);
}
