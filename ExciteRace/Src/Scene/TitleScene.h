#pragma once
#include "SceneBase.h"

class Stage;

class TitleScene : public SceneBase
{
public:

	// コンストラクタ
	TitleScene(void);

	// デストラクタ
	~TitleScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;

private:

	//インスタンス
	std::unique_ptr<Stage> stage_;

	//タイトルイメージハンドル
	int titaleImgLogo_;

};

