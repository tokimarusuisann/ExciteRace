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
		FREE,					//フリー
		FIXED_POINT,			//定点
		FOLLOW,					//追従カメラ
		ROTATE_FOLLOW,		//
		GAME_START_LERP,
		TIMEATTACK_GAME_START_LERP,
	};

	// コンストラクタ
	Camera(void);

	// デストラクタ
	~Camera(void);

	// 初期化
	void Init(void);

	// カメラを初期位置に戻す
	void SetDefault(void);

	// 更新
	void Update(void);

	// 描画前のカメラ設定
	void SetBeforeDraw(void);
	void SetBeforeDrawFree(void);
	void SetBeforeDrawFixedPoint(void);
	void SetBeforeDrawFollow(void);
	void SetBeforeDrawOriginalFollow(void);
	void SetBeforeDrawOriginalFollowStart(void);
	void SetBeforeDrawOriginalFollowTimeAttackStart(void);

	// カメラ座標
	VECTOR GetPos(void) const;

	//カメラのモード取得
	MODE GetMode(void) const;

	// 注視点座標
	VECTOR GetTargetPos(void) const;

	// カメラ方向
	VECTOR GetDir(void) const;

	float GetTime(void) const;

	//カメラモードの変更
	void ChangeMode(MODE mode);

	//追従対象の設定
	void SetFollowTarget(const Transform* target);

	void SetFollowSelectTarget(Transform target);

private:

	//追従対象
	const Transform* target_;

	Transform selectTarget_;

	//カメラモード
	MODE mode_;

	// カメラの位置
	VECTOR pos_;

	// カメラ角度
	Quaternion quaRot_;

	// 注視点
	VECTOR targetPos_;

	// カメラの上方向
	VECTOR cameraUp_;

	//時間計測用
	float stepTime_;
	float lerpUseDelta1_;
	float lerpUseDelta2_;

	//球面補間を用いた回転用
	Quaternion slerpRot_;

	float angleY_;

};

