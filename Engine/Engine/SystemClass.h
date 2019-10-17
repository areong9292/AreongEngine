// �ش� ����� ������ ���� �ʰ��ϴ� ��ũ��
#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// ���� ������ �ʴ� API�� ����ִ� win32����� �����Ѵ�
// ���� ����� include�ϴٺ��� �ߺ��� �߻��ϴµ� �� �ߺ� ���Ǹ� �������ش�
// ���� �ӵ� ���, �ַ� MFC�� �������� ���ԵǾ��ִ�
// #include <windows.h> ���� �ؾ� ��
#define WIN32_LEAN_AND_MEAN

#include <windows.h>


#include "InputClass.h"
#include "GraphicsClass.h"
#include "TimerClass.h"
#include "PositionClass.h"


// ��ü ���ø����̼��� ĸ��ȭ�ϴ� Ŭ����
// ������ â�� �����ϴ� Ŭ������
class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	// WndProc���� ���ϴ� ���� �ϰ� ����� �޼��� ������ ���� ó�� - �޼����� ����æ��!
	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	// �������Ӹ��� �����ϴ� �޼ҵ�
	bool Frame();
	// ������ �ʱ�ȭ�ϴ� �޼ҵ�
	void InitializeWindows(int&, int&);
	// ������ �ݴ� �޼ҵ�
	void ShutdownWindows();

private:
	LPCWSTR m_applicationName;
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	// ���� �Է��� ó���ϴ� Ŭ������ ���� ������
	InputClass* m_Input;
	// �׷����� ó���ϴ� Ŭ������ ���� ������
	GraphicsClass* m_Graphics;


	TimerClass* m_Timer;

	PositionClass* m_Position;

};

// ������ �ý��ۿ��� �޼��� ������ �Լ� - �̰� ����Ʈ
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

static SystemClass* ApplicationHandle = 0;

#endif