#pragma once
#include<DxLib.h>
#include "../Car.h"
#include "../../Manager/InputManager.h"

class Stage;

class SpeedCar : public Car
{
public:

	//�R���X�g���N�^
	SpeedCar(InputManager::JOYPAD_NO joyNum, Stage& stage);

	//�f�X�g���N�^
	~SpeedCar();

	//�Ԃ̏����l
	void CarInitParam(VECTOR pos) override;

	//�M�A���Ƃ̉����x
	void SyncGearSpeed(void) override;

	//MAX�X�s�[�h�擾
	float GetMaxSpeedACC(int gear) override;
};

