#include "AccelerateCar.h"

#pragma region �萔�錾
//�X�P�[��
constexpr VECTOR ACCELERATE_CAR_SCALE = { 110.0f,110.0f ,110.0f };
//�����J�[�̃��[�J���^�C���ʒu
constexpr VECTOR ACCELERATE_CAR_WHEEL_FR_LOCAL_POS = { 70, 23, 155 };
constexpr VECTOR ACCELERATE_CAR_WHEEL_FL_LOCAL_POS = { -70, 23, 155 };
constexpr VECTOR ACCELERATE_CAR_WHEEL_BR_LOCAL_POS = { 70, 28, -135 };
constexpr VECTOR ACCELERATE_CAR_WHEEL_BL_LOCAL_POS = { -70, 28, -135 };
//�����J�[�̃^�C���T�C�Y
constexpr VECTOR ACCELERATE_CAR_WHEEL_SCALE = { 120.0f ,120.0f ,120.0f };
//�����J�[�̃M�A���Ƃ�MAX�X�s�[�h
constexpr float MAX_SPEED_ACCELERATE_CAR[] = { 30.0f, 50.0f, 70.0f, 85.0f, 100.0f };
//�����J�[�̉����x
static constexpr float ACCELERATE_SPEED_GEAR[] = { 0.29f ,0.25f,0.2f,0.16f,0.13f };
#pragma endregion

AccelerateCar::AccelerateCar(InputManager::JOYPAD_NO joyNum, Stage& stage):Car(joyNum, stage)
{
}

AccelerateCar::~AccelerateCar()
{
}

void AccelerateCar::CarInitParam(VECTOR pos)
{
	//MAX�X�s�[�h������
	for (int i = 0; i < GIAR_MAX_NUM; i++)
	{
		maxSpeed_[i] = MAX_SPEED_ACCELERATE_CAR[i];
	}

	auto& resIns = ResourceManager::GetInstance();

	//��
	transformCar_.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::ACCELERATE_CAR));
	transformCar_.pos_ = pos;
	transformCar_.scl_ = ACCELERATE_CAR_SCALE;
	transformCar_.quaRot_ = Quaternion();
	transformCar_.quaRotLocal_ = Quaternion();

	//�^�C���̃��[�J���|�W�V����
	wheelInfos_[WHEEL_TYPE::FR].localPos = ACCELERATE_CAR_WHEEL_FR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::FL].localPos = ACCELERATE_CAR_WHEEL_FL_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BR].localPos = ACCELERATE_CAR_WHEEL_BR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BL].localPos = ACCELERATE_CAR_WHEEL_BL_LOCAL_POS;

	//�^�C���̌���
	wheelInfos_[WHEEL_TYPE::FR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::FL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);
	wheelInfos_[WHEEL_TYPE::BR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::BL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);

	for (auto& wheelInfo : wheelInfos_)
	{
		wheelInfos_[wheelInfo.first].transform.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::WHEEL));
		wheelInfos_[wheelInfo.first].transform.pos_ = AsoUtility::VECTOR_ZERO;
		wheelInfos_[wheelInfo.first].transform.scl_ = ACCELERATE_CAR_WHEEL_SCALE;
		wheelInfos_[wheelInfo.first].transform.Update();
	}

	transformCar_.Update();
}

void AccelerateCar::SyncGearSpeed(void)
{
	accel_ = ACCELERATE_SPEED_GEAR[gearNum_ - 1];
}

float AccelerateCar::GetMaxSpeedACC(int gear)
{
	return MAX_SPEED_ACCELERATE_CAR[gear];
}
