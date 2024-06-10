#include <DxLib.h>
#include "../Application.h"
#include "Resource.h"
#include "ResourceManager.h"

ResourceManager* ResourceManager::instance_ = nullptr;

void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}
	instance_->Init();
}

ResourceManager& ResourceManager::GetInstance(void)
{
	return *instance_;
}

void ResourceManager::Init(void)
{
	Resource res;

	//���f��

	// �X�J�C�h�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKYDOME, res);

	//�K�[�h���[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Guardrail.mv1");
	resourcesMap_.emplace(SRC::GUARDRAIL, res);

	//�K�[�h���[���̓����蔻��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Guardrail_Collision.mv1");
	resourcesMap_.emplace(SRC::GUARDRAIL_COLLISION, res);

	//��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Tree.mv1");
	resourcesMap_.emplace(SRC::TREE, res);

	//��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Road.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROAD, res);

	//�Փ˔���F��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Road_Collision.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROAD_COLLISION, res);

	//��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Wall_Collision.mv1");
	resourcesMap_.emplace(SRC::STAGE_WALL_COLLISION, res);

	//�S�[��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Goal/Goal.mv1");
	resourcesMap_.emplace(SRC::GOAL, res);

	//�S�[�������蔻��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Goal/Goal_Collision.mv1");
	resourcesMap_.emplace(SRC::GOAL_COLLISION, res);

	//�K���[�W
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "StageObject_Garage/CarGarage.mv1");
	resourcesMap_.emplace(SRC::GARAGE, res);

	//�K���[�W��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "StageObject_Garage/GarageFloor.mv1");
	resourcesMap_.emplace(SRC::GARAGE_FLOOR, res);

	//�� (Silvia�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Silvia.mv1");
	resourcesMap_.emplace(SRC::BALANCE_CAR, res);

	//�� (Bmx)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Bmx.mv1");
	resourcesMap_.emplace(SRC::ACCELERATE_CAR, res);

	//�� (Arcade)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Arcade.mv1");
	resourcesMap_.emplace(SRC::SPEED_CAR, res);

	//�Z���N�g��ʎԂ̃^�C��(Silvia�j
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Sirvia_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_BALANCE_CAR_WHEEL, res);

	//�Z���N�g��ʎԂ̃^�C��(Bmx)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Bmx_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_ACCELERATE_CAR_WHEEL, res);

	//�Z���N�g��ʎԂ̃^�C��(Arcade)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Arcade_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_SPEED_CAR_WHEEL, res);

	//�^�C��
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Wheel.mv1");
	resourcesMap_.emplace(SRC::WHEEL, res);

	//�摜
	
	//�ԉ摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Car_Image.png");
	resourcesMap_.emplace(SRC::CAR_LOGO, res);

	//�J�[�\���摜
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Cursor_Image.png");
	resourcesMap_.emplace(SRC::CURSOR, res);
	
	//����w�����S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Decision_Image.png");
	resourcesMap_.emplace(SRC::DECISION_BUTTON_LOGO, res);

	//�\���{�^�����S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Cross_Button_Image.png");
	resourcesMap_.emplace(SRC::CROSS_BUTTON_LOGO, res);

	//�o�����X�J�[���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Sirvia.png");
	resourcesMap_.emplace(SRC::SELECT_BALANCE_CAR_LOGO, res);

	//�����J�[���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Bmx.png");
	resourcesMap_.emplace(SRC::SELECT_ACCELERATE_CAR_LOGO, res);

	//�X�s�[�h�J�[���S
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Arcade.png");
	resourcesMap_.emplace(SRC::SELECT_SPEED_CAR_LOGO, res);

	//�o�[�T�X�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Versus_BackGround_Image.png");
	resourcesMap_.emplace(SRC::VERSUS_BACKGROUND, res);

	//�^�C���A�^�b�N�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimeAttack_BackGround_Image.png");
	resourcesMap_.emplace(SRC::TIMEATTACK_BACKGROUND, res);

	//�����L���O�w�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Ranking_BackGround_Image.png");
	resourcesMap_.emplace(SRC::RANKING_BACKGROUND, res);

	//�o�[�T�X�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Versus_Button_Image.png");
	resourcesMap_.emplace(SRC::VERSUS_BUTTON, res);

	//�^�C���A�^�b�N�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimeAttack_Button_Image.png");
	resourcesMap_.emplace(SRC::TIMEATTACK_BUTTON, res);

	//�����L���O�{�^��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Ranking_Button_Image.png");
	resourcesMap_.emplace(SRC::RANKING_BUTTON, res);

	//�^�R���[�^�[
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Tachometer_Image.png");
	resourcesMap_.emplace(SRC::TACHOMETER, res);

	//�j�[�h��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Needle_Image.png");
	resourcesMap_.emplace(SRC::NEEDLE, res);

	//�~�j�}�b�v
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "MiniMap_Image.png");
	resourcesMap_.emplace(SRC::MINIMAP, res);

	//�X�^�[�g�T�C��
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Start_Sign_Image.png");
	resourcesMap_.emplace(SRC::START_SIGN, res);

	//���U���g��ʔw�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Result_BackGround_Image.png");
	resourcesMap_.emplace(SRC::RESULT, res);

	//�X�R�A��ʔw�i
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Score_BackGround_Image.png");
	resourcesMap_.emplace(SRC::SCORE, res);

	// �W����
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line1_Image.png");
	resourcesMap_.emplace(SRC::LINE_1, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line2_Image.png");
	resourcesMap_.emplace(SRC::LINE_2, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line3_Image.png");
	resourcesMap_.emplace(SRC::LINE_3, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line4_Image.png");
	resourcesMap_.emplace(SRC::LINE_4, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line5_Image.png");
	resourcesMap_.emplace(SRC::LINE_5, res);

	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "line6_Image.png");
	resourcesMap_.emplace(SRC::LINE_6, res);

}

void ResourceManager::Release(void)
{
	for (auto& p : loadedMap_)
	{
		p.second->Release();
		delete p.second;
	}

	loadedMap_.clear();
}

Resource ResourceManager::Load(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return Resource();
	}
	Resource ret = *res;
	return *res;
}

int ResourceManager::LoadModelDuplicate(SRC src)
{
	Resource* res = _Load(src);
	if (res == nullptr)
	{
		return -1;
	}

	int duId = MV1DuplicateModel(res->handleId_);
	res->duplicateModelIds_.push_back(duId);

	return duId;
}

ResourceManager::ResourceManager(void)
{
}

ResourceManager::~ResourceManager(void)
{
	delete instance_;
}

Resource* ResourceManager::_Load(SRC src)
{
	const auto& lPair = loadedMap_.find(src);
	if (lPair != loadedMap_.end())
	{
		return lPair->second;
	}

	const auto& rPair = resourcesMap_.find(src);
	if (rPair == resourcesMap_.end())
	{
		// �o�^����Ă��Ȃ�
		return nullptr;
	}

	rPair->second.Load();

	// �O�̂��߃R�s�[�R���X�g���N�^
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
