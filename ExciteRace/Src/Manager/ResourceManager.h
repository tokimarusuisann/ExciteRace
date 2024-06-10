#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// ���\�[�X��
	enum class SRC
	{
		NONE,
		CAR_LOGO,
		VERSUS_BACKGROUND,
		TIMEATTACK_BACKGROUND,
		RANKING_BACKGROUND,	
		VERSUS_BUTTON,
		TIMEATTACK_BUTTON,
		RANKING_BUTTON,
		CURSOR,
		BALANCE_CAR,
		ACCELERATE_CAR,
		SPEED_CAR,
		DECISION_BUTTON_LOGO,
		CROSS_BUTTON_LOGO,
		SELECT_BALANCE_CAR_LOGO,
		SELECT_ACCELERATE_CAR_LOGO,
		SELECT_SPEED_CAR_LOGO,
		SELECT_BALANCE_CAR_WHEEL,
		SELECT_ACCELERATE_CAR_WHEEL,
		SELECT_SPEED_CAR_WHEEL,
		WHEEL,
		SKYDOME,
		STAGE_ROAD,
		STAGE_WALL_COLLISION,
		STAGE_ROAD_COLLISION,
		GUARDRAIL,
		GUARDRAIL_COLLISION,
		TREE,
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		GOAL,
		GOAL_COLLISION,
		GARAGE,
		GARAGE_FLOOR,
		LINE_1,
		LINE_2,
		LINE_3,	
		LINE_4,
		LINE_5,
		LINE_6,
		START_SIGN,
		RESULT,
		SCORE
	};

	// �����I�ɃC���X�e���X�𐶐�����
	static void CreateInstance(void);

	// �ÓI�C���X�^���X�̎擾
	static ResourceManager& GetInstance(void);

	// ������
	void Init(void);

	// ���(�V�[���ؑ֎��Ɉ�U���)
	void Release(void);

	// ���\�[�X�̃��[�h
	Resource Load(SRC src);

	// ���\�[�X�̕������[�h(���f���p)
	int LoadModelDuplicate(SRC src);

private:

	// �ÓI�C���X�^���X
	static ResourceManager* instance_;

	// ���\�[�X�Ǘ��̑Ώ�
	std::map<SRC, Resource> resourcesMap_;

	// �ǂݍ��ݍς݃��\�[�X
	std::map<SRC, Resource*> loadedMap_;

	// �f�t�H���g�R���X�g���N�^��private�ɂ��āA
	// �O�����琶���ł��Ȃ��l�ɂ���
	ResourceManager(void);

	// �f�X�g���N�^�����l
	~ResourceManager(void);

	// �������[�h
	Resource* _Load(SRC src);

};

