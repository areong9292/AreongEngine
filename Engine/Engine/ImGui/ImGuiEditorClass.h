#ifndef _IMGUIEDITORCLASS_H_
#define _IMGUIEDITORCLASS_H_

#include <d3d11.h>

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class ImGuiEditorClass
{
public:
	ImGuiEditorClass();
	ImGuiEditorClass(const ImGuiEditorClass&);
	~ImGuiEditorClass();


	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, ID3D11RenderTargetView* renderTargetView);
	void Shutdown();
	bool Render();


private:
	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RenderTargetView;



	// Our state
	bool show_demo_window = true;
	bool show_another_window = false;
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

#endif // !_IMGUIEDITORCLASS_H_