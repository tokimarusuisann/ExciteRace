#pragma once
#include "Common/Transform.h"

class Garage
{
public:

	// コンストラクタ
	Garage(void);

	// デストラクタ
	~Garage(void);

	//初期化
	void Init(void);

	//更新
	void Update(void);
	
	//描画
	void Draw(void);
	
	//ポジションセット
	void SetPos(VECTOR pos);

public:

	//ガレージのタイプ
	enum class GARAGE_TYPE
	{
		GARAGE,
		FLOOR
	};

	//タイプごとのトランスフォーム情報
	std::map<GARAGE_TYPE, Transform> garageInfos_;

};

