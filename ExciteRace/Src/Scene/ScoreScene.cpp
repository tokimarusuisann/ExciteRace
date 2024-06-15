#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/InputManager.h"
#include "../Manager/SoundManager.h"
#include "../Application.h"
#include "ScoreScene.h"

#pragma region 定数宣言
//このクラスでしか使わない
//スコアのオフセットY
constexpr int SCORE_OFFSET_Y = 50;

//スコアX座標
constexpr int SCORE_POS_X = 500;

//スコアY座標
constexpr int SCORE_POS_Y = 300;

#pragma endregion

ScoreScene::ScoreScene(void)
{
	scoreLogo_ = -1;
}

ScoreScene::~ScoreScene(void)
{
}

void ScoreScene::Init(void)
{
	scoreLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::SCORE).handleId_;
}

void ScoreScene::Update(void)
{

	auto& ins = InputManager::GetInstance();
	auto& soundIns = SoundManager::GetInstance();

	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER) || ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);

		//決定SE
		soundIns.PlaySoundA(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
	}

}

void ScoreScene::Draw(void)
{

	DrawGraph(0, 0, scoreLogo_, true);

	SetFontSize(128);

	DrawFormatString(300, 100, 0xffffff, "スコアタイム");

	SetFontSize(64);

	for (auto score : SceneManager::GetInstance().GetScores())
	{
		DrawFormatString(SCORE_POS_X, SCORE_POS_Y + (SceneManager::GetInstance().GetScores().size() * SCORE_OFFSET_Y), 0xffffff, "%.2f", score);
	}

	SetFontSize(50);

	DrawFormatString(30, Application::SCREEN_SIZE_Y - 50, 0xffffff, "Push the RightTrigger returns to the SelectScene ");

}

