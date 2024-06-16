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

#pragma region 定数宣言
//このクラスでしか使わない
//メーター回転の制限範囲
constexpr float MAX_LIMIT_RANGE = 250.0f;

//UIの高さ、幅
constexpr int UI_HEIGHT = 220;
constexpr int UI_WIDTH = 110;

//UIサイズ
constexpr float UI_SIZE = 0.4f;

//回転の中心座標
constexpr int ROT_CENTER_X = 57;
constexpr int ROT_CENTER_Y =26;

//始まりの時間計測UIのX、Yのオフセット座標
constexpr int TIME_UI_OFFSET_X =70;
constexpr int TIME_UI_OFFSET_Y =100;

//スコアタイムXのオフセット座標
constexpr int SCORE_TIME_UI_OFFSET_X =100;

//スコアタイムY座標
constexpr int SCORE_TIME_UI_Y =100;

//ギアUIのX、Y座標
constexpr int GEAR_UI_OFFSET_X =350;
constexpr int GEAR_UI_OFFSET_Y =300;

//ミニマップUIのX、Y座標
constexpr int MINIMAP_UI_POS_X =150;
constexpr int MINIMAP_UI_POS_Y =150;

//メーター、ニードルUIのX、Y座標
constexpr int METER_NEEDLE_UI_X = Application::SCREEN_SIZE_X - 300;
constexpr int METER_NEEDLE_UI_Y = Application::SCREEN_SIZE_Y - 150;

//スタートタイムの初期値
constexpr float START_TIME = 3.0f;

//ミニマップにポジション合わせるための定数
constexpr float MINIMAP_MATCH_SIZE = 400.0f;

//スピード表示座標X
constexpr int SPEED_FORMAT_POS_X = 100;

//スピード表示座標Y
constexpr int SPEED_FORMAT_POS_Y = 600;

//ブレンド率
constexpr int BLEND_PARAMETER = 50;

//ブレンド時間
constexpr float BLEND_TIME = 0.5f;

//ギアを変えるタイミング指示用時間
constexpr float GEAR_CHANGE_TIMING = 5.0f;

//集中線を出し始めるスピード
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
	//メモリ開放
	for (auto& imageInfo : imageInfos_)
	{
		DeleteGraph(imageInfos_[imageInfo.first]);
	}

}

void TimeAttackGameScene::Init(void)
{
	//画像初期化
	InitImageHandle();

	//ステージオブジェクト初期化
	InitStageObjects();

	//選択した車の初期化
	InitSelectCar();
	
	//カメラの初期化
	InitCamera();

	//当たり判定用モデルハンドルを追加
	SetupCollision();

}

void TimeAttackGameScene::Update(void)
{
	stepStartTime_ -= SceneManager::GetInstance().GetDeltaTime();

	//カウントダウン中は動けなくする
	if (stepStartTime_ > 0)
	{
		return;
	}

	//ゴールしたら
	if (car_->IsHitGoal())
	{
		ProcessGoalCollision();
	}
	else
	{
		scoreTime_ += SceneManager::GetInstance().GetDeltaTime();
	}

	//オブジェクト更新
	UpdateObject();

}

