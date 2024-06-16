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

	//継承先やたクラスで使う定数宣言
	//衝突関連
	static constexpr float COLLISION_RADIUS = 120.0f;

	//タイヤの初期の向き
	static constexpr VECTOR WHEEL_INIT_ROT = { 0.0f, 180.0f * DX_PI_F / 180, 0.0f };

	//ギアの数
	static constexpr int GIAR_MAX_NUM = 5;

	//ギアごとの移動スピード
	static constexpr float SPEED_GEAR[] = { 0.26f ,0.22f,0.17f,0.13f,0.1f };

	// コンストラクタ
	Car( InputManager::JOYPAD_NO joyNum, Stage& stage);

	// デストラクタ
	~Car(void);

	//初期化
	void Init(CAR_TYPE carType, VECTOR pos);

	//車の初期値指定
	virtual void CarInitParam(VECTOR pos) = 0;

	//更新
	void Update(void);

	//描画
	void Draw(void);

	//トランスフォーム取得
	const Transform& GetTransform(void) const;

	//位置取得
	VECTOR GetPos(void) const;
	
	//当たり判定時に必要なカプセルの上と下
	VECTOR GetPosTop(void) const;
	VECTOR GetPosBottom(void) const;

	//ギア取得
	int GetGearNum(void) const;

	//スピード取得
	float GetSpeed(void) const;

	//当たり判定に用いるコライダー取得関数
	void AddCol(int collider_);

	//ゴールの当たり判定
	bool IsHitGoal(void);

	//当たり判定時の押す力
	void SetCollisionPow(VECTOR pow);

	//パッド番号取得
	InputManager::JOYPAD_NO GetPadNo(void) const;

	virtual float GetMaxSpeedACC(int gear) = 0;

protected:

	//ぱっどの割り当て用
	InputManager::JOYPAD_NO joypadNum_;

	//ステージ参照
	Stage& stage_;

	//車
	Transform transformCar_;

	//タイヤタイプ
	enum class WHEEL_TYPE
	{
		FR,
		FL,
		BR,
		BL
	};

	//タイヤ情報
	struct WHEEL_INFO
	{
		Transform transform;
		VECTOR localPos = AsoUtility::VECTOR_ZERO;
		VECTOR rotatePos = AsoUtility::VECTOR_ZERO;
	};

	//タイヤのタイプ別の情報
	std::map<WHEEL_TYPE, WHEEL_INFO> wheelInfos_;

	//ギア変更
	virtual void ChangeGear(void);

	//スピード減速
	void Deceleration(float deceleration);

	//ギア同期
	virtual void SyncGearSpeed(void) = 0;

	//スピード関連変数
	float speed_;

	//ギア変数
	int gearNum_;

	//ギアごとのマックススピード
	float maxSpeed_[GIAR_MAX_NUM];

	//加速度
	float accel_;

private:

	//移動
	void ProcessMove(void);

	//回転
	void ProcessTurn(void);

	//主な回転処理
	void ProcessTurnDirection(int stickValue,int deadzone,float maxRot, int direction);

	//曲がる
	void Turn(double deg, VECTOR axis);

	//タイヤの同期
	void SyncWheel(void);

	//スピード加速
	void Accelerate(float acceleration);

	//当たり判定
	void Collision(void);

	//重力当たり判定
	void CollisionGrvity(void);

	//カプセルでの当たり判定
	void CollisionCapsule(void);

	//複数衝突物
	std::vector<int> colliders_;

	//移動後座標
	VECTOR movedPos_;

	//衝突判定時の押し返す力
	VECTOR collisionPow_;

	//接地衝突で使用する
	VECTOR gravityHitDown_;
	VECTOR gravityHitUp_;

	//当たり判定（縦棒）
	VECTOR dirHitFront_;
	VECTOR dirHitBack_;

	//当たり判定に必要なカプセルの上と下
	VECTOR cupsuleTopRot_;
	VECTOR cupsuleBotRot_;

	//今、プレイヤーが接地しているポリゴンの法線
	VECTOR hitNormal_;

	//接地点
	VECTOR hitPos_;

	//移動差分
	VECTOR moveDiff_;

};