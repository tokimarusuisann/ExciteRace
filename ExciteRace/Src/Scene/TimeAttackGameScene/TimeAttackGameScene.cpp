#include <DxLib.h>
#include "../../Application.h"
#include "../../Utility/AsoUtility.h"
#include "../../Manager/ResourceManager.h"
#include "../../Manager/Camera.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SceneManager.h"
#include "../../Manager/SoundManager.h"
#include "../../Object/Stage.h"
#include "../../Object/Car.h"
#include "../../Object/SkyDome.h"
#include "../../Object/Garage.h"
#include "../../Object/CarType/AccelerateCar.h"
#include "../../Object/CarType/BalanceCar.h"
#include "../../Object/CarType/SpeedCar.h"
#include "TimeAttackGameScene.h"

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�
//���[�^�[��]�̐����͈�
constexpr float MAX_LIMIT_RANGE = 250.0f;

//UI�̍����A��
constexpr int UI_HEIGHT = 220;
constexpr int UI_WIDTH = 110;

//UI�T�C�Y
constexpr float UI_SIZE = 0.4f;

//��]�̒��S���W
constexpr int ROT_CENTER_X = 57;
constexpr int ROT_CENTER_Y =26;

//�n�܂�̎��Ԍv��UI��X�AY�̃I�t�Z�b�g���W
constexpr int TIME_UI_OFFSET_X =70;
constexpr int TIME_UI_OFFSET_Y =100;

//�X�R�A�^�C��X�̃I�t�Z�b�g���W
constexpr int SCORE_TIME_UI_OFFSET_X =100;

//�X�R�A�^�C��Y���W
constexpr int SCORE_TIME_UI_Y =100;

//�M�AUI��X�AY���W
constexpr int GEAR_UI_OFFSET_X =350;
constexpr int GEAR_UI_OFFSET_Y =300;

//�~�j�}�b�vUI��X�AY���W
constexpr int MINIMAP_UI_POS_X =150;
constexpr int MINIMAP_UI_POS_Y =150;

//���[�^�[�A�j�[�h��UI��X�AY���W
constexpr int METER_NEEDLE_UI_X = Application::SCREEN_SIZE_X - 300;
constexpr int METER_NEEDLE_UI_Y = Application::SCREEN_SIZE_Y - 150;

//�X�^�[�g�^�C���̏����l
constexpr float START_TIME = 4.0f;

//�~�j�}�b�v�Ƀ|�W�V�������킹�邽�߂̒萔
constexpr float MINIMAP_MATCH_SIZE = 400.0f;

//�X�s�[�h�\�����WX
constexpr int SPEED_FORMAT_POS_X = 100;

//�X�s�[�h�\�����WY
constexpr int SPEED_FORMAT_POS_Y = 600;

//�u�����h��
constexpr int BLEND_PARAMETER = 50;

//�u�����h����
constexpr float BLEND_TIME = 0.5f;

//�M�A��ς���^�C�~���O�w���p����
constexpr float GEAR_CHANGE_TIMING = 5.0f;

//�W�������o���n�߂�X�s�[�h
constexpr float SPEED_OVER_START_LINE = 80.0f;

#pragma endregion

TimeAttackGameScene::TimeAttackGameScene(void)
{

	delta_ = 0.0f;

	scoreTime_ = 0.0f;

	stepStartTime_ = START_TIME;

}

TimeAttackGameScene::~TimeAttackGameScene(void)
{

	for (auto& imageInfo : imageInfos_)
	{
		DeleteGraph(imageInfos_[imageInfo.first]);
	}

}

