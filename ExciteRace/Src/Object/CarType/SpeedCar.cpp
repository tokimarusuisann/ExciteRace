#include "SpeedCar.h"

#pragma region 定数宣言
//初期方向
constexpr VECTOR SPEED_CAR_INIT_ROT = { 0.0f,180 * DX_PI_F / 180 ,0.0f };
//スピードカーのローカルタイヤ位置
constexpr VECTOR SPEED_CAR_WHEEL_FR_LOCAL_POS = { 78, 35, 145 };
constexpr VECTOR SPEED_CAR_WHEEL_FL_LOCAL_POS = { -78, 35, 145 };
constexpr VECTOR SPEED_CAR_WHEEL_BR_LOCAL_POS = { 78, 35, -135 };
constexpr VECTOR SPEED_CAR_WHEEL_BL_LOCAL_POS = { -78, 35, -135 };
//スピードカーのタイヤスケール
constexpr VECTOR SPEED_CAR_WHEEL_SCALE = { 130.0f, 130.0f, 130.0f };
//スピードカーのギアごとのMAXスピード
constexpr float MAX_SPEED_SPEED_CAR[] = { 35.0f, 60.0f, 80.0f, 100.0f, 110.0f };
#pragma endregion


SpeedCar::SpeedCar(InputManager::JOYPAD_NO joyNum, Stage& stage) :Car(joyNum, stage)
{
}

SpeedCar::~SpeedCar()
{
}

void SpeedCar::CarInitParam(VECTOR pos)
{
	//MAXスピード初期化
	for (int i = 0; i < GIAR_MAX_NUM; i++)
	{
		maxSpeed_[i] = MAX_SPEED_SPEED_CAR[i];
	}

	auto& resIns = ResourceManager::GetInstance();

	//車
	transformCar_.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::SPEED_CAR));
	transformCar_.pos_ = pos;
	transformCar_.scl_ = AsoUtility::VECTOR_ONE;;
	transformCar_.quaRot_ = Quaternion();
	transformCar_.quaRotLocal_ = Quaternion::Euler(SPEED_CAR_INIT_ROT);

	//タイヤのローカルポジション
	wheelInfos_[WHEEL_TYPE::FR].localPos = SPEED_CAR_WHEEL_FR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::FL].localPos = SPEED_CAR_WHEEL_FL_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BR].localPos = SPEED_CAR_WHEEL_BR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BL].localPos = SPEED_CAR_WHEEL_BL_LOCAL_POS;

	//タイヤの向き
	wheelInfos_[WHEEL_TYPE::FR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::FL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);
	wheelInfos_[WHEEL_TYPE::BR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::BL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);

	for (auto& wheelInfo : wheelInfos_)
	{
		wheelInfos_[wheelInfo.first].transform.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::WHEEL));
		wheelInfos_[wheelInfo.first].transform.pos_ = AsoUtility::VECTOR_ZERO;
		wheelInfos_[wheelInfo.first].transform.scl_ = SPEED_CAR_WHEEL_SCALE;
		wheelInfos_[wheelInfo.first].transform.Update();
	}

	transformCar_.Update();
}

void SpeedCar::SyncGearSpeed(void)
{
	accel_ = SPEED_GEAR[gearNum_ - 1];
}

float SpeedCar::GetMaxSpeedACC(int gear)
{
	return MAX_SPEED_SPEED_CAR[gear];
}
