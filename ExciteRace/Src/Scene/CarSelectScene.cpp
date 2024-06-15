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

#pragma region 定数宣言
//このクラスでしか使わない

//初期パラメーター

//バランスカーの位置
constexpr VECTOR SELECT_BALANCE_CAR_INIT_POS = { 0.0f,30.0f,0.0f };

//バランスカーのタイヤの位置
constexpr VECTOR SELECT_BARANCE_WHEEL_INIT_POS = { 0.0f,70.0f,0.0f };

//バランスカーの初期方向
constexpr VECTOR SELECT_BARANCE_CAR_INIT_ROT = { 0.0f,-200.0f * DX_PI_F / 180, 0.0f };

//加速カーの位置
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_POS = { 0.0f,40.0f,0.0f };

//加速カーのタイヤの位置
constexpr VECTOR SELECT_ACCELERATE_WHEEL_INIT_POS = { -5.0f,65.0f,13.0f };

//加速カーの初期方向
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_ROT = { 0.0f,-20.0f * DX_PI_F / 180, 0.0f };

//加速カーの大きさ
constexpr VECTOR SELECT_ACCELERATE_CAR_INIT_SCALE = { 140.0f,140.0f, 140.0f };

//加速カーのタイヤの大きさ
constexpr VECTOR SELECT_ACCELERATE_CAR_WHEEL_INIT_SCALE = { 1.4f,1.4f, 1.4f };

//スピードカーの位置
constexpr VECTOR SELECT_SPEED_CAR_INIT_POS = { 0.0f,30.0f,0.0f };

//スピードカーのタイヤの位置
constexpr VECTOR SELECT_SPEED_WHEEL_INIT_POS = { 0.0f,80.0f,10.0f }; 

//スピードカーの初期方向
constexpr VECTOR SELECT_SPEED_CAR_INIT_ROT = { 0.0f,-200.0f * DX_PI_F / 180, 0.0f };

//スピードカーの大きさ
constexpr VECTOR SELECT_SPEED_CAR_INIT_SCALE = { 1.3f,1.3f, 1.3f };

//スピードカーのタイヤの大きさ
constexpr VECTOR SELECT_SPEED_CAR_WHEEL_INIT_SCALE = { 1.3f,1.3f, 1.3f };

//UIのX位置
constexpr int UI_POSITIONS_X[] = { 300, 700, 1100 };

#pragma endregion

CarSelectScene::CarSelectScene(void)
{
	index_ = 0;
}

CarSelectScene::~CarSelectScene(void)
{
	//ロゴイメージ解放
	for (auto& handle : carLogoHandle_)
	{
		DeleteGraph(handle);
	}

	//イメージ解放
	DeleteGraph(decisionHandle_);
	DeleteGraph(crossHandle_);

	//モデル解放
	MV1DeleteModel(transformCar_.modelId_);
	MV1DeleteModel(transformWheel_.modelId_);

}

void CarSelectScene::Init(void)
{

	//Imageの初期化
	InitImage();

	//Modelの初期化
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

	//ボタンを押したときの処理
	ProcessButtonPush();

	//idxの制限
	LimitIndex();

	//パラメーターを設定
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

	//車モデル
	carModelIds_[static_cast<int>(CAR_TYPE::BALANCE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::BALANCE_CAR);
	carModelIds_[static_cast<int>(CAR_TYPE::ACCELERATE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::ACCELERATE_CAR);
	carModelIds_[static_cast<int>(CAR_TYPE::SPEED_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SPEED_CAR);

	//初期モデルパラメータ
	transformCar_.SetModel(carModelIds_[index_]);
	transformCar_.pos_ = SELECT_BALANCE_CAR_INIT_POS;
	transformCar_.scl_ = AsoUtility::VECTOR_ONE;
	transformCar_.quaRot_ = Quaternion::Euler(SELECT_BARANCE_CAR_INIT_ROT);
	transformCar_.Update();

	//タイヤモデル
	wheelModelIds_[static_cast<int>(CAR_TYPE::BALANCE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_BALANCE_CAR_WHEEL);
	wheelModelIds_[static_cast<int>(CAR_TYPE::ACCELERATE_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_ACCELERATE_CAR_WHEEL);
	wheelModelIds_[static_cast<int>(CAR_TYPE::SPEED_CAR)] = resIns.LoadModelDuplicate(ResourceManager::SRC::SELECT_SPEED_CAR_WHEEL);

	//初期モデルパラメータ
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
		//再生
		soundIns.MyPlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		//音量
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
		//停止
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

	//モデルセット
	transformCar_.SetModel(carModelIds_[idx]);

	//位置セット
	transformCar_.pos_ = pos;

	//大きさセット
	transformCar_.scl_ = scl;

	//回転セット
	transformCar_.quaRot_ = Quaternion::Euler(rot);

	//更新
	transformCar_.Update();

}

void CarSelectScene::SetParamWheel(int idx, VECTOR pos, VECTOR scl, VECTOR rot)
{

	//モデルセット
	transformWheel_.SetModel(wheelModelIds_[idx]);

	//位置セット
	transformWheel_.pos_ = pos;

	//大きさセット
	transformWheel_.scl_ = scl;

	//回転セット
	transformWheel_.quaRot_ = rot;

	//更新
	transformWheel_.Update();

}

void CarSelectScene::Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);
	//画面を初期化
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
