#pragma once
#include <vector>
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

	//画像ハンドル初期化
	void InitImageHandle(void);

	//ゲーム内で使う描画
	void DrawGame(void);

	//Ui描画
	void DrawUi(float nowSpeed , int nowGear ,int nowCarNum);

	//ニードル描画（処理が多いので関数を別で作る）
	void DrawNeedle(float nowSpeed, int nowGear , int nowCarNum);

	//集中線描画
	void DrawLine(void);

	//コントローラーの振動
	void ControllerVibration(void);

	//１Pと２Pの衝突時処理
	VECTOR Collision1PTo2P(VECTOR pos1, VECTOR pos2);

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
	std::map<IMAGE_TYPE, int> imageInfos_;

	//時間計測用変数
	float stepTime_;

	//スタート管理変数
	float stepStartTime_;

};
