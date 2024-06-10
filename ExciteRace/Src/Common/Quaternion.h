#pragma once
#include <DxLib.h>
#include <iostream>
#include <algorithm>
class Quaternion
{
public:
	
	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	double w;
	double x;
	double y;
	double z;

	Quaternion(void);
	Quaternion(VECTOR rad);
	Quaternion(double w, double x, double y, double z);

	~Quaternion(void);

	// �I�C���[�p����N�H�[�^�j�I���֕ϊ�
	static Quaternion Euler(VECTOR rad);
	static Quaternion Euler(double radX, double radY, double radZ);

	// �N�H�[�^�j�I���̍���
	static Quaternion Mult(const Quaternion& q1, const Quaternion& q2);
	Quaternion Mult(const Quaternion& q) const;

	// �w�莲���w��p���A��]������
	static Quaternion AngleAxis(double rad, VECTOR axis);

	// ���W����]������
	static VECTOR PosAxis(const Quaternion& q, VECTOR pos);
	VECTOR PosAxis(VECTOR pos) const;

	// �N�H�[�^�j�I������I�C���[�p�֕ϊ�
	static VECTOR ToEuler(const Quaternion& q);
	VECTOR ToEuler(void) const;

	// �N�H�[�^�j�I������s��֕ϊ�
	static MATRIX ToMatrix(const Quaternion& q);
	MATRIX ToMatrix(void) const;

	// �x�N�g������N�H�[�^�j�I���ɕϊ�
	static Quaternion LookRotation(VECTOR dir);
	static Quaternion LookRotation(VECTOR dir, VECTOR up);

	// �s�񂩂�N�H�[�^�j�I���ɕϊ�
	static Quaternion GetRotation(MATRIX mat);

	// ��{�x�N�g�����擾
	VECTOR GetForward(void) const;
	VECTOR GetBack(void) const;
	VECTOR GetRight(void) const;
	VECTOR GetLeft(void) const;
	VECTOR GetUp(void) const;
	VECTOR GetDown(void) const;

	// ����
	static double Dot(const Quaternion& q1, const Quaternion& q2);
	double Dot(const Quaternion& b) const;

	// ���K��
	static Quaternion Normalize(const Quaternion& q);
	Quaternion Normalized(void) const;
	void Normalize(void);

	// �t�N�H�[�^�j�I��
	Quaternion Inverse(void) const;

	// ���ʕ��
	static Quaternion Slerp(Quaternion from, Quaternion to, double t);

	// �Q�̃x�N�g���Ԃ̉�]�ʂ��擾����
	static Quaternion FromToRotation(VECTOR fromDir, VECTOR toDir);
	static Quaternion RotateTowards(const Quaternion& from, const Quaternion& to, float maxDegreesDelta);
	static double Angle(const Quaternion& q1, const Quaternion& q2);
	static Quaternion SlerpUnclamped(Quaternion a, Quaternion b, float t);
	static Quaternion Identity(void);

	double Length(void) const;
	double LengthSquared(void) const;
	VECTOR xyz(void) const;

	// �Ώە����̉�]
	void ToAngleAxis(float* angle, VECTOR* axis);

private:

	// ��{�x�N�g�����擾
	VECTOR GetDir(VECTOR dir) const;

	Quaternion operator*(float& rhs);
	const Quaternion operator*(const float& rhs);
	Quaternion operator+(Quaternion& rhs);
	const Quaternion operator+(const Quaternion& rhs);
	
};