void TimeAttackGameScene::Draw(void)
{

	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	//描画
	camera_->SetBeforeDraw();

	skyDome_->SetFollowTarget(&car_->GetTransform());
	skyDome_->Update();

	auto carNowSpeed = car_->GetSpeed();
	auto carNowGear = car_->GetGearNum();

	//オブジェクト描画
	DrawGame();

	//Ui描画
	DrawUi(carNowSpeed, carNowGear);

	//スコア描画
	DrawScore();

	//タコメーター描画
	DrawTachometer();

	//ミニマップ描画
	DrawMiniMap();

	//スピード描画
	DrawSpeedAndGear(carNowSpeed, carNowGear);

	//ニードル描画
	DrawNeedle(carNowSpeed, carNowGear);

	//スピードが一定以上なら集中線を出す
	if (SPEED_OVER_START_LINE < carNowSpeed)
	{
		DrawLine();
	}

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

void TimeAttackGameScene::InitSelectCar(void)
{

	//選んだ車のタイプ
	CAR_TYPE carType = SceneManager::GetInstance().GetCarType();

	//車の初期ポジション
	VECTOR carPos = CAR_INIT_POS_;

	//選んだ車によって能力が違うので継承で分けている
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

	car_->Init(carType, carPos);

}

void TimeAttackGameScene::InitCamera(void)
{
	camera_ = std::make_unique<Camera>();
	camera_->Init();

	//カメラ追従ターゲット指定
	camera_->SetFollowTarget(&car_->GetTransform());
}

void TimeAttackGameScene::InitStageObjects(void)
{
	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//ガレージ ステージオブジェクト
	garage_ = std::make_unique<Garage>();
	garage_->Init();
	garage_->SetPos(GARAGE_INIT_POS);
	garage_->Update();

	//ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Init();

}

void TimeAttackGameScene::SetupCollision(void)
{	

	//当たり判定をとるモデル
	car_->AddCol(stage_->GetModelIdRoadCollision());
	car_->AddCol(stage_->GetModelIdGuardRailCollision());
	car_->AddCol(stage_->GetModelIdWallCollision());

}

void TimeAttackGameScene::UpdateObject(void)
{
	car_->Update();

	camera_->Update();

	garage_->Update();

	stage_->Update();
}

void TimeAttackGameScene::ProcessGoalCollision(void)
{

	SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SCORE, true);
	SceneManager::GetInstance().SetScore(scoreTime_);

	//再生ストップ
	SoundManager& soundIns = SoundManager::GetInstance();
	soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock71b.mp3");

}

void TimeAttackGameScene::DrawGame(void)
{

	skyDome_->Draw();

	garage_->Draw();

	car_->Draw();

	stage_->Draw();

}

void TimeAttackGameScene::DrawUi(float nowSpeed, int nowGear)
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


}

void TimeAttackGameScene::DrawTachometer(void)
{
	//タコメーター表示
	DrawRotaGraph(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, UI_SIZE, 0.0f, imageInfos_[IMAGE_TYPE::TACHOMETER], true);

}

void TimeAttackGameScene::DrawSpeedAndGear(float nowSpeed, int nowGear)
{

	//今のギア速の表示
	DrawFormatString(Application::SCREEN_SIZE_X - GEAR_UI_OFFSET_X, Application::SCREEN_SIZE_Y - GEAR_UI_OFFSET_Y, 0xffffff, "%d速", car_->GetGearNum());

	auto maxSpeed = car_->GetMaxSpeedACC(nowGear - 1);

	//スピード表示
	DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0x000000, "%dkm", static_cast<int>(car_->GetSpeed() * 2));

	//ギアを変えるタイミングを赤色表示で指示
	if (nowSpeed >= maxSpeed - GEAR_CHANGE_TIMING && nowSpeed <= maxSpeed)
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0xff0000, "%dkm", static_cast<int>(nowSpeed * 2));
	}

}

void TimeAttackGameScene::DrawNeedle(float nowSpeed, int nowGear)
{

	auto maxSpeed = car_->GetMaxSpeedACC(nowGear - 1);
	auto angle = std::min(nowSpeed * MAX_LIMIT_RANGE / maxSpeed, MAX_LIMIT_RANGE);

	//ニードル描画
	DrawRectRotaGraph2(METER_NEEDLE_UI_X, METER_NEEDLE_UI_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(angle), imageInfos_[IMAGE_TYPE::NEEDLE], true);

}

void TimeAttackGameScene::DrawMiniMap(void)
{
	//ミニマップ表示
	DrawRotaGraph(MINIMAP_UI_POS_X, MINIMAP_UI_POS_Y, 1.0f, AsoUtility::Deg2RadF(-30.0f), imageInfos_[IMAGE_TYPE::MINIMAP], true);

	//位置
	Vector2 pos;
	pos.x = car_->GetPos().x / MINIMAP_MATCH_SIZE;
	pos.y = car_->GetPos().z / MINIMAP_MATCH_SIZE;

	//自機位置表示のための円
	DrawCircle(MINIMAP_UI_POS_X + pos.x, MINIMAP_UI_POS_Y + (-pos.y), 5.0f, 0xffffff, true);
}

void TimeAttackGameScene::DrawScore(void)
{
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - SCORE_TIME_UI_OFFSET_X, 100, 0xffffff, "%.2f", scoreTime_);
}

void TimeAttackGameScene::DrawLine(void)
{
	//集中線
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

