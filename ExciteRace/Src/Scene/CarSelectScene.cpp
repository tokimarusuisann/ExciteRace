#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Camera.h"
#include "../Object/Car.h"
#include "../Object/Garage.h"
#include "CarSelectScene.h"

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�

//�����p�����[�^�[

//�o�����X�J�[�̈ʒu
constexpr VECTOR SELECT_BALANCE_CAR_INIT_POS = { 0.0f,30.0f,0.0f };

//�o�����X�J�[�̃^�C���̈ʒu
constexpr VECTOR SELECT_BARANCE_WHEEL_INIT_POS = { 0.0f,70.0f,0.0f };

//�o�����X�J�[�̏�������
constexpr VECTOR SELECT_BARANCE_CAR_INIT_ROT = { 0.0f,-200.0f * DX_PI_F / 180, 0.0f };

//�����J�[�̈ʒu
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_POS = { 0.0f,40.0f,0.0f };

//�����J�[�̃^�C���̈ʒu
constexpr VECTOR SELECT_ACCELERATE_WHEEL_INIT_POS = { -5.0f,65.0f,13.0f };

//�����J�[�̏�������
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_ROT = { 0.0f,-20.0f * DX_PI_F / 180, 0.0f };

//�����J�[�̑傫��
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_SCALE = { 140.0f,140.0f, 140.0f };

//�����J�[�̃^�C���̑傫��
constexpr VECTOR SELECT_ACCELERATE_CAR_WHEEL_INIT_SCALE = { 1.4f,1.4f, 1.4f };

//�X�s�[�h�J�[�̈ʒu
constexpr VECTOR SELECT_SPEED_CAR_INIT_POS = { 0.0f,30.0f,0.0f };

//�X�s�[�h�J�[�̃^�C���̈ʒu
constexpr VECTOR SELECT_SPEED_WHEEL_INIT_POS = { 0.0f,80.0f,10.0f }; 

//�X�s�[�h�J�[�̏�������
constexpr VECTOR SELECT_SPEED_CAR_INIT_ROT = { 0.0f,-200.0f * DX_PI_F / 180, 0.0f };

//�X�s�[�h�J�[�̑傫��
constexpr VECTOR SELECT_SPEED_CAR_INIT_SCALE = { 1.3f,1.3f, 1.3f };

//�X�s�[�h�J�[�̃^�C���̑傫��
constexpr VECTOR SELECT_SPEED_CAR_WHEEL_INIT_SCALE = { 1.3f,1.3f, 1.3f };

//UI��X�ʒu
constexpr int UI_POSITIONS_X[] = { 300, 700, 1100 };

#pragma endregion

CarSelectScene::CarSelectScene(void)
{
	index_ = 0;
}

CarSelectScene::~CarSelectScene(void)
{
	//���S�C���[�W���
	for (auto& handle : carLogoHandle_)
	{
		DeleteGraph(handle);
	}

	//�C���[�W���
	DeleteGraph(decisionHandle_);
	DeleteGraph(crossHandle_);

	//���f�����
	MV1DeleteModel(transformCar_.modelId_);
	MV1DeleteModel(transformWheel_.modelId_);

}

void CarSelectScene::Init(void)
{

	//Image�̏�����
	InitImage();

	//Model�̏�����
	InitModel();

	garage_ = std::make_unique<Garage>();
	garage_->Init();
	garage_->SetPos(AsoUtility::VECTOR_ZERO);

	camera_ = std::make_unique<Camera>();
	camera_->Init();
	camera_->SetFollowSelectTarget(transformCar_);

	SoundManager& soundIns = SoundManager::GetInstance();
	soundIns.MyPlaySound(Application::PATH_SOUND + "maou_bgm_neorock78.mp3", true);
	soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);

}

