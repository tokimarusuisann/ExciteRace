#include <cmath>
#include<EffekseerForDXLIB.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "Stage.h"
#include "Car.h"

#pragma region 定数宣言

//ブレーキ時の減速
constexpr double SPEED_DOWN_BREAK_POWER = 0.6f;

//曲がるときの減速
constexpr double TURN_SPEED_DOWN_POWER = 0.05;

//ゴール時の減速
constexpr double GOAL_SPEED_DOWN_POWER = 0.3f;

//当たり判定用カプセルの上と下の位置
constexpr VECTOR CUPSULE_TOP ={ 0.0f,120.0f,180.0f };
constexpr VECTOR CUPSULE_BOT = { 0.0f,120.0f,-180.0f };

//コントローラーのデッドゾーン
constexpr int DEADZONE = 2000;

//右回転のMAX回転
constexpr float MAX_RIGHT_ROT = 32768.0f;

//左回転のMAX回転
constexpr float MAX_LEFT_ROT = -32768.0f;

//曲がる際の減算スピード制限
constexpr float SPEED_RIMIT = 40.0f;

//ブレーキ時回転量を足す際の制限回転量
constexpr float ROTATE_RIMIT_BREAK = 0.2f;

//ブレーキ時の追加の回転量
constexpr float ADD_ROTATE = 0.4f;

//ステージの当たり判定時押し返す強さ
constexpr float STAGE_COLLISION_POWER = 3.0f;

//車同士の当たり判定時押し返す強さ
constexpr float CAR_COLLISION_POWER = 50.0f;

//バイブレーションの強さ
constexpr int VIBRATION_POWER = 1000;

//バイブレーションの時間
constexpr int VIBRATION_TIME = 300;

//移動先座標の長さ
constexpr float MOVED_POS_SIZE = 20.0f;

//ギア変更時の上限
constexpr int GEAR_CHANGE_MAX_LIMIT = 4;

//ギア変更時の下限
constexpr int GEAR_CHANGE_MIN_LIMIT = 2;

//押し返す力
constexpr float COLLISION_POWER = 0.2f;


#pragma endregion

Car::Car(InputManager::JOYPAD_NO joyNum, Stage& stage) : stage_(stage)
{
	//パッド番号
	joypadNum_ = joyNum;
}

Car::~Car(void)
{
	//メモリ開放
	MV1DeleteModel(transformCar_.modelId_);

	//タイヤのメモリ解放
	for (auto& wheelInfo : wheelInfos_)
	{
		MV1DeleteModel(wheelInfos_[wheelInfo.first].transform.modelId_);
	}
}

void Car::Init(CAR_TYPE carType, VECTOR pos)
{
	//車の初期パラメーター
	CarInitParam(pos);

	//車のスピード変数
	speed_ = 0.0f;

	//当たり判定前
	dirHitFront_ = AsoUtility::VECTOR_ZERO;

	//当たり判定後ろ
	dirHitBack_ = AsoUtility::VECTOR_ZERO;

	//移動差分
	moveDiff_ = AsoUtility::VECTOR_ZERO;

	//衝突判定時の押し返す力
	collisionPow_ = AsoUtility::VECTOR_ZERO;

	//ギア変数
	gearNum_ = 1;

	//加速度初期化
	accel_ = 0.0f;

}

void Car::Update(void)
{

	//移動処理
	ProcessMove();

	//回転処理
	ProcessTurn();

	//ギアチェンジ
	ChangeGear();

	//衝突関数
	Collision();

	//タイヤ同期
	SyncWheel();

	//車の更新
	transformCar_.Update();

	//タイヤの更新
	for (auto& wheelInfo : wheelInfos_)
	{
		wheelInfos_[wheelInfo.first].transform.Update();
	}

};

void Car::Draw(void)
{

	//車の描画
	MV1DrawModel(transformCar_.modelId_);

	//タイヤの描画
	for (auto& wheelInfo : wheelInfos_)
	{
		MV1DrawModel(wheelInfos_[wheelInfo.first].transform.modelId_);
	}

}

