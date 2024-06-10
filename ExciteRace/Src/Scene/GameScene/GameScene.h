#pragma once
#include <vector>
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Garage;
class Vector2;

class GameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameScene(void);

	// �f�X�g���N�^
	~GameScene(void);

	//������
	void Init(void) override;
	
	//�摜�n���h��������
	void InitImageHandle(void);

	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;
	
	//�Q�[�����Ŏg���`��
	void DrawGame(void);

private:

	//�R���g���[���[�̐U��
	void ControllerVibration(void);

	//�PP�ƂQP�̏Փˎ�����
	VECTOR Collision1PTo2P(VECTOR pos1, VECTOR pos2);

	//�C���X�^���X
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Garage> garage_;

	//��ʕ���������̂ŎԂƃJ�������p��
	std::vector<std::unique_ptr<Car>> cars_;
	std::vector<std::unique_ptr<Camera>> cameras_;
	
	//�X�N���[������p��
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;

	//�摜�n���h���i�[�ϐ�
	enum class IMAGE_TYPE
	{
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		START_SIGN,
		LINE_1,
		LINE_2,
		LINE_3,
	};

	//�^�C�v���Ƃ̃n���h�����i�[�ϐ�
	std::map<IMAGE_TYPE, int> imageInfos_;

	//���Ԍv���p�ϐ�
	float delta_;

	//�X�^�[�g�Ǘ��ϐ�
	float stepStartTime_;

};
