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
	
	//画像ハンドル初期化
	void InitImageHandle(void);

	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;
	
	//ゲーム内で使う描画
	void DrawGame(void);

private:

	//コントローラーの振動
	void ControllerVibration(void);

	//１Pと２Pの衝突時処理
	VECTOR Collision1PTo2P(VECTOR pos1, VECTOR pos2);

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

	//画像ハンドル格納変数
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

	//タイプごとのハンドル情報格納変数
	std::map<IMAGE_TYPE, int> imageInfos_;

	//時間計測用変数
	float delta_;

	//スタート管理変数
	float stepStartTime_;

};
