#pragma once
#include<DxLib.h>
#include "../Car.h"
#include "../../Manager/InputManager.h"

class Stage;

class SpeedCar : public Car
{
public:

	//コンストラクタ
	SpeedCar(InputManager::JOYPAD_NO joyNum, Stage& stage);

	//デストラクタ
	~SpeedCar();

	//車の初期値
	void CarInitParam(VECTOR pos) override;

	//ギアごとの加速度
	void SyncGearSpeed(void) override;

	//MAXスピード取得
	float GetMaxSpeedACC(int gear) override;
};

