// 해당 헤더가 재정의 되지 않게하는 매크로
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// 자주 사용되지 않는 API를 담고있는 win32헤더를 무시한다
// 또한 헤더를 include하다보면 중복이 발생하는데 그 중복 정의를 제거해준다
// 빌드 속도 상승, 주로 MFC용 정보들이 포함되어있다
// #include <windows.h> 전에 해야 됨
#define WIN32_LEAN_AND_MEAN

#include <windows.h>


#include "InputClass.h"
#include "GraphicsClass.h"
#include "TimerClass.h"
#include "PositionClass.h"


// 전체 어플리케이션을 캡슐화하는 클래스
// 윈도우 창을 생성하는 클래스다
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	// WndProc에서 원하는 동작 하고 여기로 메세지 보내서 따로 처리 - 메세지를 가로챈다!
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	// 매프레임마다 동작하는 메소드
	bool Frame();
	// 윈도우 초기화하는 메소드
	void InitializeWindows(int&, int&);
	// 윈도우 닫는 메소드
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// 유저 입력을 처리하는 클래스에 대한 포인터
	InputClass* m_Input;
	// 그래픽을 처리하는 클래스에 대한 포인터
	GraphicsClass* m_Graphics;


	TimerClass* m_Timer;

	PositionClass* m_Position;

};

// 윈도우 시스템에서 메세지 보내는 함수 - 이게 디폴트
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif