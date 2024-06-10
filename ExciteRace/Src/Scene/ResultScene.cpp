#include <DxLib.h>
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/InputManager.h"
#include "../Application.h"
#include "ResultScene.h"

#pragma region 定数宣言
//このクラスでしか使わない
//スタートタイムの初期値
constexpr int FORMAT_POS_X = 200;
#pragma endregion

ResultScene::ResultScene(void)
{
	resultLogo_ = -1;
}

ResultScene::~ResultScene(void)
{
	DeleteGraph(resultLogo_);
}

void ResultScene::Init(void)
{
	resultLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::RESULT).handleId_;
}

void ResultScene::Update(void)
{

	auto& ins = InputManager::GetInstance();
	auto& soundIns = SoundManager::GetInstance();

	//シーン遷移
	if (ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER)|| ins.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);

		//決定SE
		soundIns.PlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		soundIns.SetVolume(MUSIC_DEFAULT_VOLUME);
	}

}

void ResultScene::Draw(void)
{

	DrawGraph(0, 0, resultLogo_, true);

	SetFontSize(128);

	//プレイヤー１が勝ちなら
	if (SceneManager::GetInstance().GetPadNo() == InputManager::JOYPAD_NO::PAD1)
	{
		DrawFormatString(FORMAT_POS_X, Application::SCREEN_SIZE_Y/2, 0xffffff, "PLAYER1");
	}
	//プレイヤー2が勝ちなら
	if (SceneManager::GetInstance().GetPadNo() == InputManager::JOYPAD_NO::PAD2)
	{
		DrawFormatString(FORMAT_POS_X, Application::SCREEN_SIZE_Y/2, 0xffffff, "PLAYER2");
	}

	SetFontSize(50);

	DrawFormatString(30, Application::SCREEN_SIZE_Y - 50, 0xffffff, "Push the RightTrigger returns to the SelectScene ");

}
