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

#pragma region �萔�錾

//�{�^���̃f�t�H���g�T�C�Y
constexpr float BUTTON_DEFAULT_SIZE = 1.0f;

//�{�^���̊g���T�C�Y
constexpr float BUTTON_SELECTED_SIZE = 1.2f;

//�{�^���̃I�t�Z�b�g���WY
constexpr int BUTTON_OFFSET_POS_Y = 100;

//�{�^���̕�
constexpr int BUTTON_WIDTH = 300;

//�{�^���̍���
constexpr int BUTTON_HEIGHT = 84;

//�{�^���̏������WY
constexpr int BUTTON_INIT_POS_Y = 500;

//�J�[�\���̏������WX
constexpr int CURSOR_INIT_POS_X = Application::SCREEN_SIZE_X / 2 - 220;

//�J�[�\���̏������WY
constexpr int CURSOR_INIT_POS_Y = 600;

//�J�[�\���̕�
static constexpr int CURSOR_WIDTH = 100;

//�J�[�\���̕�
static constexpr int CURSOR_HEIGHT = 50;

//�J�[�\���̃f�t�H���g�T�C�Y
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

	//Instance�ȗ�
	auto& resIns = ResourceManager::GetInstance();

	//�w�i
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::VERSUS_BACKGROUND).handleId_);
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::TIMEATTACK_BACKGROUND).handleId_);
	backGraund_.emplace_back(resIns.Load(ResourceManager::SRC::RANKING_BACKGROUND).handleId_);

	//�{�^���̃n���h���ƈʒu���
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::VERSUS_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y }));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::TIMEATTACK_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y + BUTTON_OFFSET_POS_Y}));
	btnInfos_.emplace_back(std::make_pair(resIns.Load(ResourceManager::SRC::RANKING_BUTTON).handleId_, Vector2{ Application::SCREEN_SIZE_X / 2, BUTTON_INIT_POS_Y + (BUTTON_OFFSET_POS_Y * 2) }));

	//�V�[���ڍs���ɕK�v��MODE���
	sceneIds_.emplace_back(SceneManager::SCENE_ID::VS_GAMESTART);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::TA_CARSELECT);
	sceneIds_.emplace_back(SceneManager::SCENE_ID::SCORE);

	//�I��ł�̂��킩��₷���悤�Ɏ����J�[�\���̏��
	cursorInfo_ = std::make_pair(ResourceManager::GetInstance().Load(ResourceManager::SRC::CURSOR).handleId_, Vector2{ CURSOR_INIT_POS_X,CURSOR_INIT_POS_Y });

	//BGM�Đ�
	PlayControlSound("maou_bgm_neorock53.mp3");

}

void SelectScene::Update(void)
{

	//�{�^���������Ƃ��̏���
	ProcessPushButton();

}

void SelectScene::Draw(void)
{

	//�w�i�`��
	DrawBackGround();

	//�{�^���`��
	DrawButtons();

	//�J�[�\���`��
	DrawCursor();

}

void SelectScene::ProcessPushButton(void)
{
	auto& inputIns = InputManager::GetInstance();
	auto& soundIns = SoundManager::GetInstance();
	auto& sceneIns = SceneManager::GetInstance();

	//�R���g���[���[
	auto cState = inputIns.GetJPadInputState(InputManager::JOYPAD_NO::PAD1);
	bool isTrgUp = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::UP);
	bool isTrgDown = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::BOTTOM);
	bool IsRightTrg = inputIns.IsPadBtnTrgDown(InputManager::JOYPAD_NO::PAD1, InputManager::JOYPAD_BTN::R_TRIGGER);

	//�I���T�E���h�t���O
	bool soundSelectFlag = false;

	//���I��
	if (isTrgUp)
	{
		soundSelectFlag = true;
		cursorInfo_.second.y = std::max(cursorInfo_.second.y - BUTTON_OFFSET_POS_Y, BUTTON_INIT_POS_Y);
	}

	//����I��
	if (isTrgDown)
	{
		soundSelectFlag = true;
		cursorInfo_.second.y = std::min(cursorInfo_.second.y + BUTTON_OFFSET_POS_Y, BUTTON_INIT_POS_Y + (BUTTON_OFFSET_POS_Y * 2));
	}

	if (soundSelectFlag)
	{
		//�T�E���h�Đ�
		PlayControlSound("button04a.mp3");
	}

	if (IsRightTrg)
	{
		for (size_t i = 0; i < sceneIds_.size(); i++)
		{
			if (cursorInfo_.second.y == btnInfos_[i].second.y)
			{
				//�Đ�
				PlayControlSound("maou_se_sound_car05.mp3");

				//BGM�Đ����~�߂�
				soundIns.StopSound(Application::PATH_SOUND + "maou_bgm_neorock53.mp3");

				//�V�[���J��
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
