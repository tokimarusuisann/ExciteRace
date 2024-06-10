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

	//�v���C���[�l��
	static constexpr int PLAYER_NUM = 2;

	// �V�[���Ǘ��p
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
	
	// �C���X�^���X�̐���
	static void CreateInstance(void);

	// �C���X�^���X�̎擾
	static SceneManager& GetInstance(void);

	void Init(void);
	void Init3D(void);
	void Update(void);
	void Draw(void);
	void Release(void);

	// ��ԑJ��
	void ChangeScene(SCENE_ID nextId, bool isFading);

	// �V�[��ID�̎擾
	SCENE_ID GetmSceneID(void);

	// �f���^�^�C���̎擾
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

	//�f�t�H���g�R���X�g���N�^��private�ɂ��āA
	//�O�����琶���ł��Ȃ��l�ɂ���
	SceneManager(void);
	~SceneManager(void);

	void ResetDeltaTime(void);

	// �V�[���J��
	void DoChangeScene(void);

	// �t�F�[�h
	void Fade(void);

	// �ÓI�C���X�^���X
	static SceneManager* instance_;

	SCENE_ID sceneID_;
	SCENE_ID waitSceneID_;

	SceneBase* scene_;
	Fader* fader_;

	bool isSceneChanging_;

	VECTOR pointLightPos_;

	// �f���^�^�C��
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