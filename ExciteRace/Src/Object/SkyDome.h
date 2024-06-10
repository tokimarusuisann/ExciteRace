#pragma once
#include "../Object/Common/Transform.h"

class SkyDome
{
public:

	// コンストラクタ
	SkyDome();

	// デストラクタ
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void SetFollowTarget(const Transform* target);

private:

	// モデル制御の基本情報
	Transform transform_;

	//追従対象
	const Transform* target_;

};

