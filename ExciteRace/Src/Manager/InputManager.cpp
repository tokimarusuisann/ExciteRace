#include <DxLib.h>
#include "InputManager.h"

InputManager* InputManager::instance_ = nullptr;

void InputManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new InputManager();
	}
	instance_->Init();
}

InputManager& InputManager::GetInstance(void)
{
	if (instance_ == nullptr)
	{
		InputManager::CreateInstance();
	}
	return *instance_;
}

void InputManager::Init(void)
{
	auto& inpIns = InputManager::GetInstance();
	// �Q�[���Ŏg�p�������L�[���A
	// ���O�ɂ����œo�^
	inpIns.Add(KEY_INPUT_SPACE);
	inpIns.Add(KEY_INPUT_N);
	inpIns.Add(KEY_INPUT_Z);
	inpIns.Add(KEY_INPUT_W);
	inpIns.Add(KEY_INPUT_A);
	inpIns.Add(KEY_INPUT_S);
	inpIns.Add(KEY_INPUT_D);
	inpIns.Add(KEY_INPUT_Q);
	inpIns.Add(KEY_INPUT_E);
	inpIns.Add(KEY_INPUT_UP);
	inpIns.Add(KEY_INPUT_DOWN);
	inpIns.Add(KEY_INPUT_LEFT);
	inpIns.Add(KEY_INPUT_RIGHT);

	InputManager::MouseInfo info;

	// ���N���b�N
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_LEFT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

	// �E�N���b�N
	info = InputManager::MouseInfo();
	info.key = MOUSE_INPUT_RIGHT;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	mouseInfos_.emplace(info.key, info);

}

