#pragma once
#include <vector>
#include <unordered_map>
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Garage;
class Vector2;

class GameScene : public SceneBase
{
public:

	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;
	
private:

	//車の初期値
	void InitCarParam(void);

	//カメラの初期値
	void InitCameraParam(void);

	//画像ハンドル初期化
	void InitImageHandle(void);

	//ステージオブジェクト初期化
	void InitStageObjects(void);

	//車とカメラの初期化
	void InitPlayerAndCamera(int index);

	//当たり判定用モデルハンドルを追加
	void SetupCollision(void);

	//車同士の当たり判定
	void ProcessCarCollision(std::unique_ptr<Car>& car1P, std::unique_ptr<Car>& car2P);

	//ゴールとの当たり判定
	void ProcessGoalCollision(std::unique_ptr<Car>& car);

	//ゲーム内で使う描画
	void DrawGame(void);

	//Ui描画
	void DrawUi(float nowSpeed , int nowGear ,int nowCarNum);

	//ニードル描画(処理が多いので関数を別で作る)
	void DrawNeedle(float nowSpeed, int nowGear , int nowCarNum);

	//集中線描画
	void DrawLine(void);

	//コントローラーの振動
	void ControllerVibration(void);

	//１Pと２Pの衝突時処理
	VECTOR CalcCollisionCar(VECTOR pos1, VECTOR pos2);

	//画像タイプ
	enum class IMAGE_TYPE
	{
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		START_SIGN,
		LINE_1,
		LINE_2,
		LINE_3,
	};

	//インスタンス
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Garage> garage_;

	//画面分割をするので車とカメラを二つ用意
	std::vector<std::unique_ptr<Car>> cars_;
	std::vector<std::unique_ptr<Camera>> cameras_;
	
	//スクリーンも二つ用意
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;

	//タイプごとのハンドル情報格納変数
	std::unordered_map<IMAGE_TYPE, int> imageInfos_;

	std::vector<VECTOR> vecPos_;
	std::vector<CAR_TYPE> carTypes_;
	std::vector<InputManager::JOYPAD_NO> padNos_;

	//時間計測用変数
	float stepTime_;

	//スタート管理変数
	float stepStartTime_;

};