void CarSelectScene::Update(void)
{

	//�{�^�����������Ƃ��̏���
	ProcessButtonPush();

	//idx�̐���
	LimitIndex();

	//�p�����[�^�[��ݒ�
	SetParam();

	camera_->Update();

	garage_->Update();

}

void CarSelectScene::InitImage(void)
{

	auto& resIns = ResourceManager::GetInstance();

	carLogoHandle_.emplace_back(resIns.Load(ResourceManager::SRC::SELECT_BALANCE_CAR_LOGO).handleId_);
	carLogoHandle_.emplace_back(resIns.Load(ResourceManager::SRC::SELECT_ACCELERATE_CAR_LOGO).handleId_);
	carLogoHandle_.emplace_back(resIns.Load(ResourceManager::SRC::SELECT_SPEED_CAR_LOGO).handleId_);

	decisionHandle_ = resIns.Load(ResourceManager::SRC::DECISION_BUTTON_LOGO).handleId_;

	crossHandle_ = resIns.Load(ResourceManager::SRC::CROSS_BUTTON_LOGO).handleId_;

}

void CarSelectScene::InitModel(void)
{

	auto& resIns = ResourceManager::GetInstance();

	//�ԃ��f��
	carModelIds_[static_cast<int>(CAR_TYPE::BALANCE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::BALANCE_CAR);
	carModelIds_[static_cast<int>(CAR_TYPE::ACCELERATE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::ACCELERATE_CAR);
	carModelIds_[static_cast<int>(CAR_TYPE::SPEED_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SPEED_CAR);

	//�������f���p�����[�^
	transformCar_.SetModel(carModelIds_[index_]);
	transformCar_.pos_ = SELECT_BALANCE_CAR_INIT_POS;
	transformCar_.scl_ = AsoUtility::VECTOR_ONE;
	transformCar_.quaRot_ = Quaternion::Euler(SELECT_BARANCE_CAR_INIT_ROT);
	transformCar_.Update();

	//�^�C�����f��
	wheelModelIds_[static_cast<int>(CAR_TYPE::BALANCE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_BALANCE_CAR_WHEEL);
	wheelModelIds_[static_cast<int>(CAR_TYPE::ACCELERATE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_ACCELERATE_CAR_WHEEL);
	wheelModelIds_[static_cast<int>(CAR_TYPE::SPEED_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_SPEED_CAR_WHEEL);

	//�������f���p�����[�^
	transformWheel_.SetModel(wheelModelIds_[index_]);
	transformWheel_.pos_ = SELECT_BARANCE_WHEEL_INIT_POS;
	transformWheel_.scl_ = AsoUtility::VECTOR_ONE;
	transformWheel_.quaRot_ = SELECT_BARANCE_CAR_INIT_ROT;
	transformWheel_.Update();

}

void CarSelectScene::ProcessButtonPush(void)
{

	InputManager& inputIns = InputManager::GetInstance();
	SceneManager& sceneIns = SceneManager::GetInstance();
	SoundManager& soundIns = SoundManager::GetInstance();

	bool isTrgRight = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::CROSS_RIGHT);
	bool isTrgLeft = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::CROSS_LEFT);

	if (isTrgRight)
	{
		index_++;
	}

	if (isTrgLeft)
	{
		index_--;
	}

	if (isTrgRight || isTrgLeft)
	{
		soundIns.MyPlaySound(Application::PATH_SOUND + "button04a.mp3", false);

		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
	}

	if (inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		sceneIns.SetCarType(static_cast<CAR_TYPE>(index_));
		sceneIns.ChangeScene(SceneManager::SCENE_ID::TIMEATTACKGAMESTART, true);
		//�Đ�
		soundIns.MyPlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		//����
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
		//��~
		soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock78.mp3");
	}

}

void CarSelectScene::LimitIndex(void)
{

	if (index_ >= MAX_INDEX)
	{
		index_ = static_cast<int>(CAR_TYPE::BALANCE_CAR);
	}

	if (index_ < MIN_INDEX)
	{
		index_ = static_cast<int>(CAR_TYPE::SPEED_CAR);
	}

}

void CarSelectScene::SetParam(void)
{

	CarInitParams carParams[] = {
		// BALANCE_CAR
		{
		SELECT_BALANCE_CAR_INIT_POS, AsoUtility::VECTOR_ONE, SELECT_BARANCE_CAR_INIT_ROT,
		SELECT_BARANCE_WHEEL_INIT_POS, AsoUtility::VECTOR_ONE
		},
		// ACCELERATE_CAR
		{
		SELECT_ACCELERATE_CAR_INIT_POS, SELECT_ACCELERATE_CAR_INIT_SCALE, SELECT_ACCELERATE_CAR_INIT_ROT,
		SELECT_ACCELERATE_WHEEL_INIT_POS, SELECT_ACCELERATE_CAR_WHEEL_INIT_SCALE
		},
		// SPEED_CAR
		{
		SELECT_SPEED_CAR_INIT_POS, SELECT_SPEED_CAR_INIT_SCALE, SELECT_SPEED_CAR_INIT_ROT,
		SELECT_SPEED_WHEEL_INIT_POS, SELECT_SPEED_CAR_WHEEL_INIT_SCALE
		}
	};

	SetParamCar(index_, carParams[index_].carPos_, carParams[index_].carScale_, carParams[index_].carRot_);
	
	SetParamWheel(index_, carParams[index_].wheelPos_, carParams[index_].wheelScale_, carParams[index_].carRot_);

}

void CarSelectScene::SetParamCar(int idx , VECTOR pos, VECTOR scl, VECTOR rot)
{

	//���f���Z�b�g
	transformCar_.SetModel(carModelIds_[idx]);

	//�ʒu�Z�b�g
	transformCar_.pos_ = pos;

	//�傫���Z�b�g
	transformCar_.scl_ = scl;

	//��]�Z�b�g
	transformCar_.quaRot_ = Quaternion::Euler(rot);

	//�X�V
	transformCar_.Update();

}

void CarSelectScene::SetParamWheel(int idx, VECTOR pos, VECTOR scl, VECTOR rot)
{

	//���f���Z�b�g
	transformWheel_.SetModel(wheelModelIds_[idx]);

	//�ʒu�Z�b�g
	transformWheel_.pos_ = pos;

	//�傫���Z�b�g
	transformWheel_.scl_ = scl;

	//��]�Z�b�g
	transformWheel_.quaRot_ = rot;

	//�X�V
	transformWheel_.Update();

}

void CarSelectScene::Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);
	//��ʂ�������
	ClearDrawScreen();

	camera_->SetBeforeDraw();

	garage_->Draw();

	MV1DrawModel(transformCar_.modelId_);
	MV1DrawModel(transformWheel_.modelId_);

	DrawBox(0, 600, Application::SCREEN_SIZE_X, 780, 0xaaaaaa, true);
	DrawBox(0, 600, Application::SCREEN_SIZE_X, 780, 0xffff00, false);

	auto size = carLogoHandle_.size();
	for (int i = 0 ; i < size ; i++)
	{
		DrawRotaGraph(300+(i*400),Application::SCREEN_SIZE_Y - 80,1.0f, 0.0f, carLogoHandle_[i], true);
	}

	DrawRotaGraph(Application::SCREEN_SIZE_X-70, Application::SCREEN_SIZE_Y-40, 0.7f, 0.0f,decisionHandle_, true);
	DrawRotaGraph(70, Application::SCREEN_SIZE_Y-40, 0.3f, 0.0f, crossHandle_, true);


	if (index_ >= MIN_INDEX && index_ < MAX_INDEX)
	{
		DrawRotaGraph(UI_POSITIONS_X[index_], Application::SCREEN_SIZE_Y - 80, 1.3f, 0.0f, carLogoHandle_[index_], true);
	}

}
