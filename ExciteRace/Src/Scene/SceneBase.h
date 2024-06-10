#pragma once
#include <DxLib.h>

class SceneBase
{
public:

	//ガレージの初期位置
	static constexpr VECTOR GARAGE_INIT_POS = { 25000.0f,0.0f,26000.0f };

	//対戦時、車の初期位置
	static constexpr VECTOR CAR_INIT_POS_1 = { 900.0f, 32.0f, 300.0f };
	static constexpr VECTOR CAR_INIT_POS_2 = { 1500.0f, 32.0f, 300.0f };

	//タイムアタック時、車の初期化
	static constexpr VECTOR CAR_INIT_POS_ = { 1200.0f, 32.0f, 300.0f };

	//MUSIC音量
	static constexpr int MUSIC_DEFAULT_VOLUME = 30;

	// コンストラクタ
	SceneBase(void);

	// デストラクタ
	virtual ~SceneBase(void);

	// 初期化処理
	virtual void Init(void);

	// 更新ステップ
	virtual void Update(void);

	// 描画処理
	virtual void Draw(void);

};

