#include "ImGuiEditorClass.h"

#include "../GraphicsClass.h"
#include "../Material.h"
#include "../ModelClass.h"
#include "../ModelListClass.h"
#include "../../../DesignPattern/Transform.h"
#include "../../../DesignPattern/Component.h"


#include "DrawInspector.h"


ImGuiEditorClass::ImGuiEditorClass()
{
}

ImGuiEditorClass::ImGuiEditorClass(const ImGuiEditorClass &)
{
}

ImGuiEditorClass::~ImGuiEditorClass()
{
}

bool ImGuiEditorClass::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, HWND hwnd, ID3D11RenderTargetView* renderTargetView, GraphicsClass* graphics, int screenWidth, int screenHeight)
{
	// 초기화로 사용한 값 저장
	m_Device = device;
	m_DeviceContext = deviceContext;
	m_RenderTargetView = renderTargetView;

	m_Graphics = graphics;
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// ImGui 초기화
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
	//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
	//io.ConfigViewportsNoAutoMerge = true;
	//io.ConfigViewportsNoTaskBarIcon = true;
	//io.ConfigViewportsNoDefaultParent = true;
	//io.ConfigDockingAlwaysTabBar = true;
	//io.ConfigDockingTransparentPayload = true;
#if 1
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleFonts;     // FIXME-DPI: THIS CURRENTLY DOESN'T WORK AS EXPECTED. DON'T USE IN USER APP!
	io.ConfigFlags |= ImGuiConfigFlags_DpiEnableScaleViewports; // FIXME-DPI
#endif

	// 마우스 커서 보이게하기
	io.MouseDrawCursor = true;

	// 초기 UI 스타일 수정
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();

	// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(device, deviceContext);

	return true;
}

void ImGuiEditorClass::Shutdown()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool ImGuiEditorClass::Render(ID3D11ShaderResourceView* shaderResourceView)
{
	ImGuiIO& io = ImGui::GetIO();

	// Start the Dear ImGui frame
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	// Draw Dock Space
	DrawDock(shaderResourceView);

	// Rendering
	ImGui::Render();
	m_DeviceContext->OMSetRenderTargets(1, &m_RenderTargetView, NULL);
	m_DeviceContext->ClearRenderTargetView(m_RenderTargetView, (float*)&clear_color);
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	// Update and Render additional Platform Windows
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	return true;
}

