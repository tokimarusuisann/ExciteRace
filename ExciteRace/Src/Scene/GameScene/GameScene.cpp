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

#pragma endregion

GameScene::GameScene(void)
{

	delta_ = 0.0f;

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

	//�PP�ƂQP�̃|�W�V����
	std::vector<VECTOR> vecPos;
	vecPos.emplace_back(CAR_INIT_POS_1);
	vecPos.emplace_back(CAR_INIT_POS_2);

	//�PP�ƂQP�̎Ԃ̃^�C�v
	std::vector<CAR_TYPE> carType;
	carType.emplace_back(CAR_TYPE::BALANCE_CAR);
	carType.emplace_back(CAR_TYPE::BALANCE_CAR);

	//�PP�ƂQP�̃p�b�h�ԍ�
	std::vector<InputManager::JOYPAD_NO> padNo;
	padNo.emplace_back(InputManager::JOYPAD_NO::PAD1);
	padNo.emplace_back(InputManager::JOYPAD_NO::PAD2);

	//�X�N���[�����Q�쐬
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));

	//�X�N���[���̃|�W�V����
	screenPos_.emplace_back(Vector2(0, 0));
	screenPos_.emplace_back(Vector2(Application::SCREEN_SIZE_X / 2, 0));

	//�摜�n���h��������
	InitImageHandle();

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

	auto playerNum = SceneManager::GetInstance().GetPlayerNum();

	for (int i = 0; i < playerNum; i++)
	{
		cameras_.emplace_back(std::make_unique<Camera>());
		cars_.emplace_back(std::make_unique<BalanceCar >(padNo[i], *stage_));

		cameras_[i]->SetFollowTarget(&cars_[i]->GetTransform());
		cameras_[i]->Init();

		//�Ԃ̏����ʒu�ƃ^�C�v��n��
		cars_[i]->Init(carType[i], vecPos[i]);
	}

	//�����蔻��̂��߂Ƀ��f���n���h����n��
	for (auto& car : cars_)
	{
		car->AddCol(stage_->GetModelIdRoadCollision());
		car->AddCol(stage_->GetModelIdGuardRailCollision());
		car->AddCol(stage_->GetModelIdWallCollision());
		car->AddCol(stage_->GetModelIdGoalCollision());
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

void GameScene::Update(void)
{
	stepStartTime_ -= SceneManager::GetInstance().GetDeltaTime();

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
			//�Ԃ��ꏏ�������������x
			if (car1P == car2P)
			{
				continue;
			}

			//�PP�J�v�Z���̏㉺
			VECTOR car1PCapsuleTop = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z + CAR_OFFSET_Z };
			VECTOR car1PCapsuleBot = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z - CAR_OFFSET_Z };

			//�QP�J�v�Z���̏㉺
			VECTOR car2PCapsuleTop = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z + CAR_OFFSET_Z };
			VECTOR car2PCapsuleBot = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z - CAR_OFFSET_Z };

			//�ԓ��m�̏Փˏ���
			if (AsoUtility::IsHitCapsuleToSphere(car1PCapsuleBot, car1PCapsuleTop, Car::COLLISION_RADIUS, car2PCapsuleTop , Car::COLLISION_RADIUS))
			{
				//�Փˌv�Z���ăZ�b�g
				car1P->SetCollisionPow(Collision1PTo2P(car1P->GetPos(), car2P->GetPos()));

				//�R���g���[���[�U��
				ControllerVibration();
			}
			if (AsoUtility::IsHitCapsuleToSphere(car2PCapsuleBot, car2PCapsuleTop, Car::COLLISION_RADIUS, car1P->GetPosTop(), Car::COLLISION_RADIUS))
			{
				//�Փˌv�Z���ăZ�b�g
				car2P->SetCollisionPow(Collision1PTo2P( car2P->GetPos(), car1P->GetPos()));

				//�R���g���[���[�U��
				ControllerVibration();
			}
		}
		//�S�[���Ƃ̓����蔻��
		if (car1P->IsHitGoal())
		{
			//�V�[���J��
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true);
			SceneManager::GetInstance().SetPadNo(car1P->GetPadNo());

			//�Đ��X�g�b�v
			SoundManager& soundIns = SoundManager::GetInstance();
			soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3");
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

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
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

		if (stepStartTime_ >= 1.0f)
		{
			DrawFormatString((Application::SCREEN_SIZE_X / 2)/2, Application::SCREEN_SIZE_Y / 2 - 100, 0xffffff, "%d", static_cast<int>(stepStartTime_));
		}
		if (stepStartTime_ <= 1.0f && stepStartTime_ >= 0.0f)
		{
			DrawRotaGraph((Application::SCREEN_SIZE_X / 2)/2, Application::SCREEN_SIZE_Y / 2, 0.6f, 0.0f, imageInfos_[IMAGE_TYPE::START_SIGN], true);
		}

		SetFontSize(64);

		auto carNowSpeed = cars_[i]->GetSpeed();
		auto carNowGear = cars_[i]->GetGearNum();

		//���̃M�A���̕\��
		DrawFormatString(Application::SCREEN_SIZE_X / 2 - 100, Application::SCREEN_SIZE_Y - 300, 0xffffff, "%d��",carNowGear);

		//�X�s�[�h�\��
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0x000000, "%dkm", static_cast<int>(carNowSpeed * 2));

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
		for (int j = 0 ; j < size ; j++)
		{
			//�ʒu
			Vector2 pos;
			pos.x = cars_[j]->GetPos().x / MINIMAP_MATCH_SIZE;
			pos.y = cars_[j]->GetPos().z / MINIMAP_MATCH_SIZE;

			//���@�ʒu�\���̂��߂̉~
			unsigned int color = 0xff0000;
			if (i == j)
			{
				color = 0xffffff;
			}
			DrawCircle(MINIMAP_UI_POS_X + pos.x, MINIMAP_UI_POS_Y + (-pos.y), CIRCLE_RADIUS, color, true);
		}

		delta_ += SceneManager::GetInstance().GetDeltaTime();

		auto num = 0.0f;

		//�M�A�y�т��̎��̏���
		if(carNowSpeed <= 30.0f&&carNowGear == 1)
		{
			num = carNowSpeed * MAX_LIMIT_RANGE / 30.0f;
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed > 30.0f &&carNowGear == 1)
		{
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed <= 50.0f &&carNowGear == 2)
		{
			num = carNowSpeed * MAX_LIMIT_RANGE / 50.0f;
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed > 50.0f &&carNowGear == 2)
		{
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed <= 70.0f &&carNowGear == 3)
		{
			num = carNowSpeed * MAX_LIMIT_RANGE / 70.0f;
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed > 70.0f &&carNowGear == 3)
		{
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed <= 85.0f &&carNowGear == 4)
		{
			num = carNowSpeed * MAX_LIMIT_RANGE / 85.0f;
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed > 85.0f &&carNowGear == 4)
		{
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}
		if (carNowSpeed <= 100.0f &&carNowGear == 5)
		{
			num = carNowSpeed * MAX_LIMIT_RANGE / 100.0f;
			DrawRectRotaGraph2(UI_POS_X,UI_POS_Y,0, 0, UI_WIDTH, UI_HEIGHT,ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num),imageInfos_[IMAGE_TYPE::NEEDLE], true);
		}

		if (carNowSpeed>80.0f)
		{

			//�W����
			if (delta_ >= 0.0f && delta_ <= 0.49f)
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
				DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_1], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if (delta_ >= 0.5f && delta_ <= 0.99f)
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
				DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_2], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if (delta_ >= 1.0f && delta_ <= 1.49f)
			{
				SetDrawBlendMode(DX_BLENDMODE_ADD, 50);
				DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_3], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}
			if (delta_ >= 1.5f)
			{
				delta_ = 0.0f;
			}

		}

		auto red = 0xff0000;

		//�M�A�ς���^�C�~���O���킩��₷�����邽�߂ɑ��x��ԐF��
		if (carNowGear == 1 && carNowSpeed >= 22.5f && carNowSpeed <= 30.0f)
		{
			DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(carNowSpeed * 2));
		}
		if (carNowGear == 2 && carNowSpeed >= 42.5f && carNowSpeed <= 50.0f)
		{
			DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(carNowSpeed * 2));
		}
		if (carNowGear == 3 && carNowSpeed >= 62.5f && carNowSpeed <= 70.0f)
		{
			DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(carNowSpeed * 2));
		}
		if (carNowGear == 4 && carNowSpeed >= 80.0f && carNowSpeed <= 85.0f)
		{
			DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(carNowSpeed * 2));
		}

	}

	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//���ߏ����͌�Ō�����
	}

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

void GameScene::ControllerVibration(void)
{
	
	//�R���g���[���[�U��
	StartJoypadVibration(DX_INPUT_PAD1, VIBRATION_POWER, VIBRATION_TIME, EFFECTINDEX);
	StartJoypadVibration(DX_INPUT_PAD2, VIBRATION_POWER, VIBRATION_TIME, EFFECTINDEX);

}

VECTOR GameScene::Collision1PTo2P(VECTOR pos1, VECTOR pos2)
{
	auto differencePos1ToPos2 = VSub(pos1, pos2);
	differencePos1ToPos2.y = 0.0f;
	differencePos1ToPos2 = VNorm(differencePos1ToPos2);
	differencePos1ToPos2 = VScale(differencePos1ToPos2, COLLISION_POWER);

	return differencePos1ToPos2;
}

