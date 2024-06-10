#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Common/Vector2.h"
class AsoUtility
{

public:

	// ���W�A��(rad)�E�x(deg)�ϊ��p
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE = { 1.0f, 1.0f, 1.0f };

	// ��]��
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// ����
	static constexpr VECTOR DIR_F = { 0.0f, 0.0f, 1.0f };
	static constexpr VECTOR DIR_B = { 0.0f, 0.0f, -1.0f };
	static constexpr VECTOR DIR_R = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_L = { -1.0f, 0.0f, 0.0f };
	static constexpr VECTOR DIR_U = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR DIR_D = { 0.0f, -1.0f, 0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;

	// �l�̌ܓ�
	static int Round(float v);

	// ������̕���
	static std::vector <std::string> Split(std::string& line, char delimiter);

	// ���W�A��(rad)����x(deg)
	static double Rad2DegD(double rad);
	static float Rad2DegF(float rad);
	static int Rad2DegI(int rad);

	// �x(deg)���烉�W�A��(rad)
	static double Deg2RadD(double deg);
	static float Deg2RadF(float deg);
	static int Deg2RadI(int deg);

	// 0�`360�x�͈̔͂Ɏ��߂�
	static double DegIn360(double deg);

	// 0(0)�`2��(360�x)�͈̔͂Ɏ��߂�
	static double RadIn2PI(double rad);

	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundRad(float from, float to);
	
	// ��]�����Ȃ����̉�]�������擾����(���v���:1�A�����v���:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// ���`���
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// �p�x�̐��`���
	static double LerpDeg(double start, double end, double t);

	// �F�̐��`���
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// �x�W�F�Ȑ�
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	

	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// �x�N�g���̒���
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static double SqrMagnitude(const VECTOR& v);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);

	// ���̓��m�̏Փ˔���
	static bool IsHitSpheres(const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);
	static bool IsHitCapsuleToSphere(VECTOR CupsulePosBottom, VECTOR CupsulePosTop, float CupsuleRadius, VECTOR ShperePos, float ShpereRadius);

	// ��r
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool EqualsVZero(const VECTOR& v1);

	// ���K��
	static VECTOR Normalize(const Vector2& v);
	static VECTOR VNormalize(const VECTOR& v);

	// 2�̃x�N�g���̊Ԃ̊p�x
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// �`��n
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);

	//static void Lookat(const VECTOR& target, const VECTOR& pos, const Quaternion rotation);

};

