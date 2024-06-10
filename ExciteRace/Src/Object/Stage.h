#pragma once
#include "Common/Transform.h"

class Stage
{
public:

	// コンストラクタ
	Stage(void);

	// デストラクタ
	~Stage(void);

	//初期化
	void Init(void);
	
	//更新
	void Update(void);

	//描画
	void Draw(void);

	//モデルID取得
	int GetModelIdRoadCollision(void);
	int GetModelIdGuardRailCollision(void);
	int GetModelIdWallCollision(void);
	int GetModelIdGoalCollision(void);

private:

	//ステージタイプ
	enum class STAGE_TYPE
	{
		ROAD,
		GUARDRAIL,
		TREE,
		GOAL,
		ROAD_COLLISION,
		WALL_COLLISION,
		GUARDRAIL_COLLISION,
		GOAL_COLLISION,
		MAX
	};

	//タイプごとのトランスフォーム情報
	std::map<STAGE_TYPE, Transform> stageInfos_;

};