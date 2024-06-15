#pragma once
#include "../Object/Common/Transform.h"
#include "../Common/Vector2.h"
#include "../Manager/SceneManager.h"
#include "SceneBase.h"

class Transform;
class Camera;
class Garage;

class SelectScene : public SceneBase
{
public:

	enum class MODE
	{
		TIMEATTACK,
		VERSUS,
		OPTION
	};

	// �R���X�g���N�^
	SelectScene(void);

	// �f�X�g���N�^
	~SelectScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;

private:

	//�{�^���������ꂽ�ۂ̏���
	void ProcessPushButton(void);

	//���y�Đ�
	void PlayControlSound(const std::string& filename);

	//�w�i�`��
	void DrawBackGround(void);

	//�{�^���`��
	void DrawButtons(void);

	//�J�[�\���`��
	void DrawCursor(void);


	std::pair<int, Vector2> cursorInfo_;

	//�w�i�̃n���h���i�[
	std::vector<int> backGraund_;

	//�{�^���̃n���h���ƈʒu���
	std::vector<std::pair<int,Vector2>> btnInfos_;

	//�V�[���ڍs���[�h�i�[
	std::vector<SceneManager::SCENE_ID> sceneIds_;

};

