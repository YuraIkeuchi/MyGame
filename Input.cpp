#include "Input.h"
#pragma comment(lib,"dinput8.lib") 
#pragma comment(lib,"dxguid.lib") 

void Input::Initialize(WinApp* winApp)
{
	this->winApp = winApp;
	HRESULT result;
	//////�L�[�{�[�h�̏�����
	ComPtr<IDirectInput8>dinput = nullptr;
	result = DirectInput8Create(winApp->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, nullptr);
	////
	//�L�[�{�[�h�f�o�C�X�̍쐬
	////
	result = dinput->CreateDevice(GUID_SysKeyboard, &devkeyboard, NULL);

	//////�ɂイ��傭�f�[�^�`���̃Z�b�g
	result = devkeyboard->SetDataFormat(&c_dfDIKeyboard);

	//�r�����䃌�x���̃Z�b�g
	result = devkeyboard->SetCooperativeLevel(winApp->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
}

void Input::Update() 
{
	HRESULT result;
	memcpy(keyPre, key, sizeof(key));
	result = devkeyboard->Acquire();
	//BYTE key[256] = {};
	result = devkeyboard->GetDeviceState(sizeof(key), key);
}

bool Input::PushKey(BYTE keyNumber)
{
	//�w��L�[�������Ă����true��Ԃ�
	if (key[keyNumber]) {
		return true;
	}
	//�����łȂ����false��Ԃ�
	return false;
}

bool Input::TriggerKey(BYTE keyNumber) {

	// �O��0�ŁA����0�łȂ���΃g���K�[
	if (!keyPre[keyNumber] && key[keyNumber]) {
		return true;
	}

	// �g���K�[�łȂ�
	return false;
}