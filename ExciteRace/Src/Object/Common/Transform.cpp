#include <DxLib.h>
#include "../../Utility/AsoUtility.h"
#include "Transform.h"

#pragma region 定数宣言

//前方向
constexpr VECTOR FORWARD_DIR = { 0.0f, 0.0f, 1.0f };

//後ろ方向
constexpr VECTOR BACK_DIR = { 0.0f,0.0f,-1.0f };

//右方向
constexpr VECTOR RIGHT_DIR = { 1.0f,0.0f,0.0f };

//左方向
constexpr VECTOR LEFT_DIR = { -1.0f,0.0f,0.0f };

//上方向
constexpr VECTOR UP_DIR = { 0.0f,1.0f,0.0f };

//下方向
constexpr VECTOR DOWN_DIR = { 0.0f,-1.0f,0.0f };

#pragma endregion


Transform::Transform(void)
{
	modelId_ = -1;

	scl_ = AsoUtility::VECTOR_ONE;
	rot_ = AsoUtility::VECTOR_ZERO;
	pos_ = AsoUtility::VECTOR_ONE;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotLocal_ = Quaternion();

}

Transform::Transform(int model)
{
	modelId_ = model;

	scl_ = AsoUtility::VECTOR_ONE;
	rot_ = AsoUtility::VECTOR_ZERO;
	pos_ = AsoUtility::VECTOR_ONE;

	matScl_ = MGetIdent();
	matRot_ = MGetIdent();
	matPos_ = MGetIdent();
	quaRot_ = Quaternion();
	quaRotLocal_ = Quaternion();
}

Transform::~Transform(void)
{
}

void Transform::Update(void)
{

	// 大きさ
	matScl_ = MGetScale(scl_);

	// 回転
	rot_ = quaRot_.ToEuler();
	matRot_ = quaRot_.ToMatrix();

	// 位置
	matPos_ = MGetTranslate(pos_);

	// 行列の合成
	MATRIX mat = MGetIdent();
	mat = MMult(mat, matScl_);
	Quaternion q = quaRot_.Mult(quaRotLocal_);
	mat = MMult(mat, q.ToMatrix());
	mat = MMult(mat, matPos_);

	// 行列をモデルに判定
	if (modelId_ != -1)
	{
		MV1SetMatrix(modelId_, mat);
	}

}

void Transform::SetModel(int model)
{
	modelId_ = model;
}

VECTOR Transform::GetForward(void) const
{
	return GetDir(FORWARD_DIR);
}

VECTOR Transform::GetBack(void) const
{
	return GetDir(BACK_DIR);
}

VECTOR Transform::GetRight(void) const
{
	return GetDir(RIGHT_DIR);
}

VECTOR Transform::GetLeft(void) const
{
	return GetDir(LEFT_DIR);
}

VECTOR Transform::GetUp(void) const
{
	return GetDir(UP_DIR);
}

VECTOR Transform::GetDown(void) const
{
	return GetDir(DOWN_DIR);
}

VECTOR Transform::GetDir(VECTOR vec) const
{
	return VNorm(VTransform(vec, quaRot_.ToMatrix()));
}
