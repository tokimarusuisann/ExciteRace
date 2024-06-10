#include <cmath>
#include<EffekseerForDXLIB.h>
#include "../Application.h"
#include "../Manager/SceneManager.h"
#include "../Manager/SoundManager.h"
#include "Stage.h"
#include "Car.h"

#pragma region �萔�錾

//�u���[�L���̌���
constexpr double SPEED_DOWN_BREAK_POWER = 0.6f;

//�Ȃ���Ƃ��̌���
constexpr double TURN_SPEED_DOWN_POWER = 0.05;

//�S�[�����̌���
constexpr double GOAL_SPEED_DOWN_POWER = 0.3f;

//�����蔻��p�J�v�Z���̏�Ɖ��̈ʒu
constexpr VECTOR CUPSULE_TOP ={ 0.0f,120.0f,180.0f };
constexpr VECTOR CUPSULE_BOT = { 0.0f,120.0f,-180.0f };

//�R���g���[���[�̃f�b�h�]�[��
constexpr int DEADZONE = 2000;

//�E��]��MAX��]
constexpr float MAX_RIGHT_ROT = 32768.0f;

//����]��MAX��]
constexpr float MAX_LEFT_ROT = -32768.0f;

//�Ȃ���ۂ̌��Z�X�s�[�h����
constexpr float SPEED_RIMIT = 40.0f;

//�u���[�L����]�ʂ𑫂��ۂ̐�����]��
constexpr float ROTATE_RIMIT_BREAK = 0.2f;

//�u���[�L���̒ǉ��̉�]��
constexpr float ADD_ROTATE = 0.4f;

//�X�e�[�W�̓����蔻�莞�����Ԃ�����
constexpr float STAGE_COLLISION_POWER = 3.0f;

//�ԓ��m�̓����蔻�莞�����Ԃ�����
constexpr float CAR_COLLISION_POWER = 50.0f;

//�o�C�u���[�V�����̋���
constexpr int VIBRATION_POWER = 1000;

//�o�C�u���[�V�����̎���
constexpr int VIBRATION_TIME = 300;

//�ړ�����W�̒���
constexpr float MOVED_POS_SIZE = 20.0f;

//�M�A�ύX���̏��
constexpr int GEAR_CHANGE_MAX_LIMIT = 4;

//�M�A�ύX���̉���
constexpr int GEAR_CHANGE_MIN_LIMIT = 2;

//�����Ԃ���
constexpr float COLLISION_POWER = 0.2f;


#pragma endregion

Car::Car(InputManager::JOYPAD_NO joyNum, Stage& stage) : stage_(stage)
{
	//�p�b�h�ԍ�
	joypadNum_ = joyNum;
}

Car::~Car(void)
{
	//�������J��
	MV1DeleteModel(transformCar_.modelId_);

	//�^�C���̃��������
	for (auto& wheelInfo : wheelInfos_)
	{
		MV1DeleteModel(wheelInfos_[wheelInfo.first].transform.modelId_);
	}
}

void Car::Init(CAR_TYPE carType, VECTOR pos)
{
	//�Ԃ̏����p�����[�^�[
	CarInitParam(pos);

	//�Ԃ̃X�s�[�h�ϐ�
	speed_ = 0.0f;

	//�����蔻��O
	dirHitFront_ = AsoUtility::VECTOR_ZERO;

	//�����蔻����
	dirHitBack_ = AsoUtility::VECTOR_ZERO;

	//�ړ�����
	moveDiff_ = AsoUtility::VECTOR_ZERO;

	//�Փ˔��莞�̉����Ԃ���
	collisionPow_ = AsoUtility::VECTOR_ZERO;

	//�M�A�ϐ�
	gearNum_ = 1;

	//�����x������
	accel_ = 0.0f;

}

void Car::Update(void)
{

	//�ړ�����
	ProcessMove();

	//��]����
	ProcessTurn();

	//�M�A�`�F���W
	ChangeGear();

	//�Փˊ֐�
	Collision();

	//�^�C������
	SyncWheel();

	//�Ԃ̍X�V
	transformCar_.Update();

	//�^�C���̍X�V
	for (auto& wheelInfo : wheelInfos_)
	{
		wheelInfos_[wheelInfo.first].transform.Update();
	}

};

void Car::Draw(void)
{

	//�Ԃ̕`��
	MV1DrawModel(transformCar_.modelId_);

	//�^�C���̕`��
	for (auto& wheelInfo : wheelInfos_)
	{
		MV1DrawModel(wheelInfos_[wheelInfo.first].transform.modelId_);
	}

}

