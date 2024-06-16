#include<DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "Stage.h"

#pragma region �萔�錾

//���̃N���X�ł����g��Ȃ�
//�X�e�[�W�f�t�H���g�T�C�Y
constexpr VECTOR STAGE_DEFAULT_SCALE = { 1.2f, 1.2f, 1.2f };

//���̓����蔻��p���f���̃|�W�V����
constexpr VECTOR ROAD_COLLISION_POS = { 0.0f,-20.0f,0.0f };

//���̓����蔻��p���f���̑傫��
constexpr VECTOR ROAD_COLLISION_SCALE = { 1.2f, 1.0f, 1.2f };

//�K�[�h���C���̓����蔻��p���f���傫��
constexpr VECTOR GUARDRAIL_COLLISION_SCALE = { 1.2f, 10.0f, 1.2f };

//�S�[���̃|�W�V����
constexpr VECTOR GOAL_POS = { 360.0f,0.0f,1000.0f };

//�S�[���̓����蔻��p���f���̃|�W�V����
constexpr VECTOR GOAL_COLLISION_POS = { 1000.0f,0.0f,0.0f };

//�ǂ̓����蔻��p���f���̃|�W�V����
constexpr VECTOR WALL_COLLISION_POS = { 0.0f,-20.0f,0.0f };

//�ǂ̓����蔻��p���f���̑傫��
constexpr VECTOR WALL_COLLISION_SCALE = { 1.2f,6.0f,1.2f };

//�S�[���̏�������
constexpr VECTOR GOAL_INIT_ROT = { 0.0f,20.0f * DX_PI_F / 180, 0.0f };

//�X�e�[�W�̏�������
constexpr VECTOR STAGE_INIT_ROT = { 0.0f,-30.0f * DX_PI_F / 180, 0.0f };

#pragma endregion

Stage::Stage(void)
{
}

Stage::~Stage(void)
{
	for (auto& stageInfos : stageInfos_)
	{
		MV1DeleteModel(stageInfos_[stageInfos.first].modelId_);
	}
}

void Stage::Init(void)
{
	auto& resIns = ResourceManager::GetInstance();

	Transform* stageInfo;

	//��
	stageInfo = &stageInfos_[STAGE_TYPE::ROAD];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::STAGE_ROAD));
	stageInfo->scl_ = STAGE_DEFAULT_SCALE;
	stageInfo->pos_ = AsoUtility::VECTOR_ZERO;
	stageInfo->quaRot_ = Quaternion();

	//�����蔻��p�̓�
	stageInfo = &stageInfos_[STAGE_TYPE::ROAD_COLLISION];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::STAGE_ROAD_COLLISION));
	stageInfo->scl_ = ROAD_COLLISION_SCALE;
	stageInfo->pos_ = ROAD_COLLISION_POS;
	stageInfo->quaRot_ = Quaternion();

	//�K�[�h���C��
	stageInfo = &stageInfos_[STAGE_TYPE::GUARDRAIL];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GUARDRAIL));
	stageInfo->scl_ = STAGE_DEFAULT_SCALE;
	stageInfo->pos_ = AsoUtility::VECTOR_ZERO;
	stageInfo->quaRot_ = Quaternion();

	//�����蔻��p�K�[�h���C��
	stageInfo = &stageInfos_[STAGE_TYPE::GUARDRAIL_COLLISION];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GUARDRAIL_COLLISION));
	stageInfo->scl_ = GUARDRAIL_COLLISION_SCALE;
	stageInfo->pos_ = AsoUtility::VECTOR_ZERO;
	stageInfo->quaRot_ = Quaternion();

	//��
	stageInfo = &stageInfos_[STAGE_TYPE::TREE];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::TREE));
	stageInfo->scl_ = STAGE_DEFAULT_SCALE;
	stageInfo->pos_ = AsoUtility::VECTOR_ZERO;
	stageInfo->quaRot_ = Quaternion();

	//�S�[��
	stageInfo = &stageInfos_[STAGE_TYPE::GOAL];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GOAL));
	stageInfo->scl_ = STAGE_DEFAULT_SCALE;
	stageInfo->pos_ = GOAL_POS;
	stageInfo->quaRot_ = GOAL_INIT_ROT;

	//�����蔻��p�S�[��
	stageInfo = &stageInfos_[STAGE_TYPE::GOAL_COLLISION];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::GOAL_COLLISION));
	stageInfo->scl_ = STAGE_DEFAULT_SCALE;
	stageInfo->pos_ = GOAL_COLLISION_POS;
	stageInfo->quaRot_ = GOAL_INIT_ROT;

	//�����蔻��p��
	stageInfo = &stageInfos_[STAGE_TYPE::WALL_COLLISION];
	stageInfo->SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::STAGE_WALL_COLLISION));
	stageInfo->scl_ = WALL_COLLISION_SCALE;
	stageInfo->pos_ = WALL_COLLISION_POS;
	stageInfo->quaRot_ = Quaternion();

	for (auto& stageInfos : stageInfos_)
	{
		stageInfos_[stageInfos.first].quaRotLocal_ = Quaternion::Euler(STAGE_INIT_ROT);
		stageInfos_[stageInfos.first].Update();
	}
}

void Stage::Update(void)
{
	for (auto& stageInfos : stageInfos_)
	{
		stageInfos_[stageInfos.first].Update();
	}
}

void Stage::Draw(void)
{
	MV1DrawModel(stageInfos_[STAGE_TYPE::ROAD].modelId_);
	MV1DrawModel(stageInfos_[STAGE_TYPE::TREE].modelId_);
	MV1DrawModel(stageInfos_[STAGE_TYPE::GUARDRAIL].modelId_);
	MV1DrawModel(stageInfos_[STAGE_TYPE::GOAL].modelId_);
}

int Stage::GetModelIdRoadCollision(void)
{
	return stageInfos_[STAGE_TYPE::ROAD_COLLISION].modelId_;
}

int Stage::GetModelIdGuardRailCollision(void)
{
	return stageInfos_[STAGE_TYPE::GUARDRAIL_COLLISION].modelId_;
}

int Stage::GetModelIdWallCollision(void)
{
	return stageInfos_[STAGE_TYPE::WALL_COLLISION].modelId_;
}

int Stage::GetModelIdGoalCollision(void)
{
	return stageInfos_[STAGE_TYPE::GOAL_COLLISION].modelId_;
}
