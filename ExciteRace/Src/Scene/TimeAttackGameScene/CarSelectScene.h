#pragma once
#include<array>
#include "../../Object/Common/Transform.h"
#include "../../Common/CollectStructure.h"
#include "../SceneBase.h"

class Transform;
class Camera;
class Garage;
class Car;

class CarSelectScene : public SceneBase
{
public:

	static constexpr int MAX_INDEX = 3;
	static constexpr int MIN_INDEX = 0;

	// �R���X�g���N�^
	CarSelectScene(void);

	// �f�X�g���N�^
	~CarSelectScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;

	//�`��
	void Draw(void) override;

private:

	struct CarInitParams {
		VECTOR carPos_;
		VECTOR carScale_;
		VECTOR carRot_;
		VECTOR wheelPos_;
		VECTOR wheelScale_;
	};

	void InitImage(void);

	void InitModel(void);

	void ProcessButtonPush(void);

	void LimitIndex(void);

	void SetParam(void);

	void SetParamCar(int idx, VECTOR pos,VECTOR scl, VECTOR rot);

	void SetParamWheel(int idx, VECTOR pos, VECTOR scl, VECTOR rot);

	CarInitParams carParams[MAX_INDEX];

	//���������g��Ȃ�����Car�N���X���Ăяo���Ȃ�
	Transform transformCar_;
	Transform transformWheel_;

	//�C���X�^���X
	std::unique_ptr<Garage> garage_;
	std::unique_ptr<Camera> camera_;

	//���f��ID���͂����Ǘ�
	int carModelIds_[static_cast<int>(CAR_TYPE::MAX)];
	int wheelModelIds_[static_cast<int>(CAR_TYPE::MAX)];

	//�C���f�b�N�X
	int index_;

	int decisionHandle_;
	int crossHandle_;
	std::vector<int> carLogoHandle_;

};

