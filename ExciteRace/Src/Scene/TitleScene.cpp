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

#pragma region �萔�錾
//���̃N���X�ł����g��Ȃ�
//�^�C�g�����S�T�C�Y
constexpr float TITLE_LOGO_SIZE = 0.5f;

//�t�H�[�}�b�g�I�t�Z�b�gX
constexpr int FORMAT_OFFSET_X = 150;

//�t�H�[�}�b�gX���W
constexpr int FORMAT_POS_X = 20;

//�t�H�[�}�b�gY���W
constexpr int FORMAT_POS_Y = 150;

//�w���t�H�[�}�b�gY���W
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

	// �^�C�g�����S
	titaleImgLogo_ = ResourceManager::GetInstance().Load(ResourceManager::SRC::CAR_LOGO).handleId_;

}

void TitleScene::Update(void)
{

	stage_->Update();

	//Instance�ȗ�
	auto& inputMngIns = InputManager::GetInstance();
	auto& soundMngIns = SoundManager::GetInstance();

	if (inputMngIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER) ||inputMngIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD2, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		//SE�Đ�
		soundMngIns.PlaySound(Application::PATH_SOUND + "maou_se_sound_car05.mp3", false);
		soundMngIns.SetVolume(MUSIC_DEFAULT_VOLUME);

		//�V�[���J��
		SceneManager::GetInstance().ChangeScene(SceneManager::SCENE_ID::SELECT, true);
	}

}

void TitleScene::Draw(void)
{

	stage_->Draw();

	// �^�C�g�����S
	DrawRotaGraph(Application::SCREEN_SIZE_X / 2, Application::SCREEN_SIZE_Y / 2, TITLE_LOGO_SIZE, 0.0f, titaleImgLogo_, true);

	SetFontSize(128);

	DrawFormatString(FORMAT_POS_X, INSTRUCT_FORMAT_POS_Y, 0xffffff, "�G�L�T�C�g���[�[�[�X�I�I");

	DrawFormatString(FORMAT_POS_X,Application::SCREEN_SIZE_Y- FORMAT_OFFSET_X, 0xffffff, "Push Right Trriger!");

}
