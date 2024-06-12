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
#include "../../Object/CarType/AccelerateCar.h"
#include "../../Object/CarType/BalanceCar.h"
#include "../../Object/CarType/SpeedCar.h"
#include "TimeAttackStartScene.h"

TimeAttackStartScene::TimeAttackStartScene(void)
{
}

TimeAttackStartScene::~TimeAttackStartScene(void)
{
}

void TimeAttackStartScene::Init(void)
{

	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//�X�e�[�W
	stage_ = std::make_unique<Stage>();
	stage_->Init();

	//�I�񂾎Ԃ̃^�C�v
	CAR_TYPE carType = SceneManager::GetInstance().GetCarType();

	//�Ԃ̏����|�W�V����
	VECTOR carPos = CAR_INIT_POS_;

	switch (carType)
	{
	case CAR_TYPE::BALANCE_CAR:
		car_ = std::make_unique<BalanceCar>(InputManager::JOYPAD_NO::PAD1, *stage_);
		break;
	case CAR_TYPE::ACCELERATE_CAR:
		car_ = std::make_unique<AccelerateCar>(InputManager::JOYPAD_NO::PAD1, *stage_);
		break;
	case CAR_TYPE::SPEED_CAR:
		car_ = std::make_unique<SpeedCar>(InputManager::JOYPAD_NO::PAD1, *stage_);
		break;
	}

	car_->Init(carType,carPos);

	camera_ = std::make_unique<Camera>();
	camera_->Init();
	camera_->SetFollowTarget(&car_->GetTransform());

	car_->Update();

	SoundManager& soundIns = SoundManager::GetInstance();
	soundIns.PlaySound(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3", true);
	soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);

}

void TimeAttackStartScene::Update(void)
{

	camera_->Update();

	if (camera_->GetTime() >= 9.0f)
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TIMEATTACKGAME, true);
	}

	//�X�J�C�h�[����Ǐ]������
	skyDome_->SetFollowTarget(&car_->GetTransform());
	skyDome_->Update();

	stage_->Update();

}

void TimeAttackStartScene::Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);
	// ��ʂ�������
	ClearDrawScreen();

	//�`��
	camera_->SetBeforeDraw();

	skyDome_->Draw();

	car_->Draw();

	stage_->Draw();

}
