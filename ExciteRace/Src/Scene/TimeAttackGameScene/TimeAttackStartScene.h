#pragma once
#include "../SceneBase.h"

class Stage;
class SkyDome;
class Car;
class Camera;
class Vector2;

class TimeAttackStartScene : public SceneBase
{
public:

	// コンストラクタ
	TimeAttackStartScene(void);

	// デストラクタ
	~TimeAttackStartScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;

private:

	//インスタンス
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Car> car_;
	std::unique_ptr<Camera> camera_;

};

