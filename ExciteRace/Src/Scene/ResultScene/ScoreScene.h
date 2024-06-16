#pragma once
#include "../SceneBase.h"

class Vector2;

class ScoreScene : public SceneBase
{
public:

	// コンストラクタ
	ScoreScene(void);

	// デストラクタ
	~ScoreScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;

private:

	//スコア画面のロゴ
	int scoreLogo_;

};

