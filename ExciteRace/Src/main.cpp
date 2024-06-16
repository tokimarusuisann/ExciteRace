#include <DxLib.h>
#include "_debug/_DebugConOut.h"
#include "_debug/_DebugDispOut.h"
#include "Application.h"

// WinMain�֐�
//---------------------------------
int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{

	// �C���X�^���X�̐���
	Application::CreateInstance();

	// �C���X�^���X�̎擾
	Application& instance = Application::GetInstance();

	if (instance.IsInitFail())
	{
		// ���������s
		TRACE("���������s");
		return -1;
	}

	// ���s
	instance.Run();

	// ���
	instance.Release();

	if (instance.IsReleaseFail())
	{
		// ������s
		TRACE("������s");
		return -1;
	}

	return 0;

}
