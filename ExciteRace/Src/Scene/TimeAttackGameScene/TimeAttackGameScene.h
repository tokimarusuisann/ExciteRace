#pragma once
#include "../SceneBase.h"

class Car;
class Camera;
class Stage;
class SkyDome;
class Garage;

class TimeAttackGameScene : public SceneBase
{
public:

	// コンストラクタ
	TimeAttackGameScene(void);

	// デストラクタ
	~TimeAttackGameScene(void);

	//初期化
	void Init(void) override;

	//画像ハンドル初期化
	void InitImageHandle(void);
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;
	


	////UI AT描画
	//void DrawUiAT(void);

	////UI MT描画
	//void DrawUiMT(void);

private:

	//UI描画
	void DrawUi(void);

	//ニードル描画（処理が多いので関数を別で作る）
	void DrawUiNeedle(float nowSpeed);

	//集中線描画
	void DrawLine(void);

	//インスタンス
	std::unique_ptr<Car> car_;
	std::unique_ptr<Camera> camera_;
	std::unique_ptr<SkyDome> skyDome_;
	std::unique_ptr<Stage> stage_;
	std::unique_ptr<Garage> garage_;

	//画像ハンドル格納変数
	enum class IMAGE_TYPE
	{
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		START_SIGN,
		LINE_4,
		LINE_5,
		LINE_6,
	};

	//タイプごとのハンドル情報格納変数
	std::map<IMAGE_TYPE, int> imageInfos_;

	//集中線管理ステップタイム
	float delta_;

	//スタート管理変数
	float stepStartTime_;

	//スコア管理変数
	float scoreTime_;

};