void TimeAttackGameScene::Init(void)
{
	//�摜������
	InitImageHandle();

	//������
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	garage_ = std::make_unique<Garage>();
	garage_->Init();
	garage_->SetPos(GARAGE_INIT_POS);
	garage_->Update();

	stage_ = std::make_unique<Stage>();
	stage_->Init();

	//�I�񂾎Ԃ̃^�C�v
	CAR_TYPE carType = SceneManager::GetInstance().GetCarType();

	//�Ԃ̏����|�W�V����
	VECTOR carPos = CAR_INIT_POS_;

	//�I�񂾎Ԃɂ���Ĕ\�͂��Ⴄ�̂Ōp���ŕ����Ă���
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

	//�����蔻����Ƃ郂�f��
	car_->AddCol(stage_->GetModelIdRoadCollision());
	car_->AddCol(stage_->GetModelIdGuardRailCollision());
	car_->AddCol(stage_->GetModelIdWallCollision());

}

void TimeAttackGameScene::InitImageHandle(void)
{

	auto& resIns = ResourceManager::GetInstance();
	imageInfos_[IMAGE_TYPE::TACHOMETER] = resIns.Load(ResourceManager::SRC::TACHOMETER).handleId_;
	imageInfos_[IMAGE_TYPE::NEEDLE] = resIns.Load(ResourceManager::SRC::NEEDLE).handleId_;
	imageInfos_[IMAGE_TYPE::MINIMAP] = resIns.Load(ResourceManager::SRC::MINIMAP).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_4] = resIns.Load(ResourceManager::SRC::LINE_4).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_5] = resIns.Load(ResourceManager::SRC::LINE_5).handleId_;
	imageInfos_[IMAGE_TYPE::LINE_6] = resIns.Load(ResourceManager::SRC::LINE_6).handleId_;
	imageInfos_[IMAGE_TYPE::START_SIGN] = resIns.Load(ResourceManager::SRC::START_SIGN).handleId_;

}

void TimeAttackGameScene::Update(void)
{

	stepStartTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//�J�E���g�_�E�����͓����Ȃ�����
	if (stepStartTime_ > 0)
	{
		return;
	}

	car_->Update();

	//�S�[��������
	if (car_->IsHitGoal())
	{

		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SCORE, true);
		SceneManager::GetInstance().SetScore(scoreTime_);

		//�Đ��X�g�b�v
		SoundManager& soundIns = SoundManager::GetInstance();
		soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3");

	}
	else
	{
		scoreTime_ += SceneManager::GetInstance().GetDeltaTime();
	}

	camera_->Update();

	garage_->Update();

	stage_->Update();


	auto& ins = InputManager::GetInstance();
	if (ins.IsTrgDown(KEY_INPUT_SPACE))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::TITLE, true);
	}

}

void TimeAttackGameScene::Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);

	// ��ʂ�������
	ClearDrawScreen();

	//�`��
	camera_->SetBeforeDraw();

	skyDome_->SetFollowTarget(&car_->GetTransform());
	skyDome_->Update();
	skyDome_->Draw();

	garage_->Draw();

	car_->Draw();

	stage_->Draw();

	auto carNowSpeed = car_->GetSpeed();

	DrawUi();

	DrawUiNeedle(carNowSpeed);

	//�X�s�[�h�����ȏ�Ȃ�W�������o��
	if (SPEED_OVER_START_LINE < carNowSpeed)
	{
		DrawLine();
	}

}

