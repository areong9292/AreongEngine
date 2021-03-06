#include "SystemClass.h"

// 어플리케이션의 시작점으로
// 필요한 모든 작업은 SystemClass 안에 캡슐화시켜서 실행만 한다
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	// system 객체를 생성한다.
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	// System 객체를 초기화하고
	result = System->Initialize();

	// 문제가 없으면 run을 호출한다.
	if (result)
	{
		// Run내부에서 루프를 돌아 종료 때까지 잡고있는다.
		System->Run();
	}

	// Run이 종료되면 System객체를 종료하고 사용했던 메모리를 반환한다.
	System->Shutdown();
	delete System;
	System = 0;

	return 0;
}