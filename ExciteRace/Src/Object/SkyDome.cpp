#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "SkyDome.h"

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�
//�X�J�C�h�[���̑傫��
constexpr VECTOR SKYDOME_SCALE = { 100.0f,100.0f,100.0f };

//�X�J�C�h�[����������
constexpr VECTOR SKYDOME_INIT_ROT = { 0.0f,180.0f * DX_PI_F / 180, 0.0f };
#pragma endregion

SkyDome::SkyDome()
{

}

SkyDome::~SkyDome(void)
{
	MV1DeleteModel(transform_.modelId_);
}

void SkyDome::Init(void)
{
	// ���f������̊�{���
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKYDOME));
	transform_.scl_ = SKYDOME_SCALE;
	transform_.quaRot_ = Quaternion::Euler(SKYDOME_INIT_ROT);
	transform_.quaRotLocal_ = Quaternion();
	transform_.Update();

	// Z�o�b�t�@����(�˂������΍�)
	MV1SetUseZBuffer(transform_.modelId_, false);
	MV1SetWriteZBuffer(transform_.modelId_, false);
}

void SkyDome::Update(void)
{
	transform_.Update();
}

void SkyDome::Draw(void)
{
	MV1DrawModel(transform_.modelId_);
}

void SkyDome::SetFollowTarget(const Transform* target)
{
	target_ = target;
	transform_.pos_ = target_->pos_;
}


