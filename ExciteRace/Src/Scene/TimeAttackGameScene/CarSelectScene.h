#pragma once
#include<array>
#include "../../Object/Common/Transform.h"
#include "../../Common/CollectStructure.h"
#include "../SceneBase.h"

class Transform;
class Camera;
class Garage;
class Car;

class CarSelectScene : public SceneBase
{
public:

	static constexpr int MAX_INDEX = 3;
	static constexpr int MIN_INDEX = 0;

	// コンストラクタ
	CarSelectScene(void);

	// デストラクタ
	~CarSelectScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

private:

	struct CarInitParams {
		VECTOR carPos_;
		VECTOR carScale_;
		VECTOR carRot_;
		VECTOR wheelPos_;
		VECTOR wheelScale_;
	};

	void InitImage(void);

	void InitModel(void);

	void ProcessButtonPush(void);

	void LimitIndex(void);

	void SetParam(void);

	void SetParamCar(int idx, VECTOR pos,VECTOR scl, VECTOR rot);

	void SetParamWheel(int idx, VECTOR pos, VECTOR scl, VECTOR rot);

	CarInitParams carParams[MAX_INDEX];

	//少ししか使わないためCarクラスを呼び出さない
	Transform transformCar_;
	Transform transformWheel_;

	//インスタンス
	std::unique_ptr<Garage> garage_;
	std::unique_ptr<Camera> camera_;

	//モデルIDをはいれる管理
	int carModelIds_[static_cast<int>(CAR_TYPE::MAX)];
	int wheelModelIds_[static_cast<int>(CAR_TYPE::MAX)];

	//インデックス
	int index_;

	int decisionHandle_;
	int crossHandle_;
	std::vector<int> carLogoHandle_;

};

