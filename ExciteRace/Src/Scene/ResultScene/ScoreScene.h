#pragma once
#include "../SceneBase.h"

class Vector2;

class ScoreScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ScoreScene(void);

	// �f�X�g���N�^
	~ScoreScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;

private:

	//�X�R�A��ʂ̃��S
	int scoreLogo_;

};

