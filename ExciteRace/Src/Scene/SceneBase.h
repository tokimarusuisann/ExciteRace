#pragma once
#include <DxLib.h>

class SceneBase
{
public:

	//�K���[�W�̏����ʒu
	static constexpr VECTOR GARAGE_INIT_POS = { 25000.0f,0.0f,26000.0f };

	//�ΐ펞�A�Ԃ̏����ʒu
	static constexpr VECTOR CAR_INIT_POS_1 = { 900.0f, 32.0f, 300.0f };
	static constexpr VECTOR CAR_INIT_POS_2 = { 1500.0f, 32.0f, 300.0f };

	//�^�C���A�^�b�N���A�Ԃ̏�����
	static constexpr VECTOR CAR_INIT_POS_ = { 1200.0f, 32.0f, 300.0f };

	//MUSIC����
	static constexpr int MUSIC_DEFAULT_VOLUME = 30;

	// �R���X�g���N�^
	SceneBase(void);

	// �f�X�g���N�^
	virtual ~SceneBase(void);

	// ����������
	virtual void Init(void);

	// �X�V�X�e�b�v
	virtual void Update(void);

	// �`�揈��
	virtual void Draw(void);

};

