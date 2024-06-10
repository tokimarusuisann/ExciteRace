#pragma once
#include <vector>
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Vector2;

class GameStartScene : public SceneBase
{
public:

	// コンストラクタ
	GameStartScene(void);

	// デストラクタ
	~GameStartScene(void);

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

	//ゲーム内で使う描画
	void DrawGame(void);

private:

	//インスタンス
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;

	//画面分割をするので車とカメラを二つ用意
	std::vector<std::unique_ptr<Car>> cars_;
	std::vector<std::unique_ptr<Camera>> cameras_;

	//スクリーンも二つ用意
	std::vector<int> cameraScreens_;
	std::vector<Vector2> screenPos_;

};