void Car::ProcessMove(void)
{
	//�M�A���Ⴂ���͉������������������͉������x��
	//�M�A���Ⴂ���͍ō������Ⴂ���������͍ō���������
	//�X�s�[�h���Ⴂ�Ƃ��ɃM�A���グ������Ɖ������Ȃ��Ȃ邪�����Ƃ��ɃM�A������������Ɖ������Ȃ��Ȃ�

	//�C���X�^���X
	auto& ins = InputManager::GetInstance();

	//�X�s�[�h����
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
		//�g���K�[�������Ă��Ȃ��Ԃ͌�������
		if (speed_ > 0.0)
		{
			Deceleration(SPEED_DOWN_BREAK_POWER);
		}
	}

	//�u���[�L
	if (ins.IsPadBtnNew(joypadNum_, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		Deceleration(SPEED_DOWN_BREAK_POWER);
	}

}

void Car::ProcessTurn(void)
{
	//�C���X�^���X
	auto& ins = InputManager::GetInstance();
	auto cState = ins.GetJPadInputState(joypadNum_);

	if (speed_ > 0.0f)
	{
		//�E����
		if (cState.AKeyLX > DEADZONE && cState.AKeyLX <= MAX_RIGHT_ROT)
		{
			ProcessTurnDirection(cState.AKeyLX, DEADZONE, MAX_RIGHT_ROT,1);
		}

		//������
		if (cState.AKeyLX < -DEADZONE && cState.AKeyLX >= MAX_LEFT_ROT)
		{
			ProcessTurnDirection(cState.AKeyLX, DEADZONE, MAX_LEFT_ROT, -1);
		}
	}

	//�����蔻��p�J�v�Z������]
	cupsuleTopRot_ = Quaternion::PosAxis(transformCar_.quaRot_, CUPSULE_TOP);
	cupsuleBotRot_ = Quaternion::PosAxis(transformCar_.quaRot_, CUPSULE_BOT);
}

void Car::ProcessTurnDirection(int stickValue, int deadzone, float maxRot, int direction)
{	
	//�n���h���R���g���[���[�œ��������������Ȃ��鏈��
	auto& ins = InputManager::GetInstance();
	
	//��]�͌v�Z
	float num = ((stickValue - deadzone) * direction)/ (maxRot - deadzone);

	// ���x�������ꍇ�͑��x�𗎂Ƃ��Ȃ���Ȃ���
	if (speed_ >= SPEED_RIMIT)
	{
		Deceleration(TURN_SPEED_DOWN_POWER);
	}

	// �u���[�L���͐��񂵂₷������
	if (num * direction > ROTATE_RIMIT_BREAK && ins.IsPadBtnNew(joypadNum_, InputManager::JOYPAD_BTN::L_TRIGGER))
	{
		num += ADD_ROTATE * direction;
	}

	//��]
	Turn(AsoUtility::Deg2RadF(num), AsoUtility::AXIS_Y);
}

void Car::Turn(double rad, VECTOR axis)
{
	//�w�莲(axis)�ɑ΂����]��(rad)����������](quaternion)�����
	Quaternion axisQua = Quaternion::AngleAxis(rad, axis);

	//�Ԃ̉�]�ɁA��ō쐬������]����������
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
	//��������
	speed_ += acceleration;
}

void Car::Deceleration(float deceleration)
{
	//��������
	speed_ -= deceleration;

	if (speed_ <= 0.0)
	{
		speed_ = 0.0;
	}
}

