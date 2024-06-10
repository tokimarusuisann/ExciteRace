#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Utility/AsoUtility.h"
#include "../Manager/Camera.h"
#include "../Object/Garage.h"
#include "SelectScene.h"


SelectScene::SelectScene(void)
{
	cursorHandle_ = -1;
}

SelectScene::~SelectScene(void)
{
}

void SelectScene::Init(void)
{
	//Instance省略
	auto& resIns = ResourceManager::GetInstance();

	//背景
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::VERSUS_BACKGROUND).handleId_);
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::TIMEATTACK_BACKGROUND).handleId_);
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::RANKING_BACKGROUND).handleId_);

	//ボタンのハンドルと位置情報
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::VERSUS_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, 500 }));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::TIMEATTACK_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, 600 }));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::RANKING_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, 700 }));

	//モード選択の場合
	modes_.emplace_back(MODE::TIMEATTACK);
	modes_.emplace_back(MODE::VERSUS);
	modes_.emplace_back(MODE::OPTION);

	//シーン移行時に必要なMODE情報
	sceneIds_.emplace_back(SceneManager::SCENE_ID::VS_GAMESTART);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::TA_CARSELECT);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::SCORE);

	//選んでるのがわかりやすいように示すカーソルの情報
	cursorHandle_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_;
	cursorPos_ = { Application::SCREEN_SIZE_X / 2-220, 600 };

	//BGM再生
	auto& soundMngIns = SoundManager::GetInstance();
	soundMngIns.PlaySound(Application::PATH_SOUND + "maou_bgm_neorock53.mp3", true);
	soundMngIns.SetVolume(MUSIC_DEFAULT_VOLUME);
}

void SelectScene::Update(void)
{

	//Instance省略
	auto& inputIns = InputManager::GetInstance();
	auto& sceneIns = SceneManager::GetInstance();
	auto& soundIns = SoundManager::GetInstance();

	//コントローラー
	auto cState = inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	bool isTrgUp = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::UP);
	bool isTrgDown = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::BOTTOM);
	bool IsRightTrg = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER);

	//選択中のサウンドフラグ
	bool soundSelectFlag = false;

	//決定時のサウンドフラグ
	bool soundDesideFlag = false;

	//上を選択
	if (isTrgUp)
	{
		soundSelectFlag = true;
		cursorPos_.y -= 100;
		if (cursorPos_.y <= 500)
		{
			cursorPos_.y = 500;
		}
	}

	//下を選択
	if (isTrgDown)
	{
		soundSelectFlag = true;
		cursorPos_.y += 100;
		if (cursorPos_.y >= 700)
		{
			cursorPos_.y = 700;
		}
	}

	//サウンド再生
	if (soundSelectFlag)
	{
		soundIns.PlaySound(Application::PATH_SOUND + "button04a.mp3", false);
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
	}

	//シーン移行
	for (int i = 0; i < sceneIds_.size(); i++)
	{
		if (IsRightTrg && cursorPos_.y == btnInfos_[i].second.y)
		{
			soundDesideFlag = true;
			sceneIns.ChangeScene(sceneIds_[i], true);
		}
	}

	//サウンド再生・停止
	if (soundDesideFlag)
	{
		//SE再生
		soundIns.PlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
		//BGM再生を止める
		soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock53.mp3");
	}

}

void SelectScene::Draw(void)
{	

	//ボタンのサイズ
	std::vector<float> btnSize;
	btnSize.emplace_back(1.0f);
	btnSize.emplace_back(1.0f);
	btnSize.emplace_back(1.0f);

	//バックグラウンド
	for (int i = 0; i < backGraund_.size(); i++)
	{
		if (cursorPos_.y == btnInfos_[i].second.y)
		{
			DrawGraph(0, 0, backGraund_[i], false);
			//選んでいるのがどこかわかりやすいようにボタンサイズを拡大
			btnSize[i] = 1.2f;
		}
	}

	//ボタンの描画
	for (int i = 0; i < btnInfos_.size(); i++)
	{
		DrawRectRotaGraph(btnInfos_[i].second.x, btnInfos_[i].second.y, 1.0f, 1.0f, 300, 84, btnSize[i], 0.0f, btnInfos_[i].first, true);
	}

	//選んでるのがわかりやすいように車のカーソルを出す
	DrawRectRotaGraph(cursorPos_.x, cursorPos_.y, 1.0f, 1.0f, 100, 50, 1.0f, 0.0f, cursorHandle_, true);

}
