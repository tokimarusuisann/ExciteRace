#pragma once
#include "SceneBase.h"

class ResultScene : public SceneBase
{
public:

	// �R���X�g���N�^
	ResultScene(void);

	// �f�X�g���N�^
	~ResultScene(void);

	//������
	void Init(void) override;

	//�X�V
	void Update(void) override;

	//�`��
	void Draw(void) override;

private:

	//���U���g���S
	int resultLogo_;

};

