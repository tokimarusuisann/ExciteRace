#pragma once
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Vector2;

class TimeAttackStartScene : public SceneBase
{
public:

	// �R���X�g���N�^
	TimeAttackStartScene(void);

	// �f�X�g���N�^
	~TimeAttackStartScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;

private:

	//�C���X�^���X
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Car> car_;
	std::unique_ptr<Camera> camera_;

};

