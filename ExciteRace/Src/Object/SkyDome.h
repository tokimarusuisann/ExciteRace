#pragma once
#include "../Object/Common/Transform.h"

class SkyDome
{
public:

	// �R���X�g���N�^
	SkyDome();

	// �f�X�g���N�^
	~SkyDome(void);

	void Init(void);
	void Update(void);
	void Draw(void);
	void SetFollowTarget(const Transform* target);

private:

	// ���f������̊�{���
	Transform transform_;

	//�Ǐ]�Ώ�
	const Transform* target_;

};

