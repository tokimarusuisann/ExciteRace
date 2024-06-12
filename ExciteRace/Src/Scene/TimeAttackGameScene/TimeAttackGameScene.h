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

	//�摜�n���h��������
	void InitImageHandle(void);
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;
	


	////UI AT�`��
	//void DrawUiAT(void);

	////UI MT�`��
	//void DrawUiMT(void);

private:

	//UI�`��
	void DrawUi(void);

	//�j�[�h���`��i�����������̂Ŋ֐���ʂō��j
	void DrawUiNeedle(float nowSpeed);

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