void InputManager::Update(void)
{

	// �L�[�{�[�h���m
	for (auto& p : keyInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = CheckHitKey(p.second.key);
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// �}�E�X���m
	mouseInput_ = GetMouseInput();
	GetMousePoint(&mousePos_.x, &mousePos_.y);

	for (auto& p : mouseInfos_)
	{
		p.second.keyOld = p.second.keyNew;
		p.second.keyNew = mouseInput_ == p.second.key;
		p.second.keyTrgDown = p.second.keyNew && !p.second.keyOld;
		p.second.keyTrgUp = !p.second.keyNew && p.second.keyOld;
	}

	// �p�b�h���
	SetJPadInState(JOYPAD_NO::KEY_PAD1);
	SetJPadInState(JOYPAD_NO::PAD1);
	SetJPadInState(JOYPAD_NO::PAD2);
	SetJPadInState(JOYPAD_NO::PAD3);
	SetJPadInState(JOYPAD_NO::PAD4);

}

void InputManager::Add(int key)
{
	InputManager::Info info = InputManager::Info();
	info.key = key;
	info.keyOld = false;
	info.keyNew = false;
	info.keyTrgDown = false;
	info.keyTrgUp = false;
	keyInfos_.emplace(key, info);
}

void InputManager::Clear(void)
{
	keyInfos_.clear();
}

bool InputManager::IsNew(int key) const
{
	return Find(key).keyNew;
}

bool InputManager::IsTrgDown(int key) const
{
	return Find(key).keyTrgDown;
}

bool InputManager::IsTrgUp(int key) const
{
	return Find(key).keyTrgUp;
}

Vector2 InputManager::GetMousePos(void) const
{
	return mousePos_;
}

int InputManager::GetMouse(void) const
{
	return mouseInput_;
}

bool InputManager::IsClickMouseLeft(void) const
{
	return mouseInput_ == MOUSE_INPUT_LEFT;
}

bool InputManager::IsClickMouseRight(void) const
{
	return mouseInput_ == MOUSE_INPUT_RIGHT;
}

bool InputManager::IsTrgMouseLeft(void) const
{
	return FindMouse(MOUSE_INPUT_LEFT).keyTrgDown;
}

bool InputManager::IsTrgMouseRight(void) const
{
	return FindMouse(MOUSE_INPUT_RIGHT).keyTrgDown;
}

InputManager::InputManager(void)
{
	mouseInput_ = -1;
}

InputManager::InputManager(const InputManager& manager)
{
}

InputManager::~InputManager(void)
{
	delete instance_;
}

const InputManager::Info& InputManager::Find(int key) const
{

	auto it = keyInfos_.find(key);
	if (it != keyInfos_.end())
	{
		return it->second;
	}

	return infoEmpty_;

}

const InputManager::MouseInfo& InputManager::FindMouse(int key) const
{
	auto it = mouseInfos_.find(key);
	if (it != mouseInfos_.end())
	{
		return it->second;
	}

	return mouseInfoEmpty_;
}

InputManager::JOYPAD_TYPE InputManager::GetJPadType(JOYPAD_NO no)
{
	return static_cast<InputManager::JOYPAD_TYPE>(GetJoypadType(static_cast<int>(no)));
}

DINPUT_JOYSTATE InputManager::GetJPadDInputState(JOYPAD_NO no)
{
	// �R���g���[�����
	GetJoypadDirectInputState(static_cast<int>(no), &joyDInState_);
	return joyDInState_;
}

XINPUT_STATE InputManager::GetJPadXInputState(JOYPAD_NO no)
{
	// �R���g���[�����
	GetJoypadXInputState(static_cast<int>(no), &joyXInState_);
	return joyXInState_;
}

void InputManager::SetJPadInState(JOYPAD_NO jpNo)
{

	int no = static_cast<int>(jpNo);
	auto stateNew = GetJPadInputState(jpNo);
	auto& stateNow = padInfos_[no];

	int max = static_cast<int>(JOYPAD_BTN::MAX);
	for (int i = 0; i < max; i++)
	{

		stateNow.ButtonsOld[i] = stateNow.ButtonsNew[i];
		stateNow.ButtonsNew[i] = stateNew.ButtonsNew[i];

		stateNow.IsOld[i] = stateNow.IsNew[i];
		//stateNow.IsNew[i] = stateNow.ButtonsNew[i] == 128 || stateNow.ButtonsNew[i] == 255;
		stateNow.IsNew[i] = stateNow.ButtonsNew[i] > 0;

		stateNow.IsTrgDown[i] = stateNow.IsNew[i] && !stateNow.IsOld[i];
		stateNow.IsTrgUp[i] = !stateNow.IsNew[i] && stateNow.IsOld[i];

		stateNow.AKeyLX = stateNew.AKeyLX;
		stateNow.AKeyLY = stateNew.AKeyLY;
		stateNow.AKeyRX = stateNew.AKeyRX;
		stateNow.AKeyRY = stateNew.AKeyRY;

	}

}

InputManager::JOYPAD_IN_STATE InputManager::GetJPadInputState(JOYPAD_NO no)
{

	JOYPAD_IN_STATE ret = JOYPAD_IN_STATE();

	auto type = GetJPadType(no);

	switch (type)
	{
	case InputManager::JOYPAD_TYPE::OTHER:
		break;
	case InputManager::JOYPAD_TYPE::XBOX_360:
	{
	}
	break;
	case InputManager::JOYPAD_TYPE::XBOX_ONE:
	{

		auto d = GetJPadDInputState(no);
		auto x = GetJPadXInputState(no);

		int idx;

		//   Y
		// X   B
		//   A

		idx = static_cast<int>(JOYPAD_BTN::R_BULLET);
		ret.ButtonsNew[idx] = d.Buttons[5];// RB

		idx = static_cast<int>(JOYPAD_BTN::R_BULLET);
		ret.ButtonsNew[idx] = d.Buttons[5];// RB

		idx = static_cast<int>(JOYPAD_BTN::L_BULLET);
		ret.ButtonsNew[idx] = d.Buttons[4];// LB

		idx = static_cast<int>(JOYPAD_BTN::START);
		ret.ButtonsNew[idx] = d.Buttons[7];// Y

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// Y

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.ButtonsNew[idx] = d.Buttons[2];// X

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[1];// B

		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.ButtonsNew[idx] = d.Buttons[0];// A

		idx = static_cast<int>(JOYPAD_BTN::R_TRIGGER);
		ret.ButtonsNew[idx] = x.RightTrigger;// R_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::L_TRIGGER);
		ret.ButtonsNew[idx] = x.LeftTrigger; // L_TRIGGER

		idx = static_cast<int>(JOYPAD_BTN::UP);
		ret.ButtonsNew[idx] = x.Buttons[0]; //���L�[
		
		idx = static_cast<int>(JOYPAD_BTN::BOTTOM);
		ret.ButtonsNew[idx] = x.Buttons[1]; //���L�[

		idx = static_cast<int>(JOYPAD_BTN::CROSS_RIGHT);
		ret.ButtonsNew[idx] = x.Buttons[3]; //���L�[

		idx = static_cast<int>(JOYPAD_BTN::CROSS_LEFT);
		ret.ButtonsNew[idx] = x.Buttons[2]; //���L�[


   		ret.AKeyLX = x.ThumbLX;
		if (x.ThumbLX > -2000 && x.ThumbLX < 2000)
		{
			x.ThumbLX = 0;
		}


		// ���X�e�B�b�N
		//ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// �E�X�e�B�b�N
		ret.AKeyRX = d.Rx;
		ret.AKeyRY = d.Ry;

	}
	break;
	case InputManager::JOYPAD_TYPE::DUAL_SHOCK_4:
		break;
	case InputManager::JOYPAD_TYPE::DUAL_SENSE:
	{

		auto d = GetJPadDInputState(no);
		int idx;

		//   ��
		// ��  �Z
		//   �~

		idx = static_cast<int>(JOYPAD_BTN::TOP);
		ret.ButtonsNew[idx] = d.Buttons[3];// ��

		idx = static_cast<int>(JOYPAD_BTN::LEFT);
		ret.ButtonsNew[idx] = d.Buttons[0];// ��

		idx = static_cast<int>(JOYPAD_BTN::RIGHT);
		ret.ButtonsNew[idx] = d.Buttons[2];// �Z

		idx = static_cast<int>(JOYPAD_BTN::DOWN);
		ret.ButtonsNew[idx] = d.Buttons[1];// �~

		// ���X�e�B�b�N
		ret.AKeyLX = d.X;
		ret.AKeyLY = d.Y;

		// �E�X�e�B�b�N
		ret.AKeyRX = d.Z;
		ret.AKeyRY = d.Rz;

	}
	break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_L:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_JOY_CON_R:
		break;
	case InputManager::JOYPAD_TYPE::SWITCH_PRO_CTRL:
		break;
	case InputManager::JOYPAD_TYPE::MAX:
		break;
	}

	return ret;

}

bool InputManager::IsPadBtnNew(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsNew[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgDown(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgDown[static_cast<int>(btn)];
}

bool InputManager::IsPadBtnTrgUp(JOYPAD_NO no, JOYPAD_BTN btn) const
{
	return padInfos_[static_cast<int>(no)].IsTrgUp[static_cast<int>(btn)];
}
