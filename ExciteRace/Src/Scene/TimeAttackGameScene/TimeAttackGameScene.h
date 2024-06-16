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
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;

private:

	//画像ハンドル初期化
	void InitImageHandle(void);

	//選択した車の初期化
	void InitSelectCar(void);

	//カメラの初期化
	void InitCamera(void);

	//ステージオブジェクト初期化
	void InitStageObjects(void);

	//当たり判定用モデルハンドルを追加
	void SetupCollision(void);

	//オブジェクト更新
	void UpdateObject(void);

	//ゴールとの当たり判定
	void ProcessGoalCollision(void);

	//ゲーム内で使うオブジェクト描画
	void DrawGame(void);

	//UI描画
	void DrawUi(float nowSpeed, int nowGear);

	//タコメーター描画
	void DrawTachometer(void);

	//スピード描画
	void DrawSpeedAndGear(float nowSpeed, int nowGear);

	//ニードル描画（処理が多いので関数を別で作る）
	void DrawNeedle(float nowSpeed, int nowGear);

	void DrawMiniMap(void);

	//スコア描画
	void DrawScore(void);

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

