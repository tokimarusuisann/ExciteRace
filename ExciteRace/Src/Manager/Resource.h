#pragma once
#include <string>
#include <vector>

class Resource
{
public:
	
	// ���\�[�X�^�C�v
	enum class TYPE
	{
		NONE,
		IMG,
		IMGS,
		MODEL,
		EFFEKSEER
	};

	// �R���X�g���N�^
	Resource(void);
	// �R���X�g���N�^
	Resource(TYPE type, const std::string& path);
	// �R���X�g���N�^(IMGS�p)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// �f�X�g���N�^
	~Resource(void);

	// �ǂݍ���
	void Load(void);

	// ���
	void Release(void);

	// �����摜�n���h����ʔz��ɃR�s�[
	void CopyHandle(int* imgs);

	// ���\�[�X�^�C�v
	TYPE type_;

	// ���\�[�X�̓ǂݍ��ݐ�
	std::string path_;

	// �摜�ƃ��f���̃n���h��ID
	int handleId_;

	// IMGS::LoadDivGraph�p
	int* handleIds_;
	int numX_;
	int numY_;
	int sizeX_;
	int sizeY_;

	// ���f�������p
	std::vector<int> duplicateModelIds_;

};

