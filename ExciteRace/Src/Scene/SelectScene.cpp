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

#pragma region 定数宣言

//ボタンのデフォルトサイズ
constexpr float BUTTON_DEFAULT_SIZE = 1.0f;

//ボタンの拡大後サイズ
constexpr float BUTTON_SELECTED_SIZE = 1.2f;

//ボタンのオフセット座標Y
constexpr int BUTTON_OFFSET_POS_Y = 100;

//ボタンの幅
constexpr int BUTTON_WIDTH = 300;

//ボタンの高さ
constexpr int BUTTON_HEIGHT = 84;

//ボタンの初期座標Y
constexpr int BUTTON_INIT_POS_Y = 500;

//カーソルの初期座標X
constexpr int CURSOR_INIT_POS_X = Application::SCREEN_SIZE_X / 2 - 220;

//カーソルの初期座標Y
constexpr int CURSOR_INIT_POS_Y = 600;

//カーソルの幅
static constexpr int CURSOR_WIDTH = 100;

//カーソルの幅
static constexpr int CURSOR_HEIGHT = 50;

//カーソルのデフォルトサイズ
constexpr float CURSOR_DEFAULT_SIZE = 1.0f;

#pragma endregion


SelectScene::SelectScene(void)
{
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
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::VERSUS_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y }));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::TIMEATTACK_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y + BUTTON_OFFSET_POS_Y}));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::RANKING_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y + (BUTTON_OFFSET_POS_Y * 2) }));

	//シーン移行時に必要なMODE情報
	sceneIds_.emplace_back(SceneManager::SCENE_ID::VS_GAMESTART);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::TA_CARSELECT);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::SCORE);

	//選んでるのがわかりやすいように示すカーソルの情報
	cursorInfo_ = std::make_pair(ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_, Vector2{ CURSOR_INIT_POS_X,CURSOR_INIT_POS_Y });

	//BGM再生
	PlayControlSound("maou_bgm_neorock53.mp3");

}

void SelectScene::Update(void)
{

	//ボタン押したときの処理
	ProcessPushButton();

}

void SelectScene::Draw(void)
{

	//背景描画
	DrawBackGround();

	//ボタン描画
	DrawButtons();

	//カーソル描画
	DrawCursor();

}

void SelectScene::ProcessPushButton(void)
{
	auto& inputIns = InputManager::GetInstance();
	auto& soundIns = SoundManager::GetInstance();
	auto& sceneIns = SceneManager::GetInstance();

	//コントローラー
	auto cState = inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	bool isTrgUp = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::UP);
	bool isTrgDown = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::BOTTOM);
	bool IsRightTrg = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER);

	//選択サウンドフラグ
	bool soundSelectFlag = false;

	//上を選択
	if (isTrgUp)
	{
		soundSelectFlag = true;
		cursorInfo_.second.y = std::max(cursorInfo_.second.y - BUTTON_OFFSET_POS_Y, BUTTON_INIT_POS_Y);
	}

	//下を選択
	if (isTrgDown)
	{
		soundSelectFlag = true;
		cursorInfo_.second.y = std::min(cursorInfo_.second.y + BUTTON_OFFSET_POS_Y, BUTTON_INIT_POS_Y + (BUTTON_OFFSET_POS_Y * 2));
	}

	if (soundSelectFlag)
	{
		//サウンド再生
		PlayControlSound("button04a.mp3");
	}

	if (IsRightTrg)
	{
		for (size_t i = 0; i < sceneIds_.size(); i++)
		{
			if (cursorInfo_.second.y == btnInfos_[i].second.y)
			{
				//再生
				PlayControlSound("maou_se_sound_car05.mp3");

				//BGM再生を止める
				soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock53.mp3");

				//シーン遷移
				sceneIns.ChangeScene(sceneIds_[i], true);
			}
		}
	}

}

void SelectScene::PlayControlSound(const std::string& filename)
{
	auto& soundIns = SoundManager::GetInstance();

	soundIns.MyPlaySound(Application::PATH_SOUND + filename, false);
	soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
}

void SelectScene::DrawBackGround(void)
{
	for (size_t i = 0; i < backGraund_.size(); i++)
	{
		if (cursorInfo_.second.y == btnInfos_[i].second.y)
		{
			DrawGraph(0, 0, backGraund_[i], false);
		}
	}
}

void SelectScene::DrawButtons(void)
{
	for (size_t i = 0; i < backGraund_.size(); i++)
	{
		float size = (cursorInfo_.second.y == btnInfos_[i].second.y) ? BUTTON_SELECTED_SIZE : BUTTON_DEFAULT_SIZE;
		DrawRectRotaGraph(btnInfos_[i].second.x, btnInfos_[i].second.y, 1.0f, 1.0f, BUTTON_WIDTH, BUTTON_HEIGHT, size, 0.0f, btnInfos_[i].first, true);
	}
}

void SelectScene::DrawCursor(void)
{
	DrawRectRotaGraph(cursorInfo_.second.x, cursorInfo_.second.y, 1.0f, 1.0f, CURSOR_WIDTH, CURSOR_HEIGHT, CURSOR_DEFAULT_SIZE, 0.0f, cursorInfo_.first, true);
}
