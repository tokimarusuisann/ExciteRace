#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "SkyDome.h"

#pragma region 定数宣言
//このクラスでしか使わない
//スカイドームの大きさ
constexpr VECTOR SKYDOME_SCALE = { 100.0f,100.0f,100.0f };

//スカイドーム初期方向
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
	// モデル制御の基本情報
	transform_.SetModel(ResourceManager::GetInstance().LoadModelDuplicate(ResourceManager::SRC::SKYDOME));
	transform_.scl_ = SKYDOME_SCALE;
	transform_.quaRot_ = Quaternion::Euler(SKYDOME_INIT_ROT);
	transform_.quaRotLocal_ = Quaternion();
	transform_.Update();

	// Zバッファ無効(突き抜け対策)
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


