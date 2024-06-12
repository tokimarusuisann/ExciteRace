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

#pragma region 定数宣言
//このクラスでしか使わない
//スタートタイムの初期値
constexpr float START_TIME = 4.0f;

//メーター回転の制限範囲
constexpr float MAX_LIMIT_RANGE = 250.0f;

//UIの高さ、幅
constexpr int UI_HEIGHT = 220;
constexpr int UI_WIDTH = 110;

//UIサイズ
constexpr float UI_SIZE = 0.4f;

//回転の中心座標
constexpr int ROT_CENTER_X = 57;
constexpr int ROT_CENTER_Y = 26;

//UIのX、Y座標
constexpr int UI_POS_X = Application::SCREEN_SIZE_X / 2 - 150;
constexpr int UI_POS_Y = Application::SCREEN_SIZE_Y - 100;

//スピード表示座標X
constexpr int SPEED_FORMAT_POS_X = 100;

//スピード表示座標Y
constexpr int SPEED_FORMAT_POS_Y = 600;

//ミニマップ表示座標X
constexpr int MINIMAP_UI_POS_X = 150;

//ミニマップ表示座標Y
constexpr int MINIMAP_UI_POS_Y = 150;

//サークルの半径
constexpr int CIRCLE_RADIUS = 5;

//ミニマップにポジション合わせるための定数
constexpr float MINIMAP_MATCH_SIZE = 400.0f;

//バイブレーションの強さ
constexpr int VIBRATION_POWER = 1000;

//バイブレーションの時間
constexpr int VIBRATION_TIME = 300;

//振動自動処理定数
constexpr int EFFECTINDEX = -1;

//衝突力
constexpr float COLLISION_POWER = 40.0f;

//車のポジションYオフセット
constexpr float CAR_OFFSET_Y = 70.0f;

//車のポジションZオフセット
constexpr float CAR_OFFSET_Z = 160.0;

//集中線を出し始めるスピード
constexpr float SPEED_OVER_START_LINE = 80.0f;

//ブレンド率
constexpr int BLEND_PARAMETER = 50;

//ブレンド時間
constexpr float BLEND_TIME = 0.5f;

//ギアを変えるタイミング指示用時間
constexpr float GEAR_CHANGE_TIMING = 5.0f;

#pragma endregion

GameScene::GameScene(void)
{

	stepTime_ = 0.0f;

	stepStartTime_ = START_TIME;

}

GameScene::~GameScene(void)
{

	//メモリ開放
	for (auto& imageInfo : imageInfos_)
	{
		DeleteGraph(imageInfos_[imageInfo.first]);
	}

}

