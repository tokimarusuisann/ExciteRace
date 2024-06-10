#include <chrono>
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Common/Fader.h"
#include "../Scene/TitleScene.h"
#include "../Scene/SelectScene.h"
#include "../Scene/CarSelectScene.h"
#include "../Scene/GameScene/GameScene.h"
#include "../Scene/GameScene/GameStartScene.h"
#include "../Scene/TimeAttackGameScene/TimeAttackStartScene.h"
#include "../Scene/TimeAttackGameScene/TimeAttackGameScene.h"
#include "../Scene/ScoreScene.h"
#include "../Scene/ResultScene.h"
#include "../Application.h"
#include "Camera.h"
#include "ResourceManager.h"
#include "SceneManager.h"

SceneManager* SceneManager::instance_ = nullptr;

void SceneManager::CreateInstance()
{
	if (instance_ == nullptr)
	{
		instance_ = new SceneManager();
	}
	instance_->Init();
}

SceneManager& SceneManager::GetInstance(void)
{
	return *instance_;
}

void SceneManager::Init(void)
{

	sceneID_ = SCENE_ID::TITLE;
	waitSceneID_ = SCENE_ID::NONE;

	fader_ = new Fader();
	fader_->Init();

	scene_ = new TitleScene();
	scene_->Init();

	isSceneChanging_ = false;

	// デルタタイム
	preTime_ = std::chrono::system_clock::now();

	//下方向　重力
	dirDown = { 0.0f,-1.0f,0.0f };

	//重力の力
	gravityPow = 9.8f;

	mainScreen_ = MakeScreen(Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// 3D用の設定
	Init3D();

}

void SceneManager::Init3D(void)
{

	// Zバッファを有効にする
	SetUseZBuffer3D(true);

	// Zバッファへの書き込みを有効にする
	SetWriteZBuffer3D(true);

	// バックカリングを有効にする
	SetUseBackCulling(true);

	// ライトの設定
	ChangeLightTypeDir({ 0.0f, -1.0f, 0.0f });
	SetUseLighting(true);

	// フォグに関連
	SetFogEnable(true);					// フォグを有効にする
	SetFogColor(170, 170, 170);			// フォグの色にする
	SetFogStartEnd(12000.0f, 13000.0f);	// フォグの開始距離

};

void SceneManager::Update(void)
{

	if (scene_ == nullptr)
	{
		return;
	}

	// デルタタイム
	auto nowTime = std::chrono::system_clock::now();
	deltaTime_ = static_cast<float>(
		std::chrono::duration_cast<std::chrono::nanoseconds>(nowTime - preTime_).count() / 1000000000.0);
	preTime_ = nowTime;

	fader_->Update();
	if (isSceneChanging_)
	{
		Fade();
	}
	else
	{
		scene_->Update();
	}

}

void SceneManager::Draw(void)
{
	
	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(mainScreen_);

	// 画面を初期化
	ClearDrawScreen();

	// Effekseerにより再生中のエフェクトを更新する。
	UpdateEffekseer3D();

	// 描画
	scene_->Draw();

	// 要リファクタリング
	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		break;
	case SCENE_ID::SELECT:
		break;	
	case SCENE_ID::TA_CARSELECT:
		break;
	case SCENE_ID::VS_GAMESTART:
		break;
	case SCENE_ID::GAME:
		break;
	case SCENE_ID::TIMEATTACKGAMESTART:
		break;	
	case SCENE_ID::TIMEATTACKGAME:
		break;	
	case SCENE_ID::SCORE:
		break;	
	case SCENE_ID::RESULT:
		break;
	}

	// Effekseerにより再生中のエフェクトを描画する。
	DrawEffekseer3D();
	
	fader_->Draw();

	// 描画先グラフィック領域の指定
	// (３Ｄ描画で使用するカメラの設定などがリセットされる)
	SetDrawScreen(DX_SCREEN_BACK);
	DrawGraph(0, 0, mainScreen_, true);
}

