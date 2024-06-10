#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// <summary>
/// モデル制御の基本情報
/// 大きさ：VECTOR基準
/// 回転　：Quaternion基準
/// 位置　：VECTOR基準
/// </summary>

class Transform
{
public:

	// コンストラクタ
	Transform(void);
	Transform(int model);
	
	// デストラクタ
	~Transform(void);
	/// <summary>
	/// モデル制御の基本情報更新
	/// </summary>
	/// <param name=""></param>
	void Update(void);

	void SetModel(int model);

	// 前方方向を取得
	VECTOR GetForward(void) const;

	// 後方方向を取得
	VECTOR GetBack(void) const;

	// 右方向を取得
	VECTOR GetRight(void) const;

	// 左方向を取得
	VECTOR GetLeft(void) const;

	// 上方向を取得
	VECTOR GetUp(void) const;

	// 下方向を取得
	VECTOR GetDown(void) const;

	// 対象方向を取得
	VECTOR GetDir(VECTOR vec)const;

	// モデルのハンドルID
	int modelId_;

	// 大きさ
	VECTOR scl_;
	// 回転
	VECTOR rot_;
	// 位置
	VECTOR pos_;

	//行列
	MATRIX matScl_;
	MATRIX matRot_;
	MATRIX matPos_;

	// 回転
	Quaternion quaRot_;

	// ローカル回転
	Quaternion quaRotLocal_;
};

