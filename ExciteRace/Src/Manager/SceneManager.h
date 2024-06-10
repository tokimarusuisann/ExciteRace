#pragma once
#include <chrono>
#include "../Common/CollectStructure.h"
#include "InputManager.h"
class SceneBase;
class Fader;
class Camera;
class ResourceManager;

class SceneManager
{
public:

	//プレイヤー人数
	static constexpr int PLAYER_NUM = 2;

	// シーン管理用
	enum class SCENE_ID
	{
		NONE,
		TITLE,
		SELECT,
		TA_CARSELECT,
		VS_GAMESTART,
		GAME,
		TIMEATTACKGAMESTART,
		TIMEATTACKGAME,
		SCORE,
		RESULT
	};
	
	// インスタンスの生成
	static void CreateInstance(void);

	// インスタンスの取得
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// 状態遷移
	void ChangeScene(SCENE_ID nextId, bool isFading);

	// シーンIDの取得
	SCENE_ID GetmSceneID(void);

	// デルタタイムの取得
	float GetDeltaTime(void) const;

	VECTOR GetGravityDir(void) const;
	float GetGravityPow(void) const;

	void SetScore(float score);
	std::vector<float> GetScores(void);

	void SetCarType(CAR_TYPE carType);
	CAR_TYPE GetCarType(void);

	InputManager::JOYPAD_NO GetPadNo(void);
	void SetPadNo(InputManager::JOYPAD_NO joyNo);

	int GetScreen(void);

	static int GetPlayerNum(void);

private:

	//デフォルトコンストラクタをprivateにして、
	//外部から生成できない様にする
	SceneManager(void);
	~SceneManager(void);

	void ResetDeltaTime(void);

	// シーン遷移
	void DoChangeScene(void);

	// フェード
	void Fade(void);

	// 静的インスタンス
	static SceneManager* instance_;

	SCENE_ID sceneID_;
	SCENE_ID waitSceneID_;

	SceneBase* scene_;
	Fader* fader_;

	bool isSceneChanging_;

	VECTOR pointLightPos_;

	// デルタタイム
	std::chrono::system_clock::time_point preTime_;
	float deltaTime_;

	VECTOR dirDown;
	float gravityPow;

	std::vector<float> scores_;

	CAR_TYPE carType_;

	InputManager::JOYPAD_NO joyNo_;

	int handle_;

	int mainScreen_;

};