void SceneManager::Release(void)
{
	delete scene_;

	delete fader_;

	DeleteGraph(mainScreen_);

	delete instance_;

}

void SceneManager::ChangeScene(SCENE_ID nextId, bool isFading)
{

	waitSceneID_ = nextId;

	if (isFading)
	{
		fader_->SetFade(Fader::STATE::FADE_OUT);
		isSceneChanging_ = true;
	}
	else
	{
		DoChangeScene();
	}

}

SceneManager::SCENE_ID SceneManager::GetmSceneID(void)
{
	return sceneID_;
}

float SceneManager::GetDeltaTime(void) const
{
	//return 1.0f / 60.0f;
	return deltaTime_;
}

VECTOR SceneManager::GetGravityDir(void) const
{
	return dirDown;
}

float SceneManager::GetGravityPow(void) const
{
	return gravityPow;
}

void SceneManager::SetScore(float score)
{
	scores_.emplace_back(score);
	//ソートする（並び替）
	if (scores_.size()>10)
	{
		scores_.pop_back();
	}
}

std::vector<float> SceneManager::GetScores(void)
{
	return scores_;
}

void SceneManager::SetCarType(CAR_TYPE carType)
{
	carType_ = carType;
}

CAR_TYPE SceneManager::GetCarType(void)
{
	return carType_;
}

InputManager::JOYPAD_NO SceneManager::GetPadNo(void)
{
	return joyNo_;
}

void SceneManager::SetPadNo(InputManager::JOYPAD_NO joyNo)
{
	joyNo_ = joyNo;
}

int SceneManager::GetScreen(void)
{
	return mainScreen_;
}

int SceneManager::GetPlayerNum(void)
{
	return PLAYER_NUM;
}

SceneManager::SceneManager(void)
{

	sceneID_ = SCENE_ID::NONE;
	waitSceneID_ = SCENE_ID::NONE;

	scene_ = nullptr;
	fader_ = nullptr;

	isSceneChanging_ = false;

	// デルタタイム
	deltaTime_ = 1.0f / 60.0f;

}

SceneManager::~SceneManager(void)
{
	delete instance_;
}

void SceneManager::ResetDeltaTime(void)
{
	deltaTime_ = 0.016f;
	preTime_ = std::chrono::system_clock::now();
}

void SceneManager::DoChangeScene(void)
{

	// リソースの解放
	ResourceManager::GetInstance().Release();

	delete scene_;

	sceneID_ = waitSceneID_;

	switch (sceneID_)
	{
	case SCENE_ID::TITLE:
		scene_ = new TitleScene();
		break;
	case SCENE_ID::SELECT:
		scene_ = new SelectScene();
		break;
	case SCENE_ID::TA_CARSELECT:
		scene_ = new CarSelectScene();
		break;	
	case SCENE_ID::VS_GAMESTART:
		scene_ = new GameStartScene();
		break;
	case SCENE_ID::GAME:
		scene_ = new GameScene();
		break;	
	case SCENE_ID::TIMEATTACKGAMESTART:
		scene_ = new TimeAttackStartScene();
		break;	
	case SCENE_ID::TIMEATTACKGAME:
		scene_ = new TimeAttackGameScene();
		break;
	case SCENE_ID::SCORE:
		scene_ = new ScoreScene();
		break;	
	case SCENE_ID::RESULT:
		scene_ = new ResultScene();
		break;
	default:
		break;
	}

	scene_->Init();

	ResetDeltaTime();

	waitSceneID_ = SCENE_ID::NONE;

}

void SceneManager::Fade(void)
{

	Fader::STATE fState = fader_->GetState();
	switch (fState)
	{
	case Fader::STATE::FADE_IN:
		if (fader_->IsEnd())
		{
			fader_->SetFade(Fader::STATE::NONE);
			isSceneChanging_ = false;
		}
		break;
	case Fader::STATE::FADE_OUT:
		if (fader_->IsEnd())
		{
			DoChangeScene();
			fader_->SetFade(Fader::STATE::FADE_IN);
		}
		break;
	}

}


