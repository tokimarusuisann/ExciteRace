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

	//コンストラクタ
	BalanceCar(InputManager::JOYPAD_NO joyNum, Stage& stage);

	//デストラクタ
	~BalanceCar();

	//車の初期値
	void CarInitParam(VECTOR pos) override;

	//この車だけATなのでChangeGear関数を持つ
	void ChangeGear(void) override;

	//ギアごとの加速度
	void SyncGearSpeed(void) override;

	//MAXスピード取得
	float GetMaxSpeedACC(int gear) override;
};

