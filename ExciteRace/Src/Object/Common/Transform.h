#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// ���f������̊�{���
/// �傫���FVECTOR�
/// ��]�@�FQuaternion�
/// �ʒu�@�FVECTOR�
/// </summary>

class Transform
{
public:

	// �R���X�g���N�^
	Transform(void);
	Transform(int model);
	
	// �f�X�g���N�^
	~Transform(void);
	/// <summary>
	/// ���f������̊�{���X�V
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	void SetModel(int model);

	// �O���������擾
	VECTOR GetForward(void) const;

	// ����������擾
	VECTOR GetBack(void) const;

	// �E�������擾
	VECTOR GetRight(void) const;

	// ���������擾
	VECTOR GetLeft(void) const;

	// ��������擾
	VECTOR GetUp(void) const;

	// ���������擾
	VECTOR GetDown(void) const;

	// �Ώە������擾
	VECTOR GetDir(VECTOR vec)const;

	// ���f���̃n���h��ID
	int modelId_;

	// �傫��
	VECTOR scl_;
	// ��]
	VECTOR rot_;
	// �ʒu
	VECTOR pos_;

	//�s��
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// ��]
	Quaternion quaRot_;

	// ���[�J����]
	Quaternion quaRotLocal_;
};

