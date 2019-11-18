#include "DrawInspector.h"


static const float SpacingForLabel = 5.0f;
static map<string, bool(*)(Component*)> handlers = {
	{"Transform", DrawTransformInspect},
	{"Material", DrawMaterialInspect}
};

DrawInspector::DrawInspector()
{
}

DrawInspector::~DrawInspector()
{
}

void DrawInspector::DrawComponent(Component* comp)
{
	auto it = handlers.find(comp->getTypeName());
	if (it != handlers.end())
	{
		// 저장해둔 map에서 컴포넌트의 타입으로 메소드를 불러온다
		ImGui::PushID(comp->getTypeName());

		// 인스펙터 기본을 그린 뒤
		//DrawDefaultInspect(comp->getTypeName());

		// 타입에 맞는 내용을 그린다
		handlers[comp->getTypeName()](comp);
		ImGui::PopID();
	}
}

static void DrawDefaultInspect(const char* name, float width)
{
	ImGui::SetNextItemWidth(ImGui::GetWindowWidth() * width);
	ImGui::LabelText("", name); ImGui::SameLine(0, SpacingForLabel);
}

static bool DrawTransformInspect(Component* comp)
{
	Transform* transform = comp->transform;

	ImGui::Separator();
	
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGui::PushID(comp);
	if (ImGui::CollapsingHeader(comp->getTypeName(), flags))
	{
		// 임시로 타입 이름
		ImGui::Text(comp->getTypeName());

		ImGui::Dummy(ImVec2(10, 10));
		ImGui::SameLine(ImGui::GetWindowWidth() - 30);
	}
	ImGui::PopID();
	return true;
}

static bool DrawMaterialInspect(Component* comp)
{
	Material* material = comp->gameObject->getComponent<Material>();

	ImGui::Separator();

	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	ImGui::PushID(comp);
	if (ImGui::CollapsingHeader(comp->getTypeName(), flags))
	{
		// 현재 선택한 모델의 쉐이더를 가져온다
		char* shaderName = ShaderManager::GetInstance()->GetShaderType(material->GetShaderType());

		// 쉐이더 수만큼 순회하여 스트링 배열을 만든다
		char* items[ShaderManager::ShaderTypeCount];
		for (int i = 0; i < ShaderManager::ShaderTypeCount; i++)
		{
			items[i] = ShaderManager::GetInstance()->GetShaderType(ShaderManager::ShaderType(i));
		}

		char* item_current = shaderName;
		if (ImGui::BeginCombo("ShaderType", item_current, 0))
		{
			for (int n = 0; n < IM_ARRAYSIZE(items); n++)
			{
				// 선택했으면 해당 머테리얼의 쉐이더를 변경한다
				bool is_selected = (item_current == items[n]);
				if (ImGui::Selectable(items[n], is_selected))
				{
					item_current = items[n];
					material->SetShaderType(ShaderManager::ShaderType(n));
				}
			}
			ImGui::EndCombo();
		}
	}
	ImGui::PopID();
	return true;
}
