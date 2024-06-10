#pragma once
#include <vector>
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Vector2;

class GameStartScene : public SceneBase
{
public:

	// �R���X�g���N�^
	GameStartScene(void);

	// �f�X�g���N�^
	~GameStartScene(void);

	//������
	void Init(void) override;

	//�X�V
	void Update(void) override;

	//�`��
	void Draw(void) override;

	//�Q�[�����Ŏg���`��
	void DrawGame(void);

private:

	//�C���X�^���X
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;

	//��ʕ���������̂ŎԂƃJ�������p��
	std::vector<std::unique_ptr<Car>> cars_;
	std::vector<std::unique_ptr<Camera>> cameras_;

	//�X�N���[������p��
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;

};

