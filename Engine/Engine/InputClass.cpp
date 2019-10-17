#include "inputclass.h"

InputClass::InputClass()
{
	m_directInput = nullptr;
	m_keyboard = nullptr;
	m_mouse = nullptr;
}

InputClass::InputClass(const InputClass& other)
{
}


InputClass::~InputClass()
{
}

bool InputClass::Initialize(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight)
{
	HRESULT result;

	// ȭ�� ���� ����
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// ���콺 �ʱ� ��ǥ ����
	m_mouseX = 0;
	m_mouseY = 0;

	// DirectInput �������̽� �ʱ�ȭ
	// DirectInput ��ü�� �ٸ� �Է� ��ġ���� �ʱ�ȭ�� �� �ִ�
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	if (FAILED(result))
	{
		return false;
	}

	// Ű���� �ʱ�ȭ
	result = m_directInput->CreateDevice(GUID_SysKeyboard, &m_keyboard, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_keyboard->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(result))
	{
		return false;
	}

	// Ű������ ���� ���� ����
	// DISCL_EXCLUSIVE - �ٸ� ���ø����̼ǵ�� �Է� ������ �������� �ʴ´�
	// �ٸ� ���ø����̼ǵ�� �Է��� �����Ϸ��� DISCL_NONEXCLUSIVE
	// DISCL_FOREGROUND - ���ø����̼��� ��׶���� �̵��� �� ��ġ�� �ڵ������� ������ȴ�
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// Ű���忡 ���� ���� ���� ���
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// ���콺 �ʱ�ȭ
	result = m_directInput->CreateDevice(GUID_SysMouse, &m_mouse, NULL);
	if (FAILED(result))
	{
		return false;
	}

	result = m_mouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(result))
	{
		return false;
	}

	// ���콺�� ���� ���� ����
	// DISCL_EXCLUSIVE - �ٸ� ���ø����̼ǵ�� �Է� ������ �������� �ʴ´�
	// �ٸ� ���ø����̼ǵ�� �Է��� �����Ϸ��� DISCL_NONEXCLUSIVE
	// DISCL_FOREGROUND - ���ø����̼��� ��׶���� �̵��� �� ��ġ�� �ڵ������� ������ȴ�
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// ���콺�� ���� ���� ���� ���
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

// �Է� ��ġ�� direct input �� ���� �������̽� ����
// Unacquire�� ���� ���� �ݳ� �� Release �Ѵ�
void InputClass::Shutdown()
{
	if (m_mouse != nullptr)
	{
		m_mouse->Unacquire();
		m_mouse->Release();
		m_mouse = nullptr;
	}

	if (m_keyboard != nullptr)
	{
		m_keyboard->Unacquire();
		m_keyboard->Release();
		m_keyboard = nullptr;
	}

	if (m_directInput != nullptr)
	{
		m_directInput->Release();
		m_directInput = nullptr;
	}

	return;
}

// ��ġ�� ���� ���¸� �о� ���� ���ۿ� ����Ѵ�
// ���¸� ���� �� ���� ������ ó���Ѵ�
bool InputClass::Frame()
{
	bool result;

	// ���� ���¸� �о� ���� ���ۿ� ���
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// ���� ���¸� �о� ���� ���ۿ� ���
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// ������׵��� ó���Ѵ�
	ProcessInput();

	return true;
}

// Ű���� esc �� �����ִ��� ����
bool InputClass::IsEscapePressed()
{
	if (m_keyboardState[DIK_ESCAPE] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftArrowPressed()
{
	if (m_keyboardState[DIK_LEFTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsRightArrowPressed()
{
	if (m_keyboardState[DIK_RIGHTARROW] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsLeftMouseButtonDown()
{
	if (m_mouseState.rgbButtons[0] & 0x80)
	{
		return true;
	}

	return false;
}

bool InputClass::IsRightMouseButtonDown()
{
	if (m_mouseState.rgbButtons[1] & 0x80)
	{
		return true;
	}

	return false;
}
// ���� ���콺�� ��ġ�� ����
void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

// Ű������ ���¸� m_keyboardState ������ �����Ѵ�
// m_keyboardState - ��� Ű�� ���� ���� �������� ���θ� �����ش�
bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// Ű���� ��Ŀ���� ���ų� ��� �Ұ��� ��� �� �����Ӹ���
		// ���� �߻� ���� �� ���� ��� ��û�Ѵ�
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_keyboard->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

bool InputClass::ReadMouse()
{
	HRESULT result;

	result = m_mouse->GetDeviceState(sizeof(DIMOUSESTATE), (LPVOID)&m_mouseState);
	if (FAILED(result))
	{
		if ((result == DIERR_INPUTLOST) || (result == DIERR_NOTACQUIRED))
		{
			m_mouse->Acquire();
		}
		else
		{
			return false;
		}
	}

	return true;
}

// ���� �������� ��������� ������ �����Ų��
void InputClass::ProcessInput()
{
	m_mouseX += m_mouseState.lX;
	m_mouseY += m_mouseState.lY;

	if (m_mouseX < 0) { m_mouseX = 0; }
	if (m_mouseY < 0) { m_mouseY = 0; }

	if (m_mouseX > m_screenWidth) { m_mouseX = m_screenWidth; }
	if (m_mouseY > m_screenHeight) { m_mouseY = m_screenHeight; }

	return;
}