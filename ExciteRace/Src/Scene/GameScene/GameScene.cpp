#include <cmath>
#include <DxLib.h>
#include "../../Application.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Manager/Camera.h"
#include "../../Utility/AsoUtility.h"
#include "../../Object/Stage.h"
#include "../../Object/Car.h"
#include "../../Object/SkyDome.h"
#include "../../Object/Garage.h"
#include "../../Object/CarType/BalanceCar.h"
#include "GameScene.h"

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�
//�X�^�[�g�^�C���̏����l
constexpr float START_TIME = 4.0f;

//���[�^�[��]�̐����͈�
constexpr float MAX_LIMIT_RANGE = 250.0f;

//UI�̍����A��
constexpr int UI_HEIGHT = 220;
constexpr int UI_WIDTH = 110;

//UI�T�C�Y
constexpr float UI_SIZE = 0.4f;

//��]�̒��S���W
constexpr int ROT_CENTER_X = 57;
constexpr int ROT_CENTER_Y = 26;

//UI��X�AY���W
constexpr int UI_POS_X = Application::SCREEN_SIZE_X / 2 - 150;
constexpr int UI_POS_Y = Application::SCREEN_SIZE_Y - 100;

//�X�s�[�h�\�����WX
constexpr int SPEED_FORMAT_POS_X = 100;

//�X�s�[�h�\�����WY
constexpr int SPEED_FORMAT_POS_Y = 600;

//�~�j�}�b�v�\�����WX
constexpr int MINIMAP_UI_POS_X = 150;

//�~�j�}�b�v�\�����WY
constexpr int MINIMAP_UI_POS_Y = 150;

//�T�[�N���̔��a
constexpr int CIRCLE_RADIUS = 5;

//�~�j�}�b�v�Ƀ|�W�V�������킹�邽�߂̒萔
constexpr float MINIMAP_MATCH_SIZE = 400.0f;

//�o�C�u���[�V�����̋���
constexpr int VIBRATION_POWER = 1000;

//�o�C�u���[�V�����̎���
constexpr int VIBRATION_TIME = 300;

//�U�����������萔
constexpr int EFFECTINDEX = -1;

//�Փ˗�
constexpr float COLLISION_POWER = 40.0f;

//�Ԃ̃|�W�V����Y�I�t�Z�b�g
constexpr float CAR_OFFSET_Y = 70.0f;

//�Ԃ̃|�W�V����Z�I�t�Z�b�g
constexpr float CAR_OFFSET_Z = 160.0;

//�W�������o���n�߂�X�s�[�h
constexpr float SPEED_OVER_START_LINE = 80.0f;

//�u�����h��
constexpr int BLEND_PARAMETER = 50;

//�u�����h����
constexpr float BLEND_TIME = 0.5f;

//�M�A��ς���^�C�~���O�w���p����
constexpr float GEAR_CHANGE_TIMING = 5.0f;

#pragma endregion

GameScene::GameScene(void)
{

	stepTime_ = 0.0f;

	stepStartTime_ = START_TIME;

}

GameScene::~GameScene(void)
{

	//�������J��
	for (auto& imageInfo : imageInfos_)
	{
		DeleteGraph(imageInfos_[imageInfo.first]);
	}

}

void GameScene::Init(void)
{

	//�Ԃ̏����l
	InitCarParam();

	//�J�����̏����l
	InitCameraParam();

	//�X�e�[�W�I�u�W�F�N�g������
	InitStageObjects();

	//�摜�n���h��������
	InitImageHandle();

	int playerNum = SceneManager::GetInstance().GetPlayerNum();

	for (int i = 0; i < playerNum; i++)
	{
		InitPlayerAndCamera(i);
	}

	//�����蔻��p���f���n���h����ǉ�
	SetupCollision();

}

