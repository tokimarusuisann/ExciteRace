#pragma once
#include <map>
#include <string>
#include "Sound.h"

class SoundManager
{
public:

	//�V���O���g���C���X�^���X���擾����
	static SoundManager& GetInstance();

	//�V���O���g���C���X�^���X���������
	static void ReleaseInstance();

	//�T�E���h���Đ�����
	void PlaySound(std::string filename, bool loop = false);

	//�T�E���h���Đ����~�߂�
	void StopSound(std::string filename);

	//�S�ẴT�E���h�̃{�����[����ݒ肷��
	void SetVolume(int volume);

private:

	//�R���X�g���N�^
	SoundManager();

	//�f�X�g���N�^
	~SoundManager();

	//�������̃I�[�o�[�w�b�h�팸�̂��߃|�C���^(���I)
	//�T�E���h�t�@�C�����Ƃ��̃T�E���h�I�u�W�F�̃}�b�v
	std::map<std::string, Sound*> sounds_;

};