void ImGuiEditorClass::DrawDock(ID3D11ShaderResourceView* shaderResourceView)
{
	static bool opt_fullscreen_persistant = true;
	bool opt_fullscreen = opt_fullscreen_persistant;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
		window_flags |= ImGuiWindowFlags_NoBackground;

	// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
	// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
	// all active windows docked into it will lose their parent and become undocked.
	// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
	// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &show_dock_window, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	dockspace_id = ImGui::GetID("Edit");

	if (ImGui::DockBuilderGetNode(dockspace_id) == NULL)
	{
		CreateDock();
	}

	// DockSpace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		// 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
		if (show_demo_window)
			ImGui::ShowDemoWindow(&show_demo_window);

		// 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			ImGui::Checkbox("Another Window", &show_another_window);

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		// 3. Show another simple window.
		if (show_another_window)
		{
			ImGui::Begin("Another Window", &show_another_window);
			ImGui::Text("Hello from another window!");
			if (ImGui::Button("Close Me"))
				show_another_window = false;
			ImGui::End();
		}

		// 씬 뷰
		{
			ImGui::Begin("scene");
			ImGui::Image((void*)shaderResourceView, ImVec2((float)m_screenWidth, (float)m_screenHeight));
			ImGui::End();
		}

		if (show_hierarchy_window)
		{
			DrawHierarchy();
		}

		if (show_inspector_window)
		{
			DrawInspect();
		}


		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Docking"))
			{
				// Disabling fullscreen would allow the window to be moved to the front of other windows,
				// which we can't undo at the moment without finer window depth/z control.
				//ImGui::MenuItem("Fullscreen", NULL, &opt_fullscreen_persistant);

				if (ImGui::MenuItem("Flag: NoSplit", "", (dockspace_flags & ImGuiDockNodeFlags_NoSplit) != 0))                 dockspace_flags ^= ImGuiDockNodeFlags_NoSplit;
				if (ImGui::MenuItem("Flag: NoResize", "", (dockspace_flags & ImGuiDockNodeFlags_NoResize) != 0))                dockspace_flags ^= ImGuiDockNodeFlags_NoResize;
				if (ImGui::MenuItem("Flag: NoDockingInCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_NoDockingInCentralNode) != 0))  dockspace_flags ^= ImGuiDockNodeFlags_NoDockingInCentralNode;
				if (ImGui::MenuItem("Flag: PassthruCentralNode", "", (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode) != 0))     dockspace_flags ^= ImGuiDockNodeFlags_PassthruCentralNode;
				if (ImGui::MenuItem("Flag: AutoHideTabBar", "", (dockspace_flags & ImGuiDockNodeFlags_AutoHideTabBar) != 0))          dockspace_flags ^= ImGuiDockNodeFlags_AutoHideTabBar;
				ImGui::Separator();
				if (ImGui::MenuItem("Close DockSpace", NULL, false, &show_dock_window != NULL))
					show_dock_window = false;
				ImGui::EndMenu();
			}
			HelpMarker(
				"When docking is enabled, you can ALWAYS dock MOST window into another! Try it now!" "\n\n"
				" > if io.ConfigDockingWithShift==false (default):" "\n"
				"   drag windows from title bar to dock" "\n"
				" > if io.ConfigDockingWithShift==true:" "\n"
				"   drag windows from anywhere and hold Shift to dock" "\n\n"
				"This demo app has nothing to do with it!" "\n\n"
				"This demo app only demonstrate the use of ImGui::DockSpace() which allows you to manually create a docking node _within_ another window. This is useful so you can decorate your main application window (e.g. with a menu bar)." "\n\n"
				"ImGui::DockSpace() comes with one hard constraint: it needs to be submitted _before_ any window which may be docked into it. Therefore, if you use a dock spot as the central point of your application, you'll probably want it to be part of the very first window you are submitting to imgui every frame." "\n\n"
				"(NB: because of this constraint, the implicit \"Debug\" window can not be docked into an explicit DockSpace() node, because that window is submitted as part of the NewFrame() call. An easy workaround is that you can create your own implicit \"Debug##2\" window after calling DockSpace() and leave it in the window stack for anyone to use.)"
			);

			// 윈도우 창 메뉴
			if (ImGui::BeginMenu("Window"))
			{
				if (ImGui::MenuItem("Hierarchy", "", show_hierarchy_window))
					show_hierarchy_window = !show_hierarchy_window;
				if (ImGui::MenuItem("Inspector", "", show_inspector_window))
					show_inspector_window = !show_inspector_window;
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

	}


	ImGui::End();

}

void ImGuiEditorClass::CreateDock()
{
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::DockBuilderRemoveNode(dockspace_id); // Clear out existing layout
	ImGui::DockBuilderAddNode(dockspace_id, ImGuiDockNodeFlags_DockSpace); // Add empty node
	ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

	ImGuiID dock_main_id = dockspace_id; // This variable will track the document node, however we are not using it here as we aren't docking anything into it.
	ImGuiID dock_id_left = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Left, 0.20f, NULL, &dock_main_id);
	ImGuiID dock_id_right = ImGui::DockBuilderSplitNode(dock_main_id, ImGuiDir_Right, 0.20f, NULL, &dock_main_id);

	ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
	ImGui::DockBuilderDockWindow("Inspector", dock_id_right);
	ImGui::DockBuilderDockWindow("Hello, world!", dock_main_id);

	ImGui::DockBuilderFinish(dockspace_id);
}

void ImGuiEditorClass::DrawHierarchy()
{
	ImGui::Begin("Hierarchy", &show_hierarchy_window);

	/*
	if (m_Graphics != nullptr)
	{
		// 그래픽 클래스에서 모델 리스트를 읽어온다
		ModelListClass* modelList = m_Graphics->GetModelList();
		
		for (int i = 0; i < modelList->GetModelCount(); i++)
		{
			if (ImGui::Selectable(modelList->GetModelName(i) + i, selectedModel == i))
			{
				selectedModel = i;
			}
		}
	}
	*/

	// 루트를 그리면 하위 자식도 다 그려진다
	DrawTransform(Transform::Root);

	ImGui::End();
}

void ImGuiEditorClass::DrawTransform(Transform * transform)
{
	ImGuiTreeNodeFlags flags = 0;
	flags |= ImGuiTreeNodeFlags_OpenOnArrow;
	flags |= ImGuiTreeNodeFlags_AllowItemOverlap;

	// 하단에 자식이 없을 경우 화살표 출력 안해줌
	if (transform->getChildCount() == 0)
	{
		flags |= ImGuiTreeNodeFlags_Leaf;
	}

	// 화살표 만든다
	ImGui::PushID((size_t)transform);
	auto nodeId = ImGui::GetID("##w");
	bool nodeOpend = ImGui::TreeNodeEx("##w", flags);

	// 현재 선택했는지 여부 저장용 변수
	bool selected = transform == selectedItem;

	// 화살표와 같은 라인에 선택 가능 오브젝트 추가
	ImGui::SameLine();
	if (ImGui::Selectable(transform->gameObject->getName(), &selected))
	{
		selectedItem = transform;
	}

	// 화살표를 선택했을 경우
	if (nodeOpend)
	{
		// 해당 오브젝트의 트랜스폼 자식들을 그려준다
		ImGui::GetStateStorage()->SetInt(nodeId, 1);
		for (int i = 0; i < transform->getChildCount(); i++)
		{
			DrawTransform(transform->getChild(i));
		}
		ImGui::TreePop();
	}
	
	ImGui::PopID();
}

void ImGuiEditorClass::DrawInspect()
{
	ImGui::Begin("Inspector", &show_inspector_window);

	// 선택한 모델이 있는 경우
	if (selectedModel != -1)
	{
		// 그래픽 클래스에서 모델 리스트를 읽어와
		// 선택된 모델을 가져온다
		i_modelList = m_Graphics->GetModelList();
		i_model = i_modelList->GetModel(selectedModel);

		// 그 후 모델의 머테리얼을 가져온다
		i_mat = i_model->GetModelMaterial();
		
		// 현재 선택한 모델의 쉐이더를 가져온다
		char* shaderName = ShaderManager::GetInstance()->GetShaderType(i_mat->GetShaderType());

		// 쉐이더 수만큼 순회하여 스트링 배열을 만든다
		char* items[ShaderManager::ShaderTypeCount];
		for (int i = 0; i < ShaderManager::ShaderTypeCount; i++)
		{
			items[i] = ShaderManager::GetInstance()->GetShaderType(ShaderManager::ShaderType(i));
		}

		char* item_current = shaderName;            // Here our selection is a single pointer stored outside the object.
		if (ImGui::BeginCombo("ShaderType", item_current, 0)) // The second parameter is the label previewed before opening the combo.
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				// 선택했으면 해당 머테리얼의 쉐이더를 변경한다
				bool is_selected = (item_current == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current = items[n];
					i_mat->SetShaderType(ShaderManager::ShaderType(n));
				}
			}
			ImGui::EndCombo();
		}
	}

	// 선택한 게임 오브젝트 트랜스폼이 있을 경우
	if (selectedItem != nullptr)
	{
		// 컴포넌트의 타입에 따라 인스펙터에 처리해준다
		if (drawInspector == nullptr)
			drawInspector = new DrawInspector();

		// 선택한 오브젝트의 모든 컴포넌트를 그려준다
		for (auto component : selectedItem->gameObject->getComponents())
		{
			drawInspector->DrawComponent(component);
		}

		// 컴포넌트 추가기능
		ImGui::Separator();
		if (ImGui::Button("Add Component"))
		{
		}

	}

	ImGui::End();
}

// Helper to display a little (?) mark which shows a tooltip when hovered.
// In your own code you may want to display an actual icon if you are using a merged icon fonts (see misc/fonts/README.txt)
void ImGuiEditorClass::HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}