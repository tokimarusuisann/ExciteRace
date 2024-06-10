#pragma once
#include "Common/Transform.h"

class Stage
{
public:

	// �R���X�g���N�^
	Stage(void);

	// �f�X�g���N�^
	~Stage(void);

	//������
	void Init(void);
	
	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//���f��ID�擾
	int GetModelIdRoadCollision(void);
	int GetModelIdGuardRailCollision(void);
	int GetModelIdWallCollision(void);
	int GetModelIdGoalCollision(void);

private:

	//�X�e�[�W�^�C�v
	enum class STAGE_TYPE
	{
		ROAD,
		GUARDRAIL,
		TREE,
		GOAL,
		ROAD_COLLISION,
		WALL_COLLISION,
		GUARDRAIL_COLLISION,
		GOAL_COLLISION,
		MAX
	};

	//�^�C�v���Ƃ̃g�����X�t�H�[�����
	std::map<STAGE_TYPE, Transform> stageInfos_;

};