void TimeAttackGameScene::DrawUi(void)
{

	SetFontSize(256);

	if (stepStartTime_ >= 1.0f)
	{
		DrawFormatString(Application::SCREEN_SIZE_X / 2 - TIME_UI_OFFSET_X, Application::SCREEN_SIZE_Y / 2 - TIME_UI_OFFSET_Y, 0xffffff, "%d", static_cast<int>(stepStartTime_));
	}
	if (stepStartTime_ <= 1.0f && stepStartTime_ >= 0.0f)
	{
		DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, 1.0f, 0.0f, imageInfos_[IMAGE_TYPE::START_SIGN], true);
	}

	SetFontSize(64);

	DrawFormatString(Application::SCREEN_SIZE_X / 2 - SCORE_TIME_UI_OFFSET_X, 100, 0xffffff, "%.2f", scoreTime_);

	//���̃M�A���̕\��
	DrawFormatString(Application::SCREEN_SIZE_X - GEAR_UI_OFFSET_X, Application::SCREEN_SIZE_Y - GEAR_UI_OFFSET_Y, 0xffffff, "%d��", car_->GetGearNum());

	auto black = 0x000000;

	//�X�s�[�h�\��
	DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, black, "%dkm", static_cast<int>(car_->GetSpeed() * 2));

	//�^�R���[�^�[�\��
	DrawRotaGraph(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, UI_SIZE, 0.0f, imageInfos_[IMAGE_TYPE::TACHOMETER], true);

	//�~�j�}�b�v�\��
	DrawRotaGraph(MINIMAP_UI_POS_X, MINIMAP_UI_POS_Y, 1.0f, AsoUtility::Deg2RadF(-30.0f), imageInfos_[IMAGE_TYPE::MINIMAP], true);

	//�ʒu
	Vector2 pos;
	pos.x = car_->GetPos().x / MINIMAP_MATCH_SIZE;
	pos.y = car_->GetPos().z / MINIMAP_MATCH_SIZE;

	//���@�ʒu�\���̂��߂̉~
	DrawCircle(MINIMAP_UI_POS_X + pos.x, MINIMAP_UI_POS_Y + (-pos.y), 5.0f, 0xffffff, true);

}

void TimeAttackGameScene::DrawUiNeedle(float nowSpeed)
{
	auto carNowGear = car_->GetGearNum();

	auto num = 0.0f;

	//�M�A�y�т��̎��̏���
	if (nowSpeed <= car_->GetMaxSpeedACC(0) && carNowGear == 1)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / car_->GetMaxSpeedACC(0);
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > car_->GetMaxSpeedACC(0) && carNowGear == 1)
	{
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= car_->GetMaxSpeedACC(1) && carNowGear == 2)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / car_->GetMaxSpeedACC(1);
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > car_->GetMaxSpeedACC(1) && carNowGear == 2)
	{
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= car_->GetMaxSpeedACC(2) && carNowGear == 3)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / car_->GetMaxSpeedACC(2);
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > car_->GetMaxSpeedACC(2) && carNowGear == 3)
	{
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= car_->GetMaxSpeedACC(3) && carNowGear == 4)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / car_->GetMaxSpeedACC(3);
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > car_->GetMaxSpeedACC(3) && carNowGear == 4)
	{
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= car_->GetMaxSpeedACC(4) && carNowGear == 5)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / car_->GetMaxSpeedACC(4);
		DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}

	auto red = 0xff0000;

	//�M�A�ς���^�C�~���O���킩��₷�����邽�߂ɑ��x��ԐF��
	if (carNowGear == 1 && nowSpeed >= car_->GetMaxSpeedACC(0) - GEAR_CHANGE_TIMING && nowSpeed <= car_->GetMaxSpeedACC(0))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (carNowGear == 2 && nowSpeed >= car_->GetMaxSpeedACC(1) - GEAR_CHANGE_TIMING && nowSpeed <= car_->GetMaxSpeedACC(1))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (carNowGear == 3 && nowSpeed >= car_->GetMaxSpeedACC(2) - GEAR_CHANGE_TIMING && nowSpeed <= car_->GetMaxSpeedACC(2))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (carNowGear == 4 && nowSpeed >= car_->GetMaxSpeedACC(3) - GEAR_CHANGE_TIMING && nowSpeed <= car_->GetMaxSpeedACC(3))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}

}

void TimeAttackGameScene::DrawLine(void)
{
	//�W����
	if (delta_ >= 0.0f && delta_ < BLEND_TIME)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_4], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (delta_ >= BLEND_TIME && delta_ < BLEND_TIME * 2)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_5], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (delta_ >= BLEND_TIME * 2 && delta_ <= BLEND_TIME * 3)
	{
		SetDrawBlendMode(DX_BLENDMODE_ADD, BLEND_PARAMETER);
		DrawGraph(0, 0, imageInfos_[IMAGE_TYPE::LINE_6], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	if (delta_ >= BLEND_TIME * 3)
	{
		delta_ = 0.0f;
	}
}