void Car::ProcessMove(void)
{
	//ギアが低い時は加速が早いが高い時は加速が遅い
	//ギアが低い時は最高速が低いが高い時は最高速が高い
	//スピードが低いときにギアを上げすぎると加速しなくなるが高いときにギアを下げすぎると加速しなくなる

	//インスタンス
	auto& ins = InputManager::GetInstance();

	//スピード制御
	if (speed_ <= 0.0f)
	{
		speed_ = 0.0f;
	}

	SyncGearSpeed();

	if (ins.IsPadBtnNew(joypadNum_, InputManager::JOYPAD_BTN::R_TRIGGER))
	{
		Accelerate(accel_);
	}
	else
	{
		//トリガーを引いていない間は減速する
		if (speed_ > 0.0)
		{
			Deceleration(SPEED_DOWN_BREAK_POWER);
		}
	}

	//ブレーキ
	if (ins.IsPadBtnNew(joypadNum_, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		Deceleration(SPEED_DOWN_BREAK_POWER);
	}

}

void Car::ProcessTurn(void)
{
	//インスタンス
	auto& ins = InputManager::GetInstance();
	auto cState = ins.GetJPadInputState(joypadNum_);

	if (speed_ > 0.0f)
	{
		//右旋回
		if (cState.AKeyLX > DEADZONE && cState.AKeyLX <= MAX_RIGHT_ROT)
		{
			ProcessTurnDirection(cState.AKeyLX, DEADZONE, MAX_RIGHT_ROT,1);
		}

		//左旋回
		if (cState.AKeyLX < -DEADZONE && cState.AKeyLX >= MAX_LEFT_ROT)
		{
			ProcessTurnDirection(cState.AKeyLX, DEADZONE, MAX_LEFT_ROT, -1);
		}
	}

	//当たり判定用カプセルを回転
	cupsuleTopRot_ = Quaternion::PosAxis(transformCar_.quaRot_, CUPSULE_TOP);
	cupsuleBotRot_ = Quaternion::PosAxis(transformCar_.quaRot_, CUPSULE_BOT);
}

void Car::ProcessTurnDirection(int stickValue, int deadzone, float maxRot, int direction)
{	
	//ハンドルコントローラーで動かした分だけ曲がる処理
	auto& ins = InputManager::GetInstance();
	
	//回転力計算
	float num = ((stickValue - deadzone) * direction)/ (maxRot - deadzone);

	// 速度が速い場合は速度を落としながら曲がる
	if (speed_ >= SPEED_RIMIT)
	{
		Deceleration(TURN_SPEED_DOWN_POWER);
	}

	// ブレーキ時は旋回しやすくする
	if (num * direction > ROTATE_RIMIT_BREAK && ins.IsPadBtnNew(joypadNum_, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		num += ADD_ROTATE * direction;
	}

	//回転
	Turn(AsoUtility::Deg2RadF(num), AsoUtility::AXIS_Y);
}

void Car::Turn(double rad, VECTOR axis)
{
	//指定軸(axis)に対する回転量(rad)を加えた回転(quaternion)を作る
	Quaternion axisQua = Quaternion::AngleAxis(rad, axis);

	//車の回転に、上で作成した回転を合成する
	transformCar_.quaRot_ = transformCar_.quaRot_.Mult(axisQua);
}

void Car::SyncWheel(void)
{
	for (auto& wheel : wheelInfos_)
	{
		wheel.second.rotatePos = Quaternion::PosAxis(transformCar_.quaRot_, wheel.second.localPos);
		wheel.second.transform.pos_ = VAdd(transformCar_.pos_, wheel.second.rotatePos);
		wheel.second.transform.quaRot_ = transformCar_.quaRot_;
	}
}

void Car::Accelerate(float acceleration)
{
	//加速処理
	speed_ += acceleration;
}

void Car::Deceleration(float deceleration)
{
	//減速処理
	speed_ -= deceleration;

	if (speed_ <= 0.0)
	{
		speed_ = 0.0;
	}
}

void Car::ChangeGear(void)
{
	//ギアチェンジと上げ下げした時の処理
	bool gearChangeFlag = false;

	auto& ins = InputManager::GetInstance();

	//ギアチェンジアップ
	if (ins.IsPadBtnTrgDown(joypadNum_, InputManager::JOYPAD_BTN::R_BULLET))
	{
		if (gearNum_ <= GEAR_CHANGE_MAX_LIMIT)
		{
			gearNum_ += 1;
			gearChangeFlag = true;
		}
	}

	//ギアチェンジダウン

	if (ins.IsPadBtnTrgDown(joypadNum_, InputManager::JOYPAD_BTN::L_BULLET))
	{
		if (gearNum_ >= GEAR_CHANGE_MIN_LIMIT)
		{
			gearNum_ -= 1;
			gearChangeFlag = true;
		}
	}

	//Lowの値はHighの値を超えてはいけない
	gearNum_ = std::clamp(gearNum_, 1, 5);

	//スピードの差分引く
	float difSpeed = speed_ - maxSpeed_[gearNum_ - 1];

	if (difSpeed > 0)
	{
		Deceleration(std::min(difSpeed, 0.5f));
	}

	//SE再生
	if (gearChangeFlag)
	{
		SoundManager::GetInstance().PlaySound(Application::PATH_SOUND + "車のギアチェンジ.mp3", false);
	}
}

const Transform& Car::GetTransform(void) const
{
	return transformCar_;
}

VECTOR Car::GetPos(void) const
{
	return transformCar_.pos_;
}

VECTOR Car::GetPosTop(void) const
{
	return  VAdd(transformCar_.pos_, cupsuleTopRot_);
}

VECTOR Car::GetPosBottom(void) const
{
	return  VAdd(transformCar_.pos_, cupsuleBotRot_);
}

int Car::GetGearNum(void) const
{
	return gearNum_;
}

float Car::GetSpeed(void) const
{
	return speed_;
}

void Car::AddCol(int collider_)
{
	colliders_.emplace_back(collider_);
}

void Car::Collision(void)
{	
	//前方向
	VECTOR forward = transformCar_.GetForward();

	//線形補間で押し戻す
	collisionPow_ = AsoUtility::Lerp(collisionPow_, AsoUtility::VECTOR_ZERO, COLLISION_POWER);

	//移動先の座標
	movedPos_ = VAdd(transformCar_.pos_, VScale(forward, speed_));
	
	//押し戻した座標を足す
	movedPos_ = VAdd(movedPos_, collisionPow_);
	
	CollisionGrvity();

	CollisionCapsule();

	//移動前座標と移動先座標の線分で衝突判定を行う
	for (auto collider : colliders_)
	{
		auto hit = MV1CollCheck_Line(collider, -1, transformCar_.pos_, movedPos_);

		//衝突していたら
		if (hit.HitFlag > 0)
		{
			auto& nomal = hit.Normal;

			//高さは無視する
			nomal = VNorm(nomal);
			nomal.y = 0.0f;

			//押し戻し処理
			movedPos_ = VAdd(hit.HitPosition, VScale(nomal, MOVED_POS_SIZE));

			if (InputManager::JOYPAD_NO::PAD1 == joypadNum_)
			{
				StartJoypadVibration(DX_INPUT_PAD1, VIBRATION_POWER, VIBRATION_TIME);
			}
			if (InputManager::JOYPAD_NO::PAD2 == joypadNum_)
			{
				StartJoypadVibration(DX_INPUT_PAD2, VIBRATION_POWER, VIBRATION_TIME);
			}
			//減速処理
			Deceleration(SPEED_DOWN_BREAK_POWER);
		}
	}
	//更新
	transformCar_.pos_ = movedPos_;

}

void Car::CollisionCapsule(void)
{
	VECTOR playerHitTop = VAdd(movedPos_, CUPSULE_TOP);
	VECTOR playerHitBot = VAdd(movedPos_, CUPSULE_BOT);

	//カプセルとステージモデルの衝突判定
	for (auto collider : colliders_)
	{
		auto hits = MV1CollCheck_Capsule(
			collider, -1,
			playerHitTop,
			playerHitBot,
			COLLISION_RADIUS
		);

		for (int i = 0; i < hits.HitNum; i++)
		{
			//一つ一つの衝突情報を取得する
			auto hit = hits.Dim[i];

			for (int tryCount = 0; tryCount < 10; tryCount++)
			{
				//処理負荷を減らすために、1ポリゴンと
				//カプセルの衝突判定にする
				int polygonHit = HitCheck_Capsule_Triangle(
					playerHitTop,
					playerHitBot,
					COLLISION_RADIUS,
					hit.Position[0],
					hit.Position[1],
					hit.Position[2]
				);

				if (polygonHit)
				{
					//衝突したポリゴンの法線
					auto nomal = hit.Normal;

					movedPos_ = VAdd(movedPos_, VScale(nomal, STAGE_COLLISION_POWER));

					//カプセルを移動させる
					playerHitBot = VAdd(movedPos_, CUPSULE_BOT);
					playerHitTop = VAdd(movedPos_, CUPSULE_TOP);

					continue;
				}

				//衝突していないので終了
				break;
			}
		}
		//検出したポリゴンとの衝突情報を後始末する
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Car::CollisionGrvity(void)
{
	//重力の方向
	VECTOR dirGravity = SceneManager::GetInstance().GetGravityDir();

	//重力の反対方向
	VECTOR dirUpGravity = VScale(dirGravity, -1.0f);

	//重力の強さ
	float gravityPow = SceneManager::GetInstance().GetGravityPow();

	//重力
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//重力処理
	movedPos_ = VAdd(movedPos_, gravity);
}

bool Car::IsHitGoal(void)
{
	bool ret = false;

	//車の回転と同期
	VECTOR syncCarDirFront = Quaternion::PosAxis(transformCar_.quaRot_, AsoUtility::DIR_F);
	VECTOR syncCarDirBack = Quaternion::PosAxis(transformCar_.quaRot_, AsoUtility::DIR_B);

	//車のちょっと前
	dirHitFront_ = VAdd(movedPos_, VScale(syncCarDirFront, CAR_COLLISION_POWER));

	//車のちょっと後ろ
	dirHitBack_ = VAdd(movedPos_, VScale(syncCarDirBack, CAR_COLLISION_POWER));

	auto hit = MV1CollCheck_Line(stage_.GetModelIdGoalCollision(), -1, dirHitFront_, dirHitBack_);
	if (hit.HitFlag > 0)
	{
		ret = true;
	}
	return ret;
}

void Car::SetCollisionPow(VECTOR pow)
{
	collisionPow_ = pow;
}

InputManager::JOYPAD_NO Car::GetPadNo(void) const
{
	return joypadNum_;
}
