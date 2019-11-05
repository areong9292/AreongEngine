#include "SystemClass.h"

// 생성자
SystemClass::SystemClass()
{
	// 객체 포인터 null 초기화
	m_Input = nullptr;
	m_Graphics = nullptr;
	m_Timer = nullptr;
	m_Position = nullptr;
}

// 복사 생성자와 소멸자는 만약 해당 정의가 없으면
// 자동으로 생성시키는 컴파일러가 있다

// 복사 생성자
SystemClass::SystemClass(const SystemClass& other)
{
}

// 소멸자 - 소멸자에 코드없고 Shutdown쪽에 정리작업 추가
// ExitThread와 같은 일부 윈도우 함수는 소멸자를 호출하지 않아 메모리 누수 발생
SystemClass::~SystemClass()
{
}

// 어플리케이션의 모든 초기화 작업 수행
// 필요한 객체(입력, 그래픽) 초기화
bool SystemClass::Initialize()
{
	int screenWidth, screenHeight;
	bool result;

	// 함수에 높이와 너비를 전달하기 전에 변수를 0으로 초기화
	screenWidth = 0;
	screenHeight = 0;

	// WinApi를 사용하여 어플리케이션이 사용할 윈도우를 생성, 초기화한다.
	InitializeWindows(screenWidth, screenHeight);

	// input객체를 생성한다.
	// 이 객체는 유저의 키보드 입력을 처리한다.
	m_Input = new InputClass;
	if (m_Input == nullptr)
	{
		return false;
	}

	// Input 객체를 초기화한다.
	result = m_Input->Initialize(m_hinstance, m_hwnd, screenWidth, screenHeight);
	if (!result)
	{
		MessageBox(m_hwnd, L"Could not initialize the input object.", L"Error", MB_OK);
		return false;
	}

	// Graphics 객체를 생성한다.
	// 이 객체는 어플리케이션의 모든 그래픽 요소를 그리는 역할을 한다.
	m_Graphics = new GraphicsClass;
	if (m_Graphics == nullptr)
	{
		return false;
	}

	// Graphics 객체를 초기화한다.
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

// 객체를 정리하는 메소드
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

	// Graphics 객체를 반환
	if (m_Graphics != nullptr)
	{
		m_Graphics->Shutdown();
		delete m_Graphics;
		m_Graphics = nullptr;
	}

	// Input 객체를 반환
	if (m_Input != nullptr)
	{
		m_Input->Shutdown();
		delete m_Input;
		m_Input = nullptr;
	}

	// 창을 종료시킨다.
	ShutdownWindows();
}

