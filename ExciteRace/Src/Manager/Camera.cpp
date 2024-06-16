#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "SceneManager.h"
#include "Camera.h"

#pragma region �萔�錾

//�J�����̏����
static constexpr VECTOR CAMERA_UP = { 0.0f,1.0f,0.0f };

// �J�����N���b�v�FNEAR
static constexpr float CAMERA_NEAR = 40.0f;

// �J�����N���b�v�FNEAR
static constexpr float CAMERA_FAR = 30000.0f;

// �J�����̏������W
static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

// �J�����ʒu�ƒ����_�Ƃ̑��΍��W
static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -200.0f, 500.0f };

//������������I���W�i�����΍��W
static constexpr VECTOR ORIGINAL_RELATIVE_CAMERA_POS = { 0.0f,300.0f,-600.0f };
static constexpr VECTOR ORIGINAL_RELATIVE_TARGET_POS = { 0.0f,-300.0f,600.0f };

//���ʐ��`��ԗp���΍��W
constexpr VECTOR SLERP_RELATIVE_CAMERA_POS = { 0.0f,250.0f,-700.0f };
constexpr VECTOR SLERP_RELATIVE_TARGET_POS = { 0.0f,-250.0f,700.0f };

//���ʐ��`���
constexpr float SLERP_TIME = 0.1f;

#pragma endregion

Camera::Camera(void)
{
	pos_ = VECTOR();
	quaRot_ = Quaternion();
	targetPos_ = VECTOR();
	cameraUp_ = VECTOR();
	angleY_ = 0.0f;
}

Camera::~Camera(void)
{
}

void Camera::Init(void)
{

	auto id = SceneManager::GetInstance().GetmSceneID();

	switch (id)
	{
	case SceneManager::SCENE_ID::TA_CARSELECT:
		ChangeMode(MODE::ROTATE_FOLLOW);
		break;
	case SceneManager::SCENE_ID::VS_GAMESTART:
		ChangeMode(MODE::GAME_START_LERP);
		break;
	case SceneManager::SCENE_ID::GAME:
	case SceneManager::SCENE_ID::TIMEATTACKGAME:
		ChangeMode(MODE::FOLLOW);
		break;
	case SceneManager::SCENE_ID::TIMEATTACKGAMESTART:
		ChangeMode(MODE::TIMEATTACK_GAME_START_LERP);
		break;
	case SceneManager::SCENE_ID::SCORE:
	case SceneManager::SCENE_ID::NONE:
	case SceneManager::SCENE_ID::TITLE:
	case SceneManager::SCENE_ID::SELECT:
	case SceneManager::SCENE_ID::RESULT:
		break;
	default:
		break;
	}

}

void Camera::SetDefault(void)
{

	// �J�����̏����ݒ�
	pos_ = DEFAULT_CAMERA_POS;
	targetPos_ = VAdd(pos_, RELATIVE_TARGET_POS);
	cameraUp_ = CAMERA_UP;

	// �J������X���ɌX���Ă��邪�A���̌X������Ԃ��X�������Ƃ���
	// quaRot_�͉�]�v�Z�p�ŁA
	// �����܂Ŏ��ƂȂ�̂́A�J�������W�ƒ����_�Ƃ���
	quaRot_ = Quaternion();

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// �N���b�v������ݒ肷��(SetDrawScreen�Ń��Z�b�g�����)
	SetCameraNearFar(CAMERA_NEAR, CAMERA_FAR);

	switch (mode_)
	{
	case Camera::MODE::FREE:
		SetBeforeDrawFree();
		break;
	case Camera::MODE::FIXED_POINT:
		SetBeforeDrawFixedPoint();
		break;
	case Camera::MODE::FOLLOW:
		SetBeforeDrawFollow();
		break;	
	case Camera::MODE::ROTATE_FOLLOW:
		SetBeforeDrawOriginalFollow();
		break;	
	case Camera::MODE::GAME_START_LERP:
		SetBeforeDrawOriginalFollowStart();
		break;
	case Camera::MODE::TIMEATTACK_GAME_START_LERP:
		SetBeforeDrawOriginalFollowTimeAttackStart();
		break;
	}

	// �J�����̐ݒ�
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	angleY_ = GetCameraAngleHRotate(); // Y

	// DX���C�u�����̃J������Effekseer�̃J�����𓯊�����B
	Effekseer_Sync3DSetting();
	
}

