#ifndef _IMGUIEDITORCLASS_H_
#define _IMGUIEDITORCLASS_H_

#include <d3d11.h>
#include <map>
#include <string>

using namespace std;

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

class GraphicsClass;
class ModelListClass;
class ModelClass;
class Material;
class Transform;
class DrawInspector;
class ImGuiEditorClass
{
public:
	ImGuiEditorClass();
	ImGuiEditorClass(const ImGuiEditorClass&);
	~ImGuiEditorClass();

	bool Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, ID3D11RenderTargetView* renderTargetView, GraphicsClass* graphics, int screenWidth, int screenHeight);
	void Shutdown();
	bool Render(ID3D11ShaderResourceView* shaderResourceView);

private:
	void DrawDock(ID3D11ShaderResourceView* shaderResourceView);
	void CreateDock();

	void DrawHierarchy();
	void DrawTransform(Transform* transform);

	void DrawInspect();

	void HelpMarker(const char* desc);

	ID3D11Device* m_Device;
	ID3D11DeviceContext* m_DeviceContext;
	ID3D11RenderTargetView* m_RenderTargetView;

	GraphicsClass* m_Graphics;

	// imgui state
	bool show_demo_window = true;
	bool show_another_window = false;
	bool show_dock_window = true;

	bool show_hierarchy_window = true;
	bool show_inspector_window = true;

	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

	ImGuiID dockspace_id;

	int m_screenWidth;
	int m_screenHeight;

	int selectedModel= -1;


	// 인스펙터용 변수
	ModelListClass* i_modelList;
	ModelClass* i_model;
	Material* i_mat;

	// 현재 어떤 게임 오브젝트를 눌렀는지
	Transform* selectedItem;

	DrawInspector* drawInspector;
};

#endif // !_IMGUIEDITORCLASS_H_