#include <string>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/Camera.h"
#include "../../Object/Stage.h"
#include "../../Object/Car.h"
#include "../../Object/SkyDome.h"
#include "../../Object/Garage.h"
#include "../../Object/CarType/BalanceCar.h"
#include "GameStartScene.h"

GameStartScene::GameStartScene(void)
{
}

GameStartScene::~GameStartScene(void)
{
}

void GameStartScene::Init(void)
{

	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	stage_ = std::make_unique<Stage>();
	stage_->Init();

	std::vector<VECTOR> vecPos;
	std::vector<InputManager::JOYPAD_NO> padNo;
	std::vector<CAR_TYPE> carType;

	vecPos.emplace_back(CAR_INIT_POS_1);
	vecPos.emplace_back(CAR_INIT_POS_2);

	padNo.emplace_back(InputManager::JOYPAD_NO::PAD1);
	padNo.emplace_back(InputManager::JOYPAD_NO::PAD2);

	carType.emplace_back(CAR_TYPE::BALANCE_CAR);
	carType.emplace_back(CAR_TYPE::BALANCE_CAR);

	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));

	screenPos_.emplace_back(Vector2(0, 0));
	screenPos_.emplace_back(Vector2(Application::SCREEN_SIZE_X / 2, 0));

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
	{
		cameras_.emplace_back(std::make_unique<Camera>());
		cars_.emplace_back(std::make_unique<BalanceCar>(padNo[i], *stage_));

		cameras_[i]->SetFollowTarget(&cars_[i]->GetTransform());
		cameras_[i]->Init();
		cars_[i]->Init(carType[i], vecPos[i]);
	}

	for (auto& car : cars_)
	{
		car->AddCol(stage_->GetModelIdRoadCollision());
		car->Update();
	}

	SoundManager& soundIns = SoundManager::GetInstance();
	soundIns.PlaySoundA(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3", true);
	soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);

}

void GameStartScene::Update(void)
{

	for (auto& cameras : cameras_)
	{
		cameras->Update();
		if (cameras->GetTime()>=9.0f)
		{
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::GAME, true);
		}
	}

	skyDome_->Update();
	stage_->Update();

}

void GameStartScene::Draw(void)
{

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
	{
		SetDrawScreen(cameraScreens_[i]);

		// âÊñ Çèâä˙âª
		ClearDrawScreen();

		cameras_[i]->SetBeforeDraw();

		skyDome_->SetFollowTarget(&cars_[i]->GetTransform());
		skyDome_->Update();
		skyDome_->Draw();

		DrawGame();
	}

	SetDrawScreen(DX_SCREEN_BACK);
	// âÊñ Çèâä˙âª
	ClearDrawScreen();

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//ìßâﬂèàóùÇÕå„Ç≈å©íºÇ∑
	}

}

void GameStartScene::DrawGame(void)
{

	for (auto& cars : cars_)
	{
		cars->Draw();
	}

	stage_->Draw();

}