void Car::ChangeGear(void)
{
	//�M�A�`�F���W�Əグ�����������̏���
	bool gearChangeFlag = false;

	auto& ins = InputManager::GetInstance();

	//�M�A�`�F���W�A�b�v
	if (ins.IsPadBtnTrgDown(joypadNum_, InputManager::JOYPAD_BTN::R_BULLET))
	{
		if (gearNum_ <= GEAR_CHANGE_MAX_LIMIT)
		{
			gearNum_ += 1;
			gearChangeFlag = true;
		}
	}

	//�M�A�`�F���W�_�E��

	if (ins.IsPadBtnTrgDown(joypadNum_, InputManager::JOYPAD_BTN::L_BULLET))
	{
		if (gearNum_ >= GEAR_CHANGE_MIN_LIMIT)
		{
			gearNum_ -= 1;
			gearChangeFlag = true;
		}
	}

	//Low�̒l��High�̒l�𒴂��Ă͂����Ȃ�
	gearNum_ = std::clamp(gearNum_, 1, 5);

	//�X�s�[�h�̍�������
	float difSpeed = speed_ - maxSpeed_[gearNum_ - 1];

	if (difSpeed > 0)
	{
		Deceleration(std::min(difSpeed, 0.5f));
	}

	//SE�Đ�
	if (gearChangeFlag)
	{
		SoundManager::GetInstance().PlaySound(Application::PATH_SOUND + "�Ԃ̃M�A�`�F���W.mp3", false);
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
	//�O����
	VECTOR forward = transformCar_.GetForward();

	//���`��Ԃŉ����߂�
	collisionPow_ = AsoUtility::Lerp(collisionPow_, AsoUtility::VECTOR_ZERO, COLLISION_POWER);

	//�ړ���̍��W
	movedPos_ = VAdd(transformCar_.pos_, VScale(forward, speed_));
	
	//�����߂������W�𑫂�
	movedPos_ = VAdd(movedPos_, collisionPow_);
	
	CollisionGrvity();

	CollisionCapsule();

	//�ړ��O���W�ƈړ�����W�̐����ŏՓ˔�����s��
	for (auto collider : colliders_)
	{
		auto hit = MV1CollCheck_Line(collider, -1, transformCar_.pos_, movedPos_);

		//�Փ˂��Ă�����
		if (hit.HitFlag > 0)
		{
			auto& nomal = hit.Normal;

			//�����͖�������
			nomal = VNorm(nomal);
			nomal.y = 0.0f;

			//�����߂�����
			movedPos_ = VAdd(hit.HitPosition, VScale(nomal, MOVED_POS_SIZE));

			if (InputManager::JOYPAD_NO::PAD1 == joypadNum_)
			{
				StartJoypadVibration(DX_INPUT_PAD1, VIBRATION_POWER, VIBRATION_TIME);
			}
			if (InputManager::JOYPAD_NO::PAD2 == joypadNum_)
			{
				StartJoypadVibration(DX_INPUT_PAD2, VIBRATION_POWER, VIBRATION_TIME);
			}
			//��������
			Deceleration(SPEED_DOWN_BREAK_POWER);
		}
	}
	//�X�V
	transformCar_.pos_ = movedPos_;

}

void Car::CollisionCapsule(void)
{
	VECTOR playerHitTop = VAdd(movedPos_, CUPSULE_TOP);
	VECTOR playerHitBot = VAdd(movedPos_, CUPSULE_BOT);

	//�J�v�Z���ƃX�e�[�W���f���̏Փ˔���
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
			//���̏Փˏ����擾����
			auto hit = hits.Dim[i];

			for (int tryCount = 0; tryCount < 10; tryCount++)
			{
				//�������ׂ����炷���߂ɁA1�|���S����
				//�J�v�Z���̏Փ˔���ɂ���
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
					//�Փ˂����|���S���̖@��
					auto nomal = hit.Normal;

					movedPos_ = VAdd(movedPos_, VScale(nomal, STAGE_COLLISION_POWER));

					//�J�v�Z�����ړ�������
					playerHitBot = VAdd(movedPos_, CUPSULE_BOT);
					playerHitTop = VAdd(movedPos_, CUPSULE_TOP);

					continue;
				}

				//�Փ˂��Ă��Ȃ��̂ŏI��
				break;
			}
		}
		//���o�����|���S���Ƃ̏Փˏ�����n������
		MV1CollResultPolyDimTerminate(hits);
	}
}

void Car::CollisionGrvity(void)
{
	//�d�͂̕���
	VECTOR dirGravity = SceneManager::GetInstance().GetGravityDir();

	//�d�͂̔��Ε���
	VECTOR dirUpGravity = VScale(dirGravity, -1.0f);

	//�d�͂̋���
	float gravityPow = SceneManager::GetInstance().GetGravityPow();

	//�d��
	VECTOR gravity = VScale(dirGravity, gravityPow);

	//�d�͏���
	movedPos_ = VAdd(movedPos_, gravity);
}

bool Car::IsHitGoal(void)
{
	bool ret = false;

	//�Ԃ̉�]�Ɠ���
	VECTOR syncCarDirFront = Quaternion::PosAxis(transformCar_.quaRot_, AsoUtility::DIR_F);
	VECTOR syncCarDirBack = Quaternion::PosAxis(transformCar_.quaRot_, AsoUtility::DIR_B);

	//�Ԃ̂�����ƑO
	dirHitFront_ = VAdd(movedPos_, VScale(syncCarDirFront, CAR_COLLISION_POWER));

	//�Ԃ̂�����ƌ��
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
