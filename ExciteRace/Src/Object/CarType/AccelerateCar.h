#pragma once
#include<DxLib.h>
#include "../Car.h"
#include "../../Manager/InputManager.h"

class Stage;

class AccelerateCar : public Car
{
public:

	//コンストラクタ
	AccelerateCar(InputManager::JOYPAD_NO joyNum, Stage& stage);

	//デストラクタ
	~AccelerateCar();

	//車の初期値
	void CarInitParam(VECTOR pos) override;

	//ギアごとの加速度
	void SyncGearSpeed(void) override;

	//MAXスピード取得
	float GetMaxSpeedACC(int gear) override;
};