void GameScene::Update(void)
{

	//�J�n���Ԍ���
	stepStartTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//�W�����`��̍ۂɎg�p���鎞��
	stepTime_ += SceneManager::GetInstance().GetDeltaTime();

	//�J�E���g�_�E�����͓����Ȃ�����
	if (stepStartTime_ > 0)
	{
		return;
	}

	for (auto& car1P : cars_)
	{

		car1P->Update();

		for (auto& car2P : cars_)
		{

			//�Ԃ��ꏏ����Ȃ�������
			if (car1P != car2P)
			{
				ProcessCarCollision(car1P, car2P);
			}

		}

		//�S�[���Ƃ̓����蔻��
		if (car1P->IsHitGoal())
		{
			ProcessGoalCollision(car1P);
		}
	}

	for (auto& cameras : cameras_)
	{
		cameras->Update();
	}

	garage_->Update();

	skyDome_->Update();

	stage_->Update();

}

void GameScene::Draw(void)
{

	int playerNum = SceneManager::GetInstance().GetPlayerNum();

	for (int i = 0; i < playerNum; i++)
	{
		SetDrawScreen(cameraScreens_[i]);

		// ��ʂ�������
		ClearDrawScreen();
		
		cameras_[i]->SetBeforeDraw();

		skyDome_->SetFollowTarget(&cars_[i]->GetTransform());
		skyDome_->Update();
		skyDome_->Draw();

		DrawGame();

		SetFontSize(256);

		auto carNowSpeed = cars_[i]->GetSpeed();
		auto carNowGear = cars_[i]->GetGearNum();

		DrawUi(carNowSpeed, carNowGear, i);

		DrawNeedle(carNowSpeed, carNowGear, i);

		//�X�s�[�h�����ȏ�Ȃ�W�������o��
		if (SPEED_OVER_START_LINE < carNowSpeed)
		{
			DrawLine();
		}

	}

	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	for (int i = 0; i < playerNum; i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//���ߏ����͌�Ō�����
	}

}

void GameScene::InitCarParam(void)
{
	//�PP�ƂQP�̃|�W�V����
	vecPos_ = { CAR_INIT_POS_1 ,CAR_INIT_POS_2 };

	//�PP�ƂQP�̎Ԃ̃^�C�v
	carTypes_ = { CAR_TYPE::BALANCE_CAR ,CAR_TYPE::BALANCE_CAR };

	//�PP�ƂQP�̃p�b�h�ԍ�
	padNos_ = { InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_NO::PAD2 };
}

void GameScene::InitCameraParam(void)
{
	auto playerNum = SceneManager::GetInstance().GetPlayerNum();

	int halfScreenWidth = Application::SCREEN_SIZE_X / 2;

	for (int i = 0; i < playerNum; i++)
	{
		//�X�N���[�����Q�쐬
		cameraScreens_.emplace_back(MakeScreen(halfScreenWidth, Application::SCREEN_SIZE_Y, true));

		//�X�N���[���̃|�W�V����
		screenPos_.emplace_back(Vector2(halfScreenWidth * i, 0));
	}
}

void GameScene::InitImageHandle(void)
{
	auto& resIns = ResourceManager::GetInstance();

	imageInfos_[IMAGE_TYPE::TACHOMETER] = resIns.Load(ResourceManager::SRC::TACHOMETER).handleId_;
	imageInfos_[IMAGE_TYPE::NEEDLE] = resIns.Load(ResourceManager::SRC::NEEDLE).handleId_;
	imageInfos_[IMAGE_TYPE::MINIMAP] = resIns.Load(ResourceManager::SRC::MINIMAP).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_1] = resIns.Load(ResourceManager::SRC::LINE_1).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_2] = resIns.Load(ResourceManager::SRC::LINE_2).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_3] = resIns.Load(ResourceManager::SRC::LINE_3).handleId_;
	imageInfos_[IMAGE_TYPE::START_SIGN] = resIns.Load(ResourceManager::SRC::START_SIGN).handleId_;
}

void GameScene::InitStageObjects(void)
{
	//�X�J�C�h�[��
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//�X�e�[�W�I�u�W�F�N�g�@�K���[�W
	garage_ = std::make_unique<Garage>();
	garage_->Init();
	garage_->SetPos(GARAGE_INIT_POS);
	garage_->Update();

	//�X�e�[�W
	stage_ = std::make_unique<Stage>();
	stage_->Init();
}

