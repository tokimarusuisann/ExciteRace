#include <string>
#include <DxLib.h>
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "../Manager/Camera.h"
#include "../Object/Stage.h"
#include "TitleScene.h"

#pragma region 定数宣言
//このクラスでしか使わない
//タイトルロゴサイズ
constexpr float TITLE_LOGO_SIZE = 0.5f;

//フォーマットオフセットX
constexpr int FORMAT_OFFSET_X = 150;

//フォーマットX座標
constexpr int FORMAT_POS_X = 20;

//フォーマットY座標
constexpr int FORMAT_POS_Y = 150;

//指示フォーマットY座標
constexpr int INSTRUCT_FORMAT_POS_Y = 50;
#pragma endregion

TitleScene::TitleScene(void)
{
	titaleImgLogo_ = -1;
}

TitleScene::~TitleScene(void)
{
	DeleteGraph(titaleImgLogo_);
}

void TitleScene::Init(void)
{

	stage_ = std::make_unique<Stage>();
	stage_->Init();

	// タイトルロゴ
	titaleImgLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CAR_LOGO).handleId_;

}

void TitleScene::Update(void)
{

	stage_->Update();

	//Instance省略
	auto& inputMngIns = InputManager::GetInstance();
	auto& soundMngIns = SoundManager::GetInstance();

	if (inputMngIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER) ||inputMngIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		//SE再生
		soundMngIns.PlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		soundMngIns.SetVolume(MUSIC_DEFAULT_VOLUME);

		//シーン遷移
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

}

void TitleScene::Draw(void)
{

	stage_->Draw();

	// タイトルロゴ
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, TITLE_LOGO_SIZE, 0.0f, titaleImgLogo_, true);

	SetFontSize(128);

	DrawFormatString(FORMAT_POS_X, INSTRUCT_FORMAT_POS_Y, 0xffffff, "エキサイトレーーース！！");

	DrawFormatString(FORMAT_POS_X,Application::SCREEN_SIZE_Y- FORMAT_OFFSET_X, 0xffffff, "Push Right Trriger!");

}
