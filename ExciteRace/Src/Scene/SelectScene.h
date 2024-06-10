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

	//�w�i�̃n���h���i�[
	std::vector<int> backGraund_;

	//�{�^���̃n���h���ƈʒu���
	std::vector<std::pair<int,Vector2>> btnInfos_;

	//�I�΂�Ă郂�[�h���
	std::vector<MODE> modes_;

	//�V�[���ڍs���[�h�i�[
	std::vector<SceneManager::SCENE_ID> sceneIds_;

	//�{�^���|�W�V����
	std::vector<Vector2> btnPos_;

	//�J�[�\���|�W�V����
	Vector2 cursorPos_;

	//�J�[�\���n���h��
	int cursorHandle_;

};

