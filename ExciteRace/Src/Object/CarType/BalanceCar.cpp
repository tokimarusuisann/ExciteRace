#include "BalanceCar.h"

#pragma region �萔�錾
//�X�P�[��
constexpr VECTOR BALANCE_CAR_SCALE = { 0.8f,0.8f ,0.8f };
//��������
constexpr VECTOR BALANCE_CAR_INIT_ROT = { 0.0f,180*DX_PI_F/180 ,0.0f};
//�o�����X�J�[�̃��[�J���^�C���ʒu
constexpr VECTOR BALANCE_CAR_WHEEL_FR_LOCAL_POS = { 70, 28, 135 };
constexpr VECTOR BALANCE_CAR_WHEEL_FL_LOCAL_POS = { -70, 28, 135 };
constexpr VECTOR BALANCE_CAR_WHEEL_BR_LOCAL_POS = { 70, 28, -135 };
constexpr VECTOR BALANCE_CAR_WHEEL_BL_LOCAL_POS = { -70, 28, -135 };
//�o�����X�J�[�̃^�C���X�P�[��
constexpr VECTOR BALANCE_CAR_WHEEL_SCALE = { 130.0f, 130.0f, 130.0f };
//�o�����X�J�[�̃M�A���Ƃ�MAX�X�s�[�h
constexpr float MAX_SPEED_BALANCE_CAR[] = { 30.0f, 50.0f, 70.0f, 85.0f, 100.0f };
#pragma endregion

BalanceCar::BalanceCar(InputManager::JOYPAD_NO joyNum, Stage& stage) : Car(joyNum, stage)
{
}

BalanceCar::~BalanceCar()
{
}

void BalanceCar::CarInitParam(VECTOR pos)
{
	auto& resIns = ResourceManager::GetInstance();

	//��
	transformCar_.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::BALANCE_CAR));
	transformCar_.pos_ = pos;
	transformCar_.scl_ = BALANCE_CAR_SCALE;
	transformCar_.quaRot_ = Quaternion();
	transformCar_.quaRotLocal_ = Quaternion::Euler(BALANCE_CAR_INIT_ROT);

	//�^�C���̃��[�J���|�W�V����
	wheelInfos_[WHEEL_TYPE::FR].localPos = BALANCE_CAR_WHEEL_FR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::FL].localPos = BALANCE_CAR_WHEEL_FL_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BR].localPos = BALANCE_CAR_WHEEL_BR_LOCAL_POS;
	wheelInfos_[WHEEL_TYPE::BL].localPos = BALANCE_CAR_WHEEL_BL_LOCAL_POS;

	//�^�C���̌���
	wheelInfos_[WHEEL_TYPE::FR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::FL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);
	wheelInfos_[WHEEL_TYPE::BR].transform.quaRotLocal_ = Quaternion();
	wheelInfos_[WHEEL_TYPE::BL].transform.quaRotLocal_ = Quaternion::Euler(WHEEL_INIT_ROT);

	for (auto& wheelInfo : wheelInfos_)
	{
		wheelInfos_[wheelInfo.first].transform.SetModel(resIns.LoadModelDuplicate(ResourceManager::SRC::WHEEL));
		wheelInfos_[wheelInfo.first].transform.pos_ = AsoUtility::VECTOR_ZERO;
		wheelInfos_[wheelInfo.first].transform.scl_ = BALANCE_CAR_WHEEL_SCALE;
		wheelInfos_[wheelInfo.first].transform.Update();
	}

	transformCar_.Update();
}

void BalanceCar::ChangeGear(void)
{	
	//�M�A�`�F���W�Əグ�����������̏���
	bool gearChangeFlag = false;

	for (int i = 0; i < 5; i++)
	{
		//AT�̂ݎ����ŃM�A�ς���
		if (speed_ >= MAX_SPEED_BALANCE_CAR[i] && gearNum_ == i + 1)
		{
			if (gearNum_ <= 4)
			{
				gearNum_ += 1; 
				gearChangeFlag = true;
			}
		}
		if (speed_ <= MAX_SPEED_BALANCE_CAR[i] && gearNum_ >= i + 2)
		{
			gearNum_ -= 1;
			gearChangeFlag = true;
		}
	}

	//Low�̒l��High�̒l�𒴂��Ă͂����Ȃ�
	gearNum_ = std::clamp(gearNum_, 1, 5);

	//�X�s�[�h�̍�������
	float difSpeed = speed_ - MAX_SPEED_BALANCE_CAR[gearNum_ - 1];
	if (difSpeed > 0)
	{
		Deceleration(std::min(difSpeed, 0.5f));
	}

	//SE�Đ�
	if (gearChangeFlag)
	{
		SoundManager::GetInstance().MyPlaySound(Application::PATH_SOUND + "�Ԃ̃M�A�`�F���W.mp3", false);
	}
}

void BalanceCar::SyncGearSpeed(void)
{
	accel_ = SPEED_GEAR[gearNum_ - 1];
}

float BalanceCar::GetMaxSpeedACC(int gear)
{
	return MAX_SPEED_BALANCE_CAR[gear];
}
