#pragma once
#include <vector>
#include <DxLib.h>
#include "../Common/Quaternion.h"
#include "../Object/Common/Transform.h"

class Transform;

class Camera
{
public:



	enum class MODE
	{
		NONE,
		FREE,					//�t���[
		FIXED_POINT,			//��_
		FOLLOW,					//�Ǐ]�J����
		ROTATE_FOLLOW,		//
		GAME_START_LERP,
		TIMEATTACK_GAME_START_LERP,
	};

	// �R���X�g���N�^
	Camera(void);

	// �f�X�g���N�^
	~Camera(void);

	// ������
	void Init(void);

	// �J�����������ʒu�ɖ߂�
	void SetDefault(void);

	// �X�V
	void Update(void);

	// �`��O�̃J�����ݒ�
	void SetBeforeDraw(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawOriginalFollow(void);
	void SetBeforeDrawOriginalFollowStart(void);
	void SetBeforeDrawOriginalFollowTimeAttackStart(void);

	// �J�������W
	VECTOR GetPos(void) const;

	//�J�����̃��[�h�擾
	MODE GetMode(void) const;

	// �����_���W
	VECTOR GetTargetPos(void) const;

	// �J��������
	VECTOR GetDir(void) const;

	float GetTime(void) const;

	//�J�������[�h�̕ύX
	void ChangeMode(MODE mode);

	//�Ǐ]�Ώۂ̐ݒ�
	void SetFollowTarget(const Transform* target);

	void SetFollowSelectTarget(Transform target);

private:

	//�Ǐ]�Ώ�
	const Transform* target_;

	Transform selectTarget_;

	//�J�������[�h
	MODE mode_;

	// �J�����̈ʒu
	VECTOR pos_;

	// �J�����p�x
	Quaternion quaRot_;

	// �����_
	VECTOR targetPos_;

	// �J�����̏����
	VECTOR cameraUp_;

	//���Ԍv���p
	float stepTime_;
	float lerpUseDelta1_;
	float lerpUseDelta2_;

	//���ʕ�Ԃ�p������]�p
	Quaternion slerpRot_;

	float angleY_;

};

