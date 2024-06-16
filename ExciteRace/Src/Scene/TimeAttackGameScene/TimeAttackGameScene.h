#pragma once
#include "../SceneBase.h"

class Car;
class Camera;
class Stage;
class SkyDome;
class Garage;

class TimeAttackGameScene : public SceneBase
{
public:

	// �R���X�g���N�^
	TimeAttackGameScene(void);

	// �f�X�g���N�^
	~TimeAttackGameScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;

private:

	//�摜�n���h��������
	void InitImageHandle(void);

	//�I�������Ԃ̏�����
	void InitSelectCar(void);

	//�J�����̏�����
	void InitCamera(void);

	//�X�e�[�W�I�u�W�F�N�g������
	void InitStageObjects(void);

	//�����蔻��p���f���n���h����ǉ�
	void SetupCollision(void);

	//�I�u�W�F�N�g�X�V
	void UpdateObject(void);

	//�S�[���Ƃ̓����蔻��
	void ProcessGoalCollision(void);

	//�Q�[�����Ŏg���I�u�W�F�N�g�`��
	void DrawGame(void);

	//UI�`��
	void DrawUi(float nowSpeed, int nowGear);

	//�^�R���[�^�[�`��
	void DrawTachometer(void);

	//�X�s�[�h�`��
	void DrawSpeedAndGear(float nowSpeed, int nowGear);

	//�j�[�h���`��i�����������̂Ŋ֐���ʂō��j
	void DrawNeedle(float nowSpeed, int nowGear);

	void DrawMiniMap(void);

	//�X�R�A�`��
	void DrawScore(void);

	//�W�����`��
	void DrawLine(void);

	//�C���X�^���X
	std::unique_ptr<Car> car_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Garage> garage_;

	//�摜�n���h���i�[�ϐ�
	enum class IMAGE_TYPE
	{
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		START_SIGN,
		LINE_4,
		LINE_5,
		LINE_6,
	};

	//�^�C�v���Ƃ̃n���h�����i�[�ϐ�
	std::map<IMAGE_TYPE, int> imageInfos_;

	//�W�����Ǘ��X�e�b�v�^�C��
	float delta_;

	//�X�^�[�g�Ǘ��ϐ�
	float stepStartTime_;

	//�X�R�A�Ǘ��ϐ�
	float scoreTime_;

};

