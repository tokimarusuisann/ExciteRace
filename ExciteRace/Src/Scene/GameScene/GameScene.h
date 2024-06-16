#pragma once
#include <vector>
#include <unordered_map>
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
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;
	
private:

	//�Ԃ̏����l
	void InitCarParam(void);

	//�J�����̏����l
	void InitCameraParam(void);

	//�摜�n���h��������
	void InitImageHandle(void);

	//�X�e�[�W�I�u�W�F�N�g������
	void InitStageObjects(void);

	//�ԂƃJ�����̏�����
	void InitPlayerAndCamera(int index);

	//�����蔻��p���f���n���h����ǉ�
	void SetupCollision(void);

	//�ԓ��m�̓����蔻��
	void ProcessCarCollision(std::unique_ptr<Car>& car1P, std::unique_ptr<Car>& car2P);

	//�S�[���Ƃ̓����蔻��
	void ProcessGoalCollision(std::unique_ptr<Car>& car);

	//�Q�[�����Ŏg���`��
	void DrawGame(void);

	//Ui�`��
	void DrawUi(float nowSpeed , int nowGear ,int nowCarNum);

	//�j�[�h���`��(�����������̂Ŋ֐���ʂō��)
	void DrawNeedle(float nowSpeed, int nowGear , int nowCarNum);

	//�W�����`��
	void DrawLine(void);

	//�R���g���[���[�̐U��
	void ControllerVibration(void);

	//�PP�ƂQP�̏Փˎ�����
	VECTOR CalcCollisionCar(VECTOR pos1, VECTOR pos2);

	//�摜�^�C�v
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

	//�^�C�v���Ƃ̃n���h�����i�[�ϐ�
	std::unordered_map<IMAGE_TYPE, int> imageInfos_;

	std::vector<VECTOR> vecPos_;
	std::vector<CAR_TYPE> carTypes_;
	std::vector<InputManager::JOYPAD_NO> padNos_;

	//���Ԍv���p�ϐ�
	float stepTime_;

	//�X�^�[�g�Ǘ��ϐ�
	float stepStartTime_;

};
