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

	// 화면 정보 저장
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// 마우스 초기 좌표 설정
	m_mouseX = 0;
	m_mouseY = 0;

	// DirectInput 인터페이스 초기화
	// DirectInput 객체로 다른 입력 장치들을 초기화할 수 있다
	result = DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_directInput, NULL);

	if (FAILED(result))
	{
		return false;
	}

	// 키보드 초기화
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

	// 키보드의 협력 레벨 설정
	// DISCL_EXCLUSIVE - 다른 어플리케이션들과 입력 감지를 공유하지 않는다
	// 다른 어플리케이션들과 입력을 공유하려면 DISCL_NONEXCLUSIVE
	// DISCL_FOREGROUND - 어플리케이션이 백그라운드로 이동할 때 장치는 자동적으로 릴리즈된다
	result = m_keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 키보드에 대한 접근 권한 취득
	result = m_keyboard->Acquire();
	if (FAILED(result))
	{
		return false;
	}

	// 마우스 초기화
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

	// 마우스의 협력 레벨 설정
	// DISCL_EXCLUSIVE - 다른 어플리케이션들과 입력 감지를 공유하지 않는다
	// 다른 어플리케이션들과 입력을 공유하려면 DISCL_NONEXCLUSIVE
	// DISCL_FOREGROUND - 어플리케이션이 백그라운드로 이동할 때 장치는 자동적으로 릴리즈된다
	result = m_mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE);
	if (FAILED(result))
	{
		return false;
	}

	// 마우스에 대한 접근 권한 취득
	result = m_mouse->Acquire();
	if (FAILED(result))
	{
		return false;
	}
	return true;
}

// 입력 장치와 direct input 에 대한 인터페이스 해제
// Unacquire로 접근 권한 반납 후 Release 한다
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

// 장치의 현재 상태를 읽어 상태 버퍼에 기록한다
// 상태를 읽은 후 변경 사항을 처리한다
bool InputClass::Frame()
{
	bool result;

	// 현재 상태를 읽어 상태 버퍼에 기록
	result = ReadKeyboard();
	if (!result)
	{
		return false;
	}

	// 현재 상태를 읽어 상태 버퍼에 기록
	result = ReadMouse();
	if (!result)
	{
		return false;
	}

	// 변경사항들을 처리한다
	ProcessInput();

	return true;
}

// 키보드 esc 가 눌려있는지 여부
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
// 현재 마우스의 위치를 리턴
void InputClass::GetMouseLocation(int& mouseX, int& mouseY)
{
	mouseX = m_mouseX;
	mouseY = m_mouseY;
	return;
}

// 키보드의 상태를 m_keyboardState 변수에 저장한다
// m_keyboardState - 모든 키에 대해 현재 눌린지의 여부를 보여준다
bool InputClass::ReadKeyboard()
{
	HRESULT result;

	result = m_keyboard->GetDeviceState(sizeof(m_keyboardState), (LPVOID)&m_keyboardState);
	if (FAILED(result))
	{
		// 키보드 포커스가 없거나 취득 불가일 경우 매 프레임마다
		// 에러 발생 없을 때 까지 계속 요청한다
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

// 이전 프레임의 변경사항을 실제로 적용시킨다
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