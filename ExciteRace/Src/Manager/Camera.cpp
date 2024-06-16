#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include "../Application.h"
#include "../Utility/AsoUtility.h"
#include "../Object/Common/Transform.h"
#include "SceneManager.h"
#include "Camera.h"

#pragma region 定数宣言

//カメラの上方向
static constexpr VECTOR CAMERA_UP = { 0.0f,1.0f,0.0f };

// カメラクリップ：NEAR
static constexpr float CAMERA_NEAR = 40.0f;

// カメラクリップ：NEAR
static constexpr float CAMERA_FAR = 30000.0f;

// カメラの初期座標
static constexpr VECTOR DEFAULT_CAMERA_POS = { 0.0f, 100.0f, -500.0f };

// カメラ位置と注視点との相対座標
static constexpr VECTOR RELATIVE_TARGET_POS = { 0.0f, -200.0f, 500.0f };

//自分が作ったオリジナル相対座標
static constexpr VECTOR ORIGINAL_RELATIVE_CAMERA_POS = { 0.0f,300.0f,-600.0f };
static constexpr VECTOR ORIGINAL_RELATIVE_TARGET_POS = { 0.0f,-300.0f,600.0f };

//球面線形補間用相対座標
constexpr VECTOR SLERP_RELATIVE_CAMERA_POS = { 0.0f,250.0f,-700.0f };
constexpr VECTOR SLERP_RELATIVE_TARGET_POS = { 0.0f,-250.0f,700.0f };

//球面線形補間
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

	// カメラの初期設定
	pos_ = DEFAULT_CAMERA_POS;
	targetPos_ = VAdd(pos_, RELATIVE_TARGET_POS);
	cameraUp_ = CAMERA_UP;

	// カメラはX軸に傾いているが、この傾いた状態を傾き無しとする
	// quaRot_は回転計算用で、
	// あくまで軸となるのは、カメラ座標と注視点とする
	quaRot_ = Quaternion();

}

void Camera::Update()
{
}

void Camera::SetBeforeDraw(void)
{

	// クリップ距離を設定する(SetDrawScreenでリセットされる)
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

	// カメラの設定
	SetCameraPositionAndTargetAndUpVec(
		pos_,
		targetPos_,
		cameraUp_
	);

	angleY_ = GetCameraAngleHRotate(); // Y

	// DXライブラリのカメラとEffekseerのカメラを同期する。
	Effekseer_Sync3DSetting();
	
}

void Camera::SetBeforeDrawFree(void)
{
	//回転処理
	VECTOR axis = AsoUtility::VECTOR_ZERO;
	float reverce = 1.0f;
	if (CheckHitKey(KEY_INPUT_UP)) { axis = AsoUtility::AXIS_X; reverce = -1.0f; }
	if (CheckHitKey(KEY_INPUT_DOWN)) { axis = AsoUtility::AXIS_X; }
	if (CheckHitKey(KEY_INPUT_LEFT)) { axis = AsoUtility::AXIS_Y; reverce = -1.0f; }
	if (CheckHitKey(KEY_INPUT_RIGHT)) { axis = AsoUtility::AXIS_Y; }
	//カメラ座標を中心として、注視点をかいてんさせる
	if (!AsoUtility::EqualsVZero(axis))
	{
		Quaternion axisQ = Quaternion::AngleAxis(AsoUtility::Deg2RadF(1.0f * reverce), axis);
		quaRot_ = quaRot_.Mult(axisQ);

		//位置情報に回転情報を反映
		VECTOR localPos = quaRot_.PosAxis(RELATIVE_TARGET_POS);

		//注視点更新
		targetPos_ = VAdd(pos_, localPos);

		//カメラの上方向更新
		cameraUp_ = quaRot_.GetUp();//普通のゲームならY軸で固定してもいいけど、３Dシューティングはこれじゃなきゃだめ
	}


	//カメラ操作ができるように
	//移動処理
	VECTOR moveDir = AsoUtility::VECTOR_ZERO;
	if (CheckHitKey(KEY_INPUT_W)) { moveDir = AsoUtility::DIR_F; }
	if (CheckHitKey(KEY_INPUT_S)) { moveDir = AsoUtility::DIR_B; }
	if (CheckHitKey(KEY_INPUT_A)) { moveDir = AsoUtility::DIR_L; }
	if (CheckHitKey(KEY_INPUT_D)) { moveDir = AsoUtility::DIR_R; }


	if (!AsoUtility::EqualsVZero(moveDir))
	{
		float speed = 10.0f;

		//移動方向をカメラ向きに合わせて回転
		VECTOR rotDir = quaRot_.PosAxis(moveDir);

		//移動させたい方向に移動量を加える
		VECTOR movePow = VScale(rotDir, speed);

		//カメラ位置と注視点を移動する
		pos_ = VAdd(pos_, movePow);
		targetPos_ = VAdd(targetPos_, movePow);

	}
}

void Camera::SetBeforeDrawFixedPoint(void)
{
	//定点カメラなので何もしない
}

void Camera::SetBeforeDrawFollow(void)
{

	//指定したオブジェクトのポジション
	VECTOR targetPos = target_->pos_;

	//指定したオブジェクトの回転
	Quaternion targetRot = target_->quaRot_;

	//今のカメラの状態
	Quaternion nowCamera = Quaternion::Euler(0.0f, angleY_, 0.0f);

	//今の状態のカメラからプレイヤーの回転まで球面補間
	slerpRot_ = Quaternion::Slerp(nowCamera, targetRot, SLERP_TIME);

	//カメラ位置と注視点を移動
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
