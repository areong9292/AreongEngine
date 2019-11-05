#include "SystemClass.h"

// ������
SystemClass::SystemClass()
{
	// ��ü ������ null �ʱ�ȭ
	m_Input = nullptr;
	m_Graphics = nullptr;
	m_Timer = nullptr;
	m_Position = nullptr;
}

// ���� �����ڿ� �Ҹ��ڴ� ���� �ش� ���ǰ� ������
// �ڵ����� ������Ű�� �����Ϸ��� �ִ�

// ���� ������
SystemClass::SystemClass(const SystemClass& other)
{
}

// �Ҹ��� - �Ҹ��ڿ� �ڵ���� Shutdown�ʿ� �����۾� �߰�
// ExitThread�� ���� �Ϻ� ������ �Լ��� �Ҹ��ڸ� ȣ������ �ʾ� �޸� ���� �߻�
SystemClass::~SystemClass()
{
}

// ���ø����̼��� ��� �ʱ�ȭ �۾� ����
// �ʿ��� ��ü(�Է�, �׷���) �ʱ�ȭ
bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// �Լ��� ���̿� �ʺ� �����ϱ� ���� ������ 0���� �ʱ�ȭ
	screenWidth = 0;
	screenHeight = 0;

	// WinApi�� ����Ͽ� ���ø����̼��� ����� �����츦 ����, �ʱ�ȭ�Ѵ�.
	InitializeWindows(screenWidth, screenHeight);

	// input��ü�� �����Ѵ�.
	// �� ��ü�� ������ Ű���� �Է��� ó���Ѵ�.
	m_Input = new InputClass;
	if (m_Input == nullptr)
	{
		return false;
	}

	// Input ��ü�� �ʱ�ȭ�Ѵ�.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Graphics ��ü�� �����Ѵ�.
	// �� ��ü�� ���ø����̼��� ��� �׷��� ��Ҹ� �׸��� ������ �Ѵ�.
	m_Graphics = new GraphicsClass;
	if (m_Graphics == nullptr)
	{
		return false;
	}

	// Graphics ��ü�� �ʱ�ȭ�Ѵ�.
	result = m_Graphics->Initialize(screenWidth, screenHeight, m_hwnd);
	if (!result)
	{
		return false;
	}

	m_Timer = new TimerClass;
	if (m_Timer == nullptr)
	{
		return false;
	}

	result = m_Timer->Initialize();
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the Timer object.", L"Error", MB_OK);
		return false;
	}

	m_Position = new PositionClass;
	if (m_Position == nullptr)
	{
		return false;
	}

	return true;
}

// ��ü�� �����ϴ� �޼ҵ�
void SystemClass::Shutdown()
{
	if (m_Position != nullptr)
	{
		delete m_Position;
		m_Position = nullptr;
	}

	if (m_Timer != nullptr)
	{
		delete m_Timer;
		m_Timer = nullptr;
	}

	// Graphics ��ü�� ��ȯ
	if (m_Graphics != nullptr)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Input ��ü�� ��ȯ
	if (m_Input != nullptr)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	// â�� �����Ų��.
	ShutdownWindows();
}

// ���α׷� ����ñ��� ������ ���鼭 ���ø����̼��� ��� �۾��� ó���Ѵ�
// ���ø����̼��� ��� �۾��� �� �������� �Ҹ��� Frame�Լ����� ����ȴ�
void SystemClass::Run()
{
	// �����ڵ�
	// while(������� ���� ����)
		// �������� �ý��� �޼��� Ȯ��
		// �޼��� ó��
		// ���ø����̼��� �۾�
		// ������ �۾��� ���α׷� ���Ḧ ���ϴ��� Ȯ��
			// ���� ���ϸ� ����, �ƴϸ� ��� �ݺ�
	
	MSG msg;
	bool done, result;

	// �޼��� ����ü �ʱ�ȭ
	ZeroMemory(&msg, sizeof(MSG));

	// �����κ��� ����޼��� ���� ������ ����
	done = false;
	while (!done)
	{
		// ������ �޼��� ó��
		// �����쿡�� �߻��ϴ� �޼����� msg ����ü�� �޾ƿ´�

		// PM_REMOVE - �޼����� ���� ��� GetMessage�� ������ ������ �Ѵ�
		// -> �޼����� ���� �� ť���� �޼����� �����ϰ� �̸� msg�� ��´�
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// Ű���� �Է� �޼����� �����Ͽ� ���α׷����� ���� �� �� �ְ���
			// -> Ű �Է��ϸ� keydown, keyup�� �����µ� translateMessage��
			// Ű�Է��� ���(KeyDown) wm_char �޼����� ������ش�
			TranslateMessage(&msg);
			// �ý��� �޼��� ť���� ���� �޼����� WndProc�� �����Ѵ�
			DispatchMessage(&msg);
		}

		// �����쿡�� ���Ḧ ��û�ϴ� ��� ���� ����
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// �� �����Ӹ��� ȭ���� �׸��� Frame �Լ��� ó���Ѵ�
			result = Frame();

			// ó���� �̻��� ��� or escŰ�� ���� ��� �ݺ� ����
			if (!result)
			{
				done = true;
			}
		}
		if (m_Input->IsEscapePressed())
		{
			done = true;
		}
	}

	return;

}

