#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:

	// コンストラクタ
	ResultScene(void);

	// デストラクタ
	~ResultScene(void);

	//初期化
	void Init(void) override;

	//更新
	void Update(void) override;

	//描画
	void Draw(void) override;

private:

	//リザルトロゴ
	int resultLogo_;

};