void GameScene::InitPlayerAndCamera(int index)
{
	cameras_.emplace_back(std::make_unique<Camera>());

	cars_.emplace_back(std::make_unique<BalanceCar>(padNos_[index], *stage_));

	//�J�����Ǐ]����I�u�W�F�N�g�w��
	cameras_[index]->SetFollowTarget(&cars_[index]->GetTransform());

	cameras_[index]->Init();

	//�Ԃ̏����ʒu�ƃ^�C�v��n��
	cars_[index]->Init(carTypes_[index], vecPos_[index]);
}

void GameScene::SetupCollision(void)
{
	for (auto& car : cars_)
	{
		car->AddCol(stage_->GetModelIdRoadCollision());
		car->AddCol(stage_->GetModelIdGuardRailCollision());
		car->AddCol(stage_->GetModelIdWallCollision());
		car->AddCol(stage_->GetModelIdGoalCollision());
	}
}

void GameScene::ProcessCarCollision(std::unique_ptr<Car>& car1P, std::unique_ptr<Car>& car2P)
{
	//�PP�J�v�Z���̏㉺
	VECTOR car1PCapsuleTop = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z + CAR_OFFSET_Z };
	VECTOR car1PCapsuleBot = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z - CAR_OFFSET_Z };

	//�QP�J�v�Z���̏㉺
	VECTOR car2PCapsuleTop = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z + CAR_OFFSET_Z };
	VECTOR car2PCapsuleBot = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z - CAR_OFFSET_Z };

	//�ԓ��m�̏Փˏ���
	if (AsoUtility::IsHitCapsuleToSphere(car1PCapsuleBot, car1PCapsuleTop, Car::COLLISION_RADIUS, car2PCapsuleTop, Car::COLLISION_RADIUS))
	{
		//�Փˌv�Z���ăZ�b�g
		car1P->SetCollisionPow(CalcCollisionCar(car1P->GetPos(), car2P->GetPos()));

		//�R���g���[���[�U��
		ControllerVibration();
	}
	if (AsoUtility::IsHitCapsuleToSphere(car2PCapsuleBot, car2PCapsuleTop, Car::COLLISION_RADIUS, car1P->GetPosTop(), Car::COLLISION_RADIUS))
	{
		//�Փˌv�Z���ăZ�b�g
		car2P->SetCollisionPow(CalcCollisionCar(car2P->GetPos(), car1P->GetPos()));

		//�R���g���[���[�U��
		ControllerVibration();
	}
}

void GameScene::ProcessGoalCollision(std::unique_ptr<Car>& car)
{
	//�V�[���J��
	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true);
	SceneManager::GetInstance().SetPadNo(car->GetPadNo());

	//�Đ��X�g�b�v
	SoundManager& soundIns = SoundManager::GetInstance();
	soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3");
}

void GameScene::DrawGame(void)
{

	garage_->Draw();

	for (auto& cars : cars_)
	{
		cars->Draw();
	}

	stage_->Draw();

}

