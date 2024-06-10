#pragma once
#include "Common/Transform.h"

class Garage
{
public:

	// �R���X�g���N�^
	Garage(void);

	// �f�X�g���N�^
	~Garage(void);

	//������
	void Init(void);

	//�X�V
	void Update(void);
	
	//�`��
	void Draw(void);
	
	//�|�W�V�����Z�b�g
	void SetPos(VECTOR pos);

public:

	//�K���[�W�̃^�C�v
	enum class GARAGE_TYPE
	{
		GARAGE,
		FLOOR
	};

	//�^�C�v���Ƃ̃g�����X�t�H�[�����
	std::map<GARAGE_TYPE, Transform> garageInfos_;

};

