#pragma once
#include<DxLib.h>
#include <map> 
#include <array> 
#include<vector>
#include "Common/Transform.h"
#include "../Application.h"
#include "../Manager/InputManager.h"
#include "../Manager/ResourceManager.h"
#include "../Utility/AsoUtility.h"
#include "../Common/CollectStructure.h"


class Stage;

class Car
{
public:

	//�p����₽�N���X�Ŏg���萔�錾
	//�Փˊ֘A
	static constexpr float COLLISION_RADIUS = 120.0f;

	//�^�C���̏����̌���
	static constexpr VECTOR WHEEL_INIT_ROT = { 0.0f, 180.0f * DX_PI_F / 180, 0.0f };

	//�M�A�̐�
	static constexpr int GIAR_MAX_NUM = 5;

	//�M�A���Ƃ̈ړ��X�s�[�h
	static constexpr float SPEED_GEAR[] = { 0.26f ,0.22f,0.17f,0.13f,0.1f };

	// �R���X�g���N�^
	Car( InputManager::JOYPAD_NO joyNum, Stage& stage);

	// �f�X�g���N�^
	~Car(void);

	//������
	void Init(CAR_TYPE carType, VECTOR pos);

	//�Ԃ̏����l�w��
	virtual void CarInitParam(VECTOR pos) = 0;

	//�X�V
	void Update(void);

	//�`��
	void Draw(void);

	//�g�����X�t�H�[���擾
	const Transform& GetTransform(void) const;

	//�ʒu�擾
	VECTOR GetPos(void) const;
	
	//�����蔻�莞�ɕK�v�ȃJ�v�Z���̏�Ɖ�
	VECTOR GetPosTop(void) const;
	VECTOR GetPosBottom(void) const;

	//�M�A�擾
	int GetGearNum(void) const;

	//�X�s�[�h�擾
	float GetSpeed(void) const;

	//�����蔻��ɗp����R���C�_�[�擾�֐�
	void AddCol(int collider_);

	//�S�[���̓����蔻��
	bool IsHitGoal(void);

	//�����蔻�莞�̉�����
	void SetCollisionPow(VECTOR pow);

	//�p�b�h�ԍ��擾
	InputManager::JOYPAD_NO GetPadNo(void) const;

	virtual float GetMaxSpeedACC(int gear) = 0;

protected:

	//�ς��ǂ̊��蓖�ėp
	InputManager::JOYPAD_NO joypadNum_;

	//�X�e�[�W�Q��
	Stage& stage_;

	//��
	Transform transformCar_;

	//�^�C���^�C�v
	enum class WHEEL_TYPE
	{
		FR,
		FL,
		BR,
		BL
	};

	//�^�C�����
	struct WHEEL_INFO
	{
		Transform transform;
		VECTOR localPos = AsoUtility::VECTOR_ZERO;
		VECTOR rotatePos = AsoUtility::VECTOR_ZERO;
	};

	//�^�C���̃^�C�v�ʂ̏��
	std::map<WHEEL_TYPE, WHEEL_INFO> wheelInfos_;

	//�M�A�ύX
	virtual void ChangeGear(void);

	//�X�s�[�h����
	void Deceleration(float deceleration);

	//�M�A����
	virtual void SyncGearSpeed(void) = 0;

	//�X�s�[�h�֘A�ϐ�
	float speed_;

	//�M�A�ϐ�
	int gearNum_;

	//�M�A���Ƃ̃}�b�N�X�X�s�[�h
	float maxSpeed_[GIAR_MAX_NUM];

	//�����x
	float accel_;

private:

	//�ړ�
	void ProcessMove(void);

	//��]
	void ProcessTurn(void);

	//��ȉ�]����
	void ProcessTurnDirection(int stickValue,int deadzone,float maxRot, int direction);

	//�Ȃ���
	void Turn(double deg, VECTOR axis);

	//�^�C���̓���
	void SyncWheel(void);

	//�X�s�[�h����
	void Accelerate(float acceleration);

	//�����蔻��
	void Collision(void);

	//�d�͓����蔻��
	void CollisionGrvity(void);

	//�J�v�Z���ł̓����蔻��
	void CollisionCapsule(void);

	//�����Փ˕�
	std::vector<int> colliders_;

	//�ړ�����W
	VECTOR movedPos_;

	//�Փ˔��莞�̉����Ԃ���
	VECTOR collisionPow_;

	//�ڒn�Փ˂Ŏg�p����
	VECTOR gravityHitDown_;
	VECTOR gravityHitUp_;

	//�����蔻��i�c�_�j
	VECTOR dirHitFront_;
	VECTOR dirHitBack_;

	//�����蔻��ɕK�v�ȃJ�v�Z���̏�Ɖ�
	VECTOR cupsuleTopRot_;
	VECTOR cupsuleBotRot_;

	//���A�v���C���[���ڒn���Ă���|���S���̖@��
	VECTOR hitNormal_;

	//�ڒn�_
	VECTOR hitPos_;

	//�ړ�����
	VECTOR moveDiff_;

};