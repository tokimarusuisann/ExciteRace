#include "../Manager/ResourceManager.h"
#include "../Manager/Resource.h"
#include "../Utility/AsoUtility.h"
#include "Garage.h"

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�
//�K���[�W�̑傫��
constexpr VECTOR GARAGE_SCALE = { 3.0f, 2.0f, 2.0f };

//���̑傫��
constexpr VECTOR FLOOR_SCALE = { 2.5f, 2.0f, 2.0f };

//�K���[�W�̏�������
constexpr VECTOR GARAGE_INIT_ROT = { 0.0f,-200 * DX_PI_F/180, 0.0f };
#pragma endregion

Garage::Garage(void)
{
}

Garage::~Garage(void)
{
	for (auto& garage : garageInfos_)
	{
		MV1DeleteModel(garageInfos_[garage.first].modelId_);
	}
}

void Garage::Init(void)
{
	auto& resIns = ResourceManager::GetInstance();
	
	//�K���[�W�{��
	Transform* garareInfo;
	garareInfo = &garageInfos_[GARAGE_TYPE::GARAGE];
	garareInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GARAGE));
	garareInfo->pos_ = AsoUtility::VECTOR_ZERO;
	garareInfo->scl_ = GARAGE_SCALE;

	//�K���[�W��
	garareInfo = &garageInfos_[GARAGE_TYPE::FLOOR];
	garareInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GARAGE_FLOOR));
	garareInfo->pos_ = AsoUtility::VECTOR_ZERO;
	garareInfo->scl_ = FLOOR_SCALE;

	for (auto& garage : garageInfos_)
	{
		garageInfos_[garage.first].quaRot_ = Quaternion::Euler(GARAGE_INIT_ROT);
		garageInfos_[garage.first].Update();
	}
}

void Garage::Update(void)
{
	for (auto& garage : garageInfos_)
	{
		garageInfos_[garage.first].Update();
	}
}

void Garage::Draw(void)
{
	for (auto& garage : garageInfos_)
	{
		MV1DrawModel(garageInfos_[garage.first].modelId_);
	}
}

void Garage::SetPos(VECTOR pos)
{
	for (auto& garage : garageInfos_)
	{
		garageInfos_[garage.first].pos_ = pos;
	}
}


