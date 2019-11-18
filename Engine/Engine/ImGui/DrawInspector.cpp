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
		// �����ص� map���� ������Ʈ�� Ÿ������ �޼ҵ带 �ҷ��´�
		ImGui::PushID(comp->getTypeName());

		// �ν����� �⺻�� �׸� ��
		//DrawDefaultInspect(comp->getTypeName());

		// Ÿ�Կ� �´� ������ �׸���
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
		// �ӽ÷� Ÿ�� �̸�
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
		// ���� ������ ���� ���̴��� �����´�
		char* shaderName = ShaderManager::GetInstance()->GetShaderType(material->GetShaderType());

		// ���̴� ����ŭ ��ȸ�Ͽ� ��Ʈ�� �迭�� �����
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
				// ���������� �ش� ���׸����� ���̴��� �����Ѵ�
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
