#pragma once
#include<DxLib.h>
#include "../Car.h"
#include "../../Application.h"
#include "../../Manager/InputManager.h"
#include "../../Manager/SoundManager.h"

class Stage;

class BalanceCar : public Car
{
public:

	//�R���X�g���N�^
	BalanceCar(InputManager::JOYPAD_NO joyNum, Stage& stage);

	//�f�X�g���N�^
	~BalanceCar();

	//�Ԃ̏����l
	void CarInitParam(VECTOR pos) override;

	//���̎Ԃ���AT�Ȃ̂�ChangeGear�֐�������
	void ChangeGear(void) override;

	//�M�A���Ƃ̉����x
	void SyncGearSpeed(void) override;

	//MAX�X�s�[�h�擾
	float GetMaxSpeedACC(int gear) override;
};