// 프로그램 종료시까지 루프를 돌면서 어플리케이션의 모든 작업을 처리한다
// 어플리케이션의 모든 작업은 매 루프마다 불리는 Frame함수에서 수행된다
void SystemClass::Run()
{
	// 수도코드
	// while(종료되지 않은 동안)
		// 윈도우의 시스템 메세지 확인
		// 메세지 처리
		// 어플리케이션의 작업
		// 유저가 작업중 프로그램 종료를 원하는지 확인
			// 종료 원하면 종료, 아니면 계속 반복
	
	MSG msg;
	bool done, result;

	// 메세지 구조체 초기화
	ZeroMemory(&msg, sizeof(MSG));

	// 유저로부터 종료메세지 받을 때까지 루프
	done = false;
	while (!done)
	{
		// 윈도우 메세지 처리
		// 윈도우에서 발생하는 메세지를 msg 구조체에 받아온다

		// PM_REMOVE - 메세지가 있을 경우 GetMessage와 동일한 동작을 한다
		// -> 메세지를 읽은 후 큐에서 메세지를 제거하고 이를 msg에 담는다
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			// 키보드 입력 메세지를 가공하여 프로그램에서 쉽게 쓸 수 있게함
			// -> 키 입력하면 keydown, keyup만 나오는데 translateMessage가
			// 키입력일 경우(KeyDown) wm_char 메세지를 만들어준다
			TranslateMessage(&msg);
			// 시스템 메세지 큐에서 꺼낸 메세지를 WndProc에 전달한다
			DispatchMessage(&msg);
		}

		// 윈도우에서 종료를 요청하는 경우 루프 종료
		if (msg.message == WM_QUIT)
		{
			done = true;
		}
		else
		{
			// 매 프레임마다 화면을 그리는 Frame 함수를 처리한다
			result = Frame();

			// 처리가 이상할 경우 or esc키를 누를 경우 반복 종료
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

// 어플리케이션의 모든 작업이 처리된다
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

	// 매 프레임마다 PositionClass 객체의 프레임 시간을 갱신한다
	m_Position->SetFrameTime(m_Timer->GetTime());

	// 왼쪽 눌럿으면 왼쪽으로
	keyDown = m_Input->IsLeftArrowPressed();
	m_Position->TurnLeft(keyDown);

	// 오른쪽 눌럿으면 오른쪽
	keyDown = m_Input->IsRightArrowPressed();
	m_Position->TurnRight(keyDown);

	// 키 입력에 따른 새 회전값을 받아온다
	m_Position->GetRotation(rotationY);

	// 마우스 이벤트 있을 경우 한번만 마우스 좌표 새로 받아온다
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

	// Graphics 객체를 통해 화면에 그리는 작업을 처리한다.
	result = m_Graphics->Frame(mouseX, mouseY, rotationY, beginCheck);
	if (!result)
	{
		return false;
	}


	return true;
}

// 윈도우의 시스템 메세지가 전달된다
LRESULT SystemClass::MessageHandler(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	return DefWindowProc(hwnd, umsg, wparam, lparam);
}

// 윈도우를 만드는 메소드
// 호출한 함수에게로 너비, 높이를 돌려준다(&)
void SystemClass::InitializeWindows(int& screenWidth, int& screenHeight)
{
	WNDCLASSEX wc;
	DEVMODE dmScreenSettings;
	int posX, posY;

	// 외부포인터를 이 객체로 설정
	ApplicationHandle = this;

	// 이 어플리케이션의 인스턴스를 가져온다
	m_hinstance = GetModuleHandle(NULL);

	// 어플리케이션의 이름을 설정합니다
	m_applicationName = L"Engine";

	// 윈도우 클래스를 기본 설정으로 맞춥니다.
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

	// 윈도우 클래스를 등록
	// 앞으로 wc와 같은 특성을 가진 윈도우를 사용하겠다고 등록
	// 운영체제는 이 윈도우 클래스의 특성을 기억한다
	RegisterClassEx(&wc);

	// 모니터 화면의 해상도를 가져옴
	// SM_CXSCREEN - 가로
	// SM_CYSCREEN - 세로
	screenWidth = GetSystemMetrics(SM_CXSCREEN);
	screenHeight = GetSystemMetrics(SM_CYSCREEN);

	// 풀스크린 모드 변수의 값에 따라 화면 설정을 한다.
	if (FULL_SCREEN)
	{
		// 만약 풀스크린 모드라면
		memset(&dmScreenSettings, 0, sizeof(dmScreenSettings));
		dmScreenSettings.dmSize = sizeof(dmScreenSettings);

		// 화면 크기를 데스크톱 크기에 맞추고
		dmScreenSettings.dmPelsWidth = (unsigned long)screenWidth;
		dmScreenSettings.dmPelsHeight = (unsigned long)screenHeight;
		
		// 색상을 32bit로 합니다.
		dmScreenSettings.dmBitsPerPel = 32;
		dmScreenSettings.dmFields = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		
		// 풀스크린에 맞는 디스플레이 설정을 합니다. - 풀스크린으로 해상도 변경
		ChangeDisplaySettings(&dmScreenSettings, CDS_FULLSCREEN);

		// 윈도우의 위치를 화면의 왼쪽 위로 맞춥니다.
		posX = posY = 0;
	}
	else
	{
		// 윈도우 모드라면 1280x720의 크기를 가지게 합니다.
		screenWidth = 1280;
		screenHeight = 720;

		// 창을 모니터의 중앙에 오도록 합니다.
		posX = (GetSystemMetrics(SM_CXSCREEN) - screenWidth) / 2;
		posY = (GetSystemMetrics(SM_CYSCREEN) - screenHeight) / 2;
	}

	// 설정한 것을 가지고 창을 만들고 그 핸들을 가져옵니다.
	m_hwnd = CreateWindowEx(
		WS_EX_APPWINDOW, m_applicationName, m_applicationName,
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_POPUP,
		posX, posY,
		screenWidth, screenHeight,
		NULL, NULL, m_hinstance, NULL);

	// 윈도우를 화면에 표시하고 포커스 설정
	ShowWindow(m_hwnd, SW_SHOW);

	// 윈도우를 최상위로 보냄
	SetForegroundWindow(m_hwnd);

	// 포커스 설정
	SetFocus(m_hwnd);

	// 마우스 커서를 표시하지 않음
	ShowCursor(false);

	return;
}

// 화면 설정을 되돌리고 윈도우와 그 핸들들을 반환한다
void SystemClass::ShutdownWindows()
{
	// 마우스 커서 표시
	ShowCursor(true);

	// 풀스크린 모드를 빠져나올 때 디스플레이 설정을 변경
	if (FULL_SCREEN)
	{
		ChangeDisplaySettings(NULL, 0);
	}

	// 윈도우를 제거한다.
	DestroyWindow(m_hwnd);
	m_hwnd = NULL;

	// 어플리케이션 인스턴스를 제거
	UnregisterClass(m_applicationName, m_hinstance);
	m_hinstance = NULL;

	// 이 클래스에 대한 외부 포인터 참조 제거
	ApplicationHandle = nullptr;

	return;
}


// WndProc -> MessageHandler -> DefWindowProc
// 윈도우 시스템에서 메세지를 보내는 곳
// 윈도우 만들때 wc.lpfnWndProc = WndProc; 구문으로
// 윈도우에서 발생하는 메세지는 이 메소드로 오게 된다
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WndProc(HWND hwnd, UINT umsg, WPARAM wparam, LPARAM lparam)
{
	if (ImGui_ImplWin32_WndProcHandler(hwnd, umsg, wparam, lparam))
		return true;

	switch (umsg)
	{
		// 윈도우 제거 확인
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}

		// 윈도우가 닫히는지 확인
		case WM_CLOSE:
		{
			PostQuitMessage(0);
			return 0;
		}

		// 다른 모든 메세지들은 system 클래스의 메세지 처리기에 전달한다
		default:
		{
			if(ApplicationHandle != nullptr)
				return ApplicationHandle->MessageHandler(hwnd, umsg, wparam, lparam);

			return 0;
		}
	}
}