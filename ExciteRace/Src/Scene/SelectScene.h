#pragma once
#include "../Object/Common/Transform.h"
#include "../Common/Vector2.h"
#include "../Manager/SceneManager.h"
#include "SceneBase.h"

class Transform;
class Camera;
class Garage;

class SelectScene : public SceneBase
{
public:

	enum class MODE
	{
		TIMEATTACK,
		VERSUS,
		OPTION
	};

	// コンストラクタ
	SelectScene(void);

	// デストラクタ
	~SelectScene(void);

	//初期化
	void Init(void) override;
	
	//更新
	void Update(void) override;
	
	//描画
	void Draw(void) override;

private:

	//背景のハンドル格納
	std::vector<int> backGraund_;

	//ボタンのハンドルと位置情報
	std::vector<std::pair<int,Vector2>> btnInfos_;

	//選ばれてるモード情報
	std::vector<MODE> modes_;

	//シーン移行モード格納
	std::vector<SceneManager::SCENE_ID> sceneIds_;

	//ボタンポジション
	std::vector<Vector2> btnPos_;

	//カーソルポジション
	Vector2 cursorPos_;

	//カーソルハンドル
	int cursorHandle_;

};