// ���ø����̼��� ��� �۾��� ó���ȴ�
bool SystemClass::Frame()
{
	bool keyDown, result;
	int mouseX, mouseY;
	float rotationY;

	bool beginCheck = false;

	m_Timer->Frame();

	result = m_Input->Frame();
	if (!result)
	{
		return false;
	}

	m_Input->GetMouseLocation(mouseX, mouseY);

	// �� �����Ӹ��� PositionClass ��ü�� ������ �ð��� �����Ѵ�
	m_Position->SetFrameTime(m_Timer->GetTime());

	// ���� �������� ��������
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	// ������ �������� ������
	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	// Ű �Է¿� ���� �� ȸ������ �޾ƿ´�
	m_Position->GetRotation(rotationY);

	// ���콺 �̺�Ʈ ���� ��� �ѹ��� ���콺 ��ǥ ���� �޾ƿ´�
	if (m_Input->IsLeftMouseButtonDown())
	{
		if (beginCheck == false)
		{
			beginCheck = true;
			m_Input->GetMouseLocation(mouseX, mouseY);
		}
	}
	else
	{
		beginCheck = false;
	}

	// Graphics ��ü�� ���� ȭ�鿡 �׸��� �۾��� ó���Ѵ�.
	result = m_Graphics->Frame(mouseX, mouseY, rotationY, beginCheck);
	if (!result)
	{
		return false;
	}


	return true;
}

// �������� �ý��� �޼����� ���޵ȴ�
LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

// �����츦 ����� �޼ҵ�
// ȣ���� �Լ����Է� �ʺ�, ���̸� �����ش�(&)
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// �ܺ������͸� �� ��ü�� ����
	ApplicationHandle = this;

	// �� ���ø����̼��� �ν��Ͻ��� �����´�
	m_hinstance = GetModuleHandle(NULL);

	// ���ø����̼��� �̸��� �����մϴ�
	m_applicationName = L"Engine";

	// ������ Ŭ������ �⺻ �������� ����ϴ�.
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = m_hinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = m_applicationName;
	wc.cbSize = sizeof(WNDCLASSEX);

	// ������ Ŭ������ ���
	// ������ wc�� ���� Ư���� ���� �����츦 ����ϰڴٰ� ���
	// �ü���� �� ������ Ŭ������ Ư���� ����Ѵ�
	RegisterClassEx(&wc);

	// ����� ȭ���� �ػ󵵸� ������
	// SM_CXSCREEN - ����
	// SM_CYSCREEN - ����
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// Ǯ��ũ�� ��� ������ ���� ���� ȭ�� ������ �Ѵ�.
	if (FULL_SCREEN)
	{
		// ���� Ǯ��ũ�� �����
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);

		// ȭ�� ũ�⸦ ����ũ�� ũ�⿡ ���߰�
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		
		// ������ 32bit�� �մϴ�.
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		// Ǯ��ũ���� �´� ���÷��� ������ �մϴ�. - Ǯ��ũ������ �ػ� ����
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// �������� ��ġ�� ȭ���� ���� ���� ����ϴ�.
		posX = posY = 0;
	}
	else
	{
		// ������ ����� 1280x720�� ũ�⸦ ������ �մϴ�.
		screenWidth = 1280;
		screenHeight = 720;

		// â�� ������� �߾ӿ� ������ �մϴ�.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// ������ ���� ������ â�� ����� �� �ڵ��� �����ɴϴ�.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY,
		screenWidth, screenHeight,
		NULL, NULL, m_hinstance, NULL);

	// �����츦 ȭ�鿡 ǥ���ϰ� ��Ŀ�� ����
	ShowWindow(m_hwnd, SW_SHOW);

	// �����츦 �ֻ����� ����
	SetForegroundWindow(m_hwnd);

	// ��Ŀ�� ����
	SetFocus(m_hwnd);

	// ���콺 Ŀ���� ǥ������ ����
	ShowCursor(false);

	return;
}

// ȭ�� ������ �ǵ����� ������� �� �ڵ���� ��ȯ�Ѵ�
void SystemClass::ShutdownWindows()
{
	// ���콺 Ŀ�� ǥ��
	ShowCursor(true);

	// Ǯ��ũ�� ��带 �������� �� ���÷��� ������ ����
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// �����츦 �����Ѵ�.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// ���ø����̼� �ν��Ͻ��� ����
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// �� Ŭ������ ���� �ܺ� ������ ���� ����
	ApplicationHandle = nullptr;

	return;
}


// WndProc -> MessageHandler -> DefWindowProc
// ������ �ý��ۿ��� �޼����� ������ ��
// ������ ���鶧 wc.lpfnWndProc = WndProc; ��������
// �����쿡�� �߻��ϴ� �޼����� �� �޼ҵ�� ���� �ȴ�
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam))
		return true;

	switch (umsg)
	{
		// ������ ���� Ȯ��
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �����찡 �������� Ȯ��
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// �ٸ� ��� �޼������� system Ŭ������ �޼��� ó���⿡ �����Ѵ�
		default:
		{
			if(ApplicationHandle != nullptr)
				return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);

			return 0;
		}
	}
}