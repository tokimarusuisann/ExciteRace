#pragma once
#include "SceneBase.h"

class Stage;

class TitleScene : public SceneBase
{
public:

	// �R���X�g���N�^
	TitleScene(void);

	// �f�X�g���N�^
	~TitleScene(void);

	//������
	void Init(void) override;
	
	//�X�V
	void Update(void) override;
	
	//�`��
	void Draw(void) override;

private:

	//�C���X�^���X
	std::unique_ptr<Stage> stage_;

	//�^�C�g���C���[�W�n���h��
	int titaleImgLogo_;

};