void GameScene::DrawUi(float nowSpeed, int nowGear, int nowCarNum)
{

	if (stepStartTime_ >= 1.0f)
	{
		DrawFormatString((Application::SCREEN_SIZE_X / 2) / 2, Application::SCREEN_SIZE_Y / 2 - 100, 0xffffff, "%d", static_cast<int>(stepStartTime_));
	}

	if (stepStartTime_ <= 1.0f && stepStartTime_ >= 0.0f)
	{
		DrawRotaGraph((Application::SCREEN_SIZE_X / 2) / 2, Application::SCREEN_SIZE_Y / 2, 0.6f, 0.0f, imageInfos_[IMAGE_TYPE::START_SIGN], true);
	}

	SetFontSize(64);

	//���̃M�A���̕\��
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 100, Application::SCREEN_SIZE_Y - 300, 0xffffff, "%d��", nowGear);

	//�X�s�[�h�\��
	DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0x000000, "%dkm", static_cast<int>(nowSpeed * 2));

	//�^�R���[�^�[�\��
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - 150.0f, Application::SCREEN_SIZE_Y - 100.0f, UI_SIZE, 0.0f, imageInfos_[IMAGE_TYPE::TACHOMETER], true);

	//�~�j�}�b�v�\��
	DrawRotaGraph(MINIMAP_UI_POS_X, MINIMAP_UI_POS_Y, 1.0f, AsoUtility::Deg2RadF(-30.0f), imageInfos_[IMAGE_TYPE::MINIMAP], true);

	//�~�j�}�b�v�ɕ\�����鎩�@�y�ѐF�ݒ�
	std::vector<unsigned int> colors;

	//�F
	colors.emplace_back(0xffffff);
	colors.emplace_back(0xff0000);

	auto size = cars_.size();
	for (int carNum = 0; carNum < size; carNum++)
	{
		//�ʒu
		Vector2 pos;
		pos.x = cars_[carNum]->GetPos().x / MINIMAP_MATCH_SIZE;
		pos.y = cars_[carNum]->GetPos().z / MINIMAP_MATCH_SIZE;

		//���@�ʒu�\���̂��߂̉~
		unsigned int color = 0xff0000;
		if (nowCarNum == carNum)
		{
			color = 0xffffff;
		}
		DrawCircle(MINIMAP_UI_POS_X + pos.x, MINIMAP_UI_POS_Y + (-pos.y), CIRCLE_RADIUS, color, true);
	}
}

void GameScene::DrawNeedle(float nowSpeed, int nowGear, int nowCarNum)
{

	//�֐��I�u�W�F�N�g
	auto DrawNeedleForGear = [&](int gear) {

		//�M�A���Ƃ̍ő呬�x
		float maxSpeed = cars_[nowCarNum]->GetMaxSpeedACC(gear - 1);

		//��Ōv�Z�ŏo���l�Ɋ�Â��Đj�̉�]�p�x
		float rot = (nowSpeed <= maxSpeed) ? nowSpeed * MAX_LIMIT_RANGE / maxSpeed : MAX_LIMIT_RANGE;

		DrawRectRotaGraph2(
			UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT,
			ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE,
			AsoUtility::Deg2RadF(rot),
			imageInfos_[IMAGE_TYPE::NEEDLE], true
		);
		};

	if (nowGear >= 1 && nowGear <= 5)
	{
		DrawNeedleForGear(nowGear);
	}

	// �M�A�ύX�^�C�~���O��ԐF�ŕ\��
	auto DrawSpeedRed = [&](int gear, float maxSpeed) {
		if (nowSpeed >= maxSpeed - GEAR_CHANGE_TIMING && nowSpeed <= maxSpeed)
		{
			DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0xff0000, "%dkm", static_cast<int>(nowSpeed * 2));
		}
		};

	if (nowGear >= 1 && nowGear <= 4)
	{
		DrawSpeedRed(nowGear, cars_[nowCarNum]->GetMaxSpeedACC(nowGear - 1));
	}

}

void GameScene::DrawLine(void)
{	
	
	//�W����
	if (stepTime_ >= 0.0f && stepTime_ < BLEND_TIME)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_1], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (stepTime_ >= BLEND_TIME && stepTime_ < BLEND_TIME * 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_2], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (stepTime_ >= BLEND_TIME * 2 && stepTime_ <= BLEND_TIME * 3)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_3], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (stepTime_ >= BLEND_TIME * 3)
	{
		stepTime_ = 0.0f;
	}
}

void GameScene::ControllerVibration(void)
{
	
	//�R���g���[���[�U��
	StartJoypadVibration(DX_INPUT_PAD1, VIBRATION_POWER, VIBRATION_TIME, EFFECTINDEX);
	StartJoypadVibration(DX_INPUT_PAD2, VIBRATION_POWER, VIBRATION_TIME, EFFECTINDEX);

}

VECTOR GameScene::CalcCollisionCar(VECTOR pos1, VECTOR pos2)
{
	auto differencePos1ToPos2 = VSub(pos1, pos2);
	differencePos1ToPos2.y = 0.0f;
	differencePos1ToPos2 = VNorm(differencePos1ToPos2);
	differencePos1ToPos2 = VScale(differencePos1ToPos2, COLLISION_POWER);

	return differencePos1ToPos2;
}

