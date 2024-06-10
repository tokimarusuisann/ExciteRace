#pragma once
#include <map>
#include <string>
#include "Resource.h"

class ResourceManager
{

public:

	// リソース名
	enum class SRC
	{
		NONE,
		CAR_LOGO,
		VERSUS_BACKGROUND,
		TIMEATTACK_BACKGROUND,
		RANKING_BACKGROUND,	
		VERSUS_BUTTON,
		TIMEATTACK_BUTTON,
		RANKING_BUTTON,
		CURSOR,
		BALANCE_CAR,
		ACCELERATE_CAR,
		SPEED_CAR,
		DECISION_BUTTON_LOGO,
		CROSS_BUTTON_LOGO,
		SELECT_BALANCE_CAR_LOGO,
		SELECT_ACCELERATE_CAR_LOGO,
		SELECT_SPEED_CAR_LOGO,
		SELECT_BALANCE_CAR_WHEEL,
		SELECT_ACCELERATE_CAR_WHEEL,
		SELECT_SPEED_CAR_WHEEL,
		WHEEL,
		SKYDOME,
		STAGE_ROAD,
		STAGE_WALL_COLLISION,
		STAGE_ROAD_COLLISION,
		GUARDRAIL,
		GUARDRAIL_COLLISION,
		TREE,
		TACHOMETER,
		NEEDLE,
		MINIMAP,
		GOAL,
		GOAL_COLLISION,
		GARAGE,
		GARAGE_FLOOR,
		LINE_1,
		LINE_2,
		LINE_3,	
		LINE_4,
		LINE_5,
		LINE_6,
		START_SIGN,
		RESULT,
		SCORE
	};

	// 明示的にインステンスを生成する
	static void CreateInstance(void);

	// 静的インスタンスの取得
	static ResourceManager& GetInstance(void);

	// 初期化
	void Init(void);

	// 解放(シーン切替時に一旦解放)
	void Release(void);

	// リソースのロード
	Resource Load(SRC src);

	// リソースの複製ロード(モデル用)
	int LoadModelDuplicate(SRC src);

private:

	// 静的インスタンス
	static ResourceManager* instance_;

	// リソース管理の対象
	std::map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::map<SRC, Resource*> loadedMap_;

	// デフォルトコンストラクタをprivateにして、
	// 外部から生成できない様にする
	ResourceManager(void);

	// デストラクタも同様
	~ResourceManager(void);

	// 内部ロード
	Resource* _Load(SRC src);

};