void GameScene::Init(void)
{

	//１Pと２Pのポジション
	std::vector<VECTOR> vecPos;
	vecPos.emplace_back(CAR_INIT_POS_1);
	vecPos.emplace_back(CAR_INIT_POS_2);

	//１Pと２Pの車のタイプ
	std::vector<CAR_TYPE> carType;
	carType.emplace_back(CAR_TYPE::BALANCE_CAR);
	carType.emplace_back(CAR_TYPE::BALANCE_CAR);

	//１Pと２Pのパッド番号
	std::vector<InputManager::JOYPAD_NO> padNo;
	padNo.emplace_back(InputManager::JOYPAD_NO::PAD1);
	padNo.emplace_back(InputManager::JOYPAD_NO::PAD2);

	//スクリーンを２つ作成
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));
	cameraScreens_.emplace_back(MakeScreen(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y, true));

	//スクリーンのポジション
	screenPos_.emplace_back(Vector2(0, 0));
	screenPos_.emplace_back(Vector2(Application::SCREEN_SIZE_X / 2, 0));

	//画像ハンドル初期化
	InitImageHandle();

	//スカイドーム
	skyDome_ = std::make_unique<SkyDome>();
	skyDome_->Init();

	//ステージオブジェクト　ガレージ
	garage_ = std::make_unique<Garage>();
	garage_->Init();
	garage_->SetPos(GARAGE_INIT_POS);
	garage_->Update();

	//ステージ
	stage_ = std::make_unique<Stage>();
	stage_->Init();

	auto playerNum = SceneManager::GetInstance().GetPlayerNum();

	for (int i = 0; i < playerNum; i++)
	{
		cameras_.emplace_back(std::make_unique<Camera>());
		cars_.emplace_back(std::make_unique<BalanceCar >(padNo[i], *stage_));

		cameras_[i]->SetFollowTarget(&cars_[i]->GetTransform());
		cameras_[i]->Init();

		//車の初期位置とタイプを渡す
		cars_[i]->Init(carType[i], vecPos[i]);
	}

	//当たり判定のためにモデルハンドルを渡す
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

	stepTime_ += SceneManager::GetInstance().GetDeltaTime();

	//カウントダウン中は動けなくする
	if (stepStartTime_ > 0)
	{
		return;
	}

	for (auto& car1P : cars_)
	{
		car1P->Update();

		for (auto& car2P : cars_)
		{
			//車が一緒だったらもう一度
			if (car1P == car2P)
			{
				continue;
			}

			//１Pカプセルの上下
			VECTOR car1PCapsuleTop = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z + CAR_OFFSET_Z };
			VECTOR car1PCapsuleBot = { car1P->GetPos().x, car1P->GetPos().y + CAR_OFFSET_Y ,car1P->GetPos().z - CAR_OFFSET_Z };

			//２Pカプセルの上下
			VECTOR car2PCapsuleTop = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z + CAR_OFFSET_Z };
			VECTOR car2PCapsuleBot = { car2P->GetPos().x, car2P->GetPos().y + CAR_OFFSET_Y ,car2P->GetPos().z - CAR_OFFSET_Z };

			//車同士の衝突処理
			if (AsoUtility::IsHitCapsuleToSphere(car1PCapsuleBot, car1PCapsuleTop, Car::COLLISION_RADIUS, car2PCapsuleTop , Car::COLLISION_RADIUS))
			{
				//衝突計算してセット
				car1P->SetCollisionPow(Collision1PTo2P(car1P->GetPos(), car2P->GetPos()));

				//コントローラー振動
				ControllerVibration();
			}
			if (AsoUtility::IsHitCapsuleToSphere(car2PCapsuleBot, car2PCapsuleTop, Car::COLLISION_RADIUS, car1P->GetPosTop(), Car::COLLISION_RADIUS))
			{
				//衝突計算してセット
				car2P->SetCollisionPow(Collision1PTo2P( car2P->GetPos(), car1P->GetPos()));

				//コントローラー振動
				ControllerVibration();
			}
		}
		//ゴールとの当たり判定
		if (car1P->IsHitGoal())
		{
			//シーン遷移
			SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::RESULT, true);
			SceneManager::GetInstance().SetPadNo(car1P->GetPadNo());

			//再生ストップ
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

		// 画面を初期化
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

		//スピードが一定以上なら集中線を出す
		if (SPEED_OVER_START_LINE < carNowSpeed)
		{
			DrawLine();
		}

	}

	SetDrawScreen(DX_SCREEN_BACK);

	// 画面を初期化
	ClearDrawScreen();

	for (int i = 0; i < SceneManager::GetInstance().GetPlayerNum(); i++)
	{
		DrawGraph(screenPos_[i].x, screenPos_[i].y, cameraScreens_[i], false);//透過処理は後で見直す
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

	//今のギア速の表示
	DrawFormatString(Application::SCREEN_SIZE_X / 2 - 100, Application::SCREEN_SIZE_Y - 300, 0xffffff, "%d速", nowGear);

	//スピード表示
	DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, 0x000000, "%dkm", static_cast<int>(nowSpeed * 2));

	//タコメーター表示
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2 - 150.0f, Application::SCREEN_SIZE_Y - 100.0f, UI_SIZE, 0.0f, imageInfos_[IMAGE_TYPE::TACHOMETER], true);

	//ミニマップ表示
	DrawRotaGraph(MINIMAP_UI_POS_X, MINIMAP_UI_POS_Y, 1.0f, AsoUtility::Deg2RadF(-30.0f), imageInfos_[IMAGE_TYPE::MINIMAP], true);

	//ミニマップに表示する自機及び色設定
	std::vector<unsigned int> colors;

	//色
	colors.emplace_back(0xffffff);
	colors.emplace_back(0xff0000);

	auto size = cars_.size();
	for (int carNum = 0; carNum < size; carNum++)
	{
		//位置
		Vector2 pos;
		pos.x = cars_[carNum]->GetPos().x / MINIMAP_MATCH_SIZE;
		pos.y = cars_[carNum]->GetPos().z / MINIMAP_MATCH_SIZE;

		//自機位置表示のための円
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
	auto num = 0.0f;

	//ギア及びその時の処理
	if (nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(0) && nowGear == 1)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / cars_[nowCarNum]->GetMaxSpeedACC(0);
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > cars_[nowCarNum]->GetMaxSpeedACC(0) && nowGear == 1)
	{
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(1) && nowGear == 2)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / cars_[nowCarNum]->GetMaxSpeedACC(1);
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > cars_[nowCarNum]->GetMaxSpeedACC(1) && nowGear == 2)
	{
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(2) && nowGear == 3)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / cars_[nowCarNum]->GetMaxSpeedACC(2);
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > cars_[nowCarNum]->GetMaxSpeedACC(2) && nowGear == 3)
	{
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(3) && nowGear == 4)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / cars_[nowCarNum]->GetMaxSpeedACC(3);
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed > cars_[nowCarNum]->GetMaxSpeedACC(3) && nowGear == 4)
	{
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(MAX_LIMIT_RANGE), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}
	if (nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(4) && nowGear == 5)
	{
		num = nowSpeed * MAX_LIMIT_RANGE / cars_[nowCarNum]->GetMaxSpeedACC(4);
		DrawRectRotaGraph2(UI_POS_X, UI_POS_Y, 0, 0, UI_WIDTH, UI_HEIGHT, ROT_CENTER_X, ROT_CENTER_Y, UI_SIZE, AsoUtility::Deg2RadF(num), imageInfos_[IMAGE_TYPE::NEEDLE], true);
	}

	auto red = 0xff0000;

	//ギア変えるタイミングをわかりやすくするために速度を赤色に
	if (nowGear == 1 && nowSpeed >= cars_[nowCarNum]->GetMaxSpeedACC(0) - GEAR_CHANGE_TIMING && nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(0))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (nowGear == 2 && nowSpeed >= cars_[nowCarNum]->GetMaxSpeedACC(1) - GEAR_CHANGE_TIMING && nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(1))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (nowGear == 3 && nowSpeed >= cars_[nowCarNum]->GetMaxSpeedACC(2) - GEAR_CHANGE_TIMING && nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(2))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
	if (nowGear == 4 && nowSpeed >= cars_[nowCarNum]->GetMaxSpeedACC(3) - GEAR_CHANGE_TIMING && nowSpeed <= cars_[nowCarNum]->GetMaxSpeedACC(3))
	{
		DrawFormatString(SPEED_FORMAT_POS_X, SPEED_FORMAT_POS_Y, red, "%dkm", static_cast<int>(nowSpeed * 2));
	}
}

void GameScene::DrawLine(void)
{	
	
	//集中線
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
	
	//コントローラー振動
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

