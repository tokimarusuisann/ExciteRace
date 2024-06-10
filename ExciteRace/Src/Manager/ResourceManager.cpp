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

	//モデル

	// スカイドーム
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "SkyDome/SkyDome.mv1");
	resourcesMap_.emplace(SRC::SKYDOME, res);

	//ガードレール
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Guardrail.mv1");
	resourcesMap_.emplace(SRC::GUARDRAIL, res);

	//ガードレールの当たり判定
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Guardrail_Collision.mv1");
	resourcesMap_.emplace(SRC::GUARDRAIL_COLLISION, res);

	//木
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Tree.mv1");
	resourcesMap_.emplace(SRC::TREE, res);

	//道
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Road.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROAD, res);

	//衝突判定：道
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Road_Collision.mv1");
	resourcesMap_.emplace(SRC::STAGE_ROAD_COLLISION, res);

	//壁
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Stage_Main/Wall_Collision.mv1");
	resourcesMap_.emplace(SRC::STAGE_WALL_COLLISION, res);

	//ゴール
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Goal/Goal.mv1");
	resourcesMap_.emplace(SRC::GOAL, res);

	//ゴール当たり判定
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Goal/Goal_Collision.mv1");
	resourcesMap_.emplace(SRC::GOAL_COLLISION, res);

	//ガレージ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "StageObject_Garage/CarGarage.mv1");
	resourcesMap_.emplace(SRC::GARAGE, res);

	//ガレージ床
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "StageObject_Garage/GarageFloor.mv1");
	resourcesMap_.emplace(SRC::GARAGE_FLOOR, res);

	//車 (Silvia）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Silvia.mv1");
	resourcesMap_.emplace(SRC::BALANCE_CAR, res);

	//車 (Bmx)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Bmx.mv1");
	resourcesMap_.emplace(SRC::ACCELERATE_CAR, res);

	//車 (Arcade)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Arcade.mv1");
	resourcesMap_.emplace(SRC::SPEED_CAR, res);

	//セレクト画面車のタイヤ(Silvia）
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Sirvia_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_BALANCE_CAR_WHEEL, res);

	//セレクト画面車のタイヤ(Bmx)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Bmx_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_ACCELERATE_CAR_WHEEL, res);

	//セレクト画面車のタイヤ(Arcade)
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Select_Car_Wheel/Arcade_Wheels.mv1");
	resourcesMap_.emplace(SRC::SELECT_SPEED_CAR_WHEEL, res);

	//タイヤ
	res = Resource(Resource::TYPE::MODEL, Application::PATH_MODEL + "Car_Models/Wheel.mv1");
	resourcesMap_.emplace(SRC::WHEEL, res);

	//画像
	
	//車画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Car_Image.png");
	resourcesMap_.emplace(SRC::CAR_LOGO, res);

	//カーソル画像
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Cursor_Image.png");
	resourcesMap_.emplace(SRC::CURSOR, res);
	
	//決定指示ロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Decision_Image.png");
	resourcesMap_.emplace(SRC::DECISION_BUTTON_LOGO, res);

	//十字ボタンロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Cross_Button_Image.png");
	resourcesMap_.emplace(SRC::CROSS_BUTTON_LOGO, res);

	//バランスカーロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Sirvia.png");
	resourcesMap_.emplace(SRC::SELECT_BALANCE_CAR_LOGO, res);

	//加速カーロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Bmx.png");
	resourcesMap_.emplace(SRC::SELECT_ACCELERATE_CAR_LOGO, res);

	//スピードカーロゴ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Arcade.png");
	resourcesMap_.emplace(SRC::SELECT_SPEED_CAR_LOGO, res);

	//バーサス背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Versus_BackGround_Image.png");
	resourcesMap_.emplace(SRC::VERSUS_BACKGROUND, res);

	//タイムアタック背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimeAttack_BackGround_Image.png");
	resourcesMap_.emplace(SRC::TIMEATTACK_BACKGROUND, res);

	//ランキング背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Ranking_BackGround_Image.png");
	resourcesMap_.emplace(SRC::RANKING_BACKGROUND, res);

	//バーサスボタン
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Versus_Button_Image.png");
	resourcesMap_.emplace(SRC::VERSUS_BUTTON, res);

	//タイムアタックボタン
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "TimeAttack_Button_Image.png");
	resourcesMap_.emplace(SRC::TIMEATTACK_BUTTON, res);

	//ランキングボタン
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Ranking_Button_Image.png");
	resourcesMap_.emplace(SRC::RANKING_BUTTON, res);

	//タコメーター
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Tachometer_Image.png");
	resourcesMap_.emplace(SRC::TACHOMETER, res);

	//ニードル
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Needle_Image.png");
	resourcesMap_.emplace(SRC::NEEDLE, res);

	//ミニマップ
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "MiniMap_Image.png");
	resourcesMap_.emplace(SRC::MINIMAP, res);

	//スタートサイン
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Start_Sign_Image.png");
	resourcesMap_.emplace(SRC::START_SIGN, res);

	//リザルト画面背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Result_BackGround_Image.png");
	resourcesMap_.emplace(SRC::RESULT, res);

	//スコア画面背景
	res = Resource(Resource::TYPE::IMG, Application::PATH_IMAGE + "Score_BackGround_Image.png");
	resourcesMap_.emplace(SRC::SCORE, res);

	// 集中線
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
		// 登録されていない
		return nullptr;
	}

	rPair->second.Load();

	// 念のためコピーコンストラクタ
	Resource* ret = new Resource(rPair->second);
	loadedMap_.emplace(src, ret);

	return ret;
}
