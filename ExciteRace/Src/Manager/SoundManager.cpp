#include "SoundManager.h"
#include "../Application.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	for (auto& pair : sounds_)
	{
		//�T�E���h�I�u�W�F�N�g���������
		delete pair.second;
	}
	//�}�b�v���N���A����
	sounds_.clear();
}

SoundManager& SoundManager::GetInstance()
{
	//�V���O���g���C���X�^���X�𐶐�����
	static SoundManager instance;
	//�C���X�^���X��Ԃ�
	return instance;
}

void SoundManager::ReleaseInstance()
{
	//�C���X�^���X�͎����I�ɔj�������
}

void SoundManager::PlaySound(std::string filename, bool loop)
{
	//�}�b�v������������
	auto soundPair = sounds_.find(filename);
	// �T�E���h���}�b�v���ɂȂ��ꍇ
	if (soundPair == sounds_.end())
	{
		//�V�����T�E���h�I�u�W�F�N�g�𐶐�����
		Sound* sound = new Sound();
		//�T�E���h��ǂݍ���
		if (sound->Load(filename))
		{
			//�T�E���h���Đ�����
			sound->Play(loop);
			//�}�b�v�ɒǉ�����
			sounds_[filename] = sound;
		}
		else
		{
			//�ǂݍ��݂Ɏ��s�����ꍇ�̓I�u�W�F�N�g���������
			delete sound;
		}
	}
	else
	{
		//���łɓǂݍ��܂�Ă���T�E���h���Đ�����
		soundPair->second->Play(loop);
	}
}

void SoundManager::StopSound(std::string filename)
{
	//�}�b�v������������
	auto soundPair = sounds_.find(filename);
	// �T�E���h���}�b�v���ɂ���ꍇ
	if (soundPair != sounds_.end())
	{
		//�T�E���h���~����
		soundPair->second->Stop();
	}
}

void SoundManager::SetVolume(int volume)
{
	for (auto& pair : sounds_)
	{
		//�T�E���h�̃{�����[����ݒ肷��
		pair.second->SetVolume(volume);
	}
}