void Camera::SetBeforeDrawFree(void)
{
	//��]����
	VECTOR axis = AsoUtility::VECTOR_ZERO;
	float reverce = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP)) { axis = AsoUtility::AXIS_X; reverce = -1.0f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { axis = AsoUtility::AXIS_X; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { axis = AsoUtility::AXIS_Y; reverce = -1.0f; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { axis = AsoUtility::AXIS_Y; }
	//�J�������W�𒆐S�Ƃ��āA�����_�������Ă񂳂���
	if (!AsoUtility::EqualsVZero(axis))
	{
		Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f * reverce), axis);
		quaRot_ = quaRot_.Mult(axisQ);

		//�ʒu���ɉ�]���𔽉f
		VECTOR localPos = quaRot_.PosAxis(RELATIVE_TARGET_POS);

		//�����_�X�V
		targetPos_ = VAdd(pos_, localPos);

		//�J�����̏�����X�V
		cameraUp_ = quaRot_.GetUp();//���ʂ̃Q�[���Ȃ�Y���ŌŒ肵�Ă��������ǁA�RD�V���[�e�B���O�͂��ꂶ��Ȃ��Ⴞ��
	}


	//�J�������삪�ł���悤��
	//�ړ�����
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (CheckHitKey(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }


	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 10.0f;

		//�ړ��������J���������ɍ��킹�ĉ�]
		VECTOR rotDir = quaRot_.PosAxis(moveDir);

		//�ړ��������������Ɉړ��ʂ�������
		VECTOR movePow = VScale(rotDir, speed);

		//�J�����ʒu�ƒ����_���ړ�����
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//��_�J�����Ȃ̂ŉ������Ȃ�
}

void Camera::SetBeforeDrawFollow(void)
{

	//�w�肵���I�u�W�F�N�g�̃|�W�V����
	VECTOR targetPos = target_->pos_;

	//�w�肵���I�u�W�F�N�g�̉�]
	Quaternion targetRot = target_->quaRot_;

	//���̃J�����̏��
	Quaternion nowCamera = Quaternion::Euler(0.0f, angleY_, 0.0f);

	//���̏�Ԃ̃J��������v���C���[�̉�]�܂ŋ��ʕ��
	slerpRot_ = Quaternion::Slerp(nowCamera, targetRot, SLERP_TIME);

	//�J�����ʒu�ƒ����_���ړ�
	VECTOR relativeCameraPos = slerpRot_.PosAxis(SLERP_RELATIVE_CAMERA_POS);
	pos_ = VAdd(targetPos, relativeCameraPos);
	VECTOR relativeTargetPos = slerpRot_.PosAxis(SLERP_RELATIVE_TARGET_POS);
	targetPos_ = VAdd(pos_, relativeTargetPos);

}

void Camera::SetBeforeDrawOriginalFollow(void)
{
	VECTOR targetPos = selectTarget_.pos_;

	Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(0.4f), AsoUtility::AXIS_Y);
	quaRot_ = quaRot_.Mult(axisQ);


	VECTOR relativeCPos = quaRot_.PosAxis(ORIGINAL_RELATIVE_CAMERA_POS);
	pos_ = VAdd(targetPos, relativeCPos);


	VECTOR relativeTPos = quaRot_.PosAxis(ORIGINAL_RELATIVE_TARGET_POS);
	targetPos_ = VAdd(pos_, relativeTPos);
}

void Camera::SetBeforeDrawOriginalFollowStart(void)
{
	stepTime_+= SceneManager::GetInstance().GetDeltaTime();

	if (stepTime_ >= 0.0f && stepTime_ <= 4.9f)
	{
		VECTOR targetPos = target_->pos_;
		pos_ = { targetPos.x+1000 ,targetPos.y + 1000,targetPos.z - 3000 };
		VECTOR movePos = { targetPos.x+1000,targetPos.y + 1000,targetPos.z + 3000 };
		lerpUseDelta1_ += SceneManager::GetInstance().GetDeltaTime() / 5.0f;
		pos_ = AsoUtility::Lerp(pos_, movePos, lerpUseDelta1_);
	}
	if (stepTime_ >= 5.0f && stepTime_ <= 9.0f)
	{
		VECTOR targetPos = target_->pos_;
		pos_ = { targetPos.x+300,targetPos.y + 300,targetPos.z - 500 };
		VECTOR movePos = { targetPos.x+300,targetPos.y + 100,targetPos.z + 300 };
		lerpUseDelta2_ += SceneManager::GetInstance().GetDeltaTime() / 1.5f;
		pos_ = AsoUtility::Lerp(pos_, movePos, lerpUseDelta2_);
	}
}

void Camera::SetBeforeDrawOriginalFollowTimeAttackStart(void)
{
	stepTime_ += SceneManager::GetInstance().GetDeltaTime();

	if (stepTime_ >= 0.0f && stepTime_ <= 4.9f)
	{
		VECTOR targetPos = target_->pos_;
		pos_ = { targetPos.x + 800 ,targetPos.y + 800,targetPos.z - 3000 };
		VECTOR movePos = { targetPos.x + 800,targetPos.y + 800,targetPos.z + 3000 };
		lerpUseDelta1_ += SceneManager::GetInstance().GetDeltaTime() / 5.0f;
		pos_ = AsoUtility::Lerp(pos_, movePos, lerpUseDelta1_);
	}
	if (stepTime_ >= 5.0f && stepTime_ <= 9.0f)
	{
		VECTOR targetPos = target_->pos_;
		pos_ = { targetPos.x + 300,targetPos.y + 300,targetPos.z - 500 };
		VECTOR movePos = { targetPos.x + 300,targetPos.y + 100,targetPos.z + 300 };
		lerpUseDelta2_ += SceneManager::GetInstance().GetDeltaTime() / 1.5f;
		pos_ = AsoUtility::Lerp(pos_, movePos, lerpUseDelta2_);
	}
}

VECTOR Camera::GetPos(void) const
{
	return pos_;
}

Camera::MODE Camera::GetMode(void) const
{
	return mode_;
}

VECTOR Camera::GetTargetPos(void) const
{
	return targetPos_;
}

VECTOR Camera::GetDir(void) const
{
	return VNorm(VSub(targetPos_, pos_));
}

float Camera::GetTime(void) const
{
	return stepTime_;
}

void Camera::ChangeMode(MODE mode)
{
	SetDefault();

	mode_ = mode;
	
	switch (mode_)
	{
	case Camera::MODE::FREE:
		break;
	case Camera::MODE::FIXED_POINT:
		break;
	case Camera::MODE::FOLLOW:
		break;
	case Camera::MODE::ROTATE_FOLLOW:
		break;	
	case Camera::MODE::GAME_START_LERP:
		break;
	case Camera::MODE::TIMEATTACK_GAME_START_LERP:
		break;
	}
}

void Camera::SetFollowTarget(const Transform* target)
{
	target_ = target;
}

void Camera::SetFollowSelectTarget(Transform target)
{
	selectTarget_ = target;